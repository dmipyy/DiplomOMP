#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#define maxMessageLength 1024
#define WR_DATA _IOW('a','a',char*)
#define RD_DATA _IOR('a','b',char*)

struct IoctlBuffer //структура данных для функции ioctl
{
	char message[maxMessageLength];
};

int main()
{
	int fd[2]; //файловый дискриптор
    struct IoctlBuffer writeData;
    struct IoctlBuffer readData;
	
	printf("\nIOCTL Based character device driver operation from user space..\n");
	
	//доработать когда будет циклический буфер
	int inputOperation = -1;
	do
	{
		printf("Choose operation: 0 - Write, 1 - Read: ");
		scanf("%d", &inputOperation);
		while(getchar() != '\n'); // очистка буфера входного потока 
		if(inputOperation == 0)
		{
			fd[0] = open("/dev/my_device",O_WRONLY); //O_RDWR open write
    
			if(fd[0] < 0)
			{
    	    	printf("cannot open the device file..\n");
    	    	return 0;
   	 		}
	
			printf("Enter the data to send...\n");
    		scanf("%[^\n\r]", writeData.message);
			printf("Writing message to the driver..\n");
	
			if (ioctl(fd[0], WR_DATA, &writeData) < 0)
			{
    			perror("ioctl: ");
    			close(fd[0]);
				return -1;
    		}
			break;
		}//- - - - - - - - - - - - - - - - - - - - - - - - -
		else if(inputOperation == 1)
		{	
			fd[1] = open("/dev/my_device",O_RDONLY); //O_RDWR open read
			
			if(fd[1] < 0)
			{
    	    	printf("cannot open the device file..\n");
    	    	return 0;
   	 		}
			
			printf("Reading message from driver...\n");
    
    		if (ioctl(fd[1],RD_DATA,&readData) < 0)
			{
    			perror("ioctl");
    			close(fd[1]);
				return -1;
    		}
			//printf("@ \n");
			printf("RESPONSE: %s\n", readData.message);
			//printf("@@ \n");
			break;
		}
		else printf("\n Bad input operation!..\n");
		//сделать вайлом когда будет циклический буфер
	}while(true);
	printf("\nCLosing driver..\n");
    close(fd[1]);
    close(fd[0]);
    
    return 0;
}
