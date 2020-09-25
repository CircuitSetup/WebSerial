#include "WebSerial.hpp"

WebSerialClass WebSerial;

void WebSerialClass::begin(AsyncWebServer *server, const char* url){
    _server = server;
    _ws = new AsyncWebSocket("/webserialws");

    _server->on(url, HTTP_GET, [](AsyncWebServerRequest *request){
        // Send Webpage
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", WEBSERIAL_HTML, WEBSERIAL_HTML_SIZE);
        response->addHeader("Content-Encoding","gzip");
        request->send(response);        
    });

    _ws->onEvent([&](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) -> void {
        if(type == WS_EVT_CONNECT){
            #if defined(DEBUG)
                DEBUG_WEB_SERIAL("Client connection received");
            #endif
        } else if(type == WS_EVT_DISCONNECT){
            #if defined(DEBUG)
                DEBUG_WEB_SERIAL("Client disconnected");
            #endif
        } else if(type == WS_EVT_DATA){
            #if defined(DEBUG)
                DEBUG_WEB_SERIAL("Received Websocket Data");
            #endif
            if(_RecvFunc != NULL){
                _RecvFunc(data, len);
            }
        }
    });

    _server->addHandler(_ws);

    #if defined(DEBUG)
        DEBUG_WEB_SERIAL("Attached AsyncWebServer along with Websockets");
    #endif
}

void WebSerialClass::msgCallback(RecvMsgHandler _recv){
    _RecvFunc = _recv;
}

size_t WebSerialClass::printf(const char *format, ...)
{
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return 0;
    };
    if(len >= sizeof(loc_buf)){
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len+1, format, arg);
    }
    va_end(arg);
    len = write((uint8_t*)temp, len);
    if(temp != loc_buf){
        free(temp);
    }
    return len;
}

// Print

void WebSerialClass::print(String m){
    _ws->textAll(m);
}

void WebSerialClass::print(const char *m){
    _ws->textAll(m);
}

void WebSerialClass::print(char *m){
    _ws->textAll(m);
}

void WebSerialClass::print(int m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint8_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint16_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint32_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(double m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(float m){
    _ws->textAll(String(m));
}


void WebSerialClass::println(String m){
    _ws->textAll(m+"\n");        
}

void WebSerialClass::println(const char *m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(char *m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(int m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint8_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint16_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint32_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(float m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(double m){
    _ws->textAll(String(m)+"\n");
}



#if defined(DEBUG)
    void WebSerialClass::DEBUG_WEB_SERIAL(const char* message){
        Serial.println("[WebSerial] "+message);
    }
#endif


