#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <systemd/sd-bus.h>
#include <parted/parted.h>
#include <parted/debug.h>

#include "block_common.h"


static int property_get_pathname(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	PedDevice *dev = (PedDevice *) userdata;
	return sd_bus_message_append(reply, "s", dev->path);
}

static int property_get_model(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	PedDevice *dev = (PedDevice *) userdata;
	return sd_bus_message_append(reply, "s", dev->model);
}

static int property_get_type(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	PedDevice *dev = (PedDevice *) userdata;
	char *type = "";

	switch (dev->type) {
	case PED_DEVICE_UNKNOWN:
		type = "UNKNOWN";
		break;
	case PED_DEVICE_SCSI:
		type = "SCSI";
		break;
	case PED_DEVICE_IDE:
		type = "UNKNOWN";
		break;
	case PED_DEVICE_DAC960:
		type = "DAC960";
		break;
	case PED_DEVICE_CPQARRAY:
		type = "CPQARRAY";
		break;
	case PED_DEVICE_FILE:
		type = "FILE";
		break;
	case PED_DEVICE_ATARAID:
		type = "ATARAID";
		break;
	case PED_DEVICE_I2O:
		type = "I2O";
		break;
	case PED_DEVICE_UBD:
		type = "UBD";
		break;
	case PED_DEVICE_DASD:
		type = "DASD";
		break;
	case PED_DEVICE_VIODASD:
		type = "VIODASD";
		break;
	case PED_DEVICE_SX8:
		type = "SX8";
		break;
	case PED_DEVICE_DM:
		type = "DM";
		break;
	case PED_DEVICE_XVD:
		type = "XVD";
		break;
	case PED_DEVICE_SDMMC:
		type = "SDMMC";
		break;
	case PED_DEVICE_VIRTBLK:
		type = "VIRTBLK";
		break;
	case PED_DEVICE_AOE:
		type = "AOE";
		break;
	case PED_DEVICE_MD:
		type = "MD";
		break;
	case PED_DEVICE_LOOP:
		type = "LOOP";
		break;

	}
	return sd_bus_message_append(reply, "s", type);
}

static const sd_bus_vtable block_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY("PathName", "s", property_get_pathname, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY("Model", "s", property_get_model, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY("Type", "s", property_get_type, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_VTABLE_END
};


int get_block_devs(sd_bus *bus, sd_bus_slot *slot) {

	PedDevice* dev = NULL;
	int r;
	char name[256];

	ped_device_probe_all();

	while ((dev = ped_device_get_next(dev)) != NULL) {

		snprintf(name, 256, "%s%s", BLOCK_PATH_BASE_NAME, dev->path);

		r = sd_bus_add_object_vtable(bus,
								&slot, name,
								BLOCK_INTERFACE_NAME,
								block_vtable, dev);
		if (r < 0) {
			fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
			return -1;
		}


	}
	return 0;
}

void * block_main_loop(void * ap){
	sd_bus_slot *slot = NULL;
	sd_bus *bus = NULL;
	int r;

	/* Connect to the user bus this time */
	r = sd_bus_open_user(&bus);
	if (r < 0) {
		fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
		goto finish;
	}


	get_block_devs(bus, slot);

	/* Take a well-known service name so that clients can find us */
	r = sd_bus_request_name(bus, BLOCK_INTERFACE_NAME, 0);

	if (r < 0) {
		fprintf(stderr, "Failed to acquire service name: %s\n", strerror(-r));
		goto finish;
	}

	for (;;) {
		/* Process requests */
		r = sd_bus_process(bus, NULL);
		if (r < 0) {
			fprintf(stderr, "Failed to process bus: %s\n", strerror(-r));
			goto finish;
		}
		if (r > 0) /* we processed a request, try to process another one, right-away */
			continue;

		/* Wait for the next request to process */
		r = sd_bus_wait(bus, (uint64_t) - 1);
		if (r < 0) {
			fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-r));
			goto finish;
		}
	}

	finish: sd_bus_slot_unref(slot);
	sd_bus_unref(bus);

	return NULL;
}

