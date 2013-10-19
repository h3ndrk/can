FILE *image;
int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;

char *image_filename;

void image_getFilename(int filenameSecond)
{
	int seconds = filenameSecond;
	int count = 0;
	while(seconds)
	{
		seconds /= 10;
		count++;
	}
	image_filename = malloc((23 + count + 4) * sizeof(char));
	sprintf(image_filename,"/usr/share/nginx/html/sensor-images/%i.png",filenameSecond);
}

void image_loadImage(void)
{
	if(DEBUG == 1)
		printf("Loading image '%s'...",image_filename);
	
	char buffer[255];
	sprintf(buffer,"chmod 0666 %s",image_filename);
	system(buffer);
	system("sync");
	system("sleep 1");
	
	if(fileExists(image_filename))
	{
		/* VALIDATE IF THE IMAGE IS A VALID PNG FILE */
		image = fopen(image_filename,"rb");
		if(!image)
		{
			if(DEBUG == 1)
				printf("\n%sError: failed to open sensor image. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
			exit(1);
		}
		else
		{
			char header[8];
			fread(header,1,8,image);
			if(png_sig_cmp((png_bytep)header,0,8))
			{
				if(DEBUG == 1)
					printf("\n%sError: failed to open sensor image: no valid PNG-FILE. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
				exit(1);
			}
		}
		
		/* GENERATE BASIC INFORMATIONS */
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
		if(!png_ptr)
		{
			if(DEBUG == 1)
				printf("\n%sError: png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
			exit(1);
		}
		
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if(!info_ptr)
		{
			png_destroy_read_struct(&png_ptr,(png_infopp)NULL,(png_infopp)NULL);
			if(DEBUG == 1)
				printf("\n%sError: png_create_info_struct(png_ptr): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
			exit(1);
		}
		
		png_init_io(png_ptr,image);
		png_set_sig_bytes(png_ptr,8);
		
		png_read_info(png_ptr,info_ptr);
		
        width = png_get_image_width(png_ptr,info_ptr);
		height = png_get_image_height(png_ptr,info_ptr);
		color_type = png_get_color_type(png_ptr,info_ptr);
		bit_depth = png_get_bit_depth(png_ptr,info_ptr);
		
		number_of_passes = png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr,info_ptr);
		
		printf(" %ix%ipx",width,height);
		
		/* RESETTING */		
		if(setjmp(png_jmpbuf(png_ptr)))
		{
			if(DEBUG == 1)
				printf("\n%sError: setjmp(png_jmpbuf(png_ptr)): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
			exit(1);
		}
		
		/* GENERATE DATA STRUCTURE */
		row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
		for(int y = 0; y < height; y++)
			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
		
		/* READING THE IMAGE */
		png_read_image(png_ptr,row_pointers);
		
		fclose(image);
	}
	else
	{
		if(DEBUG == 1)
		{
			printf("\n%sError: failed to open sensor image: no such file or directory. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
			system("ls -al /usr/share/nginx/html/sensor-images");
		}
		exit(1);
	}
	
	if(DEBUG == 1)
		printf(" [ %sOK%s ]\n",green,reset);
}

void image_writeImage(void)
{
	if(DEBUG == 1)
		printf("Writing image to '%s'...",image_filename);
	
	/* create file */
	image = fopen(image_filename,"wb");
	if(!image)
	{
		if(DEBUG == 1)
			printf("\n%sError: failed to open sensor image. Exit now.\n > 'void image_writeImage(void)'%s\n",red,reset);
		exit(1);
	}
	
	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if(!png_ptr)
	{
		if(DEBUG == 1)
			printf("\n%sError: png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL): FAIL. Exit now.%s\n > 'void image_writeImage(void)'\n",red,reset);
		exit(1);
	}
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr)
	{
		printf("\n%sError: png_create_info_struct(png_ptr): FAIL. Exit now.\n > 'void image_writeImage(void)'%s\n",red,reset);
		exit(1);
	}
	
	png_init_io(png_ptr,image);
	
	/* RESETTING */		
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		if(DEBUG == 1)
			printf("\n%sError: setjmp(png_jmpbuf(png_ptr)): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
		exit(1);
	}
	
	/* GENERATE AND WRITE HEADER OF IMAGE */
	png_set_IHDR(png_ptr,info_ptr,width,height,bit_depth,color_type,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);
	
	png_write_info(png_ptr,info_ptr);
	
	/* RESETTING */		
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		if(DEBUG == 1)
			printf("\n%sError: setjmp(png_jmpbuf(png_ptr)): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
		exit(1);
	}
	
	/* WRITE IMAGE */
	png_write_image(png_ptr,row_pointers);
	
	/* RESETTING */		
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		if(DEBUG == 1)
			printf("\n%sError: setjmp(png_jmpbuf(png_ptr)): FAIL. Exit now.\n > 'void image_loadImage(void)'%s\n",red,reset);
		exit(1);
	}
	
	/* WRITE END OF IMAGE */
	png_write_end(png_ptr,NULL);
	
	fclose(image);
	
	if(DEBUG == 1)
		printf(" [ %sOK%s ]\n",green,reset);
}

int image_getPixel(int x, int y)
{
	png_byte *row = row_pointers[y];
	png_byte *ptr = &(row[x * 4]);
	return (ptr[0] + ptr[1] + ptr[2]) / 3;
}

void image_setPixel(int x, int y, int intensity)
{
	png_byte *row = row_pointers[y];
	png_byte *ptr = &(row[x * 4]);
	ptr[0] = intensity;
	ptr[1] = intensity;
	ptr[2] = intensity;
}

void image_processImage(int filenameSecond)
{
	/* GET FILENAME */
	image_getFilename(filenameSecond);
	
	/* LOADING IMAGE FROM FILE */
	image_loadImage();
	
	/* PROCESS THE IMAGE */
	if(DEBUG == 1)
		printf("Process the image...");
	
	int thresholdMin = 254;
	int thresholdMax = 255;
	//int averageScattering = 50;
	
	int averageX = 0;
	int averageY = 0;
	int amount = 0;
	
	/* GRAYSCALE + THRESHOLD */
	for(int threshold = thresholdMin; threshold < thresholdMax; threshold++)
	{
		for(int y = ((height / 2) - 30); y < ((height / 2) + 30); y++)
		{
			png_byte *row = row_pointers[y];
			for(int x = 0; x < width; x++)
			{
				png_byte *ptr = &(row[x * 4]);
				if(((ptr[0] + ptr[1] + ptr[2]) / 3) >= threshold)
				{
					//if((amount > 10 && x + averageScattering <= averageX && x - averageScattering >= averageX && y + averageScattering <= averageY && y - averageScattering >= averageY) || (amount <= 10))
					//{
						averageX += x;
						averageY += y;
						amount += 1;
					//}
				
					ptr[0] = 0;
					ptr[1] = 0;
					ptr[2] = 255;/*
				}
				else
				{
					ptr[0] = 0;
					ptr[1] = 0;
					ptr[2] = 0;*/
				}
			}
		}
	}
	
	/* GET CENTER OF LASER */
	if(amount != 0)
	{
		averageX = (averageX / amount) + 1;
		averageY = (averageY / amount) + 1;
	}
	else
	{
		averageX = width / 2;
		averageY = height / 2;
	}
	
	/* DRAWING THE ACCEPTED RANGE WITH BLUE LINES */
	for(int x = 0; x < width; x++)
	{
		png_byte *row = row_pointers[(height / 2) + 30];
		png_byte *ptr = &(row[x * 4]);
		ptr[0] = 0;
		ptr[1] = 0;
		ptr[2] = 255;
	}
	for(int x = 0; x < width; x++)
	{
		png_byte *row = row_pointers[(height / 2) - 30];
		png_byte *ptr = &(row[x * 4]);
		ptr[0] = 0;
		ptr[1] = 0;
		ptr[2] = 255;
	}
	
	/* DRAWING THE CENTER OF LASER WITH RED LINES */
	if(amount != 0)
	{
		for(int y = 0; y < height; y++)
		{
			png_byte *row = row_pointers[y];
			png_byte *ptr = &(row[averageX * 4]);
			ptr[0] = 255;
			ptr[1] = 0;
			ptr[2] = 0;
		}
		for(int x = 0; x < width; x++)
		{
			png_byte *row = row_pointers[averageY];
			png_byte *ptr = &(row[x * 4]);
			ptr[0] = 255;
			ptr[1] = 0;
			ptr[2] = 0;
		}
	}
	
	printf("%i,%i\n",averageX,averageY);
	
	if(DEBUG == 1)
		printf(" [ %sOK%s ]\n",green,reset);
	
	/* WRITING IMAGE TO FILE */
	//image_writeImage();
}

void image_freeImage(void)
{
	/*for(int y = 0; y < height; y++)
	{
		printf("Freeing %i...\n",y);
		free(row_pointers[y]);
	}*/
	if(DEBUG == 1)
		printf("Free image memory...");
	free(row_pointers);
	if(DEBUG == 1)
		printf(" [ %sOK%s ]\n",green,reset);
}
