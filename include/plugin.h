
#ifndef _PLUGIN_H
#define _PLUGIN_H


#define INITIALZIE 			"initialize"
#define RUN					"run"
#define SHUTDOWN			"shutdown"
#define STATUS				"status"
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
