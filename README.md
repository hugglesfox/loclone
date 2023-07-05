# loclone
A tool to clone a disk image.

Main features

- Utilises FICLONE to create a copy-on-write clone of the image

- Uses FIFREEZE if the image is mounted in order to ensure consistency of the
  image

For further details see **loclone**(1).

## Building

**loclone** doesn't have any dependencies outside of the linux standard libraries.

```bash
$ make

# Optionally to install to /usr/local
$ sudo make install
```

## Notes 

The original concept was to create a tool to make use of XFS' data CoW features
in order to create snapshottable subvolumes which are backed by loopback
devices on XFS; Similar to one of the thought experiments laid out by Dave
Chinner in their talk, "XFS: Teaching an Old Dog Old Tricks" at LinuxConfAU 2018.
(found here https://www.youtube.com/watch?v=wG8FUvSGROw)

However as this concept can be applied to any filesystem that supports FICLONE,
I've made the tool more generic. However at the moment this tool only really
makes sense to be used on XFS.

The general architecture of the system is as follows:

- Sparse files are created on which a filesystem which supports FIFREEZE (xfs,
  ext4, btrfs, etc.) can be created in

- The files can then be mounted via a loopback device (generally configured in
  `/etc/fstab`) to which ever needs to be snapshotted (e.g. `/home`)

- CoW copies of the files can then be made using the FICLONE ioctl call;
  Effectively creating a snapshot of the filesystem at a point in time.

- However if the sparse file is still mounted and being written to during when
  the copy is made, it can cause data corruption. Hence ideally the filesystem
  would be unmounted but that's not feasible for some mountpoints (such as
  `/home`). Therefore the filesystem is frozen using FIFREEZE for the duration
  of the copy.

The latter two points are what this tool encompasses.

## Contributing

Contributions (technical or not) are greatly appreciated. If you find any
areas for improvement, please feel free to open a pull request.

All contributions are subject to the Gnu Public Licence v3, the full text for
which can be found in `LICENCE`.
