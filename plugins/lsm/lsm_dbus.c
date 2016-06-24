#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <systemd/sd-bus.h>
#include <parted/parted.h>
#include <parted/debug.h>
#include <libstoragemgmt/libstoragemgmt.h>

#include <glib.h>
#include <glib/gprintf.h>

#include "lsm_common.h"

#define _STD_LSM_SIM_URI "sim://"
#define _STD_LSM_HPSA_URI "hpsa://"

static GPtrArray *_conf_lsm_uri_sets = NULL;


#define _STD_LSM_CONNECTION_DEFAULT_TMO 30000



/*
 * _LsmUriSet is holding the URI and password string pointers.
 */
struct _LsmUriSet
{
  const char *uri;
  const char *password;
};



// TODO fix
int storaged_warning(const char *format, ...) {
	   va_list arg;
	   int done;

	   va_start (arg, format);
	   done = vfprintf (stdout, format, arg);
	   va_end (arg);

	   return done;
}

int storaged_debug(const char *format, ...) {
	   va_list arg;
	   int done;

	   va_start (arg, format);
	   done = vfprintf (stdout, format, arg);
	   va_end (arg);

	   return done;
}

static struct _LsmUriSet *
_lsm_uri_set_new (const char *uri, const char *pass)
{
  struct _LsmUriSet *lsm_uri_set;

  lsm_uri_set = (struct _LsmUriSet *) g_malloc (sizeof (struct _LsmUriSet));
  lsm_uri_set->uri = g_strdup (uri);
  lsm_uri_set->password = g_strdup (pass);
  return lsm_uri_set;
}




static lsm_connect *
_create_lsm_connect (struct _LsmUriSet *lsm_uri_set)
{
  lsm_connect *lsm_conn = NULL;
  lsm_error *lsm_err = NULL;
  int lsm_rc;
  const char *uri = NULL;
  const char *password = NULL;

  if (lsm_uri_set == NULL)
    {
      storaged_debug ("LSM: _create_lsm_connect (): Skip on NULL lsm_uri_set");
      return NULL;
    }
  uri = lsm_uri_set->uri;
  password = lsm_uri_set->password;

  storaged_debug ("LSM: Connecting to URI: %s", uri);
  lsm_rc = lsm_connect_password (uri, password, &lsm_conn,
                                 _STD_LSM_CONNECTION_DEFAULT_TMO,
                                 &lsm_err, LSM_CLIENT_FLAG_RSVD);
  if (lsm_rc == LSM_ERR_DAEMON_NOT_RUNNING)
    {
      storaged_warning ("LSM: The libStorageMgmt daemon is not running "
                        "(process name lsmd), try "
                        "'service libstoragemgmt start'");
      lsm_error_free (lsm_err);
      return NULL;
    }
  if (lsm_rc != LSM_ERR_OK)
    {
      storaged_warning ("LSM: Failed to connect plugin via URI '%s', "
                        "error code: %d, error message: %s",
                        uri, lsm_error_number_get (lsm_err),
                        lsm_error_message_get (lsm_err));
      lsm_error_free (lsm_err);
      return NULL;
    }
  storaged_debug ("LSM: Plugin for URI '%s' connected", uri);
  return lsm_conn;
}

/*
 * Print lsm error via storaged_warning ()
 *
 */
static void
_handle_lsm_error (const char *msg, lsm_connect *lsm_conn)
{
  lsm_error *lsm_err = NULL;

  lsm_err = lsm_error_last_get (lsm_conn);
  if (lsm_err)
    {
      storaged_warning ("%s. Error code: %d, error message: %s",
                        msg, lsm_error_number_get (lsm_err),
                        lsm_error_message_get (lsm_err));
      lsm_error_free (lsm_err);
    }
  else
    {
      storaged_warning
        ("LSM: %s. But failed to retrieve error code and message", msg);
    }
  return;
}


// TESTING TODO
void get_lsm_uris() {
	struct _LsmUriSet *lsm_uri_set = NULL;

	_conf_lsm_uri_sets = g_ptr_array_new();
    lsm_uri_set = _lsm_uri_set_new (_STD_LSM_SIM_URI, NULL);
    g_ptr_array_add (_conf_lsm_uri_sets, lsm_uri_set);

}

