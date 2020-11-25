f1.exe : main.o ext4.o fat32.o filesystem.o commands.o
	gcc main.o ext4.o fat32.o filesystem.o commands.o -lm -o infinityWar

main.o : main.c
	gcc -c main.c
ext4.o :
	gcc -c FileSystem/ext4.c
fat32.o :
	gcc -c FileSystem/fat32.c
filesystem.o :
	gcc -c FileSystem/filesystem.c
commands.o:
	gcc -c commands.c
clean :
	rm *.o
