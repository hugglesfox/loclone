/*  Copyright (C) 2023 Hayley Hughes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
			fprintf(stderr, "%s is not a valid image\n", src);
		} else {
			fprintf(stderr, "Error opening %s: %s\n", src, strerror(err));
		}

		exit(EXIT_FAILURE);
	}

	if ((err = img_freeze(&img)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not freezing\n", src);
		} else {
			fprintf(stderr, "Error freezing %s: %s\n", src, strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	if ((err = img_clone(&img, dest)) != 0) {
		fprintf(stderr, "Error cloning %s: %s\n", src, strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = img_thaw(&img)) != 0) {
		if (err == ENOENT) {
			printf("%s is not mounted, not thawing\n", src);
		} else {
			fprintf(stderr, "Error thawing %s: %s\n", src, strerror(err));
			exit(EXIT_FAILURE);
		}
	}

	img_close(img);
	exit(EXIT_SUCCESS);
}
