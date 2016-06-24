/*
 * Copyright (C) 2016 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Todd Gill <tgill@redhat.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <glib.h>
#include <limits.h>
#include <string.h>
#include <libdmmp/libdmmp.h>
#include <systemd/sd-bus.h>

#include "plugin.h"
#include "multipath_common.h"


#define TMO 10000

GList * dbus_maps = NULL;


/*
 * create an array of strings suitable to pass to dbus
 */
static gchar **make_array(int size) {

    int i;
    char **array = (char **) malloc((size + 1) * sizeof (char *));

    if (array == NULL)
        return NULL;

    for (i = 0; i <= size; i++) {
        array[i] = NULL;
    }

    return array;
}



static char *get_long_str(long value) {

    const int n = snprintf(NULL, 0, "%ld", value);

    if (n < 0)
        return NULL;

    char *buffer = malloc(n + 1);

    if (buffer == NULL)
        return NULL;

    snprintf(buffer, n + 1, "%ld", value);

    return buffer;
}

char *make_object_name(const char *base_name, unsigned long name, char *object_type)
{
    char *obj_name = NULL;

    if (base_name == NULL || name < 0) {
        return NULL;
    }

    char *instance_name = get_long_str(name);

    if (instance_name == NULL)
        goto finished;

    int size = strlen(base_name) + strlen(instance_name) + strlen(object_type) + 2;

        obj_name = malloc(size);

        if (obj_name == NULL)
            return NULL;

    snprintf(obj_name, size, "%s/%s%s", base_name, object_type, instance_name);

finished:

    if (instance_name != NULL) {
        free(instance_name);
        instance_name = NULL;
    }

        return obj_name;
}


static int get_map_property(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	struct dmmp_mpath *map = userdata;

	if (strcmp(property, MAP_NAME) == 0)
		return sd_bus_message_append(reply, "s", dmmp_mpath_name_get(map));

	if (strcmp(property, MAP_WWID) == 0)
		return sd_bus_message_append(reply, "s", dmmp_mpath_wwid_get(map));

	// TODO deal with error
	return -1;
}

static int get_group_property(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	struct dmmp_path_group *group = userdata;

	if (strcmp(property, GROUP_ID) == 0)
		return sd_bus_message_append(reply, "u", dmmp_path_group_id_get(group));

	if (strcmp(property, GROUP_PRIORITY) == 0)
		return sd_bus_message_append(reply, "u", dmmp_path_group_priority_get(group));

	if (strcmp(property, GROUP_STATUS) == 0)
		return sd_bus_message_append(reply, "s", dmmp_path_group_status_str(
				dmmp_path_group_status_get(group)));

	// TODO deal with error
	return -1;
}
static int get_path_property(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	struct dmmp_path *map_path = userdata;

	if (strcmp(property, PATH_NAME) == 0)
		return sd_bus_message_append(reply, "s", dmmp_path_blk_name_get(map_path));

	if (strcmp(property, PATH_STATUS) == 0)
		return sd_bus_message_append(reply, "s", dmmp_path_status_str(dmmp_path_status_get(map_path)));


	// TODO deal with error
	return -1;
}

