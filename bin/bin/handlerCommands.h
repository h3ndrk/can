/**
 * Execute command
 * @param commandId		command-ID to identify command (will be send to chip)
 * @param commandParam	command-parameters for the command
**/
void execCommands(int commandId, char *commandParam)
{
	int i;
	switch(commandId)
	{
		//  take picture and output the calculated distance between robot and
		// wall
		case 0:
		{
			imageRecognition();
			break;
		}
		//  test data bus: send and read an amount of bytes through the data bus,
		// will output some timing- and validation-informations
		case 1:
		{
			printf("will now perform speed and validation test with 100 bytes\n");
			
			// transmit command-ID
			dataBusSend(1);
			
			clock_t t;
			
			// perform write test
			printf("write test...");
			
			t = clock();
			
			for(i = 0; i < 100; i++)
			{
				dataBusSend(i);
			}
			
			t = clock() - t;
			
			printf(" %f seconds\n",((float)t)/CLOCKS_PER_SEC);
			
			char byte;
			bool failure = false;
			
			// perform read test
			printf("read test...");
			
			t = clock();
			
			for(i = 0; i < 100; i++)
			{
				byte = dataBusRead();
				if(byte != i)
					failure = true;
			}
			
			t = clock() - t;
			
			printf(" %f seconds\n",((float)t)/CLOCKS_PER_SEC);
			
			// get test result from mikrocontroller
			printf("get test result from mikrocontroller...");
			
			if(dataBusRead() == 1)
				printf(" ok\n");
			else
			{
				printf(" error\n");
				failure = true;
			}
			
			// output test result
			if(!failure)
			{
				printf("test successfully finished\n");
			}
			else
			{
				printf("test finished with errors\n");
			}
			break;
		}
		//  perform chip reset by setting an gpio-pin to low for a short time
		case 2:
		{
			dataBusReset();
			usleep(10000);
			break;
		}
		//  set fan speed <speed:%>
		case 3:
		{
			// transmit command-ID
			dataBusSend(3);
			
			// transmit fan speed
			int speed = 0;
			sscanf(commandParam,"%i",&speed);
			dataBusSend((char)speed);
			char byte = dataBusRead();
			if(byte == 1)
				printf("ok\n");
			else
				printf("error: wrong return byte\n");
			break;
		}
		//  enable/disable motors <state:0(off),1(on)>
		case 4:
		{
			// transmit command-ID
			dataBusSend(4);
			
			// transmit motor state
			int commandParamByte = 0;
			char byte = 0;
			sscanf(commandParam,"%i",&commandParamByte);
			if(commandParamByte == 0)
			{
				dataBusSend(0);
				byte = dataBusRead();
				if(byte == 0)
					printf("disabled\n");
				else
					printf("error: wrong return byte\n");
			}
			else if(commandParamByte == 1)
			{
				dataBusSend(1);
				byte = dataBusRead();
				if(byte == 1)
					printf("enabled\n");
				else
					printf("error: wrong return byte\n");
			}
			else
			{
				printf("error: wrong parameter\n");
			}
			break;
		}
		//  CALLBACK: drive motors <steps:amount> <leftDir:0(reverse),1(forward)>
		// <rightDir:0(reverse),1(forward)>
		case 5:
		{
			// transmit command-ID
			dataBusSend(5);
			
			// transmit motor parameters
			int steps = 0;
			int leftDir = 0;
			int rightDir = 0;
			char byte = 0;
			sscanf(commandParam,"%i %i %i",&steps,&leftDir,&rightDir);
			dataBusSend((char)steps);		// motor bits	0-7
			dataBusSend((char)(steps>>8));	// motor bits	8-15
			dataBusSend((char)(steps>>16));	// motor bits	16-23
			dataBusSend((char)(steps>>24));	// motor bits	24-31
			dataBusSend((char)leftDir);
			dataBusSend((char)rightDir);
			
			// receive confirmation
			byte = dataBusRead();
			if(byte == 1)
				printf("ok\n");
			else
				printf("error\n");
			break;
		}
		//  piezo beep <frequency:Hz> <duration:ms>
		case 6:
		{
			/* TRANSMIT COMMAND ID */
			dataBusSend(6);
			
			int frequency = 0;
			int duration = 0;
			sscanf(commandParam,"%i %i",&frequency,&duration);
			
			// transmit beep parameters
			dataBusSend((char)frequency);
			dataBusSend((char)(frequency>>8));
			dataBusSend((char)(frequency>>16));
			dataBusSend((char)(frequency>>24));
			dataBusSend((char)duration);
			dataBusSend((char)(duration>>8));
			dataBusSend((char)(duration>>16));
			dataBusSend((char)(duration>>24));
			
			// receive confirmation
			char byte = dataBusRead();
			if(byte == 1)
				printf("ok\n");
			else
				printf("error\n");
			break;
		}
		//  piezo beep test <duration:ms>
		// <stopping:ms>
		case 7:
		{
			int duration;
			int stopping;
			int frequencyArray[40] = {523,587,659,698,784,784,784,784,880,880,880,880,784,784,784,784,880,880,880,880,784,784,784,784,698,698,698,698,659,659,659,659,784,784,784,784,523,523,523,523};
			char commandParamBuffer[256];
			
			sscanf(commandParam,"%i %i",&duration,&stopping);
			
			for(i = 0; i < sizeof(frequencyArray) / sizeof(frequencyArray[0]); i++)
			{
				snprintf(commandParamBuffer,256,"%i %i",frequencyArray[i],duration);
				execCommands(6,commandParamBuffer);
				
				usleep(stopping);
			}
			break;
		}
		//  CALLBACK: drive distance <distance:mm> <direction:0(reverse),1(forward)>
		case 8:
		{
			float mm = 0.0;
			int direction = 0;
			int steps = 0;
			char commandParamBuffer[256];
			sscanf(commandParam,"%f %i",&mm,&direction);
			steps = convertMmToSteps(mm);
			snprintf(commandParamBuffer,256,"%i %i %i",steps,direction,direction);
			execCommands(5,commandParamBuffer);
			break;
		}
		//  CALLBACK: turn angle <angle:degree> <direction:0(atClock),1(againstClock)>
		case 9:
		{
			float angle = 0.0;
			int direction = 0;
			int steps = 0;
			char commandParamBuffer[256];
			sscanf(commandParam,"%f %i",&angle,&direction);
			steps = convertAngleToSteps(angle);
			snprintf(commandParamBuffer,256,"%i %i %i",steps,(direction == 0) ? 1 : 0,direction);
			execCommands(5,commandParamBuffer);
			break;
		}
		//  drive from current position to position <positionX:mm> <positionY:mm> and output
		// real position driven (because of decimal steps)
		case 10:
		{
			float x = 0.0;
			float y = 0.0;
			
			sscanf(commandParam,"%f %f",&x,&y);
			driveToPosition(x,y);
			break;
		}
		//  reset robot position
		case 11:
		{
			resetPosition();
			break;
		}
		//  turn/point to angle <angle:degree>
		case 12:
		{
			float angle = 0.0;
			
			sscanf(commandParam,"%f",&angle);
			driveToAngle(angle);
			break;
		}
		//  get adc-value of given channel via command parameters <channel:number>
		case 13:
		{
			int channel = 0;
			unsigned int value = 0;
			
			// transmit command-ID
			dataBusSend(13);
			
			// transmit adc-channel
			sscanf(commandParam,"%i",&channel);
			dataBusSend((char)channel);
			
			// receive response (16bit) from mikrocontroller
			value = dataBusRead();
			value += (dataBusRead()<<8);
			
			// output the received value
			printf("%i\n",value);
			break;
		}
		//  send all informations to IPC-Socket
		case 14:
		{
			position_t position;
			char buffer[1024];
			FILE *fd;
			char generalHostname[256];
			char generalKernel[256];
			char generalArchitecture[256];
			char generalSystem[256];
			char generalDate[256];
			char generalUptime[256];
			float cpuTemp = 0.0;
			float cpuVolts = 0.0;
			int cpuClock = 0;
			
			// general
			fd = popen("hostname","r");
			fscanf(fd,"%[^\n]",generalHostname);
			pclose(fd);
			
			fd = popen("uname -s","r");
			fscanf(fd,"%[^\n]",generalKernel);
			pclose(fd);
			
			fd = popen("uname -m","r");
			fscanf(fd,"%[^\n]",generalArchitecture);
			pclose(fd);
			
			fd = popen("uname -o","r");
			fscanf(fd,"%[^\n]",generalSystem);
			pclose(fd);
			
			fd = popen("date","r");
			fscanf(fd,"%[^\n]",generalDate);
			pclose(fd);
			
			fd = popen("uptime","r");
			fscanf(fd,"%[^\n]",generalUptime);
			pclose(fd);
			
			snprintf(buffer,1024,"general%s%s%s%s%s%s%s%s%s%s%s%s",ipcDelimiter,generalHostname,ipcDelimiter,generalKernel,ipcDelimiter,generalArchitecture,ipcDelimiter,generalSystem,ipcDelimiter,generalDate,ipcDelimiter,generalUptime);
			
			ipcSend(buffer);

			// position
			position = loadPosition();
			snprintf(buffer,1024,"position%s%f%s%f%s%f%s%s",ipcDelimiter,position.x,ipcDelimiter,position.y,ipcDelimiter,position.angle,ipcDelimiter,ipcDelimiter);
			
			ipcSend(buffer);
			
			// cpu
			fd = fopen("/usr/share/nginx/html/bin/values/cpu","r");
			fscanf(fd,"%f %f %i",&cpuTemp,&cpuVolts,&cpuClock);
			fclose(fd);
			snprintf(buffer,1024,"cpu%s%f%s%f%s%i",ipcDelimiter,cpuTemp,ipcDelimiter,cpuVolts,ipcDelimiter,cpuClock);
			
			ipcSend(buffer);
			break;
		}
		//  Update CPU and fan values
		case 15:
		{
			FILE *cpuFile;
			FILE *fanFile;
			FILE *pipe;
			const int tempLow = 37;
			const int tempHigh = 43;
			float cpuTemp;
			float cpuVolts;
			long cpuClock;
			int newFanSpeed;
			int oldFanSpeed;
			if((cpuFile = fopen("/usr/share/nginx/html/bin/values/cpu","w")) != NULL)
			{
				pipe = popen("/opt/vc/bin/vcgencmd measure_temp | cut -d '=' -f2 | sed 's/..$//'","r");
				fscanf(pipe,"%f",&cpuTemp);
				pclose(pipe);
				pipe = popen("/opt/vc/bin/vcgencmd measure_volts core | cut -d '=' -f2 | sed 's/.$//'","r");
				fscanf(pipe,"%f",&cpuVolts);
				pclose(pipe);
				pipe = popen("/opt/vc/bin/vcgencmd measure_clock arm | cut -d '=' -f2","r");
				fscanf(pipe,"%li",&cpuClock);
				pclose(pipe);
				printf("CPU temperatur: %f\nCPU volts:      %f\nCPU clock:      %li\n",cpuTemp,cpuVolts,cpuClock);
				fprintf(cpuFile,"%f %f %li",cpuTemp,cpuVolts,cpuClock);
				fclose(cpuFile);
				if((fanFile = fopen("/usr/share/nginx/html/bin/values/fan","r")) != NULL)
				{
					fscanf(fanFile,"%i %i",&oldFanSpeed,&newFanSpeed);
					fclose(fanFile);
					if((fanFile = fopen("/usr/share/nginx/html/bin/values/fan","w")) != NULL)
					{
						if(cpuTemp < 37.0) // temperature too low
						{
							newFanSpeed = 0;
						}
						else if(cpuTemp > 43.0) // temperature too high
						{
							newFanSpeed = 100;
						}
						else // temperature in range
						{
							newFanSpeed = (((cpuTemp - tempLow) / (float)(tempHigh - tempLow)) * 100.0);
						}
						printf("OLD fan speed:  %i\nNEW fan speed:  %i\n",oldFanSpeed,newFanSpeed);
						fprintf(fanFile,"%i %i",oldFanSpeed,newFanSpeed);
						fclose(fanFile);
					}
				}
			}
			break;
		}
		// if the users inputs a command-ID which is not supported output:
		// "no such command"
		default:
		{
			printf("%i:%s: no such command\n",commandId,commandParam);
			break;
		}
	}
}
