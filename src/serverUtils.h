#include <SPIFFS.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <time.h>

extern WebServer server;
extern WebSocketsServer webSocket;
extern int group;


void init_wifi();

void init_server();

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length);

void setTimeFromTimeString(const char *timeString);

void printFormattedTime();

void sendData();