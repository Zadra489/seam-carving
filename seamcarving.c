#include "seamcarving.h"
#include "c_img.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void calc_energy(struct rgb_img *im, struct rgb_img **grad){

    create_img(grad, im->height, im->width);

    int y = 0;
    for (y=0;y<im->height;y++){
        int x = 0;
        for (x=0;x<im->width;x++){
            int left = (x - 1 + im->width)  % im->width;
            int right = (x + 1) % im->width;
            int up = (y - 1 + im->height) % im->height;
            int down = (y + 1) % im->height;

            int r_x = get_pixel(im, y, left, 0) - get_pixel(im, y, right, 0);
            int r_y = get_pixel(im, up, x, 0) - get_pixel(im, down, x, 0);
            int g_x = get_pixel(im, y, left, 1) - get_pixel(im, y, right, 1);
            int g_y = get_pixel(im, up, x, 1) - get_pixel(im, down, x, 1);
            int b_x = get_pixel(im, y, left, 2) - get_pixel(im, y, right, 2);
            int b_y = get_pixel(im, up, x, 2) - get_pixel(im, down, x, 2);

            double delta_x = r_x*r_x + g_x*g_x + b_x*b_x;
            double delta_y = r_y*r_y + g_y*g_y + b_y*b_y;

            double energy = sqrt(delta_x + delta_y);

            uint8_t val = (uint8_t)(energy/10);
            set_pixel(*grad, y, x, val, val, val);

        }
    }

}
void dynamic_seam(struct rgb_img *grad, double **best_arr){

    int w = grad->width;
    int h = grad->height;
    *best_arr = malloc(h * w * sizeof(double));

    int x = 1;
    for (x=0;x<w;x++){
        (*best_arr)[x] = get_pixel(grad, 0, x, 0);
    }
    int y = 0;
    for (y = 1; y<h; y++){
        int x = 0;
        for (x=0; x<w; x++){
            double above_m;
            above_m = (*best_arr)[(y-1)*w + x];
            double above_l;
            if (x > 0){
                above_l = (*best_arr)[(y-1)*w + (x-1)];
            }
            else {
            above_l = above_m;
            }
            double above_r;
            if (x < w-1){
                above_r = (*best_arr)[(y-1)*w + (x+1)];
            } else {
                above_r = above_m;
            }
            double cur_min;
            cur_min = above_m;
            if (above_l  < cur_min){
                cur_min = above_l;
            }
            if (above_r < cur_min)
                {cur_min = above_r;
            }
            (*best_arr)[y*w + x] = get_pixel(grad, y, x, 0) + cur_min;
        }
    }
}
void recover_path(double *best, int height, int width, int **path){
    *path = malloc(height * sizeof(int));
    int min_found = 0;
    int i = 1;
    for (i = 1; i<width; i++){
        if(best[(height-1)*width+i] < best[(height-1)*width + min_found]){
            min_found = i;
        }
    }
    (*path)[height-1] = min_found;

    int j = 0;
    for (j= height - 2 ; j >= 0; j--){
        int current = (*path)[j+1];
        int best_x = current;

        if (current > 0 && best[j*width + (current-1)]<best[j*width + best_x]){
            best_x = current-1;
        }
        if (current < width-1 && best[j*width + (current+1)] < best[j*width + best_x]){
            best_x = current + 1;
        }

        (*path)[j] = best_x;
    }
}
void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    create_img(dest, src->height, src->width - 1);
    for (int y = 0; y < src->height; y++){
    int dest_x = 0;
    for (int x = 0; x < src->width; x++){
        if (x == path[y]){
            continue;
        }
        uint8_t r = get_pixel(src, y, x, 0);
        uint8_t g = get_pixel(src, y, x, 1);
        uint8_t b = get_pixel(src, y, x, 2);
        set_pixel(*dest, y, dest_x, r, g, b);
        dest_x++;
    }
}
}
