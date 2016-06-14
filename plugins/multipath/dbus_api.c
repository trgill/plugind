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


#include "plugin.h"
#include "multipath_common.h"
#include "multipath_dbus_generated.h"

#define TMO 10000

GList * dbus_maps = NULL;

static GMainLoop *main_loop = NULL;
static GDBusObjectManagerServer *object_manager = NULL;
static int dbus_lock_ready = FALSE;
static int map_instance = 0;
static int path_instance = 0;

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

static long next_map () {
    map_instance++;

    if (map_instance == LONG_MAX) {
        map_instance = 0;
    }

    return map_instance;
}

static long next_path () {
    path_instance++;

    if (path_instance == LONG_MAX) {
        path_instance = 0;
    }

    return path_instance;
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

/*
 * free the array and strings
 */
static void release_array(gchar **array, int size) {

    int i;
    if (array == NULL)
        return;

    for (i = 0; i < size; i++) {
        if (array[i] == NULL)
            continue;

        free(array[i]);
        array[i] = NULL;
    }

    free(array);
}

static char *make_map_name(unsigned long dbus_name) {

    return make_object_name(BASE_PATH, dbus_name, MAP_TYPE);

}

static char *make_path_name(int path_number, char *map_object_name) {

    if (map_object_name == NULL)
        return NULL;

    return make_object_name(map_object_name, path_number, PATH_TYPE);

}

static  gchar **get_member_paths(struct multipath_shadow *map_shadow, char *map_object_name) {

    gchar **member_array    = NULL;
    struct  path * path                 = NULL;
    int i = 0;
    GList*  iterator = NULL;

    if (map_shadow == NULL || map_shadow->paths == NULL)
        return NULL;

    int size = g_list_length(map_shadow->paths);

    member_array = make_array(size);

    if (member_array == NULL)
        return NULL;

    for (iterator = map_shadow->paths; iterator; iterator = iterator->next) {
        member_array[i] = make_path_name(i, map_object_name);
    }

    return member_array;
}

static plugin_status_t update_existing_map_object(struct dmmp_mpath *map,
                        struct multipath_shadow *map_shadow) {
	plugin_status_t return_code    = PLUGIN_OK;
    gchar **path_array              = NULL;
    multipathMap *map_object        = NULL;
    int path_count                  = g_list_length(map_shadow->paths);

    map_object = map_shadow->map_object;

    path_array = get_member_paths(map_shadow, map_shadow->dbus_name);

    if (path_array == NULL) {
        return_code = PLUGIN_ERROR_MALLOC;
        goto finished;
    }
    multipath_map_set_paths(map_object, (const gchar *const *) path_array);
/*
    multipath_map_set_uuid(map_object, map->wwid);
    multipath_map_set_size(map_object, map->size);
    multipath_map_set_features(map_object, map->features);
    multipath_map_set_pgfailback(map_object, abs(map->pgfailback));
    multipath_map_set_pg_policy(map_object, map->pgpolicy);

    multipath_map_set_bestpg(map_object, map->bestpg);
    multipath_map_set_nextpg(map_object, map->nextpg);
    multipath_map_set_queuedio(map_object, map->queuedio);
    multipath_map_set_no_path_retry(map_object, map->no_path_retry);
    multipath_map_set_nr_active(map_object, map->nr_active);
    multipath_map_set_selector(map_object, map->selector);
    multipath_map_set_pathgroups(map_object, group_json);
*/
finished:

    if (path_array != NULL) {
        release_array(path_array, path_count);
        path_array = NULL;
    }


    return return_code;
}

static plugin_status_t update_existing_map(GDBusObjectManagerServer *manager,
                                struct dmmp_mpath *map,
                                struct multipath_shadow *map_shadow) {
    plugin_status_t return_code    = PLUGIN_OK;
    struct path_shadow *path_shadow = NULL;
    struct path *path               = NULL;
    int i                           = 0;

    // TODO implement

    return return_code;
}

static plugin_status_t create_map_object(GDBusObjectManagerServer *manager,
							struct dmmp_mpath *map,
                            struct multipath_shadow *map_shadow) {
    plugin_status_t         return_code = PLUGIN_OK;

    multipathObjectSkeleton *map_interface_container    = NULL;

    if (manager == NULL || map == NULL)
        return PLUGIN_ERROR_NULL;

    map_interface_container = multipath_object_skeleton_new(map_shadow->dbus_name);

    if (map_interface_container == NULL) {
        return_code = PLUGIN_ERROR_MALLOC;
        goto finished;
    }

    map_shadow->map_object = multipath_map_skeleton_new();

    if (map_shadow->map_object == NULL) {
        return_code = PLUGIN_ERROR_MALLOC;
        goto finished;
    }

    multipath_object_skeleton_set_map(map_interface_container, map_shadow->map_object);

    g_dbus_object_manager_server_export (manager,
            G_DBUS_OBJECT_SKELETON (map_interface_container));

finished:

    if (map_interface_container != NULL) {
        g_object_unref(map_interface_container);
        map_interface_container = NULL;
    }

    return return_code;
}

static plugin_status_t add_new_map(struct dmmp_mpath *map,
                            struct multipath_shadow **map_shadow) {
    plugin_status_t rc = PLUGIN_OK;
    char *map_name      = NULL;

    const char 				*wwid 			= NULL;


    wwid = dmmp_mpath_wwid_get(map);

    *map_shadow = malloc(sizeof(struct multipath_shadow));

    if (map_shadow == NULL) {
        rc = PLUGIN_ERROR_MALLOC;
        goto error;
    }
    (*map_shadow)->paths = NULL;
    map_name = make_map_name(next_map());

    if (map_name == NULL) {
        rc = PLUGIN_ERROR_MALLOC;
        goto error;
    }

    strcpy((*map_shadow)->dbus_name, map_name);
    strcpy((*map_shadow)->wwid, wwid);

    dbus_maps =  g_list_append (dbus_maps,
    		*map_shadow);
error:

    if (map_name != NULL) {
        free(map_name);
        map_name = NULL;
    }
    return rc;
}


static plugin_status_t add_path_slot(struct multipath_shadow *multipath_shadow,
                                    struct path_shadow **path_shadow) {

    plugin_status_t       rc             = PLUGIN_OK;

    *path_shadow = malloc (sizeof(struct path_shadow));

    if (*path_shadow == NULL) {
        rc = PLUGIN_ERROR_MALLOC;
        goto error;
    }
    (*path_shadow)->path_object = NULL;
    (*path_shadow)->dbus_id = next_path();

    if (multipath_shadow->paths == NULL) {
    	multipath_shadow->paths = g_list_alloc();
    }
    multipath_shadow->paths =  g_list_append (multipath_shadow->paths,
    		*path_shadow);

error:

    return rc;
}

static struct multipath_shadow * lookup_map_shadow(struct dmmp_mpath *map) {

    struct multipath_shadow *map_shadow     = NULL;
    struct multipath_shadow *return_shadow  = NULL;
    int i                                   = 0;
    const char 				*wwid 			= NULL;
    GList*  iterator = NULL;

    wwid = dmmp_mpath_wwid_get(map);

    if (wwid == NULL)
    	return NULL;

    for (iterator = dbus_maps; iterator; iterator = iterator->next) {
    	map_shadow = iterator->data;

    	if (map_shadow == NULL)
    		return NULL;

        if (strcmp(map_shadow->wwid, wwid) == 0) {
            return_shadow = map_shadow;
            break;
        }
    }

    return return_shadow;
}

static plugin_status_t update_existing_path_object(struct dmmp_path *path, multipathMapPath *path_object ) {
    plugin_status_t            return_code = PLUGIN_OK;

    multipath_map_path_set_state(path_object, dmmp_path_status_get(path));

    /*
    multipath_map_path_set_offline(path_object, path->offline);
    multipath_map_path_set_bus(path_object, path->bus);
    multipath_map_path_set_dev(path_object, path->dev);
    multipath_map_path_set_vendor_id(path_object, path->vendor_id);
    multipath_map_path_set_product_id(path_object, path->product_id);
    multipath_map_path_set_rev(path_object, path->rev);
    multipath_map_path_set_serial(path_object, path->serial);
    multipath_map_path_set_tgt_node_name(path_object, path->tgt_node_name);
    */

    return return_code;
}


static plugin_status_t create_path_object(GDBusObjectManagerServer *manager,
                                            struct dmmp_path *path,
                                            struct path_shadow *path_shadow) {
    plugin_status_t            return_code = PLUGIN_OK;
    multipathObjectSkeleton     *path_interface_container;

    if (manager == NULL || path == NULL) {
        return_code = PLUGIN_ERROR_NULL;
        goto finished;
    }

    path_interface_container = multipath_object_skeleton_new(path_shadow->dbus_name);

    if (path_interface_container == NULL) {
        return_code = PLUGIN_ERROR_MALLOC;
        goto finished;
    }

    path_shadow->path_object = multipath_map_path_skeleton_new();
    if (path_shadow->path_object == NULL) {
        return_code = PLUGIN_ERROR_MALLOC;
        goto finished;
    }

    multipath_object_skeleton_set_map_path(path_interface_container,
            path_shadow->path_object);

    update_existing_path_object(path, path_shadow->path_object);

    g_dbus_object_manager_server_export(manager,
            G_DBUS_OBJECT_SKELETON(path_interface_container));

finished:

    if (path_interface_container != NULL) {
        g_object_unref(path_interface_container);
        path_interface_container = NULL;
    }

    return return_code;
}


static plugin_status_t create_new_map(GDBusObjectManagerServer *manager, struct dmmp_mpath *map) {
    plugin_status_t return_code                = PLUGIN_OK;
    struct multipath_shadow *map_shadow         = NULL;
    struct path_shadow      *path_shadow        = NULL;
    int                     i                   = 0;
    int     path_count              = 0;
    char*   dbus_name               = NULL;
    struct dmmp_path_group **dmmp_pgs = NULL;
    uint32_t dmmp_pg_count = 0;
    struct dmmp_path **mp_ps = NULL;
     uint32_t mp_p_count = 0;
     uint32_t j = 0;
     const char *blk_name = NULL;


    return_code = add_new_map(map, &map_shadow);

    if (return_code != PLUGIN_OK)
        goto finished;

    return_code = create_map_object(manager,
                         map,
                         map_shadow);

    if (return_code != PLUGIN_OK)
        goto finished;

    dmmp_path_group_array_get(map, &dmmp_pgs, &dmmp_pg_count);

    if (dmmp_pg_count == 0)
            printf("dmmp_path_group_array_get(): Got 0 path group\n");

	for (i = 0; i < dmmp_pg_count; ++i) {
		printf("dmmp_path_group_id_get(): %d \n",
				dmmp_path_group_id_get(dmmp_pgs[i]));
		printf("dmmp_path_group_priority_get(): %d\n",
				dmmp_path_group_priority_get(dmmp_pgs[i]));
		printf("dmmp_path_group_status_get(): %d %s\n",
				dmmp_path_group_status_get(dmmp_pgs[i]),
				dmmp_path_group_status_str(
						dmmp_path_group_status_get(dmmp_pgs[i])));
		printf("dmmp_path_group_selector_get(): %s\n",
				dmmp_path_group_selector_get(dmmp_pgs[i]));

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

			return_code = add_path_slot(map_shadow, &path_shadow);

			if (return_code != PLUGIN_OK)
				goto finished;

			strcpy(path_shadow->dev_t, blk_name);
			strcpy(path_shadow->dev, blk_name);
			dbus_name = make_path_name(j, map_shadow->dbus_name);
			strcpy(path_shadow->dbus_name, dbus_name);
			free(dbus_name);
			dbus_name = NULL;

			return_code = create_path_object(manager, mp_ps[j], path_shadow);

			if (return_code != PLUGIN_OK)
				goto finished;

		}
	}
finished:

    return return_code;
}

