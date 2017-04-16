#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include<fcntl.h>
#define PRODUCTION_NUMBER 20 
void random_string(char * string, unsigned length)
{
  /* Seed number for rand() */
  //srand((unsigned int) time(0) + getpid());
   
  /* ASCII characters 33 to 126 */
  int i;  
  for (i = 0; i < length; ++i)
    {
      string[i] = rand() % 10 + 65;
    }
 
  string[i] = '\0';  
}
 



int main(int argc, const char * argv[])
{
  
  //random_string(s, 5);
  //printf("Produced -> %s\n", s);  
   srand(time(NULL));
   int fd = open("/dev/miscdev", O_WRONLY);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }	
  int consumer = PRODUCTION_NUMBER;
  if(argc == 2){
	consumer = atoi(argv[1]);
  }
  int i=0;
  for(i=0; i<consumer; i++)
   {
  		char s[31];   		
		random_string(s, 5);
		sleep(1);
   		int ret = write(fd, s,6); // Send the string to the LKM
   		if (ret < 0)
		{
      			perror("Failed to write the message to the device.");
      			return errno;
		}
		printf("[%d] Produced--> %s \n",getpid(), s);
				
   }
	close(fd);
  return 0;	

}		
