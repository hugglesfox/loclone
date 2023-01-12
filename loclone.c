#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "img.h"

int main(int argc, char *argv[])
{
	img_t img;
	char *src, *dest;
	int opt = 0, err;

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
		case '?':
			fprintf(stderr, "Usage: %s [src] [dest]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind + 1 >= argc) {
		fprintf(stderr, "Expected arguments\n");
		exit(EXIT_FAILURE);
	}

	src = argv[optind];
	dest = argv[optind + 1];

	if ((err = img_open(&img, src)) != 0) {
		if (err == -1) {
			fprintf(stderr, "%s is not a imgle\n", src);
		} else {
			fprintf(stderr, "Error opening imgume: %s\n", strerror(err));
		}

		exit(EXIT_FAILURE);
	}

	if ((err = img_freeze(&img)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not freezing\n", src);
		} else {
			fprintf(stderr, "Error freezing imgume: %s\n", strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	if ((err = img_clone(&img, dest)) != 0) {
		fprintf(stderr, "Error cloning imgume: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = img_thaw(&img)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not thawing\n", src);
		} else {
			fprintf(stderr, "Error thawing imgume: %s\n", strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	img_close(img);
	exit(EXIT_SUCCESS);
}
