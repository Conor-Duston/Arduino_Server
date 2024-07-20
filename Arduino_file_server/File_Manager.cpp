#include <avr/pgmspace.h>
#include <string.h>
#include <ctype.h>

#include <SPI.h>

#include "File_Manager.hpp"

// How MIME lookup is supposed to work:
// load lookup array and the corisponding mime_type array to memory based on length and first letter of extension
// look for mime type corrsiponding to extension using index of mime_type. If it exists, return that extension, else return ocet type for download.

struct mime_lookup
{
    PROGMEM_PNTR extention;
    const mime_type type;
};

#pragma region File_Type_Matching

#pragma region File_Extensions
// extensions for file types that are commonly used
// Sorted by alphabetic
// a
const char aac_audio_ext[] PROGMEM = "aac";
// b

// c

const char css_ext[] PROGMEM = "css";

// d

// e
// f
// g

const char gzip_ext[] PROGMEM = "gz";

const char gif_ext[] PROGMEM = "gif";
// h
const char html_srt_ext[] PROGMEM = "htm";
const char html_lng_ext[] PROGMEM = "html";
// i
const char icon_ext[] PROGMEM = "ico";

// j
const char jpg_short_ext[] PROGMEM = "jpg";
const char jpg_long_ext[] PROGMEM = "jpeg";

const char java_script_ext[] PROGMEM = "js";

const char json_prog_ext[] PROGMEM = "json"; 
// k

// l

// m
const char java_script_module_ext[] PROGMEM = "mjs";

const char mp3_audio_ext[] PROGMEM = "mp3";

const char mpeg_vid_ext[] PROGMEM = "mpeg";



// n
// o


// p
const char png_img_ext[] PROGMEM = "png";
// q
// r
const char rich_text_ext[] PROGMEM = "rtf";
// s
const char svg_ext[] PROGMEM = "svg";
// t
const char txt_ext[] PROGMEM = "txt";
// u
// v
// w
const char waveform_audio_ext[] PROGMEM = "wav";

const char web_aud_ext[] PROGMEM = "weba";
const char web_vid_ext[] PROGMEM = "webm";
const char web_im_ext[] PROGMEM = "webp";


// x
// y
// z
const char zip_arc_ext[] PROGMEM = "zip";

//Special chars
const char zip7_ext[] PROGMEM = "7z";
#pragma endregion

#pragma region MIME_Supers
//Mime types
const char text_super[] PROGMEM = "text/";
const char image_super[] PROGMEM = "image/"; 
const char audio_super[] PROGMEM = "audio/";
const char video_super[] PROGMEM = "video/";
const char application_super[] PROGMEM = "application/";
const char font_super[] PROGMEM = "font/";
#pragma endregion

#pragma region MIME_Subtypes
//Mime subtypes
// Special default:
const char default_subtype[] PROGMEM = "octet-stream";
// a
const char* const aac_audio_subtype PROGMEM = aac_audio_ext;
// b

// c
const char* const css_subtype  PROGMEM = css_ext;

// d

// e

// f

// g
const char gzip_subtype[] PROGMEM = "gzip";

const char* const gif_subtype PROGMEM = gif_ext;

// h
const char* const html_subtype PROGMEM = html_lng_ext;

// i
const char icon_subtype[] PROGMEM = "vnd.microsoft.ico";
// j

const char* const jpeg_subtype PROGMEM = jpg_long_ext;
const char java_script_subtype[] PROGMEM = "javascript"; 

const char* const json_subtype PROGMEM = json_prog_ext; 
// k

// l
// m

const char* const java_script_module_subtype PROGMEM = java_script_subtype;

const char* const mpeg_video_subtype PROGMEM = mpeg_vid_ext;
const char* const mp3_audio_subtype PROGMEM = mpeg_video_subtype;

// n
// o
// p

const char* const png_img_subtype PROGMEM = png_img_ext;

// q
// r
const char* const rtf_subtype PROGMEM = rich_text_ext;
// s
const char svg_subtype[] PROGMEM = "svg+xml";
// t
const char text_subtype[] PROGMEM = "plain";
// u
// v
// w

const char* const waveform_subtype PROGMEM = waveform_audio_ext;

const char* const web_vid_subtype PROGMEM = web_vid_ext;
const char* const web_aud_subtype PROGMEM = web_vid_ext;

const char* const web_im_subtype PROGMEM = web_im_ext;


// x
// y
// z

const char* const zip_arc_subtype PROGMEM = zip_arc_ext;

//Special

const char zip7_subtype[] PROGMEM = "x-7z-compressed";

#pragma endregion

#pragma region Lookup_tables

const PROGMEM mime_type default_mime_type {application_super, default_subtype};

