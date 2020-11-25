#include "fat32.h"


int FAT32_getFileSystem(int volume_fd){
    //54 -> num de bytes a saltar fat12/16
    //82 -> num bytes a saltar en fat32
    //SEEK_SET -> start at the beggining
    char volume_type[9];
    if (lseek(volume_fd, 54, SEEK_SET) >= 0) {
        read(volume_fd, volume_type, 8);
        volume_type[8] = '\0';

        if (strcmp(volume_type, FAT12_STRING) == 0) return FAT12;
        if (strcmp(volume_type, FAT16_STRING) == 0) return FAT16;

        if (lseek(volume_fd, 82, SEEK_SET) > 0) {
            read(volume_fd, volume_type, 8);
            if (strcmp(volume_type, FAT32_STRING) == 0) return FAT32;
        }
    }
    return UNKNOWN;
}


//turns the number read from little to big endian
//arr -> bytes as read from the file
//size -> number of bytes read
unsigned int FAT32_littleToBigEndian(char * arr, int size){
    int i;
    unsigned int res = arr[size-1];
    for(i = size-1; i > 0; i--){
        res <<= 8;
        res += arr[i-1];
    }
    return res;
}

unsigned int FAT32_get1ByteData(int volume_fd, int offset){
    unsigned char res;
    if (lseek(volume_fd, offset, SEEK_SET) >= 0){
        read(volume_fd, &res, 1);
        return (unsigned int)res;
    }
    return -1;
}

unsigned int FAT32_get2ByteData(int volume_fd, int offset){
    char basura[2];
    if (lseek(volume_fd, offset, SEEK_SET) >= 0){
        read(volume_fd, basura, 2);
        return FAT32_littleToBigEndian(basura, 2);
    }
    return -1;
}


unsigned int FAT32_getSectorsPerFat(int volume_fd){
    //Leer del primero y si es 0 leer del segundo
    char basura[4];
    if (lseek(volume_fd, SECTORS_PER_FAT_OFFSET_1, SEEK_SET) >= 0){
        read(volume_fd, basura, 2);
        unsigned int res = FAT32_littleToBigEndian(basura, 2);
        if (res == 0){ //si es > 0 hay que buscarlo despues
            if (lseek(volume_fd, SECTORS_PER_FAT_OFFSET_2, SEEK_SET) >= 0){
                read(volume_fd, basura, 4);
                return FAT32_littleToBigEndian(basura, 4);
            }
        }else return res;
    }
    return -1;
}

unsigned int FAT32_getRootDirectoryFirstCluster(int volume_fd){

    char basura[4];

    if (lseek(volume_fd, ROOT_DIR_OFFSET, SEEK_SET) >= 0){
        read(volume_fd, basura, 4);
        return FAT32_littleToBigEndian(basura, 4);
    }
    return -1;
}


char * FAT32_getVolumenLabel(int volume_fd){
    char * label;
    label = (char * ) malloc(sizeof(char) * 12);
    if (label != NULL){
        label[11] = '\0';
        if (lseek(volume_fd, 71, SEEK_SET) >= 0){
            read(volume_fd, label, 11);
            return label;
        }
        return NULL;
    }
    return NULL;
}

