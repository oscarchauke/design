#include "serverUtils.h"

const char *ssid = "MyESP32AP";
const char *password = "password";


// Function to print formatted time
void printFormattedTime()
{
    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Convert the current time to local time
    struct tm *localTime = localtime(&currentTime);

    // Format and print the local time
    char formattedTime[80];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", localTime);
    Serial.printf("Current time: %s\n", formattedTime);
}

void setTimeFromTimeString(const char *timeString)
{

    int epochTime;
    sscanf(timeString, "{\"time\":\"%d", &epochTime);

    struct timeval tv;
    tv.tv_sec = epochTime;
    tv.tv_usec = 0;

    // Set the new time using settimeofday
    settimeofday(&tv, NULL);

    printFormattedTime();
}

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED: // if a client is disconnected, then type == WStype_DISCONNECTED
        Serial.println("Client " + String(num) + " disconnected");
        break;
    case WStype_CONNECTED: // if a client is connected, then type == WStype_CONNECTED
        Serial.println("Client " + String(num) + " connected");
        // optionally you can add code here what to do when connected
        break;
    case WStype_TEXT: // if a client has sent data, then type == WStype_TEXT
        // try to decipher the JSON string received
        String message = String((char *)payload);
        if (message[0] == '{')
        {
            Serial.println("Time data");
            Serial.println("Received message from client: " + message);
            setTimeFromTimeString(message.c_str());
        }else{
            Serial.println("Received message from client: " + message);
            if(message[6]== '1'){
                group = 1;
            }
        }
        break;
    }
}

void init_wifi()
{
    WiFi.softAP(ssid, password);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());

    while (WiFi.softAPgetStationNum() == 0)
    {
        delay(1000);
        Serial.println("Waiting for client to connect...");
    }
    Serial.printf("Client connected\n");
}

void init_server()
{
    // Mount SPIFFS storage systems
    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    server.on("/", []()
              {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
      server.send(404, "text/plain", "File not found");
    } else {
      server.streamFile(file, "text/html");
      file.close();
    } });

    server.on("/style.css", []()
              {
    File file = SPIFFS.open("/style.css", "r");
    if (!file) {
      server.send(404, "text/plain", "style.css not found");
    } else {
      server.streamFile(file, "text/css");
      file.close();
    } });

    server.on("/app.js", []()
              {
    File file = SPIFFS.open("/app.js", "r");
    if (!file) {
      server.send(404, "text/plain", "app.js not found");
    } else {
      server.streamFile(file, "application/javascript");
      file.close();
    } });

    server.on("/apexcharts.min.js", []()
              {
    File file = SPIFFS.open("/apexcharts.min.js", "r");
    if (!file) {
      server.send(404, "text/plain", "apexcharts.min.js not found");
    } else {
      server.streamFile(file, "application/javascript");
      file.close();
    } });

    server.on("/icon.png", HTTP_GET, []()
              {
  if (SPIFFS.exists("/icon.png")) {
    File file = SPIFFS.open("/icon.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  } else {
    server.send(404, "text/plain", "icon.png not found");
  } });

    server.begin();
    Serial.println("Server started");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("Sockets started");
}
