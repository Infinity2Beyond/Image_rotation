#include "..\include\bmp.h"
#include "..\include\io_file.h"
#include "..\include\rotation.h"

#include <stdio.h>

bool check_arguments(int amount)
{
    if (amount == 3) return true;
    fprintf(stderr,"Wrong amount of arguments. Need only input file and output file to work!\n");
    return false;
}

int main( int argc, char** argv ) {
    (void) argc; (void) argv;
    if (!check_arguments(argc)) return 1;

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    FILE * input = NULL;
    if(!file_open_read_or_write (&input, input_file, ABLE_READ))  return 1;

    struct image img ;
    if(from_bmp(input, &img))  return 1; //from_bmp == 0 ~ return status Read OK

    FILE *output = NULL;
    if(!file_open_read_or_write (&output, output_file, ABLE_WRITE)) return 1;

    struct image result = rotate(img);
    if(to_bmp(output,&result)) return 1; //to_bmp == 0 ~ return status Write OK

    file_close(&input);
    file_close(&output);
    image_free(&img);
    image_free(&result);
    return 0;
}


