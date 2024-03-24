#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>
#pragma pack(push, 1)

struct pixel { uint8_t b, g, r ; };

struct image {
  uint64_t width, height;
  struct pixel* data;
};
#pragma pack(pop)

struct image image_create(uint32_t width, uint32_t height);

void image_free(struct image* img);

#endif
