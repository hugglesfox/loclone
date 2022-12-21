#ifndef IMG_H
#define IMG_H

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
} img_t;

int img_open(img_t *img, const char *path);
int img_freeze(img_t *img);
int img_thaw(img_t *img);
int img_clone(img_t *img, char *dest);
void img_close(img_t img);

#endif  // IMG_H
