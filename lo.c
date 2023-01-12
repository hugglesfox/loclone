#include <errno.h>
#include <linux/limits.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lo.h"

void lo_iter_init(struct lo_iter_ctx *ctx)
{
	ctx->sys_block = opendir(_SYS_BLOCK_PATH);

	if (!ctx->sys_block) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}
}

int lo_iter_next(char *name, size_t n, struct lo_iter_ctx *ctx)
{
	struct dirent *d;

	while ((d = readdir(ctx->sys_block))) {
		if (!d)
			return errno;

		if (strlen(d->d_name) < 5)
			continue;

		if (strncmp(d->d_name, "loop", 4) != 0)
			continue;

		strncpy(name, d->d_name, n - 1);
		name[n] = '\0';

		return 0;
	}

	return -1;
}

void lo_iter_destroy(struct lo_iter_ctx *ctx)
{
	closedir(ctx->sys_block);
}

int lo_backing_file(const char *restrict name, char *restrict path, size_t n)
{
	int err;
	char b[PATH_MAX];
	FILE *fp;

	snprintf(b, sizeof(b), "%s/%s/loop/backing_file", _SYS_BLOCK_PATH, name);

	if (!(fp = fopen(b, "r")))
		return errno;

	int len = fread(path, 1, n - 1, fp);

	if ((err = ferror(fp)) != 0) {
		fclose(fp);
		return err;
	}

	path[len - 1] = '\0';  // Replace the new line char with null
	fclose(fp);

	return 0;
}

int lo_mount_point(const char *restrict name, char *restrict path, size_t n)
{
	int ret = ENOENT;

	FILE *mounts;
	struct mntent *mount;
	char dev[PATH_MAX];
	
	snprintf(dev, sizeof(dev), "/dev/%s", name);
	mounts = setmntent(_MTAB_PATH, "r");

	if (!mounts)
		return errno;

	while ((mount = getmntent(mounts)) != NULL) {
		if (strcmp(dev, mount->mnt_fsname) == 0) {		
			strcpy(path, mount->mnt_dir);
			ret = 0;
			break;
		}
	}

	endmntent(mounts);
	return ret;
}

