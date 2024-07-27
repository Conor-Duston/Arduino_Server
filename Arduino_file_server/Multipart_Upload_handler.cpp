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
const char content_string[] PROGMEM = "content-";
const char content_length[] PROGMEM = "length:";

const char content_type[] PROGMEM = "type:";
const char valid_content_type[] PROGMEM = "multipart/form-data;";
const char boundary_string[] PROGMEM = "boundary=";

//Form data header strings
const char correct_disposition[] PROGMEM = "disposition: form-data;"; 
const char filename_start[] PROGMEM = "filename=";

//File location
const char file_folder[] PROGMEM = "usr/";

Multipart_Upload_Handler::Multipart_Upload_Handler()
{
}

Multipart_Upload_Handler::~Multipart_Upload_Handler()
{
}

upload_state Multipart_Upload_Handler::parse_text_for_upload(const byte* buffer, size_t length) {
    if (this->internal_state == Empty) {
      
      bool multiline_acceptable = false;
      const byte* current_header_start = buffer;
      bool in_quotes;
      unsigned int header_length = 0;
      
      for(unsigned int i = 0; i < length - 1; i++) {
        header_length++;

        if (buffer[i] == '\n' && !multiline_acceptable) {
            
            parse_header_for_data((char *)current_file_size, header_length);

            current_header_start = &buffer[i + 1];
        }

      }

    } else if (this->internal_state == In_Progress) {

    } 

}

void Multipart_Upload_Handler::parse_header_for_data(const char* header, size_t length) {

    if (length < strlen_P(content_string) ||
        strncasecmp(header, content_string, strlen_P(content_string) - 1) != 0){
        return;
    }

    if (length > (strlen_P(content_string) + strlen_P(content_type) + strlen_P(boundary_string) - 3) && 
        strncasecmp_P(&header[strlen_P(content_string) - 1], content_type, strlen_P(content_type)) == 0)
    {
        bool type_checked = false;
        bool boundary_checked = false;

        for (size_t i = (strlen_P(content_string) + strlen_P(content_type) - 2); i < length; i++) {
            if ( header[i] == ' ' || header[i] == '\n' || header[i] == '\r') {
                continue;
            }
            if (!type_checked) {
                if (strncasecmp_P(&header[i], valid_content_type, strlen_P(valid_content_type) - 1) != 0) {
                    return;
                }
                i += strlen_P(valid_content_type) - 1;
                type_checked = true;
                continue;
            }
            if (!boundary_checked) {
                if (strncasecmp(&header[i], boundary_string, strlen_P(boundary_string) - 1) != 0) {
                    return;
                }
                i += strlen_P(boundary_string) - 1;
                boundary_checked = true;
                continue;
            }
        }
        
    }

}