#include "ext4.h"

uint32_t block_size;
uint32_t inodeCount;
uint16_t tamano;
int encontrado = 0;
int mostrarContenido = -1;
uint32_t group_inode;
int pintado = -1;

uint32_t EXT4_SearchAllFilesTree(char * fileToFind, uint16_t eh_entries, int volume_fd);
void EXT4_GetContentFile(char * fileToFind, uint16_t eh_entries, int volume_fd );
void EXT4_PrintSuccess(int volume_fd, uint64_t inode_loc, int num_inodo, uint16_t inode_size, uint32_t value);


int EXT4_getFileSystem(int volume_fd) {

    lseek(volume_fd, EXT4_OFFSET + 0x38, SEEK_SET);
	uint16_t magic_number;
    read(volume_fd, &magic_number, sizeof(uint16_t));
    if (magic_number == 0xEF53) {

        //es un ext
        lseek(volume_fd, EXT4_OFFSET + 0x60, SEEK_SET);
		uint32_t feature_compat;
        read(volume_fd, &feature_compat, sizeof(uint32_t));

         if(feature_compat & 0x40) {
             // Tiene INCOMPAT_EXTENTS (ext4)
             return EXT4;
         } else {
             lseek(volume_fd, EXT4_OFFSET + 0x5C, SEEK_SET); //
             read(volume_fd, &feature_compat, sizeof(uint32_t));
             if (feature_compat & 0x04){
                 //Tiene journal (ext3)
                 return EXT3;
             } else {
                 return EXT2;
             }
         }
    }

    return -1;
}

uint16_t EXT4_GetInodeSize(int volume_fd, int print){
    lseek(volume_fd, EXT4_OFFSET + 0x58, SEEK_SET);
	uint16_t size_volume;
    read(volume_fd, &size_volume, sizeof(uint16_t));
    if (print == 1) printf("Inode size: %d\n", size_volume);
    return size_volume;
}

void EXT4_GetNumberInodes(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x00, SEEK_SET);
	uint32_t inode_number;
    read(volume_fd, &inode_number, sizeof(uint32_t));
    printf("Number of Inodes: %d\n", inode_number);
}

void EXT4_GetFirstInode(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x54, SEEK_SET);
	uint32_t first_inode;
    read(volume_fd, &first_inode, sizeof(uint32_t));
    printf("First Inode: %d\n", first_inode);
}

void EXT4_GetInodesGroup(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x28, SEEK_SET);
    read(volume_fd, &group_inode, sizeof(uint16_t));
    printf("Inodes Group: %d\n", group_inode);
}

void EXT4_GetFreeInodes(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x10, SEEK_SET);
	uint32_t free_inode;
    read(volume_fd, &free_inode, sizeof(uint32_t));
    printf("Free Inodes: %d\n\n", free_inode);
}

float EXT4_GetBlockSize(int volume_fd, int print){
    lseek(volume_fd, EXT4_OFFSET + 0x18, SEEK_SET);
	uint32_t block_size;
    read(volume_fd, &block_size, sizeof(uint32_t));
    if (print == 1) printf("Block size: %.2f\n", pow(2, 10+block_size));
    return pow(2, 10+block_size);
}

void EXT4_GetReservedBlocks(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x154, SEEK_SET);
	uint32_t reserved_blocks_hi, reserved_blocks_lo;
    read(volume_fd, &reserved_blocks_hi, sizeof(uint32_t));
    lseek(volume_fd, EXT4_OFFSET + 0x8, SEEK_SET);
    read(volume_fd, &reserved_blocks_lo, sizeof(uint32_t));
    printf("Reserved Blocks: %lu\n", (unsigned long) ((((uint64_t) reserved_blocks_hi)
    << 32) | reserved_blocks_lo));
}

void EXT4_GetFreeBlocks(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x158, SEEK_SET);
	uint32_t free_blocks_lo, free_blocks_hi;
    read(volume_fd, &free_blocks_hi, sizeof(uint32_t));
    lseek(volume_fd, EXT4_OFFSET + 0xC, SEEK_SET);
    read(volume_fd, &free_blocks_lo, sizeof(uint32_t));
    printf("Free Blocks: %lu\n", (unsigned long) ((((uint64_t) free_blocks_hi)
    << 32) | free_blocks_lo));
}

