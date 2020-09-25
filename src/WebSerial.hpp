#ifndef WebSerial_hpp
#define WebSerial_hpp

#include "Arduino.h"
#include "stdlib_noniso.h"
#include <functional>

#if defined(ESP8266)
    #define HARDWARE "ESP8266"
    #include "ESP8266WiFi.h"
    #include "ESPAsyncTCP.h"
    #include "ESPAsyncWebServer.h"
#elif defined(ESP32)
    #define HARDWARE "ESP32"
    #include "WiFi.h"
    #include "AsyncTCP.h"
    #include "ESPAsyncWebServer.h"
#endif

#define BUFFER_SIZE 500

#include "webserial_webpage.h"

typedef std::function<void(uint8_t *data, size_t len)> RecvMsgHandler;

class WebSerialClass{

public:
    void begin(AsyncWebServer *server, const char* url = "/webserial");

    void msgCallback(RecvMsgHandler _recv);

    virtual size_t write(uint8_t); //= 0;
    size_t write(const char *str)
    {
        if(str == NULL) {
            return 0;
        }
        return write((const uint8_t *) str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size)
    {
        return write((const uint8_t *) buffer, size);
    }
    

    size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));

    // Print
    void print(String m = "");
    void print(const char *m);
    void print(char *m);
    void print(int m);
    void print(uint8_t m);
    void print(uint16_t m);
    void print(uint32_t m);
    void print(double m);
    void print(float m);


    // Print with New Line
    void println(String m = "");
    void println(const char *m);
    void println(char *m);
    void println(int m);
    void println(uint8_t m);
    void println(uint16_t m);
    void println(uint32_t m);
    void println(float m);
    void println(double m);

private:
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    RecvMsgHandler _RecvFunc = NULL;
    
    #if defined(DEBUG)
        void DEBUG_WEB_SERIAL(const char* message);
    #endif
};

extern WebSerialClass WebSerial;
#endif