#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>        // sensor image filename
#include <math.h>        // sensor image filename
#include <wiringPi.h>    // wiringPi
#include <jpeglib.h>     // libjpeg

// DEBUG = 0 = no debug messages at stdout, DEBUG = 1 = debug messages at stdout
int DEBUG = 0;

// terminal colors
char red[] = "\x1b[31m";
char green[] = "\x1b[32m";
char reset[] = "\x1b[0m";

#include "globals.h"
#include "handlerCore.h"
#include "handlerDrivingHeader.h"
#include "handlerImageRecognition.h"
#include "handlerCommands.h"
#include "handlerDriving.h"

int main(int argc, char *argv[])
{
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
		for(int i = 3; i < argc; i++)
		{
			parameterLength += strlen(argv[i]) + 1;
		}
		commandParam = malloc((parameterLength) * sizeof(char));
		for(int i = 3; i < argc; i++)
		{
			strcat(commandParam,argv[i]);
			strcat(commandParam," ");
		}
		commandParam[parameterLength - 1] = '\0';
		for(int i = 0; i < strlen(argv[1]); i++)
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
	
	// if handler isnt locked try to lock
	/*if(DEBUG == 1)
		printf("Set command lock...");
	if(!fileExists(LOCK_COMMANDS_FILE))
	{
		FILE *lockFileTemp;
		lockFileTemp = fopen(LOCK_COMMANDS_FILE,"w");
		if(lockFileTemp != NULL)
		{
			if(DEBUG == 1)
				printf(" [ %sOK%s ]\n",green,reset);
			fclose(lockFileTemp);
		}
		else
		{
			if(DEBUG == 1)
				printf(" [%sFAIL%s]\n",red,reset);
		}
	}
	else
	{
		if(DEBUG == 1)
			printf(" [%sFAIL%s]\n",red,reset);
		printf("locked\n");
		exit(0);
	}*/
	
	// execute the given command
	execCommands(commandId,commandParam);
	
	// remove command lock
	/*if(DEBUG == 1)
		printf("Remove command lock...");
	if(remove(LOCK_COMMANDS_FILE) != 0)
	{
		if(DEBUG == 1)
			printf(" [%sFAIL%s]\n",red,reset);
	}
	else
	{
		if(DEBUG == 1)
			printf(" [ %sOK%s ]\n",green,reset);
	}*/
	
	if(DEBUG)
		printf("Finished. Thank you.\n");
	
	return 0;
}
