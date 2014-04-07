
#include <iostream>
#include <fstream>
#include <chrono>
#include <pthread.h>
#include <vector>
#include "appm.h"
#include <algorithm>
#include <iterator>
#include <cmath>
#include <pthread.h>

//unsigned char *ReadPPM( char *f, int *mode, int *width, int *height );
//int WritePPM( char *f, int mode, int width, int height, unsigned char *ptr );


void make_histogram(u_char* image, int width, int height, std::vector<u_int>& histogram)
{
    for(u_int x=0; x<width; x++)
        for(u_int y=0; y<height; y++)
        {
            u_int offset = (x+y*width)*3;
            u_char valueR = image[offset + 0];
            u_char valueG = image[offset + 1];
            u_char valueB = image[offset + 2];

            u_char value = (valueR+valueG+valueB)/3;

            histogram[value]++;
            /*
            image[offset] = value;
            image[offset+1] = value;
            image[offset+2] = value;
            */
            }
}


void draw_histogram(u_char* image, int width, int height, std::vector<u_int>& histogram)
{
    u_int max_hist_value = *std::max_element(histogram.begin(), histogram.end());

    std::cout << "Max histogram value = " << max_hist_value << std::endl;

    //visualize
    for(int idx=0; idx<=256; idx++)
    {
        u_int value = (height/2.0f)*float(histogram[idx])/float(max_hist_value);

        u_int start = height-value;
        for ( ; start<=height; ++start )
        {
            image[ (start*width+idx)*3 + 0 ] = 255;
            image[ (start*width+idx)*3 + 1 ] = 255;
            image[ (start*width+idx)*3 + 2 ] = 255;
        }
    }
}

int main_seq()
{
    int mode;
    int width;
    int height;

    std::string file_name("out.ppm");
    std::vector<u_int> histogram_data(256, 0);

    u_char* image = ReadPPM(file_name.c_str(), &mode, &width, &height );

    std::cout << width << " " << height << std::endl;

    make_histogram(image, width, height, histogram_data);

    //std::copy(histogram_data.begin(), histogram_data.end(), std::ostream_iterator<u_int>(std::cout, " "));

    draw_histogram(image, width, height, histogram_data);
    WritePPM("out_seq.ppm", mode, width, height, image);
}

int main()
{
    main_seq();
    //main_threaded();
}