static const sd_bus_vtable map_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY(MAP_NAME, "s", get_map_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY(MAP_WWID, "s", get_map_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_VTABLE_END
};
static const sd_bus_vtable group_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY(GROUP_ID, "u", get_group_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY(GROUP_PRIORITY, "u", get_group_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY(GROUP_STATUS, "s", get_group_property, 0,
					SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_VTABLE_END
};
static const sd_bus_vtable path_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY(PATH_NAME, "s", get_path_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_PROPERTY(PATH_STATUS, "s", get_path_property, 0,
			SD_BUS_VTABLE_PROPERTY_CONST),
	SD_BUS_VTABLE_END
};

static plugin_status_t update_map(sd_bus *bus, sd_bus_slot *slot, struct dmmp_mpath *map) {
	plugin_status_t return_code = PLUGIN_OK;

	int i = 0;
	int path_count = 0;
	char map_name[256], group_name[256], path_name[256];
	struct dmmp_path_group **dmmp_pgs = NULL;
	uint32_t dmmp_pg_count = 0;
	struct dmmp_path **mp_ps = NULL;
	uint32_t mp_p_count = 0;
	uint32_t j = 0;
	const char *blk_name = NULL;
	int r;

	snprintf(map_name, 256, "%s/%s", MULTIPATH_BASE_PATH, dmmp_mpath_wwid_get(map));

	r = sd_bus_add_object_vtable(bus,
							&slot, map_name,
							MULTIPATH_BASE_INTERFACE,
							map_vtable, map);
	if (r < 0) {
		fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
		return -1;
	}

	dmmp_path_group_array_get(map, &dmmp_pgs, &dmmp_pg_count);

	if (dmmp_pg_count == 0)
		printf("dmmp_path_group_array_get(): Got 0 path group\n");

	for (i = 0; i < dmmp_pg_count; ++i) {


		snprintf(group_name, 256, "%s/group%d", map_name, dmmp_path_group_id_get(dmmp_pgs[i]));

		r = sd_bus_add_object_vtable(bus,
								&slot, group_name,
								MULTIPATH_BASE_INTERFACE,
								group_vtable, dmmp_pgs[i]);
		if (r < 0) {
			fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
			return -1;
		}


		/*
		 * Loop though the paths
		 */

		dmmp_path_array_get(dmmp_pgs[i], &mp_ps, &mp_p_count);
		if (mp_p_count == 0)
			printf("dmmp_path_array_get(): Got no path\n");
		for (j = 0; j < mp_p_count; ++j) {
			blk_name = dmmp_path_blk_name_get(mp_ps[j]);
			if (blk_name == NULL)
				printf("dmmp_path_blk_name_get(): Got NULL\n");


			snprintf(path_name, 256, "%s/%s", group_name, blk_name);

			r = sd_bus_add_object_vtable(bus,
									&slot, path_name,
									MULTIPATH_BASE_INTERFACE,
									group_vtable, mp_ps[j]);
			if (r < 0) {
				fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
				return -1;
			}

		}
	}
	finished:

	return return_code;
}


int sync_maps(sd_bus *bus, sd_bus_slot *slot)
{
        struct dmmp_context *ctx = NULL;
        struct dmmp_mpath **maps = NULL;
        uint32_t dmmp_mp_count = 0;
        const char *name = NULL;
        const char *wwid = NULL;
        uint32_t i = 0;
        int rc = EXIT_SUCCESS;

        ctx = dmmp_context_new();
        dmmp_context_log_priority_set(ctx, DMMP_LOG_PRIORITY_DEBUG);
        dmmp_context_userdata_set(ctx, ctx);
        dmmp_context_userdata_set(ctx, NULL);
        dmmp_context_timeout_set(ctx, TMO);
        if (dmmp_context_timeout_get(ctx) != TMO)
                printf("dmmp_context_timeout_set(): Failed to set "
                     "timeout to %u", TMO);

        if (dmmp_mpath_array_get(ctx, &maps, &dmmp_mp_count) != 0)
        	printf("dmmp_mpath_array_get(): rc != 0\n");
        if (dmmp_mp_count == 0)
        	printf("dmmp_mpath_array_get(): "
                     "Got no multipath devices\n");

        for (i = 0; i < dmmp_mp_count; ++i) {

                update_map(bus, slot, maps[i]);


        }

//	TODO fix memory leak/resource problems
//        dmmp_mpath_array_free(maps, dmmp_mp_count);
out:
//        dmmp_context_free(ctx);

        return 0;
}


/*
 * This is the main loop of the d-bus service.  It won't exit until
 * quit_dbus_main_loop() is called.
 *
 * It is should be invoked as the startup function of a thread or the caller
 * should not expect it to return.
 */
void * multipath_main_loop(void * ap) {
	sd_bus_slot *slot = NULL;
	sd_bus *bus = NULL;
	int r;

	/* Connect to the user bus this time */
	r = sd_bus_open_user(&bus);
	if (r < 0) {
		fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
		goto finish;
	}


	sync_maps(bus, slot);

	/* Take a well-known service name so that clients can find us */
	r = sd_bus_request_name(bus, MULTIPATH_BASE_INTERFACE, 0);

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
