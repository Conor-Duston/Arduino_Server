#include "HTTP_Handler.hpp"
#include "Ethernet.h"

#include <SdFat.h>
#include <Arduino.h>
#include <string.h>
#include <avr/pgmspace.h>

//Indexes of message types found in HTTP headers
#define GET_INDEX 0
#define PUT_INDEX 3
#define POST_INDEX 6
#define HEAD_INDEX 10
#define PATCH_INDEX 14
#define TRACE_INDEX 19
#define DELETE_INDEX 24
#define OPTIONS_INDEX 30
#define CONNECT_INDEX 37

//All HTTP messages in a single string. Placed together like this to conserve dynamic memory and time.
static const char http_header_strings[] PROGMEM = {"GETPUTPOSTHEADPATCHTRACEDELETEOPTIONSCONNECT"};

#define HEADER_BUFFER_SIZE 30

//text/http content header. Here to preseve flash length
static const char http_text_content[] PROGMEM = "Content-Type: text/html\r\n";

//Closes conection to server and ends the Headers portion of the HTTP message
static const char close_connection_end_header[] PROGMEM = "Connection: close\r\n\r\n";

Http_Request_Handler::Http_Request_Handler()
{}

Http_Request_Handler::~Http_Request_Handler(){
    current_client = NULL;
}

header_data Http_Request_Handler::read_request(const byte* message_buffer, const u16 message_length) {
    header_data message_data;
    u8 spaces_encountered = 0;
    
    for (int i = 0; i < message_length; i++) {
        //HTTP header changes at space character
        if (message_buffer[i] == ' ') {
            spaces_encountered ++;

            switch (spaces_encountered)
            {
            case 1:
                //Message type end, URI start
                message_data.type = get_message_type((char*)message_buffer, i);
                message_data.file_name_offset = i + 1;
                break;
            case 2:
                //File resource end, resource start
                message_data.file_name_length = i - message_data.file_name_offset;
                //This is all we care about for now
                return message_data;
            }
        }
        
    }

    return message_data;
}

void Http_Request_Handler::stream_text_file(ExFatFile* data_stream) {

    send_text_header();

    char data[MAX_DATA_BUFFER_SIZE];
    int read_chars = 0;

    read_chars = data_stream->read(data, MAX_DATA_BUFFER_SIZE);

    while (read_chars > 0) {
        current_client->write(data, read_chars);
        read_chars = data_stream->read(data, MAX_DATA_BUFFER_SIZE);
    }
}

void Http_Request_Handler::send_generic_server_error(const __FlashStringHelper *error) {
    if (current_client == NULL) {
        return;
    }
    current_client->print(F("HTTP/1.1 500 Internal Server Error\r\n"));
    
    open_error_doc();
    // Send info to user so that they are not left in dark
    current_client->print(error);

    close_error_doc();
}  

void Http_Request_Handler::send_resource_not_found() {
    if (current_client == NULL) {
        return;
    }
    current_client->print(F("HTTP/1.1 404 Not Found\r\n"));
    
    open_error_doc();
    current_client->print(F("File not found"));
    close_error_doc();
}

void Http_Request_Handler::send_text_header() {
    //Message Header:
    //HTTP Status 200 OK
    //Content-Type: text/html
    //Connection: close
    //
    current_client->print(F("HTTP/1.1 200 OK\r\n"));
    
    char buffer[HEADER_BUFFER_SIZE];

    strcpy_P(buffer, http_text_content);
    current_client->print(buffer);
    //Read final header line, send
    strcpy_P(buffer, close_connection_end_header);
    current_client->print(buffer);
}

void Http_Request_Handler::open_error_doc() {
    char buffer[HEADER_BUFFER_SIZE];
    //Read doc type from flash, send it
    strcpy_P(buffer, http_text_content);
    current_client->print(buffer);
    //Read final header line, send
    strcpy_P(buffer, close_connection_end_header);
    current_client->print(buffer);
    // Open error http doc
    current_client->print(F("<!DOCTYPE html>\r\n<html><h1>"));
}

void Http_Request_Handler::close_error_doc() {

    current_client->print(F("</h1></html>"));

}

message_type Http_Request_Handler::get_message_type(const char message_data[], const u16 message_length) {
    
    message_type ret = UNKOWN;
    
    char message_array[strlen_P(http_header_strings)];
    strcpy_P(message_array, http_header_strings);

    //Sort out strings to different lengths to make it faster to go through possible message types
    switch (message_length) {
        case 3:
            if (strncmp(message_data, &(message_array[GET_INDEX]), message_length) == 0) {
                ret = GET;
            } else if (strncmp(message_data, &(message_array[PUT_INDEX]), message_length) == 0) {
                ret = PUT;
            }
        break;
        case 4:
            if (strncmp(message_data, &(message_array[POST_INDEX]), message_length)) {
                ret = POST;
            } else if (strncmp(message_data, &(message_array[HEAD_INDEX]), message_length)) {
                ret = HEAD;
            }
        break;
        case 5:
            if (strncmp(message_data, &(message_array[TRACE_INDEX]), message_length)) {
                ret = TRACE;
            } else if (strncmp(message_data, &(message_array[PATCH_INDEX]), message_length)) {
                ret = PATCH;
            }
        break;

        case 6:
            if (strncmp(message_data, &(message_array[DELETE_INDEX]), message_length)) {
                ret = DELETE;
            }
        break;
        
        case 7:
            if (strncmp(message_data, &(message_array[OPTIONS_INDEX]), message_length)) {
                ret = OPTIONS;
            } if (strncmp(message_data, &(message_array[CONNECT_INDEX]), message_length)) {
                ret = CONNECT;
            }
        break;
    }

    return ret;
}



void Http_Request_Handler::set_client(EthernetClient* client) {
    current_client = client;
}

void Http_Request_Handler::reset_client() {
    current_client = NULL;
}


// enum message_type {
//     GET,
//     HEAD,
//     POST,
//     PUT,
//     DELETE,
//     CONNECT,
//     OPTIONS,
//     TRACE,
//     PATCH,
//     UNKOWN
// };
