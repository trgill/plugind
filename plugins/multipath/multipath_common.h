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

#ifndef MULTIPATH_COMMON_H
#define MULTIPATH_COMMON_H

#include <glib.h>
#include "multipath_dbus_generated.h"


#define VERSION "1"
#define MANAGER_NAME "/Manager"
#define BASE_PATH "/com/redhat/multipath" VERSION
#define BASE_INTERFACE "com.redhat.multipath" VERSION


void * multipath_main_loop(void * ap);
void quit_multipath_main_loop();

#define MAP_TYPE "map"
#define PATH_TYPE "path"

struct multipath_shadow {
    multipathMap        *map_object;
    unsigned long       dbus_id;
    char                dbus_name[256];
    char                wwid[256];
    GList               *paths;
};


struct path_shadow {
    multipathMapPath    *path_object;
    unsigned long       dbus_id;
    char                dbus_name[256];
    char                dev[256];
    char                dev_t[256];
};

#endif /* MULTIPATH_COMMON_H */