void FAT32_showInfo(int volume_fd){

    char system_name[9];
    system_name[8] = '\0';
    char basura[30];
    //Move back to sector size (11)
    if (lseek(volume_fd, 3, SEEK_SET) >= 0){

        //Read System Name
        read(volume_fd, system_name, 8);

        //READ sector size
        unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);

        //READ number of sectors per Cluster
        unsigned int sectors_per_cluster =  FAT32_get1ByteData(volume_fd, SECTORS_PER_CLUSTER_OFFSET);

        //READ Reserved Sectors
        unsigned int reserved_sectors = FAT32_get2ByteData(volume_fd, RESERVED_SECTORS_OFFSET);

        //Read Number of Fat copies
        int number_of_fats = FAT32_get1ByteData(volume_fd, NUMBER_OF_FAT_COPIES_OFFSET);

        //READ Max Root Entries which is worth 0 in FAT32
        unsigned int max_root_entries = FAT32_get2ByteData(volume_fd, MAX_ROOT_ENTRIES_OFFSET);

        //READ sectors per FAT
        unsigned int sectors_per_fat = FAT32_getSectorsPerFat(volume_fd);

        //Read the Volume's label
        char * label = FAT32_getVolumenLabel(volume_fd);

        printf("\n---- Filesystem Information ----\n\n");
        printf("Filesystem: FAT32\n\n");
        printf("System Name: %s\n", system_name);
        printf("Sector Size: %d\n", sector_size);
        printf("Sectors per Cluster: %d\n", sectors_per_cluster);
        printf("Reserved Sectors: %d\n", reserved_sectors);
        printf("Number of FATs: %d\n", number_of_fats);
        printf("Maximum Root Entries: %d\n", max_root_entries);
        printf("Sectors per FAT: %d\n", sectors_per_fat);
        if (label != NULL) printf("Label: %s\n", label);
        else printf("There was an error reading the Volume's label\n");
        printf("\n\n");

    }else{
        printf("Error lseek\n");
    }
}



void FAT32_toLowerCase(char * text){
    int text_size = strlen(text);
    int i;
    for (i = 0; i < text_size; i++){
        if ( text[i] >= 'A' && text[i] <= 'Z'){
            text[i] = text[i] - 'A' +'a';
        }
    }
}

int FAT32_compareShortName(unsigned char * dir_record, char * file_name){
    char name[13];
    int i, i_name = 0;
    for (i = 0; i < 11; i++){
        if (i == 8){
            name[i_name] = '.';
            i_name++;
            name[i_name] = dir_record[i];
            i_name++;
        }else if(dir_record[i] != ' '){
            name[i_name] = dir_record[i];
            i_name++;
        }
    }
    name[i_name] = '\0';
    //Convertir a minusculas
    FAT32_toLowerCase(name);
    FAT32_toLowerCase(file_name);
    if (strcmp(name, file_name) == 0) return 1;
    else return 0;
}


/*
Returns the next clsuter's position
*/
void FAT32_printNextCluster(char a, char b){
    char next_cluster_char[2];
    next_cluster_char[0] = a;
    next_cluster_char[1] = b;
    unsigned int next_cluster = FAT32_littleToBigEndian(next_cluster_char, 2);
    printf("Next cluster %u\n",next_cluster);
}

unsigned int FAT32_getNextCluster(char a, char b){
    char next_cluster_char[2];
    next_cluster_char[0] = a;
    next_cluster_char[1] = b;
    return (unsigned int) FAT32_littleToBigEndian(next_cluster_char, 2);
}

unsigned long FAT32_getNextClusterOffset(char a, char b, FATInfo fat_info){
    unsigned int next_cluster = FAT32_getNextCluster(a, b);
    return fat_info.clusters_data_initial_pos + fat_info.cluster_size * (next_cluster - CLUSTER2);
}

unsigned long FAT32_getClusterSize(int volume_fd){
    unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);
    unsigned int sectors_per_cluster =  FAT32_get1ByteData(volume_fd, SECTORS_PER_CLUSTER_OFFSET);
    return sector_size * sectors_per_cluster;
}

unsigned long FAT32_getClustersDataInitialPos(int volume_fd){
    unsigned int reserved_sectors = FAT32_get2ByteData(volume_fd, RESERVED_SECTORS_OFFSET);
    unsigned int sectors_per_fat = FAT32_getSectorsPerFat(volume_fd);
    unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);
    int number_of_fats = FAT32_get1ByteData(volume_fd, NUMBER_OF_FAT_COPIES_OFFSET);
    unsigned long clusters_data_initial_pos = reserved_sectors + (sectors_per_fat * number_of_fats);
    return clusters_data_initial_pos * sector_size;
}

