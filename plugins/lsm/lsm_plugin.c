#include <stdio.h>

#include "lsm_common.h"

int initialize() {
	printf("initialize.\n");
	return 0;
}

int run() {
	lsm_main_loop(NULL);

	return 0;
}

int shutdown() {
	printf("shutdown.\n");
	return 0;
}

const char *answer_request(char *url) {
	const char *page  = "<html><body>response from LSM plugin</body></html>";
	return page;
}

int status() {
	printf("status.\n");
	return 0;
}
