#include <avr/pgmspace.h>

#include "File_Manager.hpp"

// How MIME lookup is supposed to work:
// load lookup array and the corisponding mime_type array to memory based on length and first letter of extension
// look for mime type corrsiponding to extension using index of mime_type. If it exists, return that extension, else return ocet type for download.

struct mime_lookup
{
    PROGMEM_PNTR extention;
    mime_type type;
};


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
const char iconp_ext[] PROGMEM = "ico";

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
const char rich_text_format_ext[] PROGMEM = "rtf";
// s
// t
const char txt_shrt_ext[] PROGMEM = "txt";
const char text_lng_ext[] PROGMEM = "text";
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

//Mime types
const char text_super[] PROGMEM = "text/";
const char image_super[] PROGMEM = "image/"; 
const char audio_super[] PROGMEM = "audio/";
const char video_super[] PROGMEM = "video/";
const char application_super[] PROGMEM = "application/";
const char font_super[] PROGMEM = "font/";

//Mime subtypes
// a
const char* aac_audio_subtype = aac_audio_ext;
// b

// c
const char* css_subtype = css_ext;

// d

// e

// f

// g
const char gzip_subtype[] PROGMEM = "gzip";

const char* gif_subtype = gif_ext;

// h
const char* html_subtype = html_lng_ext;

// i
const char icon_subtypep[] PROGMEM = "vnd.microsoft.ico";
// j

const char* jpeg_subtype = jpg_long_ext;
const char javascript_subtype[] PROGMEM = "javascript"; 

const char* json_subtype = json_prog_ext; 
// k

// l
// m

const char* java_script_module_subtype = javascript_subtype;

const char* mpeg_video_subtype = mpeg_vid_ext;
const char* mp3_audio_subtype = mpeg_video_subtype;

// n
// o
// p

const char* png_img_subtype = png_img_ext;

// q
// r
const char* rtf_subtype = rich_text_format_ext;
// s
// t
const char text_subtype[] PROGMEM = "plain";
// u
// v
// w

const char* waveform_subtype = waveform_audio_ext;

const char* web_vid_subtype = web_vid_ext;
const char* web_aud_subtype = web_vid_ext;

const char* web_im_subtype = web_im_ext;


// x
// y
// z

const char* zip_arc_subtype = zip_arc_ext;

//Special

const char zip7_subtype[] PROGMEM = "x-7z-compressed";

// Lookup tables for varios extention types
const PROGMEM mime_lookup mime_lookup_table_a[] = {
    {aac_audio_ext, {audio_super, aac_audio_subtype}},
};

const PROGMEM mime_lookup mime_lookup_table_c[] = {
    {css_ext, {text_super , css_subtype}}
};

mime_type get_file_mime_type(const char* file_name) {

}