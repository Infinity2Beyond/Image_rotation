#include "..\include\image.h"

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

struct image image_create(uint32_t width, uint32_t height)
{
    return (struct image) {.width = width,
                        .height = height,
                        .data = malloc( sizeof(struct pixel) * width * height)
                       };
}

void image_free(struct image* img)
{
    free(img->data);
    free(img);
}
