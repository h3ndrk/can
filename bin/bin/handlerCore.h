/**
 * Checks whether a file exists or not
 * @param filename		the filename of the file to be checked
 * @return boolean: true = exist, false = not exist
**/
bool fileExists(const char *filename)
{
	FILE *file;
	if((file = fopen(filename,"r")) != NULL)
	{
		fclose(file);
		return true;
	}
	if(DEBUG)
		printf(" failed to open file '%s'",filename);
	return false;
}

/**
 * Writes a string to file
 * @param filename		the filename of the file to be written in
 * @param string		the string to write
**/
void writeToFile(char *filename,char *string)
{
	FILE *file;
	if((file = fopen(filename,"w")) != NULL)
	{
		fprintf(file,"%s",string);
		fclose(file);
	}
}

/**
 * Checks whether a file exists or not
 * @param filename		the filename of the file to be read
 * @return an integer read from file
**/
int readFromFile(char *filename)
{
	int tempInt = 0;
	FILE *file;
	if((file = fopen(filename,"r")) != NULL)
	{
		fscanf(file,"%i",&tempInt);
		return tempInt;
		fclose(file);
	}
	return 0;
}

/**
 * Send a byte to data bus
 * @param byte			the byte to be sent
**/
void dataBusSend(char byte)
{
	int i;
	
	if(DEBUG)
		printf("SEND: %3i %i%i%i%i%i%i%i%i",byte,(byte&(1<<0))?(1):(0),
		(byte&(1<<1))?(1):(0),(byte&(1<<2))?(1):(0),(byte&(1<<3))?(1):(0),
		(byte&(1<<4))?(1):(0),(byte&(1<<5))?(1):(0),(byte&(1<<6))?(1):(0),
		(byte&(1<<7))?(1):(0));
	
	// setting up gpio
	pinMode(23,OUTPUT);
	pinMode(25,OUTPUT);
		
	// transmitting the byte
	for(i = 0; i < 8; i++)
	{
		if(byte & (1<<i))
		{
			digitalWrite(23,1);
		}
		else
		{
			digitalWrite(23,0);
		}
		digitalWrite(25,1);
		usleep(10);
		digitalWrite(25,0);
		usleep(10);
	}
	
	// cleaning up
	digitalWrite(23,0);
	
	if(DEBUG)
		printf("\n");
}

bool interruptsDeclared = false;
static volatile int clockState = 0;

/**
 * Callback function for wiringPi interrupt(NOT USED ANY MORE)
**/
void rising(void)
{
	clockState = 1;
	printf("rising ");
}

/**
 * Callback function for wiringPi interrupt(NOT USED ANY MORE)
**/
void falling(void)
{
	clockState = 0;
	printf("falling ");
}

/**
 * Read a byte from data bus
 * @return the received byte
**/
char dataBusRead(void)
{
	char byte = 0;
	int i;
	
	if(DEBUG)
		printf("GET :");
	
	// setting up gpio
	pinMode(24,INPUT);
	pinMode(8,INPUT);
	
	// receiving the byte
	for(i = 0; i < 8; i++)
	{
		// wait for clock
		while(digitalRead(8) == 0)
		{
			usleep(1);
		}
		// read the byte
		if(digitalRead(24) == 1)
		{
			byte |= (1<<i);
		}
		else
		{
			byte &= ~(1<<i);
		}
		// wait for clock
		while(digitalRead(8) == 1)
		{
			usleep(1);
		}
	}
	
	if(DEBUG)
		printf(" %3i %i%i%i%i%i%i%i%i",byte,(byte&(1<<0))?(1):(0),(byte&(1<<1))?(1):(0),(byte&(1<<2))?(1):(0),(byte&(1<<3))?(1):(0),(byte&(1<<4))?(1):(0),(byte&(1<<5))?(1):(0),(byte&(1<<6))?(1):(0),(byte&(1<<7))?(1):(0));
	
	if(DEBUG)
		printf("\n");
	
	return byte;
}

/**
 * Send a complete command to data bus(NOT USED ANY MORE)
 * @param commandId		command-ID of the command
 * @param commandParam	command-Parameters of the command
 * @param termination	a flag whether the handler should send a termination
**/
void dataBusSendCommand(int commandId,char *commandParam,bool termination)
{
	if(commandId <= 255 && strlen(commandParam) > 0)
	{
		// transmit command-ID
		char character = (char)commandId;
		dataBusSend(character);
		
		// split and transmit command-Parameters
		int commandParamByte = 0;
		char *string = strtok(commandParam," ");
		while(string != NULL)
		{
			sscanf(string,"%i",&commandParamByte);
			dataBusSend((char)commandParamByte);
			string = strtok(NULL," ");
		}
		
		// termination byte
		if(termination)
			dataBusSend(255);
	}
}

/**
 * Reset the chip from handler
**/
void dataBusReset(void)
{
	// setting up gpio
	pinMode(18,OUTPUT);
	
	// perform reset
	digitalWrite(18,0);
	usleep(10000);
	digitalWrite(18,1);
}

void ipcSend(char *msg)
{
	int sockfd;
	struct sockaddr_in servaddr;

	// wait for server
	usleep(100000);
	
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		fprintf(stderr,"Failed to create socket.\nWill exit now.\n");
		exit(1);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(1028);
	
	if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr) <= 0)
	{
		fprintf(stderr,"Failed to set server address.\nWill exit now.\n");
		exit(1);
	}
	
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		fprintf(stderr,"Failed to connect.\nWill exit now.\n");
		exit(1);
	}
	
	write(sockfd,msg,(int)strlen(msg));
	
	close(sockfd);
}
