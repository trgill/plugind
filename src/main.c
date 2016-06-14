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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <glib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>

#include "plugin.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 8888  // TODO change this
static GSList 		*plugin_list = NULL;
static sem_t        exit_sem;

int answer_to_connection (void *cls, struct MHD_Connection *connection, 
                          const char *url, 
                          const char *method, const char *version, 
                          const char *upload_data, 
                          size_t *upload_data_size, void **con_cls)
{

	struct plugin *plugin = NULL;
	GSList* iterator = NULL;
	const char *answer = NULL;
	struct MHD_Response *response;
	int ret;

	if (plugin_list == NULL)
		return PLUGIN_ERROR_NULL;

	for (iterator = plugin_list; iterator; iterator = iterator->next) {
		plugin = iterator->data;


		// TODO need to parse the URL - this is too simple.
		if (strstr(url, plugin->name) != NULL) {
			answer = plugin->answer_request(url);
		}
	}

	if (answer == NULL) {
		answer = "<html><body>response not claimed by any plugin</body></html>";
	}

	response = MHD_create_response_from_buffer(strlen(answer), (void*) answer,
			MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);

	return ret;
}

plugin_status_t unload_shared_library(struct plugin *plugin) {

    dlclose(plugin->handle);

    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_CLOSE;
    }

    return PLUGIN_OK;
}

plugin_status_t load_shared_library(struct plugin *plugin) {


    plugin->handle = dlopen (plugin->library_name, RTLD_LAZY);

    if (!plugin->handle) {
        fputs (plugin->library_name, stderr);
        fputs (dlerror(), stderr);
        exit(1);
    }

    plugin->initialize = dlsym(plugin->handle, INITIALZIE);
    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_NOTFOUND;
    }

    plugin->run = dlsym(plugin->handle, RUN);
    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_NOTFOUND;
    }

    plugin->shutdown = dlsym(plugin->handle, SHUTDOWN);
    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_NOTFOUND;
    }

    plugin->answer_request = dlsym(plugin->handle, ANSWER_REQUEST);
    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_NOTFOUND;
    }

    plugin->status = dlsym(plugin->handle, STATUS);
    if ((plugin->error = dlerror()) != NULL)  {
        fputs(plugin->error, stderr);
        return PLUGIN_ERROR_NOTFOUND;
    }
    return PLUGIN_OK;

}

plugin_status_t start_plugins() {
	pthread_t plugin_thread;

	return PLUGIN_OK;
}

plugin_status_t create_plugin_list(GSList **list) {
	struct plugin *plugin;


	// TODO read from conf file
	plugin = malloc(sizeof(struct plugin));

    if (plugin == NULL)
    	return PLUGIN_ERROR_MALLOC;

	plugin->library_name = "libmpath.so.0.0";
	plugin->name = "multipath";
	*list =  g_slist_append (*list, plugin);


	plugin = malloc(sizeof(struct plugin));

    if (plugin == NULL)
    	return PLUGIN_ERROR_MALLOC;

	plugin->library_name = "liblsm.so.0.0";
	plugin->name = "lsm";
	*list =  g_slist_append (*list, plugin);

	return PLUGIN_OK;
}

plugin_status_t release_plugin_list(GSList **list) {
	return PLUGIN_OK;
}

void *start_plugin(void *ap) {
	struct plugin *plugin = ap;

    if (plugin == NULL)
    	return NULL;

	(*plugin->initialize)("hello");
	(*plugin->run)("hello");
	return NULL;
}

plugin_status_t start_plugin_list(GSList *plugin_list) {
    struct plugin *plugin = NULL;
    GSList*  iterator = NULL;
    int rc;

    pthread_t check_thr;

    if (plugin_list == NULL)
    	return PLUGIN_ERROR_NULL;

    for (iterator = plugin_list; iterator; iterator = iterator->next) {
    	plugin = iterator->data;
    	if (load_shared_library(plugin) != PLUGIN_OK) {
    		printf("Failed to load %s\n", plugin->library_name);
    	} else {

    		if ((rc = pthread_create(&(plugin->plugin_thread),
    				NULL,
					start_plugin,
					plugin))) {
    			printf("Failed to create thread\n");
    		}
    	}
    }

    return PLUGIN_OK;
}

int main(int argc, char **argv) {

    struct plugin plugin;
    sem_init(&exit_sem, 0, 0);

	if (create_plugin_list(&plugin_list) != PLUGIN_OK) {
		printf("Failure to load list\n");
		exit(1);
	}

	if (start_plugin_list(plugin_list) != PLUGIN_OK) {
		printf("Failure to start plugin list\n");
		exit(1);
	}

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, 
                             &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) 
	printf("Failed to start HTTP daemon");

    while(sem_wait(&exit_sem) != 0);

    MHD_stop_daemon (daemon);

    printf("exiting...\n");

}
