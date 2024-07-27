/**
  Conor Duston
  Objective: Simple file server with ability to:
  Upload files from network device to arduino, 
  Download files from arduino,
  and display images on a screen off of arduino 

  Notes:
  Arduino mega is used : 
  SS for ethernet is pin 10
  SS for SD card is 4
  MISO is 50
  MOSI is 51
  Hardware SS is 53
  SCK is 52

  Uses webpage found sd card to access files
**/

#include <SPI.h>
#include <Ethernet.h>
#include <SdFat.h>

#include "HTTP_Handler.hpp"
#include "File_Manager.hpp"
#include "Multipart_Upload_Handler.hpp"

// Server
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAF, 0x01, 0xFF
};

// Note: 10.0.0.x Ip addresses work
IPAddress ip(10, 0, 0, 2);

EthernetServer server(80);

// File
SdExFat sd;
ExFile file;
boolean sd_working;

//Request Handling
Http_Request_Handler http_handler;

//Multipart form parsing
Multipart_Upload_Handler temp;

const uint8_t SD_CS_PIN = 4;
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI)

#define DATA_BFFR_SIZE 1024

byte data_buffer[DATA_BFFR_SIZE];

//Need the slash to behave like it came from a client
const char default_destination[] PROGMEM = "/index.html";

void setup() {
  // put your setup code here, to run once:
  Ethernet.init(10);

  Serial.begin(115200);

  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found. Need shield for Server."));
    while (true) {
      delay(1);
    }
  }
  
  server.begin();
  Serial.print(F("server is at "));
  Serial.println(Ethernet.localIP());

  if (!sd.begin(SD_CONFIG)) {
    Serial.println(F("Cannot configure SD card"));
    sd.initErrorPrint(&Serial);
  }
}

void stream_file(char* file_name, u16 file_length);

void loop() {
  
  EthernetClient client = server.available();

  if (client) {
    Serial.println(F("new client"));
        
    http_handler.set_client(&client);
     while (client.connected()) {
        if (client.available()) {
          
          u16 num_bytes_read = client.read(data_buffer, DATA_BFFR_SIZE - 1);
          
          header_data header_data = http_handler.read_request(data_buffer, num_bytes_read);
          data_buffer[num_bytes_read] = '\0';
          
          // Serial.write((char*)data_buffer);

          // Serial.print(F("Message Type: "));
          // Serial.println(header_data.type);

          // // RI stands for Resource Identifier, aka file name
          // Serial.print(F("RI length: "));
          // Serial.println(header_data.file_name_length);

          // Serial.print(F("RI Offset: "));
          // Serial.println(header_data.file_name_offset);

          // Serial.print(F("RI: "));

          //data_buffer[header_data.file_name_offset + header_data.file_name_length] = '\0';
          
          char* file_name_pointer = (char*)&data_buffer[header_data.file_name_offset];
          
          // Serial.print(F("File Name Requested: "));
          // Serial.println(file_name_pointer);
          
         

          const __FlashStringHelper *unsupported_action_msg = F("Unsupported action");

          switch (header_data.type) {
            case GET:
              
              stream_file(file_name_pointer, header_data.file_name_length);

            break;

            case HEAD:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case POST:

              if (header_data.file_name_length < 2) {
                http_handler.send_generic_server_error(unsupported_action_msg);
                break;
              }
              //Serial.write("Post Message received");
              
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case PUT:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case DELETE:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case CONNECT:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case OPTIONS:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case TRACE:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case PATCH:
              http_handler.send_generic_server_error(unsupported_action_msg);
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;

            case UNKOWN:
            default:
              http_handler.send_generic_server_error(F("Unkown message type or header to long"));
              ////delete data_buffer;
              file_name_pointer = NULL;
            break;
          }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println(F("client disconnected"));
     }
  }
}

void stream_file(char* file_name, u16 file_length) {
  //int read_chars = 0;
  char file_name_array[strlen_P(default_destination)];
  char *file_name_pointer = file_name;

  if (file_length < 2) {
    strcpy_P(file_name_array, default_destination);
    file_name_pointer = file_name_array;
  }
  
  
  //Serial.print(F("file: "));
  //Serial.println(file_name_pointer);

  bool file_found;
  file_found = file.open(file_name_pointer, O_RDONLY);
  //Serial.println(F("File Open attempt"));

  if (!file_found) {
    //Serial.println(F("File not found"));
    if (!file.exists(file_name_pointer)) {
      //Return that file does not exist
      http_handler.send_resource_not_found();
    } else {
      http_handler.send_generic_server_error(F("File could not be opened"));
    }
    return;
  
  } else {
    //Serial.print(F("Mime type reached"));
    mime_type file_type = get_file_mime_type(file_name_pointer);
    
    char mime_string[MAX_MIME_LENGTH];

    strcpy_P(mime_string, file_type.super_type);

    strcat_P(mime_string, file_type.sub_type);

    //Serial.print(F("Mime type requested: "));
    Serial.println(mime_string);
    
    // Stream data from file
    http_handler.stream_typed_file(&file, data_buffer, DATA_BFFR_SIZE, mime_string);

    file.close();
  }
}