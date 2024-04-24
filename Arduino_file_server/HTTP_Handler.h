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
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNKOWN
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