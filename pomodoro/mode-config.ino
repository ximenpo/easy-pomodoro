#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

static  const byte          DNS_PORT = 53;
static  IPAddress          ap_ip_(192, 168, 1, 1);
static  DNSServer           dns_;
static  ESP8266WebServer  web_(80);

void  config_setup() {

}

void  config_loop() {
  dns_.processNextRequest();
  web_.handleClient();
}