void EXT4_GetTotalBlocks(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x150, SEEK_SET);
	uint32_t total_blocks_lo, total_blocks_hi;
    read(volume_fd, &total_blocks_hi, sizeof(uint32_t));
    lseek(volume_fd, EXT4_OFFSET + 0x4, SEEK_SET);
    read(volume_fd, &total_blocks_lo, sizeof(uint32_t));
    printf("Total Blocks: %lu\n", (unsigned long) ((((uint64_t) total_blocks_hi)
    << 32) | total_blocks_lo));
}

void EXT4_GetFirstBlock(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x14, SEEK_SET);
	uint32_t first_block;
    read(volume_fd, &first_block, sizeof(uint32_t));
    printf("First Block: %d\n", first_block);
}

void EXT4_GetBlockGroup(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x20, SEEK_SET);
	uint32_t block_group;
    read(volume_fd, &block_group, sizeof(uint32_t));
    printf("Block Group: %d\n", block_group);
}

void EXT4_FragsGroup(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x24, SEEK_SET);
	uint32_t frags_group;
    read(volume_fd, &frags_group, sizeof(uint32_t));
    printf("Frags Group: %d\n\n", frags_group);
}

void EXT4_VolumeName(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x78, SEEK_SET);
	char volume_name[16];
    read(volume_fd, &volume_name, sizeof(char)*16);
    printf("Volume Name: %s\n", volume_name);
}

void EXT4_LastCheck(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x40, SEEK_SET);
	uint32_t last_check;
    read(volume_fd, &last_check, sizeof(uint32_t));
    time_t last_t = last_check;
    printf("Last Check: %s", ctime(&last_t));
}

void EXT4_LastMount(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x2C, SEEK_SET);
	uint32_t last_mount;
    read(volume_fd, &last_mount, sizeof(uint32_t));
    time_t last_t = last_mount;
    printf("Last Mount: %s", ctime(&last_t));
}

void EXT4_LastWritten(int volume_fd){
    lseek(volume_fd, EXT4_OFFSET + 0x30, SEEK_SET);
	uint32_t last_written;
    read(volume_fd, &last_written, sizeof(uint32_t));
    time_t last_t = last_written;
    printf("Last Written: %s", ctime(&last_t));
}

uint64_t EXT4_GetLocationTable(int volume_fd) {

    uint32_t low;
	uint32_t high;
    uint64_t table_loc;

    lseek(volume_fd, EXT4_OFFSET + 0x60, SEEK_CUR);
    read(volume_fd, &high, sizeof(uint32_t));

    if(high == 0x80){

        uint16_t desc_size;
        lseek(volume_fd, EXT4_OFFSET + 0xFE, SEEK_CUR);
        read(volume_fd, &desc_size, sizeof(uint16_t));

        if(block_size == 1024){
            lseek(volume_fd, (block_size*2) + 0x08, SEEK_SET);
            read(volume_fd, &(low), sizeof(low));
            table_loc = low;
        }else{
            lseek(volume_fd, block_size+0x08, SEEK_CUR);
            read(volume_fd, &low, sizeof(uint32_t));
            table_loc = (((uint64_t) (high)) << 32) | low;
        }

    }else{

        if (block_size == 1024) {
            lseek(volume_fd, (block_size*2) + 0x08, SEEK_SET);
            read(volume_fd, &(low), sizeof(low));
        } else {
            lseek(volume_fd, (block_size) + 0x08, SEEK_SET);
            read(volume_fd, &(low), sizeof(low));
        }

        table_loc = low;
    }

    return table_loc;
}

