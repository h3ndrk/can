#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h> // sensor image filename
#include <math.h> // sensor image filename
#include <wiringPi.h> // wiringPi
#include <jpeglib.h> // libjpeg
#include <sys/socket.h> // IPC-Socket
#include <sys/types.h> // IPC-Socket
#include <netdb.h> // IPC-Socket
#include <netinet/in.h> // IPC-Socket
#include <arpa/inet.h> // IPC-Socket

// DEBUG = 0 = no debug messages at stdout, DEBUG = 1 = debug messages at stdout
int DEBUG = 0;

char ipcDelimiter[] = "|";

#include "handlerCore.h"
#include "handlerDrivingHeader.h"
#include "handlerImageRecognition.h"
#include "handlerCommands.h"
#include "handlerDriving.h"

int main(int argc, char *argv[])
{
	int i;
	int commandId = 0;
	char *commandParam;
	
	// disable buffering output to make (s)printf thread-safe
	setbuf(stdout,NULL);
	setbuf(stdin,NULL);
	
	if(wiringPiSetupGpio() == -1)
	{
		printf("Error while initializing wiringPi. Exit now.\n");
		exit(1);
	}
	
	// process command line parameters
	if(argc > 3)
	{
		int parameterLength = 0;
		sscanf(argv[2],"%i",&commandId);
		for(i = 3; i < argc; i++)
		{
			parameterLength += strlen(argv[i]) + 1;
		}
		commandParam = malloc((parameterLength) * sizeof(char));
		for(i = 3; i < argc; i++)
		{
			strcat(commandParam,argv[i]);
			strcat(commandParam," ");
		}
		commandParam[parameterLength - 1] = '\0';
		for(i = 0; i < strlen(argv[1]); i++)
		{
			switch(argv[1][i])
			{
				case 'd':
					DEBUG = 1;
					break;
			}
		}
	}
	else
	{
		printf("Usage: can-handler <options> <commandId> <commandParameter>\n");
		exit(1);
	}
	
	if(commandId != 15) // do not allow the fan control to access the databus
	{
		FILE *fanFile;
		if((fanFile = fopen("/usr/share/nginx/html/bin/values/fan","r")) != NULL)
		{
			int oldFanSpeed, newFanSpeed;
			fscanf(fanFile,"%i %i",&oldFanSpeed,&newFanSpeed);
			fclose(fanFile);
			if(oldFanSpeed != newFanSpeed)
			{
				char *fanParam = malloc((((newFanSpeed > 0)?((int)log10((double)newFanSpeed) + 1):(1)) + 1) * sizeof(char));
				sprintf(fanParam,"%i",newFanSpeed);
				if((fanFile = fopen("/usr/share/nginx/html/bin/values/fan","w")) != NULL)
				{
					fprintf(fanFile,"%i %i",newFanSpeed,newFanSpeed);
					fclose(fanFile);
				}
				execCommands(3,fanParam);
				free(fanParam);
			}
		}
	}
	
	// execute the given command
	execCommands(commandId,commandParam);
	
	if(DEBUG)
		printf("Finished. Thank you.\n");
	
	return 0;
}
