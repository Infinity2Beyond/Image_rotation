#include "..\include\bmp.h"
#include "..\include\image.h"
#include "..\include\io_file.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define BMP_CONSTANT_SIGNATURE 0x4D42
#define BMP_CONSTANT_RESERVED 0
#define BMP_HORIZONTAL_DEFAULT 2834
#define BMP_VERTICAL_DEFAULT 2834
#define BMP_AMOUNT_COLORS_DEFAULT 0
#define BMP_AMOUNT_IMPORTANT_COLORS_DEFAULT 0
#define BMP_OFFSET_DEFAULT 54
#define BMP_COMPRESSION_DEFAULT 0
#define BMP_CONSTANT_PLANE 1
#define BMP_BPP_COUNT_DEFAULT 24
#define BMP_STRUCTURE_SIZE_DEFAULT 40 //BITMAPINFOHEADER


#pragma pack(push, 1)
struct bmp_header
{
        uint16_t bfType; //identification, must be "BM" - 4D42 hex
        uint32_t  bfileSize; //BMP file's size
        uint32_t bfReserved; // reserved, must == 0
        uint32_t bOffBits; //offset to data field
        uint32_t biSize; //structure size in bytes {40(BITMAPINFOHEADER) or 108(BITMAPV4HEADER) or 124(BITMAPV5HEADER)}
        uint32_t biWidth; //image's width in pixels
        uint32_t  biHeight; //image's height in pixels
        uint16_t  biPlanes; //always 1
        uint16_t biBitCount; //amount of bits per pixel (0 | 1 | 4 | 8 | 16 | 24 | 32)
        uint32_t biCompression; // compression type (0=none, 1=RLE-8, 2=RLE-4,...)
        uint32_t biSizeImage; //image's size in bytes
        uint32_t biXPelsPerMeter; //horizontal resolution - DPI
        uint32_t biYPelsPerMeter; //vertical resolution - DPI
        uint32_t biClrUsed; //amount of colors used (can be 0)
        uint32_t  biClrImportant; //amount of essential colors (can be 0)
};
#pragma pack(pop)

static const char* const status_description_read[] = {
        [READ_OK]                     = "Image is loaded successfully!\n",
        [READ_INVALID_SIGNATURE]      = "Found invalid Signature. Check file format)\n",
        [READ_INVALID_RESERVED]       = "Found invalid Reserved in header\n",
        [READ_INVALID_STRUCTURE_SIZE] = "Found invalid Structure size in header\n",
        [READ_INVALID_PLANES]         = "Found invalid Planes in header\n",
        [READ_INVALID_BITCOUNT]       = "Found invalid Amount of bits per pixel in header\n",
        [READ_INVALID_COMPARISION]    = "Found invalid Compression type in header",
        [READ_INVALID_HEADER]         = "Found invalid Header\n",
        [READ_ERROR]                  = "Found error while reading data from file\n"
};
static const char* const status_description_write[] = {
        [WRITE_OK]      = "Image is saved successfully!\n",
        [WRITE_ERROR]   = "Found error while writing data to file\n"
};
static const size_t sizes = sizeof(struct bmp_header);

static bool bmp_header_read_valid ( FILE* file, struct bmp_header* header) {
    //size_t sizes = sizeof(struct bmp_header);
    return (fread(header,1,sizes,file) == sizes);
}
static bool bmp_header_write_valid ( FILE* file, struct bmp_header* header) {
    //size_t sizes = sizeof(struct bmp_header);
    return (fwrite(header,1,sizes,file) == sizes);
}

static uint32_t bmp_achieve_padding(struct image const *img)  {
    return (uint32_t) (img->width%4);
}

