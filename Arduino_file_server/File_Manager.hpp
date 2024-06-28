#ifndef FILE_MANAGER_h
#define FILE_MANAGER_h

// Note: this type is to denote that the character string needs to be loaded from programmem.
// While type may be const char*, referance is in PROGMEM referance, not const char * referance
// Load data into buffer from calling code using pgm_read_ptr or other methods as nessisary.
#define PROGMEM_PNTR const char*

// Returns progam memory adress of appropriate mime type given file name.
PROGMEM_PNTR get_file_mime_type(const char* file_name);

#endif