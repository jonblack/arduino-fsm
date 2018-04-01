/////////////////////////////////////////////////////////////////

#include "Fsm.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/////////////////////////////////////////////////////////////////

#define WIFI_SSID    "ThisJenIsTheInternet"
#define WIFI_PASSWD  "*********"

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

/////////////////////////////////////////////////////////////////

State state_red_light("red", on_red);
State state_yellow_from_red("yellow (red)", on_yellow);
State state_yellow_from_green("yellow (green)", on_yellow);
State state_green_light("green", on_green);
Fsm fsm(&state_red_light);

ESP8266WebServer server(80);

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
    delay(50);
  }
  Serial.println("\RUNNING: graph visualizationn\n");

  fsm.add_timed_transition(&state_red_light, &state_yellow_from_red, 6000);
  fsm.add_timed_transition(&state_yellow_from_red, &state_green_light, 1000);
  fsm.add_timed_transition(&state_green_light, &state_yellow_from_green, 6000);
  fsm.add_timed_transition(&state_yellow_from_green, &state_red_light, 2000, NULL, "test");

  Serial.println(fsm.get_dot_definition());

  if (connectToWiFi(WIFI_SSID, WIFI_PASSWD, 10000)) {
    Serial.print("Open http://");
    Serial.print(WiFi.localIP());
    Serial.print(" in your browser.\n");

    server.on( "/", showGraph);
    server.onNotFound(handleNotFound);
    server.begin();
  } else {
    Serial.println("Could not connect to WiFi.");
  }
  Serial.println();
}

/////////////////////////////////////////////////////////////////

void loop() {
  fsm.run_machine();
  server.handleClient();
}

/////////////////////////////////////////////////////////////////

void handleNotFound() {
  server.send(404, "text/plain", "File Not Found\n");
}

/////////////////////////////////////////////////////////////////

void showGraph() {
  String message = "";
  message += "<html>\n";
  message += "<head>\n";
  message += "<meta http-equiv='refresh' content='1'>\n";
  message += "<title>GraphVizArt</title>\n";
  message += "</head>\n";
  message += "<body>\n";
  message += "<a href='http://gravizo.com/'><img src='https://g.gravizo.com/svg?" + fsm.get_dot_definition() + "'/></a>";
  message += "</body>\n";
  message += "</html>\n";
  server.send ( 200, "text/html", message);
}

/////////////////////////////////////////////////////////////////
