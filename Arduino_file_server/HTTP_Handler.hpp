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


//All valid HTTP message types as well as unkown for ill- formatted messages
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

//Data contained in the header of a http message
struct header_data
{
    message_type type = UNKOWN;
    u16 file_name_offset = 0;
    u16 file_name_length = 0;
};

// Http_Request_Handler is the class for handling http message protocols
// This includes extracting data from http messages received by the arduino
// and adding header to send information from the arduino to the client 
class Http_Request_Handler
{
    public:

        Http_Request_Handler();
        ~Http_Request_Handler();

        //Reads message from client, returns request type
        header_data read_request(byte* const message_buffer, const u16 num_bytes_received );
        
        // Streams data from exFat File, tells client that it is a text file
        // Not generic for speed increase - testing reveled that a small text file goes down from 
        // 2000 ms down to 22 ms when choosing to use exfat file pointer rather than generic stream
        void stream_text_file(ExFatFile* const data_stream, byte* message_buffer, const u16 buffer_size);

        // Streams data from exFat file, telling client that it is a file of type mime_type. See stream_text_file
        // for explanation of why the generic type is not chosen.
        void stream_typed_file(ExFatFile* const file, byte* message_buffer, const u16 buffer_size, 
                                const char* mime_type);

        //Sends generic server error for failures on server side stuff
        void send_generic_server_error(const __FlashStringHelper *error);
        
        //Sends 404 Not found error, for when a resource is not found
        void send_resource_not_found();
        
        //Sets the Client that the HTTP request handler will work with
        void set_client(EthernetClient* client);
        
        //Sets the client to NULL when needed
        void reset_client();

    protected:
        //Send the HTTP header for html documents
        void send_html_header();
        
        //Sends the content header of type content_type
        void send_content_type_header(const char* const content_type);

        //Gets the message type from an HTTP header
        message_type get_message_type(const char message_data[], const u16 message_length);
        
    private:
        // Function to print headers open http docs for error info
        void open_error_doc();
        
        // Function to close out http doc after error is printed
        void close_error_doc();

        //Client the http handler will work with
        EthernetClient* current_client = NULL;
        
        
};



#endif