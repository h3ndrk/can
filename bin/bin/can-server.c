#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr()
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <math.h> // log10

#define PORT_WEBSOCKET 1027
#define PORT_IPC 1028
int clientworkerAmount = 0;
int clientworkerRead = 0;
char clientworkerBuffer[1024];

unsigned long globalKey = 9997110; // ascii characters: "c", "a", "n" -> %i%i%i

// compile with "-lpthread" and "-D_REENTRANT"

/**
 * This function is a callback function to clean up and free memory
**/
char *key;
void generateKeyCleanUp(void)
{
	free(key);
}

/**
 * This function will generate server key with the given client key
 * @param header		the http header in which is the key
 * @return the string of the server key
**/
char *generateKey(char *header)
{
	char command[] = "php -f /usr/share/nginx/html/bin/can-server-key.php ";
	int keyLength = 0;
	char *temp;
	FILE *pipe;
	
	// get client key
	if(strstr(header,"Sec-WebSocket-Key: ") != 0)
	{
		key = strstr(header,"Sec-WebSocket-Key: ") + 19;
		sscanf(key,"%s",key);
	}
	else
		fprintf(stderr,"Error: Failed to retrieve key.\nWill  exit now.\n");
	
	// generate command
	temp = malloc((strlen(command) + strlen(key) + 1) * sizeof(char));
	sprintf(temp,"%s%s",command,key);
	
	// calculate server key
	if((pipe = popen(temp,"r")) != NULL)
	{
		free(temp);
		fscanf(pipe,"%i",&keyLength);
		key = malloc((keyLength + 1) * sizeof(char));
		fscanf(pipe,"%s",key);
		pclose(pipe);
		printf(" '%s'",key);
	}
	
	// register clean up function
	atexit(generateKeyCleanUp);
	
	return key;
}

void processSend(int sockfd,char *message)
{
	char *buffer;
	int frameShift = 0;
	
	printf("[%3i]: Write to client: '%s'\n",sockfd,message);
	
	if(strlen(message) < 126)
	{
		frameShift = 2;
		buffer = malloc(((int)strlen(message) + frameShift + 1) * sizeof(char));
		
		buffer[0] = '\x81'; // 129 = text
		buffer[1] = (char)strlen(message);
	}
	else if(strlen(message) > 127 && strlen(message) < 65536)
	{
		frameShift = 4;
		buffer = malloc(((int)strlen(message) + frameShift + 1) * sizeof(char));
		
		buffer[0] = '\x81'; // 129 = text
		buffer[1] = '\x7e'; // 126 = two size bytes
		buffer[2] = (char)(strlen(message)>>8);
		buffer[3] = (char)strlen(message);
	}
	else
	{
		frameShift = 10;
		buffer = malloc(((int)strlen(message) + frameShift + 1) * sizeof(char));
		
		buffer[0] = '\x81'; // 129 = text
		buffer[1] = '\x7f'; // 127 = eight size bytes
		buffer[2] = (char)(strlen(message)>>56);
		buffer[3] = (char)(strlen(message)>>48);
		buffer[4] = (char)(strlen(message)>>40);
		buffer[5] = (char)(strlen(message)>>32);
		buffer[6] = (char)(strlen(message)>>24);
		buffer[7] = (char)(strlen(message)>>16);
		buffer[8] = (char)(strlen(message)>>8);
		buffer[9] = (char)strlen(message);
	}
	
	// append
	sprintf(buffer + frameShift,"%s",message);
	
	write(sockfd,buffer,(int)strlen(message) + frameShift);
	
	free(buffer);
}

/**
 * Replace characters in string
 * @param search			needle to search
 * @param replace			character to replace
 * @param subject			string to search in
 * @return the replaced string
**/
void str_replace(char search,char replace,char *subject)
{
	int i;
	for(i = 0; i < strlen(subject); i++)
	{
		if(subject[i] == '\n')
			subject[i] = ' ';
	}
}

/**
 * Put the given string in front of the other string
 * @param subject			string to be prefixed
 * @param prefix			prefix string
**/
void prefix_with(char *subject,char *prefix)
{
	memmove(subject + strlen(prefix),subject,strlen(subject));
	memcpy(subject,prefix,strlen(prefix));
}

void *executeCommand(void *param)
{
	char *msg = (char *)param;
	int sockfd = 0;
	char *command;
	int i;
	FILE *pipe;
	char buffer[1024];
	char prefix[] = "command|";

	printf("[EXE]: Execute thread started, '%s'\n",msg);
	sscanf(msg,"%i",&sockfd);
	// strlen(msg) - (%i) - 1(:) + 1('\0')
	command = malloc((strlen(msg) - ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) - 1 + 1) * sizeof(char));
	for(i = 0; i < (strlen(msg) - ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) - 1 + 1); i++)
	{
		command[i] = msg[i + ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) + 1];
	}
	command[(strlen(msg) - ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) - 1)] = '\0';
	pipe = popen(command,"r");
	while(fgets(buffer,1023 - strlen(prefix),pipe))
	{
		str_replace('\n',' ',buffer); // replace newlines
		prefix_with(buffer,prefix); // prefix command
		processSend(sockfd,buffer); // send command
		memset(buffer,0,1023); // clean command
	}
	pclose(pipe);
	free(command);
	printf("[EXE]: Execute thread done\n");
	pthread_exit(NULL);
}

