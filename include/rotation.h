#ifndef _ROTATION_H_
#define _ROTATIOn_H_

#include "image.h"


enum status_rotation {
    ABLE_START,
    ABLE_END
};

struct image rotate( struct image const original);

#endif
