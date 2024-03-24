#include "..\include\io_file.h"

static const char* const status_description_file[] = {
    [ABLE_READ] = "rb",
    [ABLE_WRITE]= "wb",
    [ABlE_FAIL] = "File not found\n"
};

bool file_open_read_or_write(FILE** file, char const* file_path, enum status_file mode )
{
    *file = fopen(file_path, status_description_file[mode]);
    if (*file) return true;
    fprintf(stderr,"%s",status_description_file[ABlE_FAIL]);
    return false;
}

bool file_close(FILE** file)
{
    return (fclose(*file) == 0);
}
