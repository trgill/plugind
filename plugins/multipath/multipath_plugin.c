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
