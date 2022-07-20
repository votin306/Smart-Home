String api_city, api_temp, api_weather, api_hud, api_speed_wind;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en_EN">

<head meta charset="UTF-8">
    <title>Smart home</title>
    <meta name="viewport" content="width=device-width, initialscale=1">
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0px auto;
            text-align: center;
            background-color: rgb(38, 50, 53);
        }

        .button-agree {
            background-color: #0a460c;
            color: rgb(52, 185, 86);
            padding: 5px 30px;
            text-decoration: none;
            font-size: 30px;
            margin: 2px;
            cursor: default;
            border-radius: 8px;
        }

        .button-decline {
            background-color: #5f1616;
            color: rgb(255, 79, 79);
            padding: 5px 30px;
            text-decoration: none;
            font-size: 30px;
            margin: 5px;
            cursor: default;
            border-radius: 8px;
        }

        .button-send-info {
            background-color: #00a8a0;
            color: rgb(19, 78, 68);
            text-decoration: none;
            font-size: 20px;
            cursor: default;
            border-radius: 8px;
            align-items:flex-end;
        }

        .info-text {
            color: rgb(0, 132, 255);
            font-size: 15px;
        }

        .time-text {
            color: rgb(98, 173, 170);
            font-size: 19px;
        }

        .service-text {
            color: rgb(51, 129, 93);
        }

        .button-text-explanation {
            color: aquamarine;
            font-size: 20px;
            align-items:flex-start;
        }

        .title {
            color: rgb(50, 106, 114);
            font-size: 30px;
        }

        .line {
            border-bottom: 5px solid rgba(16, 183, 224, 0.815);
        }

        .textbox-clock {
            width: 55px;
            height: 20px;
        }
    </style>
</head>

<body>
    <p><span style="font-size:38px">Smart home </span><span style="font-size:18px">powered by ftx</span></p>
    <p><span class="time-text">Time: </span><span class="time-text">%TIME%</span></p>
    <div class="line"></div>
    <p><a class="title">Data from OpenWeatherMap</a></p>
    <p><a class="service-text">City: <span>%APICITY%</span></a></p>
    <p><a class="info-text">Temperature (*C): <span>%APITEMP%</span></a></p>
    <p><a class="info-text">Weather: <span>%APIWEATHER%</span></a></p>
    <p><a class="info-text">Huidumity: <span>%APIHUD%</span></a></p>
    <p><a class="info-text">Speed wind (m/sec): <span>%APIWIND%</span></a></p>
    <div class="line"></div>
    <p><a class="title">Data from barometer module</a></p>
    <p><a class="info-text">Pressure (mmHg): <span>%BMPPRES%</span></a></p>
    <p><a class="info-text">Temperature (*C): <span>%BMPTEMP%</span></a></p>
    <p><a class="info-text">Huidumity: <span>%BMPHUD%</span></a></p>
    <div class="line"></div>

    <p><span class="button-text-explanation">Fan 1 </span>
    <a href="/upd-fan-sector?on=1"><button class="button-agree">ON</button></a><a href="/upd-fan-sector?on=0"><button class="button-decline">OFF</button></a></p>
    <form action="/upd-fan-sector">
        <p><span class="button-text-explanation">Temperature limit  </span>
        <input class="textbox-clock" maxlength="2" placeholder="Temp" type="number" name="limit-temp">
        <input type="submit" value="Send" class="button-send-info">
    </form>
    <div class="line"></div>
    <p><span class="button-text-explanation">Light 1 </span>
    <a href="/update?pin=2&state_pin=1"><button class="button-agree">ON</button></a><a href="/update?pin=2&state_pin=0"><button class="button-decline">OFF</button></a></p>
    <form action="/upd-1-sector">
        <p><span class="button-text-explanation">Begin in </span>
        <input class="textbox-clock" maxlength="2" placeholder="hour" type="text" name="hour-begin-1"><span> :
                                </span><span><input class="textbox-clock" maxlength="2" placeholder="min" name="min-begin-1"><span> :
                                    </span><span><input class="textbox-clock" maxlength="2" placeholder="sec" name="sec-begin-1">
                                        <span class="button-text-explanation">end in</span>
                                        <input class="textbox-clock" maxlength="2" placeholder="hour" name="hour-end-1">
                                        <span> : </span>
                                        <span><input class="textbox-clock" maxlength="2" placeholder="min" name="min-end-1">
                                            <span> : </span>
                                            <input class="textbox-clock" maxlength="2" placeholder="sec" name="sec-end-1">
                                            <input type="submit" value="Send" class="button-send-info">
  </form></body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "ЧетА такой страницы не найдено :(");
}

void create_async_server() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (ENABLE_AUHT == 1) {
      if (!request->authenticate(login_auth, password_auth)) {
        return request->requestAuthentication();
      }
    }
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/upd-fan-sector", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if (request->hasParam("limit-temp")) {
      temp_limit_fan = request->getParam("limit-temp")->value();
    }
    if (request->hasParam("on")) {
      String on_fan = request->getParam("on")->value();
      if (on_fan.toInt() == 1) {
        send_data_fan(1);
      } else {
        send_data_fan(0);
      }
    }
    request->redirect("/");
  });

  server.on("/upd-1-sector", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if (request->hasParam("hour-begin-1")) {
      hour_begin_1 = request->getParam("hour-begin-1")->value();
    }
    if (request->hasParam("min-begin-1")) {
      min_begin_1 = request->getParam("min-begin-1")->value();
    }
    if (request->hasParam("sec-begin-1")) {
      sec_begin_1 = request->getParam("sec-begin-1")->value();
    }
    if (request->hasParam("hour-end-1")) {
      hour_end_1 = request->getParam("hour-end-1")->value();
    }
    if (request->hasParam("min-end-1")) {
      min_end_1 = request->getParam("min-end-1")->value();
    }
    if (request->hasParam("sec-end-1")) {
      sec_end_1 = request->getParam("sec-end-1")->value();
    }
    request->redirect("/");
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String pin, state_pin;
    if (request->hasParam("pin")) {
      pin = request->getParam("pin")->value();
    }
    if (request->hasParam("state_pin")) {
      state_pin = request->getParam("state_pin")->value();
      send_data(pin.toInt(), state_pin.toInt());
    }
    request->redirect("/");
  });

  server.onNotFound(notFound);
  server.begin();
}

String processor(const String& var) {
  //Serial.println(var);
  if (var == "APICITY") {
    String city = api_city;
    return city;
  }
  if (var == "APITEMP") {
    String temp = api_temp;
    return temp;
  }
  if (var == "APIWEATHER") {
    String weather = api_weather;
    return weather;
  }
  if (var == "APIHUD") {
    String hud = api_hud;
    return hud;
  }
  if (var == "APIWIND") {
    String wind = api_speed_wind;
    return wind;
  }
  if (var == "BMPPRES") {
    String pressure = bmp_press;
    return pressure;
  }
  if (var == "BMPTEMP") {
    String bmppress = bmp_temp;
    return bmppress;
  }
  if (var == "BMPHUD") {
    String bmphud = bmp_hud;
    return bmphud;
  }
  if (var == "TIME") {
    DateTime now = rtc.now();
    String time_rtc = String(hour_);
    time_rtc += ":";
    time_rtc += String(min_);
    time_rtc += ":";
    time_rtc += String(sec_);
    return time_rtc;
  }
  return String();
}
