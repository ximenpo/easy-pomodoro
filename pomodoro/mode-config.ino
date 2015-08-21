
static  IPAddress          ap_ip_(192, 168, 1, 1);
static  ESP8266WebServer  web_(80);

void  web_root() {
  web_.send(200, "text/html", ""
            "<!doctype html>"
            "<html>"
            "<head>"
            "<meta charset='utf-8'>"
            "<meta name='viewport' content='width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes' />"
            "<title>pomodoro config</title>"
            "<link rel='stylesheet' type='text/css' href='/style.css' />"
            "</head>"
            ""
            "<body>"
            ""
            "<h1>pomodoro 参数配置</h1>"
            ""
            "<form method='post' action='/config'>"
            "<fieldset>"
            "    <legend>工作参数</legend>"
            "    <ol>"
            "        <li>每个番茄工作<input name='work_minites' type='number' min='1' max='255'>分钟，休息<input name='break_minites' type='number' min='1' max='255'>分钟</li>"
            "        <li>连续工作<input name='long_break_work_times' type='number' min='1' max='10'>个番茄后，休息<input name='long_break_minites' type='number' min='1' max='255'>分钟</li>"
            "        <li>开始或取消一个番茄时，需在<input name='confirm_seconds' type='number' min='1' max='255'>秒内再次确认</li>"
            "    </ol>"
            "    <input id='m1' name='slient_mode' value='1' type='checkbox'><label for='m1'>启用声音提醒</label>"
            "</fieldset>"
            ""
            "<hr>"
            ""
            "<fieldset>"
            "    <legend>联网参数</legend>"
            "    <ul>"
            "        <li><label class='prefix' for='w1'>Wifi名称</label>   <input id='w1' name='wifi_ssid'></li>"
            "        <li><label class='prefix' for='w2'>Wifi密码</label>   <input id='w2' name='wifi_password'></li>"
            "        <li><label class='prefix' for='a1'>设备ID</label>     <input id='a1' name='app_id'></li>"
            "        <li><label class='prefix' for='a2'>设备Key</label>    <input id='a2' name='app_key'></li>"
            "    </ul>"
            "</fieldset>"
            ""
            "<hr>"
            ""
            "<input type='submit' value='保存并重启'><input type='reset' value='重置数据' onclick='javascript:reset_fields()'>"
            "</form>"
            ""
            "<script src='/data.js'>"
            "    reset_fields();"
            "</script>"
            ""
            "</body>"
            "</html>"
            "");
}

void  web_css() {
  web_.send(200, "text/css", u8R"---(
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
    width: 32pt;
    font-size: 150%;
    background-color: #eeeeee;
}
input[type="submit"],input[type="reset"] {
    width: 150pt;
    font-size: 150%;
}
.prefix {
    display:inline-block;
    width:64pt;
}
)---");
}

void  web_js() {
  char  buf[1024];
  memset(buf, 0, sizeof(buf));
  snprintf(buf, sizeof(buf) - 1, u8R"---(
function  _field(name) {
  return  document.getElementsByName(name)[0];
}
function  reset_fields() {
  _field('wifi_ssid').value = '%s';
  _field('wifi_password').value = '%s';
  _field('app_id').value = '%s';
  _field('app_key').value = '%s';

  _field('work_minites').value  = '%d';
  _field('break_minites').value  = '%d';
  _field('long_break_minites').value  = '%d';
  _field('long_break_work_times').value  = '%d';
  _field('confirm_seconds').value  = '%d';
  _field('slient_mode').checked  = (%d == 1);
}
reset_fields();
  )---", config_.wifi_ssid, config_.wifi_password,
            config_.app_id, config_.app_key,
            config_.work_minites, config_.break_minites,
            config_.long_break_minites, config_.long_break_work_times,
            config_.confirm_seconds, config_.slient_mode ? 0 : 1
           );
  web_.send(200, "application/javascript", buf);
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
    uint8_t tmp;
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "wifi_ssid")) break;
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "wifi_password")) break;

    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "app_id")) break;
    if (!web_arg_read_string(config_.wifi_ssid, sizeof(config_.wifi_ssid), "app_key")) break;

    if (!web_arg_read_uint8(&config_.work_minites, "work_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.break_minites, "break_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.long_break_minites, "long_break_minites", 1, 255))break;
    if (!web_arg_read_uint8(&config_.long_break_work_times, "long_break_work_times", 1, 255))break;

    if (!web_arg_read_uint8(&config_.confirm_seconds, "confirm_seconds", 0, 255))break;
    if (!web_arg_read_uint8(&tmp, "slient_mode", 0, 1))break;
    config_.slient_mode = (tmp == 0) ? 1 : 0;

    // all right, restart now
    //timeout_.reset();
    config_.save();
    web_.send(200, "text/html", ""
              "<html><head>"
              "<meta charset='utf-8'>"
              "</head>"
              "<body>"
              "配置更新成功, 正在重启设备 ..."
              "</body>"
              "</html>");
    return;
  } while (false);

  // something wrong, need reconfig.
  web_root();
}

void  web_notfound() {
  web_.send(404, "text/plain", "not found");
}

void  config_setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip_, ap_ip_, IPAddress(255, 255, 255, 0));
  WiFi.softAP(config_.ap_ssid, config_.ap_password);

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
  if (timeout_.check_timeout(timer_.now(), 500)) {
    config_.save();
    ESP.restart();
  }

  web_.handleClient();

  led_config_update();
}