struct bmp_header bmp_header_create(struct image const *img){
    struct  bmp_header header = {
            .bfType = BMP_CONSTANT_SIGNATURE,
            .bfileSize = sizeof(struct bmp_header) + img->width * img->height * sizeof(struct pixel)
                        + img->height* bmp_achieve_padding(img),
            .bfReserved = BMP_CONSTANT_RESERVED,
            .bOffBits = BMP_OFFSET_DEFAULT,
            .biSize = BMP_STRUCTURE_SIZE_DEFAULT,
            .biWidth = img->width,
            .biHeight = img->height,
            .biPlanes = BMP_CONSTANT_PLANE,
            .biBitCount = BMP_BPP_COUNT_DEFAULT,
            .biCompression =  BMP_COMPRESSION_DEFAULT,
            .biSizeImage = img->width * img->height * sizeof(struct pixel)
                           + img->height* bmp_achieve_padding(img),
            .biXPelsPerMeter = BMP_HORIZONTAL_DEFAULT,
            .biYPelsPerMeter = BMP_VERTICAL_DEFAULT,
            .biClrUsed = BMP_AMOUNT_COLORS_DEFAULT,
            .biClrImportant = BMP_AMOUNT_IMPORTANT_COLORS_DEFAULT};
    return header;
}

//header with different setups
/*
static struct bmp_header bmp_header_create_with_setup_offbits(struct bmp_header* header, uint32_t boffbits)
{
    header->bOffBits = boffbits;
    header->bfileSize = header->bfileSize - BMP_OFFSET_DEFAULT + boffbits ;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_clrused(struct bmp_header* header, uint32_t clrused)
{
    header->biClrUsed = clrused;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_clrimportant(struct bmp_header* header, uint32_t clrimportant)
{
    header->biClrImportant = clrimportant;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_xpelspermeter(struct bmp_header* header, uint32_t xpelspermeter)
{
    header->biXPelsPerMeter = xpelspermeter;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_ypelspermeter(struct bmp_header* header, uint32_t ypelspermeter)
{
    header->biYPelsPerMeter = ypelspermeter;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_bitcount(struct bmp_header* header, uint16_t bitcount)
{
    header->biBitCount = bitcount;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_compression(struct bmp_header* header, uint32_t compression)
{
    header->biCompression = compression;
    return *header;
}
static struct bmp_header bmp_header_create_with_setup_bisize(struct bmp_header* header, uint32_t bisize)
{

    header->biSize = bisize;
    return *header;
}
*/
//checking header's configuration
static bool bmp_header_valid_signature(struct bmp_header header)
{
    return(header.bfType == BMP_CONSTANT_SIGNATURE);
}
static bool bmp_header_valid_reserved(struct bmp_header header)
{
    return(header.bfReserved == BMP_CONSTANT_RESERVED);
}
static bool bmp_header_valid_planes(struct bmp_header header)
{
    return(header.biPlanes == BMP_CONSTANT_PLANE);
}
static bool bmp_header_valid_structure_size (uint32_t size)
{
    return (size == 40 || size == 108 || size == 124 );
}
static bool bmp_header_valid_compression (uint32_t compression)
{
    return (compression == 0 || compression == 1 || compression == 2 || compression == 3 || compression == 4
            || compression == 5|| compression == 6|| compression == 11|| compression == 12|| compression == 13);
}
static bool bmp_header_valid_bitCount (uint16_t bitcount)
{
    return (bitcount == 0 || bitcount == 1 || bitcount == 4 || bitcount == 8 || bitcount == 16 || bitcount == 24 || bitcount == 32);
}
static enum status_read bmp_header_valid(const struct bmp_header header)
{
    if (!bmp_header_valid_signature(header)) return READ_INVALID_SIGNATURE;
    if (!bmp_header_valid_reserved(header)) return READ_INVALID_RESERVED;
    if (!bmp_header_valid_structure_size(header.biSize)) return READ_INVALID_STRUCTURE_SIZE;
    if (!bmp_header_valid_planes(header)) return READ_INVALID_PLANES;
    if (!bmp_header_valid_bitCount(header.biBitCount)) return READ_INVALID_BITCOUNT;
    if (!bmp_header_valid_compression(header.biCompression)) return READ_INVALID_COMPARISION;

