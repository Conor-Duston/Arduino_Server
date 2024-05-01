/*
    Author: Conor Duston
    Objective: Provide interface for interacting with simple Http requests
*/
#ifndef HTTP_Handler_h
#define HTTP_Handler_h

#include "Arduino.h"
#include <Ethernet.h>
#include <SdFat.h>

#define MAX_DATA_BUFFER_SIZE 255

enum message_type : u8 {
    GET,        // No body, return has body, already implemented
    HEAD,       // No body, return does not have body, returns header fields for GET method
    POST,       // Has body, return has body, is for stuff like adding users "Sends data to server"
    PUT,        // Has body, return can have body, creates new resource or replaces resource
    DELETE,     // May have body, return can have body, deletes resource
    CONNECT,    // No body, no body response, starts 2 way communication <- Do not implement, is for proxy servers
    OPTIONS,    // No body, can return body, returns allowed message types
    TRACE,      // No body, return has body, loopback test
    PATCH,      // No body, may return with body, is for modifying objects on sever
    UNKOWN      // Default message type, is for case when correct header is not found
};

struct header_data
{
    message_type type;
    u16 file_name_offset;
    u16 file_name_length;
};

class Http_Request_Handler
{
    public:
        Http_Request_Handler();
        ~Http_Request_Handler();

        //Reads message from client, returns request type
        header_data read_request(byte* message_buffer, u16 message_length );
        
        //TODO: Implement this method
        void send_data(const byte data[], const int num_size);
        
        // Streams data from exFat File, tells client that it is a text file
        // Not generic for speed increase - testing revieled that a small text file goes down from 
        // 2000 ms down to 22 ms when choosing to use exfat file pointer rather than generic stream
        void stream_text_file(ExFatFile* data_stream);

        //Sends generic server error for failures on server side stuff
        void send_generic_server_error(const char error[]);
        
        //Sends 404 Not found error
        void send_resource_not_found();
        
        //Sets the Client that the HTTP request handler will work with
        void set_client(EthernetClient* client);
        
        //Sets the client to NULL in case of dangling pointer issue
        void reset_client();

    protected:
        //Send the HTTP status code for text headers
        void send_text_header();
        
        //Gets the message type from an HTTP header
        message_type get_message_type(const char message_data[], const u16 message_length);
        
    private:
        EthernetClient* current_client = NULL;
};

#endif