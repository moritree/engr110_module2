# engr110_module2

# Documentation

## Motors

**Motor 5: Top motor (Y axis)**

- 48 -> Reset to midpoint
- greater than 48 -> down
- less than 48 -> up

**Motor 1: Bottom motor (X axis)**

- 48 -> Reset to midpoint
- greater than 48 -> right
- less than 48 -> left

## imageProcessing class

This class handles the image processing aspects of the project.

### Detecting red

#### Determining whether a pixel is red

This is done in the `bool pixelIsRed(int row, int col)` method - you give it the coordinates of a pixel, and it will return true if the pixel is red. Simply checking that the red value of the pixel is above a certain threshold is not enough to know if a pixel is red, as white will also have a high red value. We must also check that the red/blue and red/green ratios are above a threshold. These thresholds are stored as ratioThreshold and redThreshold and have been experimentally determined in order to accurately detect red.

#### Determining whether a red object is present
 
`bool isRedPresent()` will return true if at least one red pixel is found in the image.

### Finding an object's coordinates

This is done with the `vector<int> findRedObject()` method, which will return the row and column pixel coordinates of a red object on the screen.

This uses a rudimentary algorithm, which is good enough for simple cases where there is only one object. For each row and each column, we count the number of red pixels in the row/column. From there, the centre is determined to be the intersection of the row and column with the largest number of red pixels.

This algorithm does not tell you anything about the shape of the object, and would likely not work if there were more than one object on screen.

### Output

`posterizeRed()` colours each pixel in the image red or white, depending on whether it is red.
