#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include "Multipart_Upload_Handler.hpp"

// Notes: Lines start with --, followed by content-type boundary
// Form ends with -- after content type
// Boundary Delimiters must be 70 or fewer characters. If A non- alpha characters appears, it must be enclosed in quotes.

enum header_progress : u8 {
    size = 0x1,
    boundary = 0x2,
    connection = 0x4
};

// Header strings
const char content_length[] PROGMEM = "Content-Length: ";

const char content_type_header[] PROGMEM = "Content-Type: ";
const char valid_type[] PROGMEM = "multipart/form-data";
const char boundary[] PROGMEM = "boundary=";
//Form data
const char correct_disposition[] PROGMEM = "content-disposition: form-data;"; 

const char filename_start[] PROGMEM = "filename=";

const char file_folder[] PROGMEM = "usr/";

Multipart_Upload_Handler::Multipart_Upload_Handler()
{
}

Multipart_Upload_Handler::~Multipart_Upload_Handler()
{
}

upload_state Multipart_Upload_Handler::parse_text_for_upload(const byte* buffer, size_t length) {
    if (this->internal_state == Empty) {
      const byte* offset = buffer;
      
      for(unsigned int i = 0; i < length; i++) {
        
      }

    } else if (this->internal_state == In_Progress) {

    } 

}

void Multipart_Upload_Handler::parse_header_for_data(const char* header, size_t length) {
    if ( (length < 10) || (header[0] != 'c' && header[0] != 'C')) {
        return;
    }
    if (length > (strlen_P(content_type_header) + strlen_P(valid_type) + strlen_P(boundary))
        & strncasecmp_P(header, content_type_header, strlen_P(content_type_header)) == 0) {
        
        
    }
    

}