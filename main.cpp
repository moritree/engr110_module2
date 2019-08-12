#include <iostream>
#include "E101.h"
#include <bits/stdc++.h>

using namespace std;

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240 

class ImageProcessing {
private:
	const double ratioThreshold = 1.5;
	const double redThreshold = 150;
	const double kP = 0.03;
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
	boolean isObjectCircular(int row, int col) {
		if (!pixelIsRed(row, col)) return false;
		return true;
		
		// TO DO
		// Flood fill to find leftmost, rightmost, topmost, and bottommost pixels
		// Check inside the corners - if red is found, it is not a circle
	}
	
	vector<double> getError(int row, int col) {
		vector<double> err;
		err.push_back((CAMERA_WIDTH / 2 - row) * Kp);
		err.push_back((CAMERA_HEIGHT / 2 - col) * Kp);
		
		return err;
	}
};

class MotorControl {
private:
	const int xMotor = 1;
	const int yMotor = 5;
	
	int xCurrent = 48;
	int yCurrent = 48;
	
	int xReset = 48;
	int yReset = 48;
public:
	void adjust(int errX, int errY) {
		xCurrent += errX;
		yCurrent += errY;
		
		set_motors(xMotor, xCurrent);
		set_motors(yMotor, yCurrent);
		
		hardware_exchange();
	}
	
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
  
  	int yMid = CAMERA_HEIGHT/2;
  	int count = 0;
	vector<double> error;
  	double Kp = 0.03;

 	open_screen_stream();
  
  	ImageProcessing ip;
	MotorControl mc;
  
  	while(count < 50){
	  	take_picture();
	  	update_screen();
	  
		// Only track the sun if it is found
		if (ip.isRedPresent()) {
			vector<int> coords = ip.findRedObject();
	  		error = ip.getError(coords[0], coords[1]);
			mc.adjust((int)error[0], (int)error[1]);
			
	  		printf("Motor: %d\n",motorAdjust);
	  		printf("error: %f\n",error);
		}
		
	  	sleep1(100);
	  
	  	count ++;
  	}
  	stoph();
}