uint32_t EXT4_findFile(int volume_fd, char * fileToFind, int num_inodo) {

    // sacamos el tamaño del bloque para poder pasar al Block Group y encontrar
    // el tamaño de inodo.
    int valor_lseek = 0;
    uint32_t value = 0;
    off_t offset = lseek(volume_fd, 0, SEEK_CUR);
    if (offset == -1) {
        printf("Error con el lseek\n");
        return -1;
    }
    block_size = (uint32_t) EXT4_GetBlockSize(volume_fd, 0);

    uint32_t feature_incompat = 0;
    lseek(volume_fd, EXT4_OFFSET + 0x60,  SEEK_SET);
    read(volume_fd, &feature_incompat, sizeof(uint32_t));

    uint64_t inode_loc;
    inode_loc = EXT4_GetLocationTable(volume_fd);


    //obtenemos el inode count
    valor_lseek = lseek(volume_fd, EXT4_OFFSET + 0x0, SEEK_SET);
    if (valor_lseek == -1) {
        printf("Error con el lseek\n");
        return -1;
    }

    read(volume_fd, &inodeCount, sizeof(uint32_t));

    //obtenemos el tamaño del inodo para poder saltarlo e ir al 2 (root directory)
    uint16_t inode_size;
    inode_size = EXT4_GetInodeSize(volume_fd, 0);

    //obtenemos el header del extent tree (info de ese directorio en concreto)
    ExtentTreeHeader extentTreeHeader;

    valor_lseek = lseek(volume_fd, ((inode_loc*block_size) + (inode_size * ((num_inodo-1) ))) + 0x28,  SEEK_SET);
    if (valor_lseek == -1) {
        printf("Ha habido un problema con el lseek\n");
        return -1;
    }
    read (volume_fd, &extentTreeHeader, sizeof(extentTreeHeader));

    /* --------- Printfs para debuggar ---------- */
    //printf("depth: %d\n", extentTreeHeader.eh_depth);
    //printf("magic: %d\n", extentTreeHeader.eh_magic);
    //printf("entries: %d\n", extentTreeHeader.eh_entries);

    //empezamos la busqueda
    if (mostrarContenido == 1 || mostrarContenido == -1) {
        if (extentTreeHeader.eh_depth != 0) {
            //printf("depth != 0 2\n");
            value = EXT4_SearchAllFilesTree(fileToFind, extentTreeHeader.eh_entries, volume_fd);
        } else {
            value = EXT4_SearchAllFilesLeaf(fileToFind, extentTreeHeader.eh_entries, volume_fd);
        }
    } else if (mostrarContenido == 2) {
        if (extentTreeHeader.eh_depth != 0) {
            //printf("depth != 0\n");
            value = EXT4_SearchAllFilesTree(fileToFind, extentTreeHeader.eh_entries, volume_fd);

        } else {
            mostrarContenido = 3;
            //value = EXT4_SearchAllFilesLeaf(fileToFind, extentTreeHeader.eh_entries, volume_fd);

            EXT4_GetContentFile(fileToFind, extentTreeHeader.eh_entries, volume_fd);
        }
    }



    //si value > 0 quiere decir que ha encontrado el fichero
    //printf("MostrarContenido: %d\n", mostrarContenido);
    //printf("value: %d\n", value);

    if (value > 0 && mostrarContenido != 2 && mostrarContenido != 3) {

        if (mostrarContenido == 1) {
            mostrarContenido = 2;
            printf("\nFile found! Showing content...\n");
            EXT4_findFile(volume_fd, fileToFind, value);
            return -1;
        }

        EXT4_PrintSuccess(volume_fd,inode_loc,num_inodo, inode_size, value);


        return value;

    }

    lseek(volume_fd, offset, SEEK_SET);
    return value;

}

void EXT4_PrintSuccess(int volume_fd, uint64_t inode_loc, int num_inodo, uint16_t inode_size, uint32_t value) {
    printf("\n+------------------------------------------");
    printf("\n+ File found!\n");
    size_t valor_lseek;
    valor_lseek = lseek(volume_fd, ((inode_loc*block_size) + (inode_size * (num_inodo-1))) + 0x6C,  SEEK_SET);
    if (valor_lseek == -1) {
        printf("Ha habido un problema con el lseek\n");
    }
    uint32_t file_size_hi, file_size_low;

    read (volume_fd, &file_size_hi, sizeof(uint32_t));

    valor_lseek = lseek(volume_fd, ((inode_loc*block_size) + (inode_size * (num_inodo-1))) + 0x4,  SEEK_SET);
    if (valor_lseek == -1) {
        printf("Ha habido un problema con el lseek\n");
    }
    read (volume_fd, &file_size_low, sizeof(uint32_t));
    unsigned long sizeFile = (unsigned long) ((((uint64_t) file_size_hi) << 32) | file_size_low);

    printf("+ Size: %lu bytes.   ", sizeof(sizeFile));
    valor_lseek = lseek(volume_fd, ((inode_loc * block_size) + (inode_size * (num_inodo-1))) + 0x90,  SEEK_SET);
    if (valor_lseek == -1) {
        printf("Ha habido un problema con el lseek\n");
    }
    uint32_t time;
    read (volume_fd, &time, sizeof(uint32_t));
    time_t creacion_t = time;
    printf("Created on: %s", ctime(&creacion_t));
    printf("+------------------------------------------\n");
}