static void sync_removed_maps(struct dmmp_mpath **maps) {

    struct  multipath *map                      = NULL;
    struct  multipath_shadow *map_shadow        = NULL;
    struct  path_shadow  *path_shadow           = NULL;
    int i = 0, k                                = 0;

    // TODO implement
}


static plugin_status_t update_maps(GDBusObjectManagerServer *manager,
								struct dmmp_mpath **maps,
								uint32_t mp_count)
{
    plugin_status_t return_code        = PLUGIN_OK;
    int i                               = 0;
    struct multipath_shadow *map_shadow = NULL;


    if (maps == NULL || manager == NULL) {
        return PLUGIN_ERROR_NULL;
    }

    /*
     * Loop through the maps
     */
    for (i = 0; i < mp_count; ++i) {

        map_shadow = lookup_map_shadow(maps[i]);

        if (map_shadow != NULL) {

            return_code = update_existing_map(manager, maps[i], map_shadow);

            if (return_code != PLUGIN_OK)
                goto finished;
        } else {

            return_code = create_new_map(manager, maps[i]);

            if (return_code != PLUGIN_OK)
                goto finished;
        }


    }

    sync_removed_maps(maps);

finished:

    return return_code;
}


int sync_maps()
{
        struct dmmp_context *ctx = NULL;
        struct dmmp_mpath **dmmp_mps = NULL;
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

        if (dmmp_mpath_array_get(ctx, &dmmp_mps, &dmmp_mp_count) != 0)
        	printf("dmmp_mpath_array_get(): rc != 0\n");
        if (dmmp_mp_count == 0)
        	printf("dmmp_mpath_array_get(): "
                     "Got no multipath devices\n");

        for (i = 0; i < dmmp_mp_count; ++i) {
                name = dmmp_mpath_name_get(dmmp_mps[i]);
                wwid = dmmp_mpath_wwid_get(dmmp_mps[i]);
                if ((name == NULL) ||(wwid == NULL))
                	printf("dmmp_mpath_array_get(): Got NULL name or wwid");
                printf("dmmp_mpath_array_get(): Got mpath: %s %s\n", name, wwid);

        }

    	update_maps(object_manager, dmmp_mps, dmmp_mp_count);

        dmmp_mpath_array_free(dmmp_mps, dmmp_mp_count);
out:
        dmmp_context_free(ctx);

}

void quit_multipath_main_loop()
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
	quit_multipath_main_loop();
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

	multipathManager *multipathManager = multipath_manager_skeleton_new();

	g_dbus_interface_skeleton_export(
			G_DBUS_INTERFACE_SKELETON(multipathManager), connection,
			BASE_PATH MANAGER_NAME, &error);

	finished:

	dbus_lock_ready = TRUE;

	sync_maps();
}


/*
 * This is the main loop of the d-bus service.  It won't exit until
 * quit_dbus_main_loop() is called.
 *
 * It is should be invoked as the startup function of a thread or the caller
 * should not expect it to return.
 */
void * multipath_main_loop(void * ap) {
	guint owner_id;

	dbus_maps = g_list_alloc();

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
