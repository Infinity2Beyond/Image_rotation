#ifndef _IOFILE_H_
#define _IOFILE_H_

#include <stdbool.h>
#include <stdio.h>

enum status_file {
    ABLE_READ,
    ABLE_WRITE,
    ABlE_FAIL
};
bool file_open_read_or_write(FILE** file, char const* file_path, enum status_file mode);
bool file_close(FILE** file);


#endif
