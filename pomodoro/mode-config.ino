#include  <DNSServer.h>
#include  <ESP8266WebServer.h>

static  const byte          DNS_PORT = 53;
static  IPAddress          ap_ip_(192, 168, 1, 1);
static  DNSServer           dns_;
static  ESP8266WebServer  web_(80);

void  web_root() {
  web_.send(200, "text/html", R"---(
<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<title>pomodoro config</title>
<link rel="stylesheet" type="text/css" href="/style.css" />
</head>

<body>

<h1>pomodoro 参数配置</h1>

<form method="get" action="/config">
<fieldset>
    <legend>工作参数</legend>
    <ol>
        <li>每个番茄工作<input name="work_minites" type="number" min="1" max="255">分钟，休息<input name="break_minites" type="number" min="1" max="255">分钟</li>
        <li>连续工作<input name="long_break_work_times" type="number" min="1" max="10">个番茄后，休息<input name="long_break_minites" type="number" min="1" max="255">分钟</li>
        <li>开始或取消一个番茄时，需在<input name="confirm_seconds" type="number" min="1" max="255">秒内再次确认</li>
    </ol>
    <input id="m1" name="slient_mode" value="1" type="checkbox"><label for="m1">启用声音提醒</label>
</fieldset>

<hr>

<fieldset>
    <legend>联网参数</legend>
    <ul>
        <li><label class="prefix" for="w1">Wifi名称</label>   <input id="w1" name="wifi_ssid"></li>
        <li><label class="prefix" for="w2">Wifi密码</label>   <input id="w2" name="wifi_password"></li>
        <li><label class="prefix" for="a1">设备ID</label>     <input id="a1" name="app_id"></li>
        <li><label class="prefix" for="a2">设备Key</label>    <input id="a2" name="app_key"></li>
    </ul>
</fieldset>

<hr>

<input type="submit" value="保存并重启"><input type="reset" value="重置数据" onclick="javascript:reset_fields()">
</form>

<script src="/data.js">
    reset_fields();
</script>

</body>
</html>
)---");
}

void  web_css() {
  web_.send(200, "text/css", R"---(
body {
    background-color: #cccccc;
    font-family: Arial, Helvetica, Sans-Serif;
    color: #000088;
}
h1 {
    text-align: center;
}
input[type="number"] {
    ime-mode: disabled;
    width: 24pt;
    background-color: #eeeeee;
}
input[type="submit"],input[type="reset"] {
    width: 100pt;
}
.prefix {
    display:inline-block;
    width:64pt;
}
)---");
}

void  web_js() {
  web_.send(200, "application/javascript", R"---(
function  reset_fields() {
  
}
  )---");
}

bool  web_arg_read_uint8(uint8_t* pValue, const char* pArgName, uint8_t min, uint8_t max) {
  const String& str = web_.arg(pArgName);
  uint8_t val = atoi(str.c_str());
  if (val >= min && val <= max) {
    *pValue = val;
    return  true;
  }
  return  false;
}

bool  web_arg_read_string(char* pBuf, size_t size, const char* pArgName) {
  memset(pBuf, 0, size);
  const String& str = web_.arg(pArgName);
  strncpy(pBuf, str.c_str(), size - 1);
  return  true;
}

void  web_config() {
  do {
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "wifi_ssid")) break;
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "wifi_password")) break;

    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "app_id")) break;
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "app_key")) break;

    if (!web_arg_read_uint8(&config_.work_minites, "work_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.break_minites, "break_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.long_break_minites, "long_break_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.long_break_work_times, "long_break_work_times", 1, 255))break;

    if (!web_arg_read_uint8(&config_.confirm_seconds, "confirm_seconds", 0, 255))break;
    if (!web_arg_read_uint8(&config_.slient_mode, "slient_mode", 0, 1))break;

    // all right, restart now
    timeout_.reset();
    web_.send(200, "text/plain", "配置更新成功, 正在重启设备 ...");
  } while (false);

  // something wrong, need reconfig.
  web_root();
}

void  web_notfound() {
  web_root();
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
    web_.on("/data.js", web_js);
    web_.on("/style.css", web_css);
    web_.on("/config", web_config);
    web_.onNotFound(web_notfound);
    web_.begin();
  }

  timeout_.init();
  timeout_.done();

  led_init();
  led_setblink(true);
}

void  config_loop() {
  web_.handleClient();

  dns_.processNextRequest();
  led_config_update();

  if (timeout_.check_timeout(timer_.now(), 1000)) {
    ESP.restart();
  }
}

