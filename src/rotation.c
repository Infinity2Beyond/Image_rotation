#include "..\include\rotation.h"
#include <stdio.h>

static const char* status_description_rotation[] = {
    [ABLE_START] = "Rotation starts!\n",
    [ABLE_END]   = "Rotation completed!\n"
};

void status_print_rotation (enum status_rotation rickroll)
{
    printf("%s", status_description_rotation[rickroll]);
}

struct image rotate( struct image original )
{
    struct image result = image_create(original.height, original.width);

    status_print_rotation(ABLE_START);
    for (size_t row = 0; row < original.height; row++)
    {
        for (size_t collum = 0; collum < original.width; collum++)
        {
            result.data[collum*original.height + (original.height - row - 1 )] =
            original.data[row*original.width + collum];
        }
    }
    status_print_rotation(ABLE_END);
    //image_free(&original);
    return result;
}
