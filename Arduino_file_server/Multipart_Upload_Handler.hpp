#ifndef MULTIPART_UPLOAD_H
#define MULTIPART_UPLOAD_H

#include <SdFat.h>
#include <Ethernet.h>

#define MAX_BOUNDRY_LENGTH 70

enum Upload_Handler_State
{
    Done,
    In_Progress,
    Error
};


class Multipart_Upload_Handler
{
public:
    Multipart_Upload_Handler();
    ~Multipart_Upload_Handler();



private:
    uint64_t current_file_size = 0;
    uint64_t  bytes_written = 0;
    char boundry_delemeter[MAX_BOUNDRY_LENGTH + 1];
    ExFile* file;
};

#endif