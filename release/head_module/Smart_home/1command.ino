String bmp_press, bmp_temp, bmp_hud;
String hour_begin_1, min_begin_1, sec_begin_1, hour_end_1, min_end_1, sec_end_1;
String temp_limit_fan;

void getBMP280values() {
  bmp_press = bmp.readPressure() / 133, 3;
  bmp_temp = bmp.readTemperature();
  bmp_hud = bmp.readHumidity();
}

int hour_, min_, sec_;
unsigned long timing_enc;
unsigned long timing;
unsigned long timing_fan;
byte page;

void update_fan_state() {
  if (millis() - timing_fan > 10000) {
    timing_fan = millis();
    if (bmp_temp.toInt() < temp_limit_fan.toInt()){
      send_data_fan(1);
      Serial.println("fan on");
    }else{
      send_data_fan(0);
      Serial.println("fan off");
    }
  }
}

void update_api_weather() {
  if (millis() - timing > 300000) {
    timing = millis();
    getWeatherProcess();
    getBmpProcess();
    Serial.println("info internet update");
  }
}
void encoder_process() {
  enc.tick();
  if (enc.left()) {
    lcd.clear();
    page--;
  }
  /*if (page == 255) {
    page = 5;
    }*/
  if (enc.right()) {
    lcd.clear();
    page++;
  }
  /*if (page == 5) {page = 0;}*/
  if (enc.held()) {
    lcd.clear();
  }
  switch (page) {
    case 0: api_page_weather();
      break;
    case 1: bmp_data();
      break;
    case 2: api_page_weather();
      break;
    case 3: api_page_weather();
      break;
    case 4: api_page_weather();
      break;
  }
}

void time_upd() {
  DateTime now = rtc.now();
  hour_ = now.hour();
  min_ = now.minute();
  sec_ = now.second();
}

void background_checker() {
  if (hour_ == hour_begin_1.toInt() && min_ == min_begin_1.toInt() && sec_ == sec_begin_1.toInt()) {
    digitalWrite(2, 1);
    send_data(2, 1);
  }
  if (hour_ == hour_end_1.toInt() && min_ == min_end_1.toInt() && sec_ == sec_end_1.toInt()) {
    digitalWrite(2, 0);
    send_data(2, 0);
  }
}
