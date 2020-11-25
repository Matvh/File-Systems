#include "filesystem.h"


int FILESYSTEM_openFile(char * volume){
    //intentar abrir el volumen. Si no abre damos un Error
    int volume_fd = open(volume, O_RDONLY);
    if (volume_fd < 0){
        printf("\nError. Filesystem %s not found.\n\n", volume);
    }
    return volume_fd;
}


int FILESYSTEM_getFileSystemType(int volume_fd){

    int file_system = FAT32_getFileSystem(volume_fd);
    //Check if its a FAT filesystem
    if (file_system != -1){
        return file_system;
    }
    file_system = EXT4_getFileSystem(volume_fd);
    if (file_system != -1){
        //TODO ver si es ext2/3/4
        return file_system;
    }else{
        return -1; //not recognized
    }
}

#define FILE_NOT_RECOGNIZED "File System not recognized"
//shows the info of the volume
void FILESYSTEM_showInfo(char * volume){
    //char * file_not_recognized = "File System not recognized";
    int file_system_type;
    int volume_fd = FILESYSTEM_openFile(volume);
    //If the volume was succesfully opened
    if (volume_fd > 0) {
        switch (FILESYSTEM_getFileSystemType(volume_fd)) {
            case FAT12:
                printf("%s (FAT12)\n", FILE_NOT_RECOGNIZED);
                break;
            case FAT16:
                printf("%s (FAT16)\n", FILE_NOT_RECOGNIZED);
                break;
            case FAT32:
                FAT32_showInfo(volume_fd);
                break;
            case EXT2:
                printf("%s (EXT2)\n", FILE_NOT_RECOGNIZED);
                break;
            case EXT3:
                printf("%s (EXT3)\n", FILE_NOT_RECOGNIZED);
                break;
            case EXT4:
                EXT4_showInfo(volume_fd);

                break;
            default:
                printf("%s\n",FILE_NOT_RECOGNIZED);
                break;
        }
    }
}

//searches if the file exists in the volume_name
void FILESYSTEM_searchFile(char * volume, char * file_name){
        int volume_fd = FILESYSTEM_openFile(volume);
        int file_system = FILESYSTEM_getFileSystemType(volume_fd);
        unsigned long file_pos;

        switch (file_system) {
            case EXT4:
                EXT4_findFile(volume_fd, file_name, 2);
                if (encontrado == 0) printf("\nFile not found!\n");
                break;
            case FAT32:
                file_pos = FAT32_searchFile(volume_fd, file_name, 1);
                if (file_pos == -1) printf("\nThe file %s was not found in the Volume\n\n", file_name);
                break;
            case EXT2:
                printf("%s (EXT2)\n", FILE_NOT_RECOGNIZED);
                break;
            case EXT3:
                printf("%s (EXT3)\n", FILE_NOT_RECOGNIZED);
                break;
            case FAT12:
                printf("%s (FAT12)\n", FILE_NOT_RECOGNIZED);
                break;
            case FAT16:
                printf("%s (FAT16)\n", FILE_NOT_RECOGNIZED);
                break;
            default:
                printf("%s\n",FILE_NOT_RECOGNIZED);
                break;
        }
}

//shows the content of the file
void FILESYSTEM_showFileContent(char * volume, char * file_name){
    int volume_fd = FILESYSTEM_openFile(volume);
    int file_system = FILESYSTEM_getFileSystemType(volume_fd);

    switch (file_system) {
        case EXT4:
            //TODO hacerlo
            mostrarContenido = 1;
            EXT4_findFile(volume_fd, file_name, 2);
            if (encontrado == 0) {
                printf("\nFile not found!\n");
            }
            break;
        case FAT32:
            FAT32_printFileInfo(volume_fd, file_name, 0);
            break;
        case EXT2:
            printf("%s (EXT2)\n", FILE_NOT_RECOGNIZED);
            break;
        case EXT3:
            printf("%s (EXT3)\n", FILE_NOT_RECOGNIZED);
            break;
        case FAT12:
            printf("%s (FAT12)\n", FILE_NOT_RECOGNIZED);
            break;
        case FAT16:
            printf("%s (FAT16)\n", FILE_NOT_RECOGNIZED);
            break;
        default:
            printf("%s\n",FILE_NOT_RECOGNIZED);
            break;
    }
}
