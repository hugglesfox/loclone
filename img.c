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

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "img.h"
#include "lo.h"


int img_mount_point(img_t *img, char *mnt_path, size_t n)
{
	int ret = ENOENT;

	struct lo_iter_ctx ctx;
	char lo[PATH_MAX], lo_bf[PATH_MAX];

	lo_iter_init(&ctx);

	while (lo_iter_next(lo, sizeof(lo), &ctx) != -1) {
		if (lo_backing_file(lo, lo_bf, sizeof(lo_bf)) != 0) {
			continue;
		}

		if (strcmp(lo_bf, img->path) == 0) {
			ret = lo_mount_point(lo, mnt_path);
		}
	}

	lo_iter_destroy(&ctx);
	return ret;
}

int img_mount_fd(img_t *img)
{
	int ret, mnt_fd;
	char mnt_path[PATH_MAX];

	if ((ret = img_mount_point(img, mnt_path, sizeof(mnt_path))) != 0) {
		errno = ret;
		return -1;
	}

	if ((mnt_fd = open(mnt_path, O_RDONLY | O_DIRECTORY)) == -1) {
		return -1;
	}

	return mnt_fd;
}

int img_open(img_t *img, const char *path) {
	struct stat sb;
	char *abs_path;

	if (stat(path, &sb) != 0)
		return errno;

	if (!S_ISREG(sb.st_mode))
		return -1;

	if (!(abs_path = realpath(path, NULL)))
		return errno;

	img->path = abs_path;
	img->fd = open(path, O_RDONLY);

	if (img->fd == -1)
		return errno;

	return 0;
}

int img_freeze(img_t *img)
{
	int fd;

	if ((fd = img_mount_fd(img)) == -1) {
		return errno;
	}

	if (ioctl(fd, FIFREEZE, 0) == -1) {
		return errno;
	}

	return 0;
}

int img_thaw(img_t *img)
{
	int fd;

	if ((fd = img_mount_fd(img)) == -1) {
		return errno;
	}

	if (ioctl(fd, FITHAW, 0) == -1) {
		return errno;
	}

	return 0;
}

int img_clone(img_t *img, char *dest)
{
	struct stat sb;
	int dest_fd;

	if (stat(img->path, &sb) != 0)
		return errno;

	dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, sb.st_mode);

	if (dest_fd == -1)
		return errno;

	if (ioctl(dest_fd, FICLONE, img->fd) == -1)
		return errno;

	return 0;
}

void img_close(img_t img)
{
	free(img.path);
	close(img.fd);
}
