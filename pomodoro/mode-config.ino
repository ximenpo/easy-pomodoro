#include  <DNSServer.h>
#include  <ESP8266WebServer.h>

static  const byte          DNS_PORT = 53;
static  IPAddress          ap_ip_(192, 168, 1, 1);
static  DNSServer           dns_;
static  ESP8266WebServer  web_(80);

void  config_setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip_, ap_ip_, IPAddress(255, 255, 255, 0));
  WiFi.softAP("pomodoro", "123456");

  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  dns_.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dns_.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  dns_.start(DNS_PORT, "pomodoro", ap_ip_);

  // simple HTTP server to see that DNS server is working
  web_.onNotFound([]() {
    String message = "pomodoro now!\n\n";
    message += "URI: ";
    message += web_.uri();

    web_.send(200, "text/plain", message);
  });
  web_.begin();
}

void  config_loop() {
  dns_.processNextRequest();
  web_.handleClient();
}

