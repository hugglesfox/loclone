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
int lo_mount_point(const char *name, char *mnt, size_t n);

#endif  // LO_H
