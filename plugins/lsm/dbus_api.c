#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <glib.h>
#include <limits.h>
#include <string.h>
#include <libdmmp/libdmmp.h>

#include "lsm_common.h"
#include "lsm_dbus_generated.h"


static GMainLoop *main_loop = NULL;
static GDBusObjectManagerServer *object_manager = NULL;
static int dbus_lock_ready = FALSE;


void quit_lsm_main_loop()
{
    if (main_loop != NULL) {
        printf("quit_dbus_main_loop\n");
        g_main_loop_quit(main_loop);
    }

}

/*
 * This can happen if another process claims the "name" on the
 * bus.  If the dbus name is lost, exit the dbus main loop.
 *
 */
static void on_name_lost(GDBusConnection *connection, const gchar *name,
		gpointer user_data) {
	quit_lsm_main_loop();
	object_manager = NULL;
}

static void on_name_acquired(GDBusConnection *connection, const gchar *name,
		gpointer user_data) {
}

/*
 * When this function is completed, the dbus service is ready.
 * Release the dbus_lock it is complete to allow calls into the
 * public dbus_xxx calls to proceed.
 *
 */
static void on_bus_acquired(GDBusConnection *connection, const gchar *name,
		gpointer user_data) {
	GDBusObjectSkeleton *manager_interface_container = NULL;
	GError *error = NULL;

	// Create an object manager
	object_manager = g_dbus_object_manager_server_new(BASE_PATH);

	if (object_manager == NULL)
		goto finished;

	manager_interface_container = g_dbus_object_skeleton_new(
			BASE_PATH MANAGER_NAME);

	if (manager_interface_container == NULL)
		goto finished;

	g_dbus_object_manager_server_export(object_manager,
			G_DBUS_OBJECT_SKELETON(manager_interface_container));

	g_dbus_object_manager_server_set_connection(object_manager, connection);

	lsmOrgStoragedStoragedDriveLSM *lsmManager = lsm_org_storaged_storaged_drive_lsm_skeleton_new();

	g_dbus_interface_skeleton_export(
			G_DBUS_INTERFACE_SKELETON(lsmManager), connection,
			BASE_PATH MANAGER_NAME, &error);

	finished:

	dbus_lock_ready = TRUE;

}


/*
 * This is the main loop of the d-bus service.  It won't exit until
 * quit_dbus_main_loop() is called.
 *
 * It is should be invoked as the startup function of a thread or the caller
 * should not expect it to return.
 */
void * lsm_main_loop(void * ap) {
	guint owner_id;


	/* Set up DBus name, see callback */
	owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
	BASE_INTERFACE, G_BUS_NAME_OWNER_FLAGS_REPLACE, on_bus_acquired,
			on_name_acquired, on_name_lost,
			NULL,
			NULL);

	main_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(main_loop);

	g_bus_unown_name(owner_id);
	g_main_loop_unref(main_loop);

	g_object_unref(object_manager);

	return NULL;
}
