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

	/**
	* Draws a black cross on the intersection of the row and column with the most red
	*/
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
