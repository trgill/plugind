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

#include "block_common.h"

int initialize() {
	printf("initialize.\n");
	return 0;
}

int run() {
	block_main_loop(NULL);
	return 0;
}

int shutdown() {
	printf("shutdown.\n");
	return 0;
}

const char * answer_request(char *url) {
	return "<html><body>response from block plugin</body></html>";
}

int status() {
	printf("status.\n");
	return 0;
}
