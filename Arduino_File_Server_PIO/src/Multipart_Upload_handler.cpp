#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include "Multipart_Upload_Handler.hpp"

// Notes: Lines start with --, followed by content-type boundary
// Form ends with -- after content type
// Boundary Delimiters must be 70 or fewer characters. If A non- alpha characters appears, it must be enclosed in quotes.


// Header strings
const char required_content_type_string[] PROGMEM = "content-type:";

const char valid_content_type[] PROGMEM = "multipart/form-data;";

const char boundary_string[] PROGMEM = "boundary=";

//Form data header strings
const char correct_disposition[] PROGMEM = "content-disposition: form-data"; 
const char filename_start[] PROGMEM = "filename=";

//File location
const char file_folder[] PROGMEM = "usr/";

//Error flags
const char boundary_invalid_char_error[] PROGMEM = "Boundary string is not correctly formatted";
const char boundary_not_present_error[] PROGMEM = "Boundary is not present";
const char duplicate_boundary_header[] PROGMEM = "More than 1 boundary string is present";

Multipart_Upload_Handler::Multipart_Upload_Handler()
{
}

Multipart_Upload_Handler::~Multipart_Upload_Handler()
{
    if (last_line != nullptr) {
        free(last_line);
        last_line = nullptr;
    }
}

// Function to parse the text received from buffer into appropriate files.
// Does verification of headers, does not do verification of file names
upload_state Multipart_Upload_Handler::parse_text_for_upload(const byte* buffer, size_t length) {
    const byte* body = buffer;
    if (this->internal_state == Empty) {
        //Serial.println((char *)buffer);
        body = parse_headers(buffer, length);
        if (body == NULL) {
            return Error;
        }

        this->internal_state = In_Progress;
    } 
    if (this->internal_state == In_Progress) {
        if (body != buffer) {
            // Start at offset, account for offset by subtracting number of characters parsed
            Serial.println(F("Parsing body at offset"));
            parse_body(body, length - (body - buffer));
        } else {
            Serial.println(F("Parsing body at beginning"));
            // For repeat calls
            parse_body(buffer, length);
        }
        // Error return check goes hear when finished
    } 
    return Error;
}

