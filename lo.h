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

#ifndef LO_H
#define LO_H

#include <dirent.h>
#include <stddef.h>

#define _SYS_BLOCK_PATH "/sys/block"
#define _MTAB_PATH "/proc/mounts"

struct lo_iter_ctx {
	DIR *sys_block;
};

/*
 * Gets the name (e.g loop0) of the next available loopback device
 * Returns 0 on success and -1 if the end of the loopback stream has been reached
 */
int lo_iter_next(char *name, size_t n, struct lo_iter_ctx *ctx);
void lo_iter_init(struct lo_iter_ctx *ctx);
void lo_iter_destroy(struct lo_iter_ctx *ctx);

/*
 * Gets the path of the backing file for the loopback device of the given name.
 * Returns 0 on success and ENOENT if the device doesn't have a backing file
 */
int lo_backing_file(const char *name, char *path, size_t n);

/*
 * Gets the mount point of the loopback device of the given name.
 * Returns 0 on success and ENOENT if the device doesn't have a backing file
 */
int lo_mount_point(const char *name, char *mnt);

#endif  // LO_H
