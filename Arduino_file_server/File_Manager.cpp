#include <avr/pgmspace.h>

#include "File_Manager.hpp"

//extensions for file types that are commonly used
const char aac_audio_ext[] PROGMEM = ".aac";

const char html_srt_ext[] PROGMEM = ".htm";
const char html_lng_ext[] PROGMEM = ".html";

PROGMEM_PNTR get_file_mime_type(const char* file_name) {

}