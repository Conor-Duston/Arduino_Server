#ifndef MULTIPART_UPLOAD_H
#define MULTIPART_UPLOAD_H

#include <SdFat.h>
#include <Ethernet.h>

#define MAX_BOUNDARY_LENGTH 70
#define MAX_PARTIAL_LENGTH 1024

#define PROGMEM_PNTR const char*

// Notes for how the upload_handler will work:
// will try to create and upload file. If file exists, it will fail.

enum upload_state
{
    Empty,
    Verify_File_Permissions,
    In_Progress,
    Done,
    Error
};

class Multipart_Upload_Handler
{
public:
    Multipart_Upload_Handler();
    ~Multipart_Upload_Handler();

    upload_state parse_text_for_upload(const byte* data_buffer, size_t length);

protected:
    

private:

    const byte* parse_headers(const byte* data_buffer, size_t length);
    
    void parse_body(const byte* data_buffer, size_t length);

    uint64_t current_file_size = 0;
    uint64_t  bytes_written = 0;
    char boundary_delimeter[MAX_BOUNDARY_LENGTH + 1];
    char* last_line = nullptr;

    ExFile* file;

    upload_state internal_state;

    PROGMEM_PNTR error_description = nullptr;
};

#endif