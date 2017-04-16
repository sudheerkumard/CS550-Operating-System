#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#define BUFF_SIZE 500
#define PLOTTER "plotter.dat"
#define FILE_PERMISSION O_RDWR
typedef struct time_struct{
    unsigned long address;
    long time;
}time_struct;

time_struct array[BUFF_SIZE];


int main(int argc, char ** argv)
{
    int fd = open("/proc/proc_device", FILE_PERMISSION);
	if(fd == -1){
		fprintf(stderr,"Error in open.\n");
	}
    FILE *fp = fopen(PLOTTER, "w");
	if(fp == NULL){
		fprintf(stderr,"Error in open.\n");
	}
   	rewind(fp);


   if ( -1 == read(fd, &array, sizeof(array)) ){
		fprintf(stderr,"Connot complete read.\n");
	}

    int i = 0 ;
   
    for(i = 0; i < BUFF_SIZE; i++){
        	array[i].address = 0;
		array[i].time = 0;
	}
    for(i = 0; i < BUFF_SIZE; i++){
		fprintf(fp, "%ld %lu\n", array[i].time, array[i].address);
	}

    close(fd);
    fclose(fp);
	fprintf(stderr,"\n user lever read complete.\n");
    return 0;
}
