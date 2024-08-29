# Hey! I'm Filing Here

In this lab, I successfully implemented the following a 1 MiB ext2 file system with 2 directories (`root` and `lost+found`), 1 regular file (`hello-world`), and 1 symbolic link (`hello`).

## Building

Run `make` to compile `ext2-create.c`, and run`./ext2-create`  to create `cs111 -base.img`.

## Running

Run `dumpe2fs cs111-base.img`, we have the following output:

````
dumpe2fs 1.46.2 (28-Feb -2021)
Filesystem volume name: cs111-base
Last mounted on: <not available >
Filesystem UUID: 5a1eab1e -1337-1337-1337-c0ffeec0ffee
Filesystem magic number: 0xEF53
Filesystem revision #: 0 (original)
Filesystem features: (none)
Default mount options: (none)
Filesystem state: clean
Errors behavior: Continue
Filesystem OS type: Linux
Inode count: 128
Block count: 1024
Reserved block count: 0
Free blocks: 1000
Free inodes: 115
First block: 1
Block size: 1024
Fragment size: 1024
Blocks per group: 8192
Fragments per group: 8192
Inodes per group: 128
Inode blocks per group: 16
Last mount time: n/a
Last write time: Wed Aug 28 01:43:03 2024
Mount count: 0
Maximum mount count: -1
Last checked: Wed Aug 28 01:43:03 2024
Check interval: 1 (0:00:01)
Next check after: Wed Aug 28 01:43:03 2024
Reserved blocks uid: 0 (user root)
Reserved blocks gid: 0 (group root)
Group 0: (Blocks 1-1023)
Primary superblock at 1, Group descriptors at 2-2
Block bitmap at 3 (+2)
Inode bitmap at 4 (+3)
Inode table at 5-20 (+4)
1000 free blocks , 115 free inodes , 2 directories
Free blocks: 24-1023
Free inodes: 14-128
````

Run `fsck.ext2 cs111-base.img`, we have the following output:

````
e2fsck 1.46.4 (18-Aug-2021)
cs111 -base has gone 0 days without being checked, check forced.
Pass 1: Checking inodes , blocks , and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
cs111-base: 13/128 files (0.0% non-contiguous), 24/1024 blocks
````

Run `mkdir mnt` and `sudo mount -o loop cs111-base.img mnt` to  mount the filesystem.

Then run `ls -ain mnt/`, we have the following output:

````
total 7
2   drwxr-xr-x 3    0    0  1024  .
519	drwxr-xr-x 5 1000	1000  4096  ..
13  lrw-r--r-- 1 1000 1000  11    hello -> hello -world
12  -rw-r--r-- 1 1000 1000  12    hello -world
11  drwxr-xr-x 2    0    0  1024  lost+found
````

which is exactly the filesystem.


## Cleaning up

Run `sudo umount mnt`  to unmount the filesystem and `rmdir mnt` to delete the directory used for mounting.

Then `make clean` to remove the executables.
