#include <stdio.h>
#include <mpath_cmd.h>
#include "multipath_common.h"

int initialize() {
	printf("initialize.\n");
	return 0;
}

int run() {
	multipath_main_loop(NULL);
	return 0;
}

int shutdown() {
	printf("shutdown.\n");
	return 0;
}

const char * answer_request(char *url) {
	const char *page;
    int socket_fd = -1;

    socket_fd = mpath_connect();
    if (socket_fd == -1) {
    		printf("Failed to connect\n");
            goto out;
    }

    if (mpath_process_cmd(socket_fd, "show maps json",
                          (char **)&page, 30000) != 0) {

    		printf("Failed to execute command\n");
            mpath_disconnect(socket_fd);
            // TODO deal with failure
            goto out;
    }
    mpath_disconnect(socket_fd);

out:

	return page;
}

int status() {
	printf("status.\n");
	return 0;
}