static int get_volume_property(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {

	lsm_volume *lsm_vol = userdata;

	if (strcmp(property, VOLUME_ID) == 0)
		return sd_bus_message_append(reply, "s", lsm_volume_id_get (lsm_vol));
	if (strcmp(property, VOLUME_NAME) == 0)
		return sd_bus_message_append(reply, "s", lsm_volume_id_get (lsm_vol));
	if (strcmp(property, BLOCK_SIZE) == 0)
		return sd_bus_message_append(reply, "u", lsm_volume_id_get (lsm_vol));
	if (strcmp(property, NUMBER_OF_BLOCKS) == 0)
		return sd_bus_message_append(reply, "u", lsm_volume_id_get (lsm_vol));
	if (strcmp(property, SYSTEM_ID) == 0)
		return sd_bus_message_append(reply, "s", lsm_volume_id_get (lsm_vol));
	if (strcmp(property, POOL_ID) == 0)
		return sd_bus_message_append(reply, "s", lsm_volume_id_get (lsm_vol));

	// TODO this right?
	return -1;
}

static const sd_bus_vtable lsm_volume_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY(VOLUME_ID, "s", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(VOLUME_NAME, "s", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(BLOCK_SIZE, "u", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(NUMBER_OF_BLOCKS, "u", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(SYSTEM_ID, "s", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(POOL_ID, "s", get_volume_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_VTABLE_END
};


void populate_volume_data(sd_bus *bus, sd_bus_slot *slot, lsm_volume **volume_array, int volume_count) {
	lsm_volume *lsm_vol = NULL;
	guint i, r;
	char name[256];

	for (i = 0; i < volume_count; ++i) {
		lsm_vol = volume_array[i];

		snprintf(name, 256, "%s/%s", LSM_PATH_BASE_NAME,  lsm_volume_id_get (lsm_vol));

		fprintf(stdout, "lsm_vol = ", name);

		r = sd_bus_add_object_vtable(bus,
								&slot, name,
								LSM_INTERFACE_NAME, /* interface name */
								lsm_volume_vtable, lsm_vol);
	}
}

static int property_get_pool_property(sd_bus *bus, const char *path,
		const char *interface, const char *property, sd_bus_message *reply,
		void *userdata, sd_bus_error *error) {
	lsm_pool *lsm_pool = userdata;

	if (strcmp(property, POOL_ID) == 0)
		return sd_bus_message_append(reply, "s", lsm_pool_id_get (lsm_pool));
	if (strcmp(property, POOL_NAME) == 0)
		return sd_bus_message_append(reply, "s", lsm_pool_name_get (lsm_pool));
	if (strcmp(property, TOTAL_SPACE) == 0)
		return sd_bus_message_append(reply, "u", lsm_pool_total_space_get (lsm_pool));
	if (strcmp(property, FREE_SPACE) == 0)
		return sd_bus_message_append(reply, "u", lsm_pool_free_space_get (lsm_pool));
	if (strcmp(property, POOL_STATUS) == 0)
		return sd_bus_message_append(reply, "u", lsm_pool_status_get (lsm_pool));
	if (strcmp(property, POOL_STATUS_INFO) == 0)
		return sd_bus_message_append(reply, "s", lsm_pool_status_info_get (lsm_pool));
	if (strcmp(property, STATUS) == 0)
		return sd_bus_message_append(reply, "u", lsm_pool_status_get (lsm_pool));
	if (strcmp(property, STATUS_INFO) == 0)
		return sd_bus_message_append(reply, "s", lsm_pool_status_info_get (lsm_pool));
	if (strcmp(property, SYSTEM_ID) == 0)
		return sd_bus_message_append(reply, "s", lsm_pool_system_id_get (lsm_pool));
	if (strcmp(property, ELEMENT_TYPE) == 0)
		return sd_bus_message_append(reply, "u", lsm_pool_element_type_get (lsm_pool));

	//TODO is this correct?
	return -1;

}
static const sd_bus_vtable lsm_pool_vtable[] = {
	SD_BUS_VTABLE_START(0),
	SD_BUS_PROPERTY(POOL_ID, "s", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(POOL_NAME, "s", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(TOTAL_SPACE, "u", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(FREE_SPACE, "u", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(POOL_STATUS, "u", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(POOL_STATUS_INFO, "s", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(STATUS, "u", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(STATUS_INFO, "s", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(SYSTEM_ID, "s", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_PROPERTY(ELEMENT_TYPE, "u", property_get_pool_property, 0, SD_BUS_VTABLE_PROPERTY_EMITS_CHANGE),
	SD_BUS_VTABLE_END
};

void populate_pool_data(sd_bus *bus, sd_bus_slot *slot, lsm_pool **pool_array, int pool_count) {
	lsm_pool *lsm_pool = NULL;
	guint i, r;
	char name[256];

	for (i = 0; i < pool_count; ++i) {
		lsm_pool = pool_array[i];

		snprintf(name, 256, "%s/%s", LSM_PATH_BASE_NAME, lsm_pool_id_get(lsm_pool));
		printf("lsm_vol = ", name);

		r = sd_bus_add_object_vtable(bus,
								&slot, name,
								LSM_INTERFACE_NAME, /* interface name */
								lsm_pool_vtable, lsm_pool);
	}

}

void
std_lsm_data_init (sd_bus *bus, sd_bus_slot *slot)
{
  struct _LsmUriSet *lsm_uri_set = NULL;
  lsm_connect *lsm_conn = NULL;
  lsm_volume **lsm_vols = NULL;
  lsm_pool **lsm_pls = NULL;
  uint32_t lsm_vol_count = 0;
  uint32_t lsm_pl_count = 0;
  GPtrArray *lsm_pl_array = NULL;
  guint i = 0, rc = 0;

  get_lsm_uris(); // TODO fix this

  for (i =0; i < _conf_lsm_uri_sets->len; ++i)
    {
      lsm_uri_set = g_ptr_array_index (_conf_lsm_uri_sets, i);
      lsm_conn = _create_lsm_connect (lsm_uri_set);
      if (lsm_conn == NULL)
        continue;

      rc = lsm_volume_list (lsm_conn, NULL, NULL, &lsm_vols, &lsm_vol_count,
                            LSM_CLIENT_FLAG_RSVD);

      if (rc != LSM_ERR_OK)
    	  return;

      if (lsm_vol_count > 0)
    	  populate_volume_data(bus, slot, lsm_vols, lsm_vol_count);

      rc = lsm_pool_list (lsm_conn, NULL, NULL, &lsm_pls,
                              &lsm_pl_count, LSM_CLIENT_FLAG_RSVD);

      if (rc != LSM_ERR_OK)
    	  return;

      if (lsm_vol_count > 0)
    	  populate_pool_data(bus, slot, lsm_pls, lsm_pl_count);

    }
}




void * lsm_main_loop(void * ap){
	sd_bus_slot *slot = NULL;
	sd_bus *bus = NULL;
	int r;

	/* Connect to the user bus this time */
	r = sd_bus_open_user(&bus);
	if (r < 0) {
		fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
		goto finish;
	}

	std_lsm_data_init(bus, slot);

	char *x = LSM_INTERFACE_NAME;

	r = sd_bus_request_name(bus, LSM_INTERFACE_NAME, 0);

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