/**
 * This function processes the clients
 * @param sockfd		the socket id
 * @return status (0 = close, 1 = normal operation)
**/
int processRead(int sockfd)
{
	int n,i;
	int yes = 1;
	int frameShift = 0;
	char buffer[1024];
	char byte = 0;
	char masks[4];
	int commandKey = 0;
	int commandId = 0;
	char commandParam[1024];
	char *decoded;
	char *executeThreadParam; // workaround for empty string in thread
	pthread_t executeThread;
	
	// 0.1 sec timeout for receiving
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	
	n = read(sockfd,buffer,1023);
	if(n > 0) // else no content
	{
		if(buffer[0] == '\x88' && buffer[1] == '\x80') // close event
		{
			return 0;
		}
		else
		{
			if(buffer[0] == '\x81' || buffer[0] == '\0')
			{
				frameShift = 2;
				byte = buffer[1] & 127;
				if(byte == 126)
					frameShift = 4;
				else if(byte == 127)
					frameShift = 10;
				
				for(i = 0; i < 4; i++)
				{
					masks[i] = buffer[frameShift + i];
				}
				
				decoded = malloc((n - frameShift - 4 + 1) * sizeof(char));
				memset(decoded,0,(n - frameShift - 4));
				
				for(i = 0; i < (n - frameShift - 4); i++)
				{
					decoded[i] = (char)(buffer[i + frameShift + 4] ^ masks[i % 4]);
				}
				
				decoded[n - frameShift - 4] = '\0';
				
				// get commandId and commandParam
				sscanf(decoded,"%i:%i:%[^\n]",&commandKey,&commandId,commandParam);
				
				if(commandKey == globalKey)
				{
					// generate command
					char commandPrefix[] = "/usr/share/nginx/html/bin/can-handler - ";
					// n -  frameShift - 4(decode) + strlen(commandPrefix) + 1(\0) + strlen(sockfd) + 1(:)
					decoded = realloc(decoded,(n - frameShift - 4 + strlen(commandPrefix) + 1 + 2 + ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) + 1) * sizeof(char));
					sprintf(decoded,"%i:%s%i %s",sockfd,commandPrefix,commandId,commandParam);
					executeThreadParam = malloc((n - frameShift - 4 + strlen(commandPrefix) + 1 + 2 + ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) + 1) * sizeof(char));
					memcpy(executeThreadParam,decoded,(n - frameShift - 4 + strlen(commandPrefix) + 1 + 2 + ((sockfd > 0)?((int)log10((double)sockfd) + 1):(1)) + 1) * sizeof(char));
					// create thread to handle command execution
					pthread_create(&executeThread,NULL,executeCommand,(void *)executeThreadParam);
				}
				else
				{
					fprintf(stderr,"[%3i]: Error: Global key not matched.\nNothing to do.\n",sockfd);
				}
				free(decoded);
			}
			else
			{
				printf("[%3i]: Error: Communication error.\n",sockfd);
			}
		}
	}
	return 1;
}

void *process(void *param)
{
	int n;
	int sockfd = *(int *)param;
	char buffer[1024];
	
	clientworkerAmount += 1;
	clientworkerRead += 1;
	
	printf("[%3i]: clientWorker start, %i clientworker(s) started, handshake:",sockfd,clientworkerAmount);
	
	// perform handshake
	memset(buffer,1024,0);
	if((n = read(sockfd,buffer,1023)) < 0)
	{
		fprintf(stderr,"[%3i]: Error: Failed to read from socket.\nWill exit now.\n",sockfd);
		exit(1);
	}
	
	buffer[n] = '\0';
	snprintf(buffer,1024,"HTTP/1.1 101 Websocket Protocol Handshake\r\nUpgrade: Websocket\r\nConnection: Upgrade\r\nSec-Websocket-Accept: %s\r\nSec-WebSocket-Protocol: chat\r\n\r\n",generateKey(buffer));
	write(sockfd,buffer,strlen(buffer));
	printf(" done.\n");
	
	// handle communication
	while(1)
	{
		// close server socket
		if(processRead(sockfd) == 0)
			break;
		
		if(strlen(clientworkerBuffer) > 0)
		{
			processSend(sockfd,clientworkerBuffer);
			clientworkerRead += 1;
			if(clientworkerRead == clientworkerAmount)
				clientworkerBuffer[0] = '\0';
		}
	}
	
	close(sockfd);
	
	clientworkerRead -= 1;
	clientworkerAmount -= 1;
	
	printf("[%3i]: clientWorker done, %i clientworker(s) left\n",sockfd,clientworkerAmount);
	
	pthread_exit(NULL);
}

