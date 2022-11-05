#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L

#include "subvol.h"

int subvol_mount_point(subvol_t *subvol, char *mnt_path, size_t n)
{
	int ret = ENOENT;

	struct lo_iter_ctx ctx;
	char lo[PATH_MAX], lo_bf[PATH_MAX];

	lo_iter_init(&ctx);

	while (lo_iter_next(lo, sizeof(lo), &ctx) != -1) {
		if (lo_backing_file(lo, lo_bf, sizeof(lo_bf)) != 0) {		
			continue;
		}

		if (strcmp(lo_bf, subvol->path) == 0) {
			ret = lo_mount_point(lo, mnt_path, n);
		}
	}

	lo_iter_destroy(&ctx);
	return ret;
}

int open_subvol(subvol_t *subvol, const char *path) {
	struct stat sb;
	char *abs_path;

	if (stat(path, &sb) != 0)
		return errno;
	
	if (!S_ISREG(sb.st_mode))
		return -1;

	if (!(abs_path = realpath(path, NULL)))
		return errno;

	subvol->path = abs_path;
	subvol->fd = open(path, O_RDONLY);

	if (subvol->fd == -1)
		return errno;

	return 0;
}


int subvol_mount_fd(subvol_t *subvol)
{
	int ret, mnt_fd;
	char mnt_path[PATH_MAX];

	if ((ret = subvol_mount_point(subvol, mnt_path, sizeof(mnt_path))) != 0) {
		errno = ret;
		return -1;
	}

	if ((mnt_fd = open(mnt_path, O_RDONLY | O_DIRECTORY)) == -1) {
		return -1;
	}
	
	return mnt_fd;
}

int freeze_subvol(subvol_t *subvol)
{
	int fd;

	if ((fd = subvol_mount_fd(subvol)) == -1) {
		return errno;
	}

	if (ioctl(fd, FIFREEZE, 0) == -1) {
		return errno;
	}

	return 0;
}

int thaw_subvol(subvol_t *subvol)
{
	int fd;

	if ((fd = subvol_mount_fd(subvol)) == -1) {
		return errno;
	}

	if (ioctl(fd, FITHAW, 0) == -1) {
		return errno;
	}

	return 0;
}

int clone_subvol(subvol_t *subvol, char *dest)
{
	struct stat sb;
	int dest_fd;

	if (stat(subvol->path, &sb) != 0)
		return errno;

	dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, sb.st_mode);

	if (dest_fd == -1)
		return errno;

	if (ioctl(dest_fd, FICLONE, subvol->fd) == -1)
		return errno;

	return 0;
}

void close_subvol(subvol_t subvol)
{
	free(subvol.path);
	close(subvol.fd);
}
