
/*
 * multipath_common.h
 *
 *      Author: Todd Gill
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

