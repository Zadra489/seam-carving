# Seam Carving Algorithm

This project was an assignment from ESC190: Data Structures and Algorithms
Credit: the assignment was originally designed by Josh Hug. Port to C by Michael Guerzhoy.

## Purpose: 

This program defines header files with functions that allow a user to dynamically shrink an image while maintaining key features using an energy function
For use, import and use as header files in your C program. 

## Details:

c_img.c was provided for this assignment and is needed for file type conversion such that seamcarving.c can run efficiently.
For the final purpose of seam carving, seamcarving.h provides all the functions one would need. 

## Included Functions in seamcarving.h:

### void calc_energy(struct rgb_img *im, struct rgb_img **grad)

This function utilizes gradient descent to calculate the energy of each pixel, finding how important each pixel is in the makeup of the image
This is done by finding how different each pixel is from it's neighbours using it's RGB values. 

### void dynamic_seam(struct rgb_img *grad, double **best_arr)

This function finds all paths, calculating the lowest total energy to reach each pixel within the image.
Each pixel finds which of the pixels before it has the lowest cost to reach and adds that cost to it's own cost recursively. 

### void recover_path(double *best, int height, int width, int **path)

This function takes in the energy field and calculates the path with the lowest energy usage.
Using dynamic programming principles, this ensures the most optimal path is always taken each epoch. 

### void remove_seam(struct rgb_img *src, struct rgb **dest, int *path) 

All other functions contribute towards this function, intended for user applications
This function removes a seam with the lowest energy in the image, retaining key details.

All other functions assist remove_seam in it's purpose for the sake of readable code. 

