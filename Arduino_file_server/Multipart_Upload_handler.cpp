#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include "Multipart_Upload_Handler.hpp"

// Notes: Lines start with --, followed by content-type boundary
// Form ends with -- after content type
// Boundary Delimiters must be 70 or fewer characters. If A non- alpha characters appears, it must be enclosed in quotes.


const char content_type_header[] PROGMEM = "Content-Type: ";
const char valid_type[] PROGMEM = "multipart/form-data";

const char correct_disposition[] PROGMEM = "content-disposition: form-data"; 

const char filename_start[] PROGMEM = "filename";

Multipart_Upload_Handler::Multipart_Upload_Handler()
{
}

Multipart_Upload_Handler::~Multipart_Upload_Handler()
{
}