unsigned long FAT32_getFatTableInitialOffset(int volume_fd){
    unsigned int reserved_sectors = FAT32_get2ByteData(volume_fd, RESERVED_SECTORS_OFFSET);
    unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);
    return reserved_sectors * sector_size;
}

unsigned int FAT32_getNextClusterFromFAT(int volume_fd, unsigned int fat_offset){
    unsigned long fat_table_pos = FAT32_getFatTableInitialOffset(volume_fd);
    unsigned int sectors_per_fat = FAT32_getSectorsPerFat(volume_fd);
    unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);
    unsigned long total_fat_table_entries = (sectors_per_fat * sector_size);
    unsigned int next_cluster;
    lseek(volume_fd,(fat_table_pos + (4 * fat_offset)) , SEEK_SET);
    read(volume_fd, &next_cluster, sizeof(unsigned int));
    return next_cluster;
}


char * FAT32_printLongName(unsigned char * dir_record){

    char extracted_name[27];
    int i_name = 0;
    int i;
    char * name;

    for (i = 1; i < 11; i++){
        if (dir_record[i] >= 32 && dir_record[i] <= 126){
            extracted_name[i_name] = dir_record[i];
            i_name ++;
        }
    }
    for (i = 14; i < 26; i++){
        if (dir_record[i] >= 32 && dir_record[i] <= 126){
            extracted_name[i_name] = dir_record[i];
            i_name ++;
        }
    }
    for (i = 28; i < 32; i++){
        if (dir_record[i] >= 32 && dir_record[i] <= 126){
            extracted_name[i_name] = dir_record[i];
            i_name ++;
        }
    }
    extracted_name[i_name] = '\0';
    name = (char*) malloc(sizeof(char) * strlen(extracted_name));
    strcpy(name, extracted_name);
    return name;
}

void FAT32_freeString(char ** string, unsigned char * checksum){
    if(*string != NULL){
        free(*string);
        *string = NULL;
        *checksum = 0;
    }
}
void FAT32_printFileSizeAndDate(unsigned char * dir_record){
    char size[4] = {dir_record[28], dir_record[29], dir_record[30], dir_record[31]};
    char fecha_char[2] = {dir_record[16], dir_record[17]};
    unsigned int fecha = FAT32_littleToBigEndian(fecha_char, 2);
    int yyyy = fecha & 0xFE00;
    yyyy >>= 9;
    yyyy &= 0x7F;
    int mm = fecha & 0xFE;
    mm >>= 4;
    mm &= 0xF;
    int dd = fecha & 0x1F;
    printf("File Found!\n");
    printf("Size: bytes %u\n", FAT32_littleToBigEndian(size, 4));
    printf("Created on: %d/%d/%d\n", dd, mm, 1980 + yyyy);
    printf("\n");
}

/*
Skips directories . and ..
*/
int FAT32_isValidDirectory(unsigned char * dir_record){
    char size[4] = {dir_record[28], dir_record[29], dir_record[30], dir_record[31]};
    if (dir_record[0] == 0x2E) return 0;
    if (FAT32_littleToBigEndian(size, 4) == 0) return 1;
    return 0;
}

void printFATTable(int volume_fd){

    unsigned long fat_table_pos = FAT32_getFatTableInitialOffset(volume_fd);
    unsigned int sectors_per_fat = FAT32_getSectorsPerFat(volume_fd);
    unsigned int sector_size = FAT32_get2ByteData(volume_fd, SECTOR_SIZE_OFFSET);
    unsigned long total_fat_table_size = (sectors_per_fat * sector_size);
    unsigned int next_cluster;
    lseek(volume_fd,fat_table_pos, SEEK_SET);
    unsigned int size = total_fat_table_size / 32;
    int i;
    for (i = 0; i < size; i++){
        read(volume_fd, &next_cluster, sizeof(unsigned int));
        printf("%u-",next_cluster);
    }
    printf("\n");
}

