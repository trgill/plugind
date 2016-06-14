
/*
 * lsm_common.h
 *
 *      Author: Todd Gill
 */

#ifndef LSM_COMMON_H
#define LSM_COMMON_H

#include <glib.h>
#include "lsm_dbus_generated.h"


#define VERSION "1"
#define MANAGER_NAME "/Manager"
#define BASE_PATH "org/storaged/Storaged/Drive" VERSION
#define BASE_INTERFACE "org.storaged.Storaged.Drive" VERSION


void * lsm_main_loop(void * ap);
void quit_lsm_main_loop();

#endif /* LSM_COMMON_H */

