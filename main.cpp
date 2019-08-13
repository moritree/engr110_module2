#include <iostream>
#include "E101.h"
#include <bits/stdc++.h>

using namespace std;

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240 

class ImageProcessing {
private:
	const double ratioThreshold = 1.7;
	const double redThreshold = 100;
	const double kP = 0.035;
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

	/**
	* Returns the coordinates the intersection of the row and column with the most red
	*/
	vector<int> findRedObject() {
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

		printf("Object coordinates: (%d, %d)\n", reddestCol, reddestRow);
		vector<int> vect;
		vect.push_back(reddestRow);
		vect.push_back(reddestCol);

		return vect;
	}
	
	/**
	* Method to determine whether the object located at (row, col) is circular
	*/
	bool isObjectCircular(int row, int col) {
		if (!pixelIsRed(row, col)) return false;
		return true;
		
		// TO DO
		// Flood fill to find leftmost, rightmost, topmost, and bottommost pixels
		// Check inside the corners - if red is found, it is not a circle
	}
	
	/**
	* Returns the X and Y error values of the given coordinates 
	* relative to the center of the screen
	*/
	vector<double> getError(int row, int col) {
		vector<double> err;
		err.push_back((CAMERA_WIDTH / 2 - col) * kP);
		err.push_back((CAMERA_HEIGHT / 2 - row) * kP);
		
		return err;
	}
	
	bool isMoreThanHalf(int objRow, int objCol) { 
		int minX = CAMERA_WIDTH;
		int maxX = 0;
		int minY = CAMERA_HEIGHT;
		int maxY = 0;
		for (int row = 0; row < CAMERA_HEIGHT; row ++) {
			for (int col = 0; col < CAMERA_WIDTH; col ++) {
				if (pixelIsRed(row, col)) {
					if (minX > col) minX = col;
					if (maxX < col) maxX = col;
					if (minY > row) minY = row;
					if (maxY < row) maxY = row;
				}
			}
		}
		
		int dx = maxX - minX;
		int dy = maxY - minY;
		
		return (dx * 2 > dy && dy * 2 > dx);    
	}
};

class MotorControl {
private:
	const int xMotor = 1;
	const int yMotor = 5;
	
	int xCurrent = 48;
	int yCurrent = 48;
	
	int xReset = 52;
	int yReset = 52;
public:
	/**
	* Proportional adjustment control mechanism
	*/
	void adjust(int errX, int errY) {
		xCurrent -= errX;
		yCurrent += errY;
		
		set_motors(xMotor, xCurrent);
		set_motors(yMotor, yCurrent);
		
		hardware_exchange();
	}
	
	/**
	* Sets the motors back to their "reset" position
	*/ 
	void reset() {
		xCurrent = xReset;
		yCurrent = yReset;
		
		set_motors(xMotor, xCurrent);
		set_motors(yMotor, yCurrent);
		
		hardware_exchange();
	}
};

int main()
{
  	int err;
 	 cout<<" Hello"<<endl;
 	 err = init(0);
 	 cout<<"After init() error="<<err<<endl;

  	int count = 0;
	vector<double> error;

 	open_screen_stream();
  
  	ImageProcessing ip;
	MotorControl mc;
  
  	while(count < 100){
		printf("\n- FRAME %d -\n", count);
	  	take_picture();
		
		ip.posterizeRed();
	  
		// Only track the sun if it is found
		if (ip.isRedPresent()) {
			vector<int> coords = ip.findRedObject();
			
			if (ip.isMoreThanHalf(coords[0], coords[1])) {
				error = ip.getError(coords[0], coords[1]);
				mc.adjust((int)error[0], (int)error[1]);
				
				printf("Error: %f, %f\n", error[0], error[1]);
			} else {
				printf("Less than half present\n");
				mc.reset();
			}
		} else {
			printf("No red object\n");
			mc.reset();
		}
		
		update_screen();
		
	  	sleep1(100);
	  
	  	count ++;
  	}
  	stoph();
}