/**
    checks if the file is: ERASED_FILE, SHORT_NAME_FILE
                          LONG_NAME_FILE,  DIRECTORY
**/
int FAT32_getRecordType(unsigned char * dir_record){
    //if (dir_record[0] == 0x00) return 0;
    //else if(dir_record[0] == 0x2E) return 0;
    if (dir_record[0] == 0xE5) return ERASED_FILE;
    else if (dir_record[11] & 32) return SHORT_NAME_FILE;
    else if (dir_record[11] & 0x10) return DIRECTORY;
    else if ( (dir_record[11] & 0x0F) && (dir_record[12] == 0) && (dir_record[26] == 0 ) && (dir_record[27] == 0)) return LONG_NAME_FILE;
    else return 0;
}

void printname(unsigned char * dir_record){
    int i;
    for (i = 0; i < 11; i++){
        printf("%c", dir_record[i]);
    }
    printf("\n");
}

//TODO ver que otro datos hay que pasarle a la funcion
unsigned long FAT32_DirDiscovery(int volume_fd, unsigned int current_cluster, FATInfo fat_info, char * file_name, int print_info){
    unsigned char dir_record[DIRECTORY_ENTRY_SIZE];
    char * short_name;
    char file_size_char[4];
    unsigned long next_pos = 0;
    unsigned long pos_actual = 0;
    unsigned long next_cluster = 0;
    LongName long_name;
    long_name.checksum = 0;
    long_name.name = NULL;
    char * name_buffer = NULL;
    unsigned char checksum = 0;

    int i;
    unsigned long current_pos = fat_info.clusters_data_initial_pos + fat_info.cluster_size * (current_cluster - CLUSTER2);

    if (lseek(volume_fd, current_pos, SEEK_SET) >= 0){
        do{
            read(volume_fd, dir_record, DIRECTORY_ENTRY_SIZE); //read the dir_record
            pos_actual += DIRECTORY_ENTRY_SIZE;
            if(dir_record[0]=='u'){
                printname(dir_record);
            }
            switch (FAT32_getRecordType(dir_record)) {

                case SHORT_NAME_FILE:
                    if (FAT32_compareShortName(dir_record, file_name)){
                        if(print_info) FAT32_printFileSizeAndDate(dir_record);
                        return FAT32_getNextCluster(dir_record[26], dir_record[27]);
                    }
                    FAT32_freeString(&name_buffer, &checksum);
                    break;
                case LONG_NAME_FILE:

                    long_name.name = FAT32_printLongName(dir_record);
                    //printf("%s  checksum %X y checksum viejo %X \n",long_name.name, dir_record[13], checksum);

                    if (name_buffer == NULL){ //String nueva
                        name_buffer = long_name.name;
                        //printf("%s\n",long_name.name);
                        checksum = dir_record[13];
                    }else if(checksum == dir_record[13]){ //Continuacion
                        long_name.name = realloc(long_name.name, strlen(long_name.name) + strlen(name_buffer) + 1);
                        // printf("mismo checksum-----\n");
                         //printf("%s\n", long_name.name);
                        // printf("%s\n", name_buffer);
                        strcat(long_name.name, name_buffer);
                        free(name_buffer);
                        name_buffer = NULL;
                        name_buffer = long_name.name;
                        //printf("name_buffer %s\n", name_buffer);
                    }else{ //No es Continuacion hay que empezar de 0
                        free(name_buffer);
                        name_buffer = NULL;
                        name_buffer = long_name.name;
                        checksum = dir_record[13];
                        //printf("name_NO es igual %s\n", name_buffer);
                    }
                    FAT32_toLowerCase(name_buffer);
                    FAT32_toLowerCase(file_name);
                    if(strcmp(name_buffer, file_name) == 0){
                        read(volume_fd, dir_record, DIRECTORY_ENTRY_SIZE);
                        if(print_info) FAT32_printFileSizeAndDate(dir_record);
                        return FAT32_getNextCluster(dir_record[26], dir_record[27]);
                    }
                    break;
                case DIRECTORY:
                    FAT32_freeString(&name_buffer, &checksum);
                    if(FAT32_isValidDirectory(dir_record)){
                        next_pos = FAT32_getNextCluster(dir_record[26], dir_record[27]);
                        next_cluster = FAT32_DirDiscovery(volume_fd, next_pos, fat_info, file_name, print_info);
                        lseek(volume_fd, (current_pos+pos_actual), SEEK_SET);
                        if(next_cluster != -1) return next_cluster;
                    }
                    break;
                default:
                    FAT32_freeString(&name_buffer, &checksum);
                    break;
            }
        }while( (pos_actual < fat_info.cluster_size) && (dir_record[0] != 0x0) );

        unsigned int next_fat_cluster = FAT32_getNextClusterFromFAT(volume_fd, current_cluster);
        if ((next_fat_cluster < 0xFFFFFF8) && (next_cluster != 0x0)){
            next_cluster = FAT32_DirDiscovery(volume_fd, next_fat_cluster, fat_info, file_name, print_info);
            lseek(volume_fd, (current_pos+pos_actual), SEEK_SET);
            if (next_cluster != -1) return next_cluster;
        }
    }else{
        printf("lseek failed\n");
        return -1;
    }
    return -1;
}

