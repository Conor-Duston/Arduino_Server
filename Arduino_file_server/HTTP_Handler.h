/*
    Author: Conor Duston
    Objective: Provide interface for interacting with simple Http requests
*/
#ifndef HTTP_Handler_h
#define HTTP_Handler_h

#include "Arduino.h"
#include <Ethernet.h>


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

        void send_generic_server_error(const char error[]);

        void set_client(EthernetClient* client);
        void reset_client();

        bool send_data(const byte data[], const int num_size);
        bool stream_data();

    protected:

        void send_okay_header();
        message_type get_message_type(char message_data[], u16 message_length);
        
    private:
        EthernetClient* current_client = NULL;
};

#endif