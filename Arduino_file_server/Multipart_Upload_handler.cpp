#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include "Multipart_Upload_Handler.hpp"

// Notes: Lines start with --, followed by content-type boundary
// Form ends with -- after content type
// Boundary Delimiters must be 70 or fewer characters. If A non- alpha characters appears, it must be enclosed in quotes.

enum header_types : u8 {
    length = 0x1,
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
    if (file_name != nullptr) {
        free(file_name);
        file_name == nullptr;
    }
}

upload_state Multipart_Upload_Handler::parse_text_for_upload(const byte* buffer, size_t length) {
    if (this->internal_state == Empty) {
      
        bool multiline_acceptable = false;
        bool in_quotes = false;
        bool acceptable_line = true;
        unsigned int line_iterator = 0;
        unsigned int header_length = 0;
        bool new_line;
        header_types acceptable_types = header_types( length | boundary | connection);
        
        for(unsigned int i = 0; i < length; i++) {
            if (buffer[i] == '\n') {
                
                //Serial.println();
                
                if (new_line) {
                // end of header has been hit
                return;
                }
                new_line = true;
                acceptable_line = true;
                multiline_acceptable = false;
                line_iterator = 0;
                acceptable_types = header_types( length | boundary | connection);
                continue;
            }

            if (buffer[i] == ';') {
                multiline_acceptable = true;
            }

            if (acceptable_line) {
                if (line_iterator < strlen_P(content_string)) {
                    char curr_char = pgm_read_byte_near(content_string + line_iterator);
                    if (curr_char != tolower(buffer[i])) {
                        acceptable_line == false;
                    }
                    continue;
                }
                if (acceptable_types & length != 0) {
                    if (line_iterator - (strlen_P(content_string) - 1) < strlen_P(content_length)) {
                        char curr_char = pgm_read_byte_near(content_length + line_iterator - strlen_P(content_string) - 1);
                        if (curr_char != tolower(buffer[i])) {
                            acceptable_types ^ length;
                        }
                        continue;    
                    }
                    
                    if (isDigit(buffer[i])) {
                        this;
                    }

                }

                if (acceptable_types & boundary != 0) {
                    if (line_iterator - (strlen_P(content_string) - 1) < strlen_P(content_type)) {
                        char curr_char = pgm_read_byte_near(content_type + line_iterator - strlen_P(content_string) - 1);
                        if (curr_char != tolower(buffer[i])) {
                            acceptable_types ^ boundary;
                        }
                        continue;
                    }

                }

            }
        }

    } else if (this->internal_state == In_Progress) {

    } 

}