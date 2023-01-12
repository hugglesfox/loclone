#ifndef IMG_H
#define IMG_H

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
