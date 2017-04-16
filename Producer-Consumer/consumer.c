#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include<fcntl.h>
#define CONSUMER_NUMBER 20




int main(int argc, const char * argv[] )
{
  
  
   
   int fd = open("/dev/miscdev", O_RDONLY);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }	
   int consumer = CONSUMER_NUMBER;
  if(argc == 2){
	consumer = atoi(argv[1]);
  }
  int i=0;
  for(i=0; i< consumer; i++)
   {
			
  		char receive[10];		
		//printf("Reading from the device...\n");
   		int ret = read(fd, receive, 10);        // Read the response from the LKM
   		if (ret < 0){
      			perror("Failed to read the message from the device.");
      			return errno;
		}
                printf("[%d] Consumed--> %s\n",getpid(), receive);
		sleep(1);
   }
   close(fd);	
  return 0;	

}		
