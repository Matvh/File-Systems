#ifndef FILETYPES_H
#define FILETYPES_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>

#define FAT12_STRING "FAT12   "
#define FAT16_STRING "FAT16   "
#define FAT32_STRING "FAT32   "

#define FAT12 1
#define FAT16 2
#define FAT32 3
#define EXT2 4
#define EXT3 5
#define EXT4 6
#define UNKNOWN -1

#define EXT4_OFFSET 1024

#endif
