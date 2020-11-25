#ifndef FAT32_H
#define FAT32_H

#include "filetypes.h"

#define SECTOR_SIZE_OFFSET 11
#define SECTORS_PER_CLUSTER_OFFSET 13
#define RESERVED_SECTORS_OFFSET 14
#define NUMBER_OF_FAT_COPIES_OFFSET 16
#define MAX_ROOT_ENTRIES_OFFSET 17
#define SECTORS_PER_FAT_OFFSET_1 22
#define SECTORS_PER_FAT_OFFSET_2 36
#define ROOT_DIR_OFFSET 44

#define CLUSTER2 2
#define ERASED_FILE 1
#define SHORT_NAME_FILE 2
#define LONG_NAME_FILE 3
#define DIRECTORY 4
#define DIRECTORY_ENTRY_SIZE 32

#define MAX_LONG_NAME 255

typedef struct {
    unsigned long clusters_data_initial_pos;
    unsigned long cluster_size;
}FATInfo;

typedef struct {
    char * name;
    char * checksum;
}LongName;

//see if the system is FAT12, FAT16, FAT32 or UNKNOWN
int FAT32_getFileSystem(int volume_fd);

//prints the information of the volume
void FAT32_showInfo(int volume_fd);

//find the file in the volume
//returns -1 if the file was not found or
//the file's position if it was found
unsigned long FAT32_searchFile(int volume_fd, char * filename, int print_info);

//Prints the information inside the file if the file exists
void FAT32_printFileInfo(int volume_fd, char * file_name, int print_info);

//turns the number read from little to big endian
//arr -> bytes as read from the file
//size -> number of bytes read
unsigned int FAT32_littleToBigEndian(char * arr, int size);

#endif
