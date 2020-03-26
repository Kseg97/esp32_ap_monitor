#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FS.h>   //Include File System Headers
#include "SPIFFS.h"

const char* htmlfile = "/index.html";

WebServer server(80);

const char* ssid = "COVID_device_1";
const char* password = "12345678";
const bool debug = false; //Dev: false for production

//===============================================================
// Sensor logic (here and in loop)
//===============================================================
const long interval = 50; //update time
float sensorSignal() {
  return random(100);
}
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot(){
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plane","");
}
 
void handleADC() {
 int s1 = (int) sensorSignal();
 int s2 = (int) sensorSignal();
 String valSensor1 = String(s1);
 String valSensor2 = String(s2);

 // Deben ir separados por coma, ver index.html. Se actualizan cada 100ms allá (a causa de AJAX javascript)
 server.send(200, "text/plane", valSensor1 + "," + valSensor2); //Send ADC value only to client ajax request
}

void handleWebRequests(){
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}
//===============================================================
// Setup
//===============================================================
void setup() {
  delay(1000);
  Serial.begin(115200);
  if(debug) Serial.println();

  //Initialize File System
  SPIFFS.begin();
  if(debug) Serial.println("File System Initialized");

  randomSeed(analogRead(0)); // TEST. Output generation only
  
//ESP32 As access point -----------------------------------------
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  if(debug) {
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }
//----------------------------------------------------------------
  //Initialize Webserver
  server.on("/",handleRoot);
  server.on("/readADC",handleADC); //Reads ADC function is called from out index.html
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin();  
}
//===============================================================
// Loop
//===============================================================
long previousMillis = 0;
void loop(void){
  server.handleClient();
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 
    
    //Serial.println((String)sensorSignal() + " ");
  }
}
//===============================================================
// SPIFFS
//===============================================================
bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