uint32_t EXT4_SearchAllFilesTree(char * fileToFind, uint16_t eh_entries, int volume_fd) {

    int i;
	ExtentTreeIndexNode *extentTreeIndex;
	uint32_t value = 0;
    uint64_t next_block_extent_node = 0;
    size_t valor_lseek = -1;
	extentTreeIndex = (ExtentTreeIndexNode *) malloc(sizeof(ExtentTreeIndexNode) * eh_entries);
    if (extentTreeIndex == NULL) {
        printf("Ha habido un problema de memoria (SearchAllFilesTree)\n");
    }

	//read(volume_fd, &extentTreeIndex, sizeof(extentTreeIndex) * eh_entries);
    read(volume_fd, extentTreeIndex, sizeof(extentTreeIndex));
	for (i = 0; i < eh_entries; i++) {
        next_block_extent_node = 0;
		//uint64_t extentPoints = 0;
		ExtentTreeHeader extentTreeHeader;

        next_block_extent_node = extentTreeIndex[i].ei_leaf_hi;
        next_block_extent_node <<= 32;
        next_block_extent_node |= extentTreeIndex[i].ei_leaf_lo;
        //next_block_extent_node = (next_block_extent_node << 32) | extentTreeIndex[i].ei_leaf_lo;
        //extentPoints = ((extentPoints | extentTreeIndex[i].ei_leaf_hi) << 32) | extentTreeIndex[i].ei_leaf_lo;

		valor_lseek = lseek(volume_fd, next_block_extent_node * block_size, SEEK_SET);
        if (valor_lseek == -1) {
            printf("Error con el lseek\n");
        }
		read(volume_fd, &extentTreeHeader, sizeof(extentTreeHeader));
        if (mostrarContenido == 0) {
    		if (extentTreeHeader.eh_depth != 0) {
                value = EXT4_SearchAllFilesTree(fileToFind, extentTreeHeader.eh_entries, volume_fd);
    		} else {
                value = EXT4_SearchAllFilesLeaf(fileToFind, extentTreeHeader.eh_entries, volume_fd);
    		}
        } else if( mostrarContenido == 2) {

            if (extentTreeHeader.eh_depth != 0) {
                value = EXT4_SearchAllFilesTree(fileToFind, extentTreeHeader.eh_entries, volume_fd);
    		} else {
                EXT4_GetContentFile(fileToFind, extentTreeHeader.eh_entries, volume_fd);
    		}
        }

	}

    return value;
}

size_t EXT4_GetNameSize(FileInformation file) {
    if (file.rec_len > (256 + 8)) {
        return sizeof(char) * file.name_len;
    } else {
        return sizeof(char) * file.rec_len - 8;
    }
}

uint32_t EXT4_SearchAllFilesLeaf(char * fileToFind, uint16_t eh_entries, int volume_fd) {
    //vamos a guardar todas las entradas de ese fichero en un array dinamico
    ExtentTreeNode * extentTreeIndexNode;
    size_t size;
    char * file_name;
    int tamanoBloques = 0;
    extentTreeIndexNode = (ExtentTreeNode *) malloc(sizeof(ExtentTreeNode) * eh_entries);
    read(volume_fd, extentTreeIndexNode, sizeof(ExtentTreeNode) * eh_entries);
    FileInformation file;
    int i;
    uint64_t next_block_extent_node = 0;
    off_t valor_lseek = 0;
    //printf("eh_entries: %d\n", eh_entries);

    for (i = 0; i < eh_entries; i++) {

        next_block_extent_node |= extentTreeIndexNode[i].ee_start_hi;
        next_block_extent_node = (next_block_extent_node << 32) | extentTreeIndexNode[i].ee_start_lo;

        valor_lseek = lseek(volume_fd, next_block_extent_node * block_size, SEEK_SET);

        if (valor_lseek == -1) {
            printf("Ha habido un problema con el lseek\n");
            return -1;
        }

        read(volume_fd, &file, sizeof(FileInformation));
        tamanoBloques += sizeof(FileInformation);

        // vamos a ir recorriendo los ficheros del directorio
        while(file.inode != 0 && (extentTreeIndexNode[i].ee_len * block_size) > tamanoBloques) {
            size = EXT4_GetNameSize(file);
            file_name = (char *) malloc(size);

            /*if (file_name == NULL) {
                printf("Ha habido un problema de memoria... cerrando\n");
                return -1;
            }*/

            read(volume_fd, file_name, size);
            tamanoBloques += size;

            if (EXT4_EsFichero(file_name, file.file_type, fileToFind)) {
                encontrado = 1;
                tamano = file.rec_len;
                return file.inode;
            }

            if (EXT4_EsCarpeta(file_name, file.file_type)) {

                    int value = EXT4_findFile(volume_fd, fileToFind, file.inode);

                    if (next_block_extent_node == value) {
                        free(file_name);
                        free(extentTreeIndexNode);
    					return (uint32_t) next_block_extent_node;
                    }
            }

            read(volume_fd, &file, sizeof(FileInformation));
            tamanoBloques += sizeof(FileInformation);
            if (file_name != NULL) free(file_name);


    }

    }
    free (extentTreeIndexNode);
    return 0;

}

void EXT4_GetContentFile(char * fileToFind, uint16_t eh_entries, int volume_fd ) {
    ExtentTreeNode * extentTreeIndexNode;
    extentTreeIndexNode = (ExtentTreeNode *) malloc(sizeof(ExtentTreeNode) * eh_entries);
    read(volume_fd, extentTreeIndexNode, sizeof(ExtentTreeNode) * eh_entries);

    int i, j;
    uint64_t next_block_extent_node = 0;
    off_t valor_lseek = 0;
    char letra;
    for (i = 0; i < eh_entries; i++) {
        if (extentTreeIndexNode[i].ee_len > 32768) {
			continue;
        }

        next_block_extent_node |= extentTreeIndexNode[i].ee_start_hi;
        next_block_extent_node = (next_block_extent_node << 32) | extentTreeIndexNode[i].ee_start_lo;

        valor_lseek = lseek(volume_fd, next_block_extent_node * block_size, SEEK_SET);

        if (valor_lseek == -1) {
            printf("Ha habido un problema con el lseek\n");
        }

        int tamanoFichero = extentTreeIndexNode[i].ee_len * block_size;
        //printf("tamano: %d\n", tamanoFichero);
        for (j = 0; j < tamanoFichero && pintado != 1 ; j++) {

            read (volume_fd, &letra, sizeof(char));
            printf("%c", letra);
        }
        pintado = 1;
        //printf("j: %d\n", j);
        //printf("\n--------------- FIN DEL ARCHIVO ----------------\n");



    }
    free(extentTreeIndexNode);
}

int EXT4_EsFichero(char * file_name, uint8_t tipo_fichero, char * fileToFind) {
    return file_name != NULL && strcmp(file_name, fileToFind) == 0 && tipo_fichero == 1;
}

int EXT4_EsCarpeta(char * file_name, uint8_t tipo_fichero) {
    return file_name != NULL && strcmp(file_name, ".") != 0 && strcmp(file_name, "..") != 0 && tipo_fichero == 2;
}


void EXT4_showInfo(int volume_fd){

    printf("----Filesystem Information ----\n");
    printf("Filesystem: EXT4\n\n");
    printf("INODE INFO\n");
    EXT4_GetInodeSize(volume_fd, 1);
    EXT4_GetNumberInodes(volume_fd);
    EXT4_GetFirstInode(volume_fd);
    EXT4_GetInodesGroup(volume_fd);
    EXT4_GetFreeInodes(volume_fd);
    printf("BLOCK INFO\n");
    EXT4_GetBlockSize(volume_fd, 1);
    EXT4_GetReservedBlocks(volume_fd);
    EXT4_GetFreeBlocks(volume_fd);
    EXT4_GetTotalBlocks(volume_fd);
    EXT4_GetFirstBlock(volume_fd);
    EXT4_GetBlockGroup(volume_fd);
    EXT4_FragsGroup(volume_fd);
    printf("VOLUME INFO\n");
    EXT4_VolumeName(volume_fd);
    EXT4_LastCheck(volume_fd);
    EXT4_LastMount(volume_fd);
    EXT4_LastWritten(volume_fd);
    printf("\n");

}
