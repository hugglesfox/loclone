#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "subvol.h"

int main(int argc, char *argv[])
{
	subvol_t subvol;
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

	if ((err = open_subvol(&subvol, src)) != 0) {
		if (err == -1) {
			fprintf(stderr, "%s is not a file\n", src);
		} else {
			fprintf(stderr, "Error opening subvolume: %s\n", strerror(err));
		}

		exit(EXIT_FAILURE);
	}

	if ((err = freeze_subvol(&subvol)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not freezing\n", src);
		} else {
			fprintf(stderr, "Error freezing subvolume: %s\n", strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	if ((err = clone_subvol(&subvol, dest)) != 0) {
		fprintf(stderr, "Error cloning subvolume: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = thaw_subvol(&subvol)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not thawing\n", src);
		} else {
			fprintf(stderr, "Error thawing subvolume: %s\n", strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
