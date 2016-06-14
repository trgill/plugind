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

#ifndef _PLUGIN_H
#define _PLUGIN_H


#define INITIALZIE 		"initialize"
#define RUN			"run"
#define SHUTDOWN		"shutdown"
#define STATUS			"status"
#define ANSWER_REQUEST		"answer_request"


typedef enum
{
	PLUGIN_OK = 0,
	PLUGIN_ERROR = 100,
	PLUGIN_ERROR_NULL = 101,
	PLUGIN_ERROR_MALLOC = 102,
	PLUGIN_ERROR_NOTFOUND = 103,
	PLUGIN_ERROR_CLOSE = 104,
} plugin_status_t;

struct plugin {
    void *handle;
    char *name;
    char *library_name;
    char *error;
    pthread_t plugin_thread;
    int (*initialize)();
    int (*run)();
    int (*shutdown)();
    int (*status)();
    const char * (*answer_request)(const char *url);
};

#endif
