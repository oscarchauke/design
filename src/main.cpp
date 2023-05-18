#include <Arduino.h>
#include "serverUtils.h"

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int group = 0;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  init_wifi();

  init_server();
}

void loop()
{
  server.handleClient();
  webSocket.loop();

  if(group != 0){
    Serial.printf("Saving data to %d\n", group);
    delay(1000);
    printFormattedTime();
  }
}


