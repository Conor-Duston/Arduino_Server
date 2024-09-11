#ifndef FILE_MANAGER_h
#define FILE_MANAGER_h

// Note: this type is to denote that the character string needs to be loaded from program mem.
// While type may be const char*, reference is in PROGMEM reference, not const char * reference
// Load data into buffer from calling code using pgm_read_ptr or other methods as needed.
#define PROGMEM_PNTR const char*

// Maximum length a MIME type can be based on adding the size of the sub and super types
// currently supported. This is not the longest a MIME type can be (255)
#define MAX_MIME_LENGTH 50

// Structure containing pointers to addresses in program memory where the sub and super types are.
struct mime_type {
    PROGMEM_PNTR super_type;
    PROGMEM_PNTR sub_type;
};

// Returns program memory address of appropriate mime type given file name.
const mime_type get_file_mime_type(const char* file_name);

const mime_type get_default_mime_type();
#endif