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
