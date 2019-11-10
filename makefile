mainWAR: 
	gcc main.c -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw -c
	gcc draw.c -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw -c
	gcc main.o draw.o -lncursesw -ltinfo -o main