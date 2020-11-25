#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ext4.h"
#include "fat32.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

//Opens the file
//returns -1 if the file could not be found
//returns > 2 if the file was opened correctly.
int FILESYSTEM_openFile(char * volume);

/*Returns the file format of the volume
fat12
fat16
fat32
ext2
ext3
ext4
not recognized
*/
int FILESYSTEM_getFileSystemType(int volume_fd);

//shows the info of the volume
void FILESYSTEM_showInfo(char * volume);

//searches if the file exists in the volume_name
void FILESYSTEM_searchFile(char * volume, char * file_name);

//shows the content of the file
void FILESYSTEM_showFileContent(char * volume, char * file_name);


//TODO Comandas opcional

#endif
