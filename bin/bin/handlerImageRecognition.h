void imageRecognition(void)
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	int width = 0;
	int height = 0;
	int rowStride = 0;
	JSAMPARRAY rowPointer;
	FILE *pipe;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	
	// preparing pipe
	if((pipe = popen("/opt/vc/bin/raspistill -w 640 -h 480 -n -t 0 -o -","r")) == NULL)
	{
		fprintf(stderr,"Failed to open pipe\nExit now.\n");
		exit(1);
	}
	
	// create jpeg decompressor
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,pipe);
	(void)jpeg_read_header(&cinfo,TRUE);
	(void)jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;
	rowStride = width * cinfo.output_components;
	rowPointer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,rowStride,1);
	
	printf("Read JPEG...");
	
	// create pixel array
	int ***pixel;
	pixel = malloc(height * sizeof(int **));
	for(int y = 0; y < height; y++)
	{
		pixel[y] = malloc(width * sizeof(int *));
		for(int x = 0; x < width; x++)
		{
			pixel[y][x] = malloc(3 * sizeof(int));
		}
	}
	
	// read lines of image
	for(int y = 0; cinfo.output_scanline < cinfo.output_height; y++)
	{
		(void)jpeg_read_scanlines(&cinfo,rowPointer,1);
		for(int x = 0; x < cinfo.output_width; x++)
		{
			r = rowPointer[0][cinfo.output_components * x];
			if(cinfo.output_components > 2)
			{
				g = rowPointer[0][cinfo.output_components * x + 1];
				b = rowPointer[0][cinfo.output_components * x + 2];
			}
			else
			{
				g = r;
				b = r;
			}
			pixel[y][x][0] = r;
			pixel[y][x][1] = g;
			pixel[y][x][2] = b;
		}
	}
	
	printf(" done.\nProcessing...");
	
	// process the image
	double averageX = 0;
	double averageY = 0;
	int averageCount = 0;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			// contrast
			pixel[y][x][0] = (pixel[y][x][0] < 128) ? 0 : 255;
			pixel[y][x][1] = (pixel[y][x][1] < 128) ? 0 : 255;
			pixel[y][x][2] = (pixel[y][x][2] < 128) ? 0 : 255;
			
			/*// grayscale
			pixel[y][x][0] = (pixel[y][x][0] + pixel[y][x][1] + pixel[y][x][2]) / 3;
			pixel[y][x][1] = (pixel[y][x][0] + pixel[y][x][1] + pixel[y][x][2]) / 3;
			pixel[y][x][2] = (pixel[y][x][0] + pixel[y][x][1] + pixel[y][x][2]) / 3;
			
			// contrast
			pixel[y][x][0] = (pixel[y][x][0] < 128) ? 0 : 255;
			pixel[y][x][1] = (pixel[y][x][1] < 128) ? 0 : 255;
			pixel[y][x][2] = (pixel[y][x][2] < 128) ? 0 : 255;*/
			
			// calculate average for all red pixels
			if(pixel[y][x][0] == 255 && pixel[y][x][1] == 0 && pixel[y][x][2] == 0)
			{
				averageX += x;
				averageY += y;
				averageCount += 1;
			}
		}
	}
	
	if(averageCount != 0)
	{
		averageX /= averageCount;
		averageY /= averageCount;
		printf(" laser at %f %f. %i pixels found.",averageX,averageY,averageCount);
	}
	else
	{
		printf(" laser out of range.");
	}
	
	printf(" done.\n");
	
	// freeing memory
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			free(pixel[y][x]);
		}
		free(pixel[y]);
	}
	free(pixel);
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	pclose(pipe);
}
