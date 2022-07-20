
void getWeatherProcess() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице OpenWeatherMap с указанными параметрами
    http.begin(endpoint + key);
    int httpCode = http.GET();  // Делаем запрос
    if (httpCode > 0) { // проверяем ответ сервера
      // выводим ответ сервера
      String payload = http.getString();
      StaticJsonDocument<1500> doc;    //Memory pool. Поставил наугад для демонстрации
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      const char* name = doc["name"];
      api_city = name;

      String temp = doc["main"]["temp"];
      api_temp = temp.substring(0, 2);

      String weather = doc["weather"][0]["main"];
      api_weather = weather;

      String hud = doc["main"]["humidity"];
      api_hud = hud;

      String speed_wind = doc["wind"]["speed"];
      api_speed_wind = speed_wind;
    }
    else if (httpCode == 404) {
      Serial.println("Город не найден");
    } else {
      Serial.println("Не удалось получить данные");
    }
    http.end(); // освобождаем ресурсы микроконтроллера
  }
}

void getBmpProcess() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(ip_bmp);
    int httpCode = http.GET();  // Делаем запрос
    if (httpCode > 0) { // проверяем ответ сервера
      // выводим ответ сервера
      String payload = http.getString();
      StaticJsonDocument<1500> doc;    //Memory pool
      DeserializationError error = deserializeJson(doc, payload);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }

      String pressure = doc["pressure"];
      bmp_press = pressure;

      String temperature = doc["temp"];
      bmp_temp = temperature;

      String hud = doc["hud"];
      bmp_hud = hud;

      http.end(); // освобождаем ресурсы микроконтроллера
    }
  }
}
