/**
 * Execute command
 * @param commandId		command-ID to identify command (will be send to chip)
 * @param commandParam	command-parameters for the command
**/
void execCommands(int commandId, char *commandParam)
{
	switch(commandId)
	{
		//  take picture and output the calculated distance between robot and
		// wall
		case 0:
		{
			imageRecognition();
			/*time_t current = time(NULL);
			int seconds = (int)current;
			int count = 0;
			while(seconds)
			{
				seconds /= 10;
				count++;
			}
			char *commandTemp = malloc((70 + 13 + count + 4) * sizeof(char));
			sprintf(commandTemp,"/opt/vc/bin/raspistill -w 640 -h 480 -n -t 0 -o /usr/share/nginx/html/sensor-images/%i.jpg",(int)current);
			if(DEBUG)
				printf("Capturing the picture...");
			system(commandTemp);
			if(DEBUG)
				printf(" done.\n");
			free(commandTemp);
			int thresholdMax = 0;
			int thresholdMin = 0;
			int clipping = 0;
			sscanf(commandParam,"%i %i %i",&thresholdMin,&thresholdMax,&clipping);
			if(fileExists("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMin") && fileExists("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMax") && fileExists("/usr/share/nginx/html/bin/imageRecognitionValues/clipping"))
			{
				FILE *imageRecognitionValue;
				imageRecognitionValue = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMin","w");
				fprintf(imageRecognitionValue,"%i",thresholdMin);
				fclose(imageRecognitionValue);
				imageRecognitionValue = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/thresholdMax","w");
				fprintf(imageRecognitionValue,"%i",thresholdMax);
				fclose(imageRecognitionValue);
				imageRecognitionValue = fopen("/usr/share/nginx/html/bin/imageRecognitionValues/clipping","w");
				fprintf(imageRecognitionValue,"%i",clipping);
				fclose(imageRecognitionValue);
			}
			if(DEBUG == 1)
			{
				commandTemp = malloc((79 + count + 4) * sizeof(char));
				sprintf(commandTemp,"php -f /usr/share/nginx/html/bin/imageRecognition.php /usr/share/nginx/html/sensor-images/%i.jpg d",(int)current);
			}
			else
			{
				commandTemp = malloc((77 + count + 4) * sizeof(char));
				sprintf(commandTemp,"php -f /usr/share/nginx/html/bin/imageRecognition.php /usr/share/nginx/html/sensor-images/%i.jpg",(int)current);
			}
			if(DEBUG)
				printf("Processing the image... ");
			system(commandTemp);*/
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
			
			for(int i = 0; i < 100; i++)
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
			
			for(int i = 0; i < 100; i++)
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
		//  set fan speed <speed:0-100(percent)>
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
		//  CALLBACK: drive motors <steps:amountOfSteps> <leftDir:0(reverse),1(forward)>
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
		//  piezo beep <frequency:frequency(Hz)> <duration:duration(ms)>
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
		//  piezo beep test <duration:duration(ms)>
		// <stopping:timeBetweenTone(ms)>
		case 7:
		{
			int duration;
			int stopping;
			int frequencyArray[40] = {523,587,659,698,784,784,784,784,880,880,880,880,784,784,784,784,880,880,880,880,784,784,784,784,698,698,698,698,659,659,659,659,784,784,784,784,523,523,523,523};
			char commandParamBuffer[255];
			
			sscanf(commandParam,"%i %i",&duration,&stopping);
			
			for(int i = 0; i < sizeof(frequencyArray) / sizeof(frequencyArray[0]); i++)
			{
				snprintf(commandParamBuffer,255,"%i %i",frequencyArray[i],duration);
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
			char commandParamBuffer[255];
			sscanf(commandParam,"%f %i",&mm,&direction);
			steps = convertMmToSteps(mm);
			snprintf(commandParamBuffer,255,"%i %i %i",steps,direction,direction);
			execCommands(5,commandParamBuffer);
			break;
		}
		//  CALLBACK: turn angle <angle:degree> <direction:0(atClock),1(againstClock)>
		case 9:
		{
			float angle = 0.0;
			int direction = 0;
			int steps = 0;
			char commandParamBuffer[255];
			sscanf(commandParam,"%f %i",&angle,&direction);
			steps = convertAngleToSteps(angle);
			snprintf(commandParamBuffer,255,"%i %i %i",steps,(direction == 0) ? 1 : 0,direction);
			execCommands(5,commandParamBuffer);
			break;
		}
		//  drive from (0|0) to position <positionX:mm> <positionY:mm> and output
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
		// if the users inputs a command-ID which is not supported output:
		// "no such command"
		default:
		{
			printf("%i:%s: %sno such command%s\n",commandId,commandParam,red,reset);
			break;
		}
	}
}