void *processIpc(void *param)
{
	int n,i;
	int sockfd = *(int *)param;
	char buffer[1024];
	
	printf("[IPC]: clientWorker start\n");
	
	// 0.1 sec timeout for receiving
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));
	
	memset(buffer,1024,0);
	if((n = read(sockfd,buffer,1023)) < 0)
	{
		fprintf(stderr,"Error: Failed to read from socket.\nWill exit now.\n");
		exit(1);
	}
	buffer[n] = '\0';
	printf("[IPC]: Received string: '%s'\n[IPC]: Waiting for clientworkers... ",buffer);
	
	// cleaning up
	close(sockfd);
	
	// transfer to websocket clientworker
	while(clientworkerRead < clientworkerAmount);
	printf(" done.\n");
	clientworkerRead = 0;
	memset(clientworkerBuffer,1024,0);
	for(i = 0; i < n; i++)
	{
		clientworkerBuffer[i] = buffer[i];
	}
	clientworkerBuffer[n] = '\0';
	
	printf("[IPC]: clientWorker done\n");
	
	pthread_exit(NULL);
}

void *ipcServer(void *threadId)
{
	pthread_t ipcClientworkerThread;
	
	printf("[IPC]: Starting server\n");
	int sockfd,newsockfd,clilen;
	int yes = 1;
	struct sockaddr_in servaddr,cliaddr;
	
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		fprintf(stderr,"Error: [IPC]: Failed to create ipc socket.\nWill exit now.\n");
		exit(1);
	}
	
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	
	memset(&servaddr,sizeof(servaddr),0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT_IPC);
	
	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		fprintf(stderr,"Error: [IPC]: Failed to bind ipc. Maybe try again later.\nWill exit now.\n");
		exit(1);
	}
	
	if(listen(sockfd,5) < 0)
	{
		fprintf(stderr,"Error: [IPC]: Failed to listen ipc.\nWill exit now.\n");
		exit(1);
	}
	
	printf("[IPC]: Server started on port %i\n",PORT_IPC);
	
	clilen = sizeof(cliaddr);
	while(1)
	{
		if((newsockfd = accept(sockfd,(struct sockaddr *)&cliaddr,(socklen_t *)&clilen)) < 0)
		{
			fprintf(stderr,"Error: [IPC]: Failed to accept ipc.\nWill exit now.\n");
			exit(1);
		}
		printf("[IPC]: Incoming connection from %i.%i.%i.%i\n",(int)(cliaddr.sin_addr.s_addr & 0xFF),(int)((cliaddr.sin_addr.s_addr & 0xFF00) >> 8),(int)((cliaddr.sin_addr.s_addr & 0xFF0000) >> 16),(int)((cliaddr.sin_addr.s_addr & 0xFF000000) >> 24));
		if((int)(cliaddr.sin_addr.s_addr) == inet_addr("127.0.0.1"))
		{
			pthread_create(&ipcClientworkerThread,NULL,processIpc,&newsockfd);
		}
		else
		{
			printf("[IPC]: Error: Connecting rejected.\n");
			close(newsockfd);
		}
	}
	
	close(sockfd);
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t ipcThread,clientworkerThread;
	
	// disable buffering output to make (s)printf thread-safe
	setbuf(stdout,NULL);
	setbuf(stdin,NULL);
	
	// starting ipc server as thread
	pthread_create(&ipcThread,NULL,ipcServer,NULL);
	
	printf("[LOG]: Starting server\n");
	
	int yes = 1;
	int sockfd,newsockfd,clilen;
	struct sockaddr_in servaddr,cliaddr;
	
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		fprintf(stderr,"Error: Failed to create socket.\nWill exit now.\n");
		exit(1);
	}
	
	memset(&servaddr,sizeof(servaddr),0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT_WEBSOCKET);
	
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	
	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		fprintf(stderr,"Error: Failed to bind. Maybe try again later.\nWill exit now.\n");
		exit(1);
	}
	
	if(listen(sockfd,5) < 0)
	{
		fprintf(stderr,"Error: Failed to listen.\nWill exit now.\n");
		exit(1);
	}
	
	printf("[LOG]: Server started on port %i\n",PORT_WEBSOCKET);
	
	clilen = sizeof(cliaddr);
	while(1)
	{
		if((newsockfd = accept(sockfd,(struct sockaddr *)&cliaddr,(socklen_t *)&clilen)) < 0)
		{
			fprintf(stderr,"Error: Failed to accept.\nWill exit now.\n");
			exit(1);
		}
		printf("[%3i]: Incoming connection from %i.%i.%i.%i\n",newsockfd,(int)(cliaddr.sin_addr.s_addr & 0xFF),(int)((cliaddr.sin_addr.s_addr & 0xFF00) >> 8),(int)((cliaddr.sin_addr.s_addr & 0xFF0000) >> 16),(int)((cliaddr.sin_addr.s_addr & 0xFF000000) >> 24));
		pthread_create(&clientworkerThread,NULL,process,&newsockfd);
	}
	
	close(sockfd);
	
	return 0;
}
