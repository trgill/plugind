
/*
 * lsm_common.h
 *
 *      Author: Todd Gill
 */

#ifndef LSM_COMMON
#define LSM_COMMON

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif


#define VERSION "1"
#define MANAGER_NAME "/Manager"
#define LSM_PATH_BASE_NAME "/org/storaged/Storaged/Lsm" VERSION
#define LSM_INTERFACE_NAME "org.storaged.Storaged.Lsm" VERSION


/* Volume Property Definitions */
#define VOLUME_ID 			"VolumeId"
#define VOLUME_NAME 		"VolumeName"
#define BLOCK_SIZE 			"BlockSize"
#define NUMBER_OF_BLOCKS 	"NumberOfBlocks"
#define SYSTEM_ID 			"SystemId"
#define POOL_ID 			"PoolId"

/* Pool Property Definitions */

#define POOL_ID 			"PoolId"
#define POOL_NAME 			"PoolName"
#define TOTAL_SPACE 		"TotalSpace"
#define FREE_SPACE 			"FreeSpace"
#define POOL_STATUS 		"PoolStatus"
#define POOL_STATUS_INFO 	"PoolStatusInfo"
#define STATUS 				"Status"
#define STATUS_INFO			"StatusInfo"
#define SYSTEM_ID 			"SystemId"
#define ELEMENT_TYPE 		"ElementType"

void * lsm_main_loop(void * ap);
void quit_lsm_main_loop();

#endif /* LSM_COMMON */