/*
returns -1 if the file was not found or
the file's position if it was found
*/
unsigned long FAT32_searchFile(int volume_fd, char * file_name, int print_info) {
    //Get the FATInfo data that is necesary
    FATInfo fat_info;
    fat_info.clusters_data_initial_pos = FAT32_getClustersDataInitialPos(volume_fd);
    fat_info.cluster_size = FAT32_getClusterSize(volume_fd);
    //Root Dir located and offset
    unsigned long root_dir = FAT32_getRootDirectoryFirstCluster(volume_fd);

    //Call the function to get the file's cluster
    unsigned long next_cluster = FAT32_DirDiscovery(volume_fd, root_dir,fat_info, file_name, print_info);
    if (next_cluster != -1) return next_cluster;
    else return -1;
}


void FAT32_printFile(int volume_fd, FATInfo fat_info, unsigned long file_cluster){
    unsigned long file_pos = (file_cluster - CLUSTER2) * fat_info.cluster_size + fat_info.clusters_data_initial_pos;
    unsigned long cluster_read = 0;
    char c;
    if (lseek(volume_fd, file_pos, SEEK_SET) >= 0){
        do{
            read(volume_fd, &c, 1);
            cluster_read++;
            printf("%c",c);
        }while(cluster_read < fat_info.cluster_size && c != 0);

        unsigned int next_cluster = FAT32_getNextClusterFromFAT(volume_fd, file_cluster);
        if((next_cluster != 0x0FFFFFF8) && (next_cluster != 0x0FFFFFFF) && (next_cluster != 0)){
            FAT32_printFile(volume_fd, fat_info, next_cluster);
        }//else {printf("Ya no hay mas clusters, el siguiente es: %X\n", next_cluster);}
    }else printf("lseek error\n");
}

void FAT32_printFileInfo(int volume_fd, char * file_name, int print_info){
    unsigned long file_cluster = FAT32_searchFile(volume_fd, file_name, print_info);

    FATInfo fat_info;
    fat_info.cluster_size = FAT32_getClusterSize(volume_fd);
    fat_info.clusters_data_initial_pos = FAT32_getClustersDataInitialPos(volume_fd);

    if (file_cluster != -1) {
        printf("File %s:\n",file_name);
        printf("------------------\n");
        FAT32_printFile(volume_fd, fat_info, file_cluster);
        printf("\n");
    }else printf("The file %s was not found in the Volume\n", file_name);
}