// This function is created to parse the starting headers of the HTTP message for data required for file uploads and 
// modify the state of the upload handler to reflect the data.
// This function Returns address of the start of the file body if headers are successfully parsed, else returns null
const byte* Multipart_Upload_Handler::parse_headers(const byte* buffer, size_t length) {
    bool multiline_acceptable = false;
    bool reading_value = false;
    bool read_content_type = false;
    bool read_boundary = false;
    bool read_boundary_value = false;

    bool parse_line = true;
    unsigned int line_iterator = 0;
    unsigned int spare_iterator = 0; 
    
    uint8_t header_lines = 0;

    bool new_line = false;
    
    for(unsigned int i = 0; i < length; i++) {
        
        if (buffer[i] == '\n' && !multiline_acceptable) {
            //Serial.println();
            header_lines++;
            if (new_line) {
                // end of header has been hit
                // Serial.print(F("Header lines processed: "));
                // Serial.println(header_lines, DEC);

                if (read_boundary_value && this->internal_state != Error) {
                    
                    this->internal_state = In_Progress;
                    
                    // Serial.print(F("Boundary: "));
                    // Serial.println(boundary_delimeter);

                } else {
                    
                    this->internal_state = Error;
                    this->error_description = boundary_not_present_error;
                }
                return &buffer[i];
            }

            new_line = true;
            parse_line = true;
            line_iterator = 0;
            
            continue;
        }
        
        if (buffer[i] != '\r' && new_line) {
            new_line = false;
        }

        if (buffer[i] == ';') {
            multiline_acceptable = true;
        } else if (multiline_acceptable && !(buffer[i] == '\n')) {
            multiline_acceptable = false;
        }

        if (parse_line) {

            if (line_iterator < (strlen_P(required_content_type_string))) {
                
                char curr_char = pgm_read_byte_near(required_content_type_string + line_iterator);
                char lower_buffer = tolower(buffer[i]);
                if (curr_char != lower_buffer ) {
                    parse_line = false;
                    // Serial.print(F("Line "));
                    // Serial.println(header_lines, DEC);
                    // Serial.print(F("Failed on character "));
                    // Serial.print(line_iterator, DEC);
                    // Serial.println();
                    // Serial.print(F("Expected char: "));
                    // Serial.print(curr_char);
                    // Serial.println();
                    // Serial.print(F("Actual Char: "));
                    // Serial.println(lower_buffer);
                    // Serial.println();
                }
                line_iterator++;
                continue;
            }
            
            //Serial.println(F("Pasted header check"));
            
            if(!read_boundary_value) {
                //Serial.println(F("Started boundary lookup"));
                if (!reading_value && !multiline_acceptable) {
                    if (!isWhitespace(buffer[i])) {
                        reading_value = true;
                    }
                } else if (!reading_value) {
                    if (!isWhitespace(buffer[i]) && !(buffer[i] == '\n') && !(buffer[i] == '\r')) {
                        reading_value = true;
                    }
                }

                if (reading_value) {
                    if (!read_content_type) {
                        if (spare_iterator < strlen_P(valid_content_type)) {
                            
                            char curr_char = pgm_read_byte_near(valid_content_type + spare_iterator);
                            
                            if (curr_char != tolower(buffer[i])) {
                                parse_line = false;
                                spare_iterator = 0;
                                // Serial.println(F("Content type not acceptable"));
                                // Serial.print(F("Expected char: "));
                                // Serial.println(curr_char);
                                // Serial.print(F("Actual Char: "));
                                // Serial.println((char)buffer[i]);
                                // Serial.println();
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
                    
                    if (!read_boundary) {
                        if (spare_iterator < strlen_P(boundary_string) ) {
                            char curr_char = pgm_read_byte_near(boundary_string + spare_iterator);
                            if (curr_char != tolower(buffer[i])) {
                                parse_line = false;
                                spare_iterator = 0;
                                // Serial.println(F("Boundary string not present"));
                                // Serial.print(F("Expected char: "));
                                // Serial.println(curr_char);
                                // Serial.print(F("Actual Char: "));
                                // Serial.println((char)buffer[i]);
                                continue;
                            }
                            spare_iterator++;
                        } else {
                            spare_iterator = 0;
                            read_boundary = true;
                        }
                    }
                    
                    // Notes: First checks for ASCII characters as per requirements in the MIME RFC, 
                    // then checks to make sure that the iterator will not go out of bounds of the boundary string.
                    if (read_boundary) {
                        if (!isAscii(buffer[i])) {
                            Serial.println(F("Invalid character found"));
                            this->internal_state = Error;
                            this->error_description = boundary_invalid_char_error;
                            continue;
                        }
                        
                        if (spare_iterator > 69 && (char)buffer[i] != '\n' ) {
                            Serial.println(F("To many characters found"));
                            this->internal_state = Error;
                            this->error_description = boundary_invalid_char_error;
                            continue;
                        }

                        if (isControl(buffer[i]) || buffer[i] == ' ') {
                            this->boundary_delimeter[spare_iterator + 1] = '\0';
                            this->boundary_delimeter_length = spare_iterator;
                            spare_iterator = 0;
                            
                            Serial.print(F("Parsed Boundary String: "));
                            Serial.println(this->boundary_delimeter);

                            if (read_boundary_value) {
                                this->internal_state = Error;
                                this->error_description = duplicate_boundary_header;
                            }
                            continue;
                        }

                        this->boundary_delimeter[spare_iterator] = (char)buffer[i];

                        // Serial.print(F("Boundary Char: ")); 
                        // Serial.println((char)buffer[i]);
                        // Serial.print(F("Gotten Char: "));
                        // Serial.println(boundary_delimeter[spare_iterator]);
                        
                        spare_iterator++;
                        
                        continue;
                    }
                }
            }
        }
    }
    return NULL;
}

// Function parses the body of a VERIFIED multipart upload for files and extracts 
// and prints the names in the files.
// Update this to return error, request callback, or done?
void Multipart_Upload_Handler::parse_body(const byte* buffer, size_t length) {
    body_parse_state this_state = Out_Of_Boundary;
    body_parse_state prev_state = Out_Of_Boundary;

    u16 character_in_line = 0;

    u16 subfield_start = 0;

    bool parse_line = true;
    bool parse_field = true;
    bool parsing_field = false;
    bool in_quotes = false;
    bool new_line = true;
    bool correct_field = true;
    const byte* body_pointer = nullptr;

    const byte* file_name_pointer = nullptr;
    u16 file_name_length = 0;

    for (unsigned int i = 0; i < length; i++) {
        if (!parse_line) {
            if (buffer[i] == '\n') {
                parse_line = true;
                character_in_line = 0;
            }
            continue;
        }

        switch (this_state) {
        case Out_Of_Boundary:
            if (character_in_line > 1 || buffer[i] != '-') {
                parse_line = false;
                continue;
            } 
            if (character_in_line == 1 ){
                this_state = Parsing_For_Boundary;
                prev_state = Out_Of_Boundary;
            }
            break;
        
        case Parsing_For_Boundary:
            // First, check to make sure we are reading in the boundary
            if ((character_in_line - 2 < this->boundary_delimeter_length) && buffer[i] != boundary_delimeter[character_in_line - 2]) {
                //If we haven't read the boundary, continue with what was done before
                Serial.println(F("Did not parse correct character"));
                this_state = prev_state;
            } else if ((character_in_line - 2 == this->boundary_delimeter_length)) {
                // If we hit the first character in the CLRF, we are in a new section and need to parse header
                if (buffer[i] == '\r') {
                    Serial.println(F("Parsed in boundary delimeter"));
                    this_state = Section_Header;
                    prev_state = Parsing_For_Boundary;
                    if (body_pointer != nullptr) {
                        //Write to file the body up until this line minus the extra CLRF for the end pointer
                        write_to_file(body_pointer, (&buffer[i] - body_pointer - character_in_line - 2));
                        body_pointer = nullptr;
                    } 
                    //If another character is hit, it means that the line is not providing a boundary delimeter, can return to prior state
                } if (buffer[i] != '-') {
                    Serial.println(F("Found character not in boundary string, returning to prior section"));
                    this_state = prev_state;
                }
            } else if (character_in_line - 2 == (this->boundary_delimeter_length + 1)) {
                if (buffer[i] != '-') {
                    Serial.println(F("Found character not in boundary string, returning to prior section"));
                    this_state = prev_state;
                } else {
                    if (body_pointer != nullptr) {
                        //Write to file the body up until this line minus the extra CLRF for the end pointer
                        Serial.println(F("Found final boundary delimeter, exiting string"));
                        write_to_file(body_pointer, (&buffer[i] - body_pointer - character_in_line - 2));
                        body_pointer = nullptr;
                        this_state = Done_Parse;
                    }
                    break;
                }
            }
            break;
        
        case Section_Header:
            if (correct_field && character_in_line < strlen_P(correct_disposition)) {
                char cmp = pgm_read_byte_near(correct_disposition + character_in_line);
                if (cmp != tolower(buffer[i])) {
                    Serial.print(F("Found character not in buffer. Buffer: "));
                    Serial.print(buffer[i]);
                    Serial.print(F(" comparison character: "));
                    Serial.println(cmp);
                    correct_field = false;
                }
            } else if (correct_field) {
                if (!parse_field && buffer[i] == ';') {
                    parse_field = true;
                } else {
                    if (buffer[i] != ' '  && !parsing_field) {
                        parsing_field = true;
                        subfield_start = character_in_line;
                    } else if (!parsing_field) {
                        continue;
                    }
                    if ((character_in_line - subfield_start) < strlen_P(filename_start)) {
                        char cmp = pgm_read_byte_near(filename_start + character_in_line - subfield_start);
                        if (cmp != buffer[i]) {
                            parse_field = false;
                            parsing_field = false;
                            continue;
                        }
                    } else {
                        if (!in_quotes && buffer[i] == '"') {
                            in_quotes = true;
                            continue;
                        } if (in_quotes && buffer[i] == '"') {
                            in_quotes = false;
                            parsing_field = false;
                            continue;
                        }
                        if (file_name_pointer == nullptr) {
                            file_name_pointer = &buffer[i];
                        }
                        file_name_length++;
                    }
                }
            } 
            
            if (new_line && buffer[i] == '\n') {
                prev_state = Section_Header;
                this_state = Section_Body;
            }
            if (buffer[i] == '\n') {
                new_line = true;
            }
            if (buffer[i] != '\r') {
                new_line = false;
            } 
            break;
        
        case Section_Body:
            if (!new_line) {
                Serial.write(file_name_pointer, file_name_length);
                new_line = true;
            } if (prev_state == Section_Header && body_pointer == nullptr) {
                body_pointer = &buffer[i];
            }
            break;
        
        case Done_Parse:
            return;
        }

        character_in_line++;

        if(buffer[i] == '\n') {
            character_in_line = 0;
            parse_line = true;
        }
    }
}

void Multipart_Upload_Handler::write_to_file(const byte* buffer, size_t length) {
    //Show what is written to file for now
    Serial.write(buffer, length);
}