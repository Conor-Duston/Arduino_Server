#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include "Multipart_Upload_Handler.hpp"

// Notes: Lines start with --, followed by content-type boundary
// Form ends with -- after content type
// Boundary Delimiters must be 70 or fewer characters. If A non- alpha characters appears, it must be enclosed in quotes.


// Header strings
const char required_content_string[] PROGMEM = "content-type";

const char valid_content_type[] PROGMEM = "multipart/form-data;";

const char boundary_string[] PROGMEM = "boundary=";

//Form data header strings
const char correct_disposition[] PROGMEM = "disposition: form-data;"; 
const char filename_start[] PROGMEM = "filename=";

//File location
const char file_folder[] PROGMEM = "usr/";

const char boundary_error[] PROGMEM = "Boundary delimeter contains non-ASCII character";

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
        bool reading_value = false;
        bool read_content_type = false;
        bool read_boundary_value = false;

        bool acceptable_line = true;
        unsigned int line_iterator = 0;
        unsigned int spare_iterator = 0; 

        bool new_line = false;
        
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
                continue;
            }
            
            if (buffer[i] != '\r' && new_line) {
                new_line = false;
            }

            if (buffer[i] == ';') {
                multiline_acceptable = true;
            } else if (multiline_acceptable) {
                multiline_acceptable = false;
            }

            if (acceptable_line) {
                if (line_iterator < strlen_P(required_content_string)) {
                    char curr_char = pgm_read_byte_near(required_content_string + line_iterator);
                    if (curr_char != tolower(buffer[i])) {
                        acceptable_line = false;
                    }
                    line_iterator++;
                    continue;
                }
                if (!read_content_type) {
                    if (!reading_value && !multiline_acceptable) {
                        if (!isWhitespace(buffer[i])) {
                            reading_value = true;
                        }
                    } else if (!reading_value) {
                        if (!isWhitespace(buffer[i]) || buffer[i] == '\n' || buffer[i] == '\r') {
                            reading_value = true;
                        }
                    }
                    if (reading_value) {
                        if (!read_content_type) {
                            if (spare_iterator < strlen_P(valid_content_type)) {
                                    char curr_char = pgm_read_byte_near(valid_content_type + spare_iterator);
                                if (curr_char != tolower(buffer[i])) {
                                    acceptable_line = false;
                                    spare_iterator = 0;
                                }
                                line_iterator++;
                                spare_iterator++;
                                continue;
                            } else {
                                spare_iterator = 0;
                                read_content_type = true;
                                reading_value = false;
                                line_iterator++;
                                continue;
                            }
                        }
                        
                        if (spare_iterator < strlen_P(boundary_string)) {
                            char curr_char = pgm_read_byte_near(boundary_string + spare_iterator);
                            if (curr_char != tolower(buffer[i])) {
                                acceptable_line = false;
                                spare_iterator = 0;
                            }
                            // Notes: First checks for ASCII characters as per requirements in the MIME RFC, 
                            // then checks to make sure that the iterator will not go out of bounds of the boundary string.
                            else if (isAscii(buffer[i]) && (spare_iterator - strlen_P(boundary_string) + 1 < 70)) {
                                boundary_delimeter[spare_iterator - strlen_P(boundary_string) + 1] = buffer[i];
                            } else {
                                this->internal_state = Error;
                            }
                        }
                        
                    }
                       
                }
            }

        }
    } else if (this->internal_state == In_Progress) {

    } 

}