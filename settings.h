#include <PubSubClient.h>

//#define DEBUG_SERIAL    1
//#define DEBUG_MQTT      1 
//#define useModulPower   1

#define SERIAL_RATE     115200    // Serial speed for status info

#define MAX485_DE       5         // D1 OUTPUT, DE pin on the TTL to RS485 converter
#define MAX485_RE_NEG   4         // D2 OUTPUT, RE pin on the TTL to RS485 converter
#define MAX485_RX       14        // D5 UART, RO pin on the TTL to RS485 converter
#define MAX485_TX       12        // D6 UART, DI pin on the TTL to RS485 converter
#define STATUS_LED      2         // Status LED on the Wemos D1 mini (D4)


#define UPDATE_MODBUS   2         // 1: modbus device is read every second
#define UPDATE_STATUS   30        // 10: status mqtt message is sent every 10 seconds
#define RGBSTATUSDELAY  500       // delay for turning off the status led
#define WIFICHECK       500       // how often check lost wifi connection

#define RGBLED_PIN D3        // Neopixel led D3
#define NUM_LEDS 1
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  64        // Default LED brightness.

#ifndef HEADER_FILE_H_SETTINGS
#define HEADER_FILE_H_SETTINGS

  // Update the below parameters for your project
  // Also check NTP.h for some parameters as well
  extern const char* ssid;                  // Wifi SSID
  extern const char* password;              // Wifi password
  extern const char* mqtt_server;           // MQTT server
  extern const char* mqtt_user;             // MQTT userid
  extern const char* mqtt_password;         // MQTT password
  extern const char* clientID;              // MQTT client ID
  extern const char* topicRoot;             // MQTT root topic for the device, keep / at the end


  // Comment the entire second below for dynamic IP (including the define)
  // #define FIXEDIP   1
  //IPAddress local_IP(192, 168, 1, 205);         // Set your Static IP address
  //IPAddress gateway(192, 168, 1, 254);          // Set your Gateway IP address
  //IPAddress subnet(255, 255, 255, 0);
  //IPAddress primaryDNS(192, 168, 1, 254);   //optional
  //IPAddress secondaryDNS(8, 8, 4, 4); //optional

  void onConnectedToMqttBroker(PubSubClient& mqtt);
  void onReceivedMqttMessage(PubSubClient& mqtt, const char* topic, const char* payload);
#endif

