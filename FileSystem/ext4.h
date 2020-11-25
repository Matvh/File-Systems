#ifndef EXT4_H
#define EXT4_H

#include "filetypes.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

extern int encontrado;
extern int mostrarContenido;

typedef struct __attribute__((packed)) {
    uint16_t eh_magic;
	uint16_t eh_entries; //numero de ficheros/directorios que contiene el directorio actual
	uint16_t eh_max;
	uint16_t eh_depth; //profundidad del directorio actual
    uint32_t eh_generation;

} ExtentTreeHeader;

typedef struct __attribute__((packed)) {
    uint32_t ei_block;
	uint32_t ei_leaf_lo; //tamaño del directorio (parte baja)
	uint16_t ei_leaf_hi; //tamaño del directorio (parte alta)
	uint16_t ei_unused;
} ExtentTreeIndexNode;

typedef struct __attribute__((packed)){
	uint32_t ee_block;
	uint16_t ee_len;
	uint16_t ee_start_hi; //donde esta la información del fichero (parte alta)
	uint32_t ee_start_lo; //donde esta la información del fichero (parte baja)
} ExtentTreeNode;

typedef struct  __attribute__((packed)){
	uint32_t inode;
	uint16_t rec_len;
	uint8_t name_len;
	uint8_t file_type;
} FileInformation;

int EXT4_getFileSystem(int volume_fd);
uint16_t EXT4_GetInodeSize(int volume_fd, int print);
void EXT4_GetNumberInodes(int volume_fd);
float EXT4_GetBlockSize(int volume_fd, int print);
void EXT4_GetFreeInodes(int volume_fd);
void EXT4_GetInodesGroup(int volume_fd);
void EXT4_GetFirstInode(int volume_fd);
void EXT4_GetReservedBlocks(int volume_fd);
void EXT4_GetFreeBlocks(int volume_fd);
void EXT4_GetTotalBlocks(int volume_fd);
void EXT4_GetFirstBlock(int volume_fd);
void EXT4_GetBlockGroup(int volume_fd);
void EXT4_FragsGroup(int volume_fd);
void EXT4_VolumeName(int volume_fd);
void EXT4_LastCheck(int volume_fd);
void EXT4_LastMount(int volume_fd);
void EXT4_LastWritten(int volume_fd);

void EXT4_showInfo(int volume_fd);

//find the file in the volume
uint32_t EXT4_findFile(int volume_fd, char * fileToFind, int num_inodo);
int EXT4_EsCarpeta(char * file_name, uint8_t tipo_fichero);
int EXT4_EsFichero(char * file_name, uint8_t tipo_fichero, char * fileToFind);
uint32_t EXT4_ContinueSearching(char * fileToFind, uint16_t, int volume_fd);
uint32_t EXT4_SearchAllFilesLeaf(char * fileToFind, uint16_t, int volume_fd);
uint32_t EXT4_SearchAllFilesTree(char * fileToFind, uint16_t eh_entries, int volume_fd);
#endif
