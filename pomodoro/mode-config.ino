#include  <DNSServer.h>
#include  <ESP8266WebServer.h>

static  const byte          DNS_PORT = 53;
static  IPAddress          ap_ip_(192, 168, 1, 1);
static  DNSServer           dns_;
static  ESP8266WebServer  web_(80);

static  const char*   html_root = R"(
<!doctype html>
<html lang="en">
 <head>
  <meta charset="UTF-8">
  <title>pomodoro config</title>
  <style>
    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
  </style>
 </head>

 <body>
  <form method="post" action="/config">
  
  </form>

 </body>
</html>
)";

void  web_notfound() {
  web_.send(404, "text/plain", "not found");
}

void  web_root() {
  web_.send(200, "text/html", html_root);
}

void  web_js() {
  web_.send(200, "text/html", "alert('ok')");
}

void  web_config() {
  if (web_.method() != HTTP_POST) {
    web_notfound();
    return;
  }

  web_.send(200, "text/plain", "配置更新成功, 正在重启设备 ...");
}

void  config_setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip_, ap_ip_, IPAddress(255, 255, 255, 0));
  WiFi.softAP(config_.ap_ssid, config_.ap_password);

  {
    dns_.setTTL(300);
    dns_.start(DNS_PORT, "*", ap_ip_);
  }

  {
    web_.on("/", web_root);
    web_.on("/js", web_js);
    web_.on("/config", web_config);
    web_.onNotFound(web_notfound);
    web_.begin();
  }

  led_init();
  led_setblink(true);
}

void  config_loop() {
  web_.handleClient();

  dns_.processNextRequest();
  led_config_update();
}

