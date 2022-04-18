/////////////////////////////////////////////////////////////////

#include "Fsm.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Button2.h>

/////////////////////////////////////////////////////////////////

#define WIFI_SSID    "SSID"
#define WIFI_PASSWD  "*********"

#define BUTTON_WAS_PRESSED  1

/////////////////////////////////////////////////////////////////

void on_red() {
  Serial.println("State: RED");
}
 
void on_yellow() {
  Serial.println("State: YELLOW");
}

void on_green() {
  Serial.println("State: GREEN");
}

void on_button_press() {
  Serial.println("BUTTON!");
}

/////////////////////////////////////////////////////////////////

State red_light("R", on_red);
State green_light("G", on_green);
State yellow_from_red("Y (R)", on_yellow);
State yellow_from_green("Y(G)", on_yellow);

State button_pressed("BTN", on_button_press);

Fsm fsm(&red_light);

ESP8266WebServer server(80);

Button2 btn;

/////////////////////////////////////////////////////////////////

bool connectToWiFi(char* ssid, char* passwd, int timeout_after =-1) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  // Wait for connection
  long now = millis();
  while ((WiFi.status() != WL_CONNECTED) && ((timeout_after > -1) && (millis() < (now + timeout_after)))) {
    delay(500);
  }
  return (WiFi.status() == WL_CONNECTED);
}

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }
  Serial.println();
  Serial.println();

  Serial.println("RUNNING: graph visualization\n");
  // defines a traffic light
  fsm.add_timed_transition(&red_light, &yellow_from_red, 6000, NULL);
  fsm.add_timed_transition(&yellow_from_red, &green_light, 1000, NULL);
  fsm.add_timed_transition(&green_light, &yellow_from_green, 6000, NULL);
  fsm.add_timed_transition(&yellow_from_green, &red_light, 2000, NULL);

/*
  // adds a "press for green" button
  fsm.add_transition(&yellow_from_red, &button_pressed, BUTTON_WAS_PRESSED, NULL);
  fsm.add_transition(&yellow_from_green, &button_pressed, BUTTON_WAS_PRESSED, NULL);
  fsm.add_transition(&red_light, &button_pressed, BUTTON_WAS_PRESSED, NULL);
  fsm.add_transition(&green_light, &button_pressed, BUTTON_WAS_PRESSED, NULL);
  fsm.add_timed_transition(&button_pressed, &yellow_from_red, 2000, NULL);

  btn.begin(D4);
  btn.setTapHandler([](Button2 b) {
    fsm.trigger(BUTTON_WAS_PRESSED);
  });
  
*/
  // to print out the .dot definition
  // Serial.println(fsm.get_dot_definition());

  if (!connectToWiFi(WIFI_SSID, WIFI_PASSWD, 10000)) {
    Serial.println(F("Could not connect to WiFi."));
  } else {
    Serial.print(F("Open http://"));
    Serial.print(WiFi.localIP());
    Serial.print(F(" in your browser.\n"));

    server.on( "/", showGraph);
    server.onNotFound([](){
      server.send(404, F("text/plain"), F("File Not Found\n"));
    });
    server.begin();
  }
}

/////////////////////////////////////////////////////////////////

void loop() {
  btn.loop();
  fsm.run_machine();
  server.handleClient();
}

/////////////////////////////////////////////////////////////////

void handleNotFound() {
  server.send(404, F("text/plain"), F("File Not Found\n"));
}

/////////////////////////////////////////////////////////////////

void showGraph() {
  String message = "";
  message += F("<html>\n");
  message += F("<head>\n");
  message += F("<meta http-equiv='refresh' content='1'>\n");
  message += F("<title>GraphVizArt</title>\n");
  message += F("</head>\n");
  message += F("<body>\n");
  message += F("<a href='http://gravizo.com/'><img src='https://g.gravizo.com/svg?") + fsm.get_dot_definition() + F("'/></a>");
  message += F("</body>\n");
  message += F("</html>\n");
  server.send ( 200, F("text/html"), message);
}

/////////////////////////////////////////////////////////////////