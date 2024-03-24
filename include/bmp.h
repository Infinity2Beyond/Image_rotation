#ifndef _BMP_H_
#define _BMP_H_

#include "image.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

enum status_read  {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_RESERVED,
    READ_INVALID_STRUCTURE_SIZE,
    READ_INVALID_PLANES,
    READ_INVALID_BITCOUNT,
    READ_INVALID_COMPARISION,
    READ_INVALID_HEADER,
    READ_ERROR
};
enum  status_write  {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum status_read from_bmp( FILE* in, struct image* img );
enum status_write to_bmp(FILE* out, struct image* img );

struct bmp_header bmp_header_create(struct image const *img);

enum status_read status_print_read(enum status_read);
enum status_write status_print_write(enum status_write);

#endif

