#ifndef SUBVOL_H
#define SUBVOL_H

#include <errno.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lo.h"

typedef struct {
	char *path;
	int fd;
} subvol_t;

int open_subvol(subvol_t *subvol, const char *path);
int freeze_subvol(subvol_t *subvol);
int thaw_subvol(subvol_t *subvol);
int clone_subvol(subvol_t *subvol, char *dest);
void close_subvol(subvol_t subvol);

#endif  // SUBHOL_H