// Lookup tables for varios extention types
const PROGMEM mime_lookup mime_lookup_table_a[] {
    {aac_audio_ext, {audio_super, aac_audio_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_c[] = {
    {css_ext, {text_super , css_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_g[] = {
    {gzip_ext, {application_super, gzip_subtype}},
    {gif_ext, {image_super, gif_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_h[] {
    {html_lng_ext, {text_super, html_subtype}},
    {html_srt_ext, {text_super, html_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_i[] {
    {icon_ext, {image_super, icon_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_j[] {
    {jpg_long_ext, {image_super, jpeg_subtype}},
    {jpg_short_ext, {image_super, jpeg_subtype}},
    {java_script_ext, {text_super, java_script_subtype}},
    {json_prog_ext, {application_super, json_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_m[] {
    {java_script_module_ext, {text_super, java_script_module_subtype}},
    {mp3_audio_ext, {audio_super, mp3_audio_subtype}},
    {mpeg_vid_ext, {video_super, mpeg_video_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_p[] {
    {png_img_ext, {image_super, png_img_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_r[] {
    {rich_text_ext, {application_super, rtf_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_s[] {
    {svg_ext, {image_super, svg_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_t[] {
    {txt_ext, {text_super, text_subtype}},
};

const PROGMEM mime_lookup mime_lookup_table_w[] {
    {waveform_audio_ext, {audio_super, waveform_subtype}},
    {web_aud_ext, {audio_super, web_aud_subtype}},
    {web_vid_ext, {video_super, web_vid_subtype}},
    {web_im_ext, {image_super, web_im_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_z[] {
    {zip_arc_ext, {application_super, zip_arc_subtype}}
};

const PROGMEM mime_lookup mime_lookup_table_special[] {
    {zip7_ext, {application_super, zip7_subtype}}
};


#pragma endregion

#pragma endregion

const mime_type get_default_mime_type() {
    mime_type ret;
    
    ret.super_type = (const char*)pgm_read_ptr(default_mime_type.super_type);
    ret.sub_type = (const char*)pgm_read_ptr(default_mime_type.sub_type);

    return ret;
}

mime_type search_table(const mime_lookup* mime_table_in_progmem, const unsigned int table_size, 
                        const char* extension, const unsigned int extension_length) {
    
    //Check every entry for level. If it exists, return that entries pointers, else return defualt.
    for (int i = 0; i < table_size; i++) {
        char temp[30];
        
        // Raw data: 3 pointers; 1 for extension type, 1 for super, 1 for sub
        // All pointers are close pointers for now, be careful to not fill up progmem to much
        PROGMEM_PNTR extension_pointer = (char *)pgm_read_ptr(&mime_table_in_progmem[i]);
        
        // strncpy_P(temp, extension_pointer, 10);
        // Serial.println(temp);

        if (strncmp_P(extension, extension_pointer, extension_length) == 0) {
            // Declare mime_type for return
            mime_type ret;
            // Set a pointer same length as short pointer to be the pointer to the table in memory
            // Pointers are uint16_ts for short pointers, hence type here
            uint16_t* temp_pntr = (uint16_t *)&mime_table_in_progmem[i];
            // Read the pointer at the address plus 1
            ret.super_type = (char*)pgm_read_ptr(++temp_pntr);
            // Read pntr again plus 1 placement
            ret.sub_type = (char*)pgm_read_ptr(++temp_pntr);
            
            // Check to be sure that the correct things are read
            // strncpy_P(temp, ret.sub_type, 30);
            // Serial.println(temp);

            // strncpy_P(temp, ret.super_type, 30);
            // Serial.println(temp);

            return ret;
        }
    }

    return get_default_mime_type();

}


const mime_type get_file_mime_type(const char* file_name) {
    const char* file_extension = strrchr(file_name, '.');
    mime_type ret;

    // Serial.print(F("File extension: "));
    // Serial.println(&file_extension[1]);

    unsigned int ext_length = strlen(&file_extension[1]) + 1;
    // If the file extension is not found or the length is less than 2 (ts, the shortest recognized extension)
    // Return the defualt type
    if (file_extension == NULL || ext_length < 3 || ext_length > 5) {
        return get_default_mime_type();
    }



    if (!isalpha(file_extension[1])) {
        return search_table(mime_lookup_table_special, sizeof(mime_lookup_table_special) / sizeof(mime_lookup),
        &file_extension[1], ext_length);
    }
    switch (file_extension[1])
    {
    case 'a':
        return search_table(mime_lookup_table_a, sizeof(mime_lookup_table_a) / sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'b':
        return get_default_mime_type();
        break;
    case 'c':
        return search_table(mime_lookup_table_c, sizeof(mime_lookup_table_c) / sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'd':
        return get_default_mime_type();
        break;
    case 'e':
        return get_default_mime_type();
        break;
    case 'f':
        return get_default_mime_type();
        break;
    case 'g':
        return search_table(mime_lookup_table_g, sizeof(mime_lookup_table_g) / sizeof(mime_lookup),
        &file_extension[1], ext_length);
        break;
    case 'h':
        return search_table(mime_lookup_table_h, sizeof(mime_lookup_table_h)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'i':
        return search_table(mime_lookup_table_i, sizeof(mime_lookup_table_i)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'j':
        return search_table(mime_lookup_table_j, sizeof(mime_lookup_table_j)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'k':
        return get_default_mime_type();
        break;
    case 'l':
        return get_default_mime_type();
        break;
    case 'm':
        return search_table(mime_lookup_table_m, sizeof(mime_lookup_table_m)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'n':
        return get_default_mime_type();
        break;
    case 'o':
        return get_default_mime_type();
        break;
    case 'p':
        return search_table(mime_lookup_table_p, sizeof(mime_lookup_table_p)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'q':
        return get_default_mime_type();
        break;
    case 'r':
        return search_table(mime_lookup_table_r, sizeof(mime_lookup_table_r)/ sizeof(mime_lookup),
        &file_extension[1], ext_length);
        break;
    case 's':
        return search_table(mime_lookup_table_s, sizeof(mime_lookup_table_s)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 't':
        return search_table(mime_lookup_table_t, sizeof(mime_lookup_table_t)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'u':
        return get_default_mime_type();
        break;
    case 'v':
        return get_default_mime_type();
        break;
    case 'w':
        return search_table(mime_lookup_table_w, sizeof(mime_lookup_table_w)/ sizeof(mime_lookup), 
        &file_extension[1], ext_length);
        break;
    case 'x':
        return get_default_mime_type();
        break;
    case 'y':
        return get_default_mime_type();
        break;
    case 'z':
        return search_table(mime_lookup_table_z, sizeof(mime_lookup_table_z)/ sizeof(mime_lookup),
        &file_extension[1], ext_length);
        break;
    default:
        return get_default_mime_type();
    }
}