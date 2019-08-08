/*Program to emulate image processing part of
 * E101 library on PC
 * Either Linux or mingw
 * Use at your own risk.
 * Feel free to modify */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminocity
// for pixel located at (row,column)
unsigned char get_pixel( int row,int col, int color)
{
    // calculate address in 1D array of pixels
    int address = CAMERA_WIDTH*row*3 + col*3;
    if ((row < 0 ) || (row > CAMERA_HEIGHT) )
    {
        printf("row is out of range\n");
        return -1;
    }
    if ( (col< 0) || (col > CAMERA_WIDTH))
    {
        printf("column is out of range\n");
        return -1;
    }

    if (color==0)
    {
        return (pixels_buf[address]);
    }
    if (color==1)
    {
        return (pixels_buf[address + 1]);
    }
    if (color==2)
    {
        return (pixels_buf[address + 2]);
    }
    if (color==3)
    {
        unsigned char y = (pixels_buf[address] + pixels_buf[address+1] +pixels_buf[address+2])/3;
        return y;
    }
    printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
    return -2; //error
}

int set_pixel(int row, int col, char red, char green,char blue)
{
    int address = CAMERA_WIDTH*row*3 + col*3;
    if ((address < 0) || (address>CAMERA_WIDTH*CAMERA_HEIGHT*4))
    {
        printf("SetPixel(): wrong x,y coordinates\n");
        return -1;
    }
    pixels_buf[address] = red;
    pixels_buf[address+1] = green;
    pixels_buf[address+2] = blue;
    return 0;
}


int ReadPPM(const char *filename)
{
  //char buff[16];
  FILE *fp=fopen(filename, "rb");
   if (!fp) {
     printf("Unable to open file '%s'\n", filename);
     return -1;
  }
  // read the header
  char ch;
  if ( fscanf(fp,"P%c\n",&ch) != 1 || ch != '6')
  {
	  printf("file is wrong format\n");
	  return -2;
  }
  // skip comments
  ch = getc(fp);
  while(ch == '#')
  {
	  do {
		  ch = getc(fp);
	  } while (ch != '\n');
	  ch = getc(fp);
  }
 
   if (!isdigit(ch))  printf("Wrong header\n");
   ungetc(ch,fp);  
  //read width,height and max color value
  int width, height, maxval;
  int res = fscanf(fp,"%d%d%d\n",&width,&height,&maxval);
  printf("Open file: width=%d height=%d\n",width,height);
  

  int size = width*height*3;
  
  int num =fread((void*) pixels_buf, 1,size,fp);
  if (num!=size) {
	printf("can not allocate image data memory: file=%s num=%d size=%d\n",
	filename,num,size);
	return -3;
   }
  fclose(fp);
  return 0;

} 


int SavePPM(char fn[5]){
  //save image into ppm file
  FILE *fp;
  char fname[9];
  sprintf(fname,"%s",fn);
  fp = fopen(fname,"wb");
  if ( !fp){
           printf("Unable to open the file\n");
           return -1;
   }
   // write file header
   fprintf(fp,"P6\n %d %d %d\n",CAMERA_WIDTH , CAMERA_HEIGHT,255);
   int ind = 0;
   int row = 0;
   int col = 0;
   char red;
   char green;
   char blue;
   for ( row = 0 ; row < CAMERA_HEIGHT; row++){
     for ( col = 0 ; col < CAMERA_WIDTH; col++){
        red =  pixels_buf[ind];
		green =  pixels_buf[ind+1];
		blue =  pixels_buf[ind+2];
		fprintf(fp,"%c%c%c",red,green,blue);
		ind = ind + 3;
     }
   }
   fflush(fp);
   fclose(fp);
   return 0;
}

class imageProcessing {
private:
	static const double ratioThreshold = 1.5;
	static const double redThreshold = 150;

public:
	/**
	* Returns true if there is at least one red pixel in the given image
	*/
	bool isRedPresent() {
		for (int i = 0; i < CAMERA_HEIGHT; i ++) {
			for (int j = 0; j < CAMERA_HEIGHT; j ++) {
				if (pixelIsRed(i, j)) return true;
			}
		}
		return false;
	}

	/**
	* Returns true if the pixel is red, and false otherwise
	*/
	bool pixelIsRed(int row, int col) {
		double red = (double)get_pixel(row, col, 0);
		double green = (double)get_pixel(row, col, 1);
		double blue = (double)get_pixel(row, col, 2);

		if (red/blue > ratioThreshold && red/green > ratioThreshold && red > redThreshold)
			return true;
		else return false;
	}

	/**
	* Colours the image red/white depending on whether each pixel is red or not
	*/
	void posterizeRed() {
		for (int i = 0; i < CAMERA_HEIGHT; i ++) {
			for (int j = 0; j < CAMERA_WIDTH; j ++) {
				if (pixelIsRed(i, j)) set_pixel(i, j, 255, 0, 0);
				else set_pixel(i, j, 255, 255, 255);
			}
		}
	}

	void findRedObject() {
		int redInRow [CAMERA_HEIGHT] = { 0 };
		int redInCol [CAMERA_WIDTH] = { 0 };
		for (int i = 0; i < CAMERA_HEIGHT; i ++) {
			for (int j = 0; j < CAMERA_WIDTH; j ++) {
				if (pixelIsRed(i, j)) {
					redInRow[i] ++;
					redInCol[j] ++;
				}
			}
		}

		// Find the row with the largest amount of red
		int max = 0;
		int reddestRow;
		for (int i = 0; i < CAMERA_HEIGHT; i ++) {
			if (redInRow[i] > max) {
				reddestRow = i;
				max = redInRow[i];
			}
		}

		// Find the column with the largest amount of red
		max = 0;
		int reddestCol;
		for (int i = 0; i < CAMERA_WIDTH; i ++) {
			if (redInCol[i] > max) {
				reddestCol = i;
				max = redInCol[i];
			}
		}

		// Draw a cross at the object location
		for (int i = reddestRow - 5; i <= reddestRow + 5; i ++) set_pixel(i, reddestCol, 0, 0, 0);
		for (int i = reddestCol - 5; i <= reddestCol + 5; i ++) set_pixel(reddestRow, i, 0, 0, 0);

		printf("Object coordinates: (%d, %d)\n", reddestCol, reddestRow);
	}
};

int main()
{
	// enter image file name
	char file_name[7];
	printf(" Enter input image file name(with extension:\n");
	scanf("%s",file_name);
	printf(" You enter:%s\n",file_name);
	// read image file
	if (ReadPPM(file_name) != 0){
		printf(" Can not open file\n");
		return -1;
	};
	
	// do your processing here
	imageProcessing ip;
	printf("isRedPresent: %d\n", ip.isRedPresent());
	ip.posterizeRed();
	ip.findRedObject();
	
	printf(" Enter output image file name(with extension:\n");
	scanf("%s",file_name);
	printf(" You enter:%s\n",file_name);
	if (SavePPM(file_name) != 0){
		printf(" Can not save file\n");
		return -1;
	};
	
    
	return 0;
}
