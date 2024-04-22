#include "HTTP_Handler.h"
#include "Ethernet.h"
#include <Arduino.h>
#include <string.h>

Http_Request_Handler::Http_Request_Handler()
{}

Http_Request_Handler::~Http_Request_Handler(){
    current_client = NULL;
}

header_data Http_Request_Handler::read_request(byte* message_buffer, u16 message_length) {
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

void Http_Request_Handler::send_generic_server_error( const char error[]) {
    if (current_client == NULL) {
        return;
    }
    current_client->print(F("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"));
    // Send info to user so that they are not left in dark
    current_client->print(F("<!DOCTYPE HTML>\r\n<html><h1>"));
    current_client->print(error);
    current_client->print(F("</h1></html>"));
}

void Http_Request_Handler::set_client(EthernetClient* client) {
    current_client = client;
}

void Http_Request_Handler::reset_client() {
    current_client = NULL;
}

message_type Http_Request_Handler::get_message_type(char message_data[], u16 message_length) {
    message_type ret = UNKOWN;
    //Set end of string to be where message data ends
    message_data[message_length] = '\0';
    
    //Sort out strings to different lengths to make it faster to go through possible message types
    switch (message_length) {
        case 3:
            if (strcmp(message_data, (char*)F("GET")) == 0) {
                ret = GET;
            } else if (strcmp(message_data, (char*)F("PUT")) == 0) {
                ret = PUT;
            }
        break;
        
        case 4:

        break;

        case 5:

        break;

        case 6:

        break;
        
        case 7:

        break;
    }


    return ret;
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