    return READ_OK;
}
/*
static struct bmp_header bmp_read_and_check(FILE* input)
{
    struct bmp_header header;
    if (!bmp_header_read_valid(input,&header)) return header;
    return header;
}
*/
//read header and image from file input
enum status_read from_bmp(FILE* input, struct image* img )
{
    //struct bmp_header header = bmp_read_and_check(input);
    struct bmp_header header ;

    if (!bmp_header_read_valid(input,&header)) return status_print_read(READ_INVALID_HEADER);

    if (bmp_header_valid(header)) return status_print_read(bmp_header_valid(header));

    *img = image_create(header.biWidth, header.biHeight);

    const uint32_t padding = bmp_achieve_padding(img);
    for (size_t i = 0; i < img->height ; i++){

        if (fread(&(img->data[i * img->width]), sizeof(struct pixel), img->width, input) != img->width)
        {
            image_free(img);
            return status_print_read(READ_ERROR);
        }
        if(fseek(input, padding, SEEK_CUR)!=0)
        {
            image_free(img);
            return status_print_read(READ_ERROR);
        }
    }
    return status_print_read(READ_OK);
}
//write header and image to file output
/*
static struct bmp_header bmp_out_create (FILE* input, struct image* img)
{
    //struct bmp_header headin = bmp_read_and_check(input);
    struct bmp_header headout = bmp_header_create(img);

    if (headin.bOffBits != BMP_OFFSET_DEFAULT) headout = bmp_header_create_with_setup_offbits(&headout, headin.bOffBits);
    if (headin.biSize != BMP_STRUCTURE_SIZE_DEFAULT) headout = bmp_header_create_with_setup_bisize(&headout, headin.biSize);
    if (headin.biBitCount != BMP_BPP_COUNT_DEFAULT) headout = bmp_header_create_with_setup_bitcount(&headout, headin.biBitCount);
    if (headin.biCompression != BMP_COMPRESSION_DEFAULT) headout = bmp_header_create_with_setup_compression(&headout, headin.biCompression);
    if (headin.biXPelsPerMeter != BMP_HORIZONTAL_DEFAULT) headout = bmp_header_create_with_setup_xpelspermeter(&headout, headin.biXPelsPerMeter);
    if (headin.biYPelsPerMeter != BMP_VERTICAL_DEFAULT) headout = bmp_header_create_with_setup_ypelspermeter(&headout, headin.biYPelsPerMeter);
    if (headin.biClrUsed != BMP_AMOUNT_COLORS_DEFAULT) headout = bmp_header_create_with_setup_clrused(&headout, headin.biClrUsed);
    if (headin.biClrImportant != BMP_AMOUNT_IMPORTANT_COLORS_DEFAULT) headout = bmp_header_create_with_setup_clrimportant(&headout, headin.biClrImportant);

    return headout;
}
*/
enum status_write to_bmp(FILE* output, struct image* img)
{
    struct bmp_header headout = bmp_header_create(img);
    //struct bmp_header headout = bmp_out_create(input, img);

    if(!bmp_header_write_valid(output,&headout)) return status_print_write(WRITE_ERROR);

    const struct pixel empty[] = {{0},{0},{0}};
    const uint32_t padding = bmp_achieve_padding(img);

    for (size_t row = 0; row < img->height; row++)
    {
        if (!fwrite(&img->data[row * img->width], sizeof(struct pixel), img->width, output))
            {
                return status_print_write(WRITE_ERROR);
            }
        if(padding) {
                if (!fwrite(empty, 1, padding, output))
                {
                    return status_print_write(WRITE_ERROR);
                }
        }
    }
    return status_print_write(WRITE_OK);
}
//print out status
enum status_read status_print_read(enum status_read status)
{
    if (status != READ_OK) {
        fprintf(stderr,"%s", status_description_read[status]);
        return status;
    }
    printf("%s", status_description_read[status]);
    return status;
}
enum status_write status_print_write(enum status_write status)
{
    if (status != WRITE_OK) {
        fprintf(stderr,"%s", status_description_write[status]);
        return status;
    }
    printf("%s", status_description_write[status]);
    return status;
}



