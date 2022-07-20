/* **************Умный дом(Основной модуль)**************
 * Разработал: FtXivan (Вотинцев Иван)  
 * Версия: beta 1.7
 * Репозиторий: https://github.com/votin306/Smart-Home
 * Связь с разрабом: ВК - https://vk.com/just_ftx; 
 *                   Discord - Ванек Кек)#1700                  
 *                   
 * ------------------История обновлений------------------
 * beta 1.0 
 *  Открытие сервера и обработка энкодера
 * beta 1.1 - 1.5 
 *  Переделка серверной части
 *  Управление с помощью экнодера
 *  Добавление дисплея (LCD)
 *  Запросы на OpenWeatherMap
 *  Время с "интернета", а точнее с протокола NTP
 *  Оптимизация сервера...
 * beta 1.6 - 1.9
 *  Добавление в систему новый датчик  
 *  Удаление NTP
*/

//------------Настройка системы------------
//=========================================
//******Настройка Wi-fi сети(и интернета)****
const char* ssid = "ASUS_60"; //Название(SSID) WI-FI
const char* password = "3369aef687cd"; //Пароль от WI-FI

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Chusovoy,567434,pt&APPID="; //Сайт OpenWeatherMap, с которого берётся инфа о погоде. Подробнее в репозитории(в самом начале)
const String key = "4b79ee3cc5f9a79b300fad9af01428fd"; //Токен OpenWeatherMap
const String ip_bmp = "http://192.168.1.219/"; //IP модуля к котрому подключён барометр

#define ENABLE_AUHT 1
const char* login_auth = "FtXivan";
const char* password_auth = "asuste123qwert";
//=========================================
//*******Настройка NRF24(радио модуль)*******
#define PIN_CE_NRF24 4  //Пин CE подключенный к ESP32
#define PIN_CSN_NRF24 5 //Пин CSN подключенный к ESP32
//=========================================


#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <EncButton2.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "RTClib.h"

RTC_DS3231 rtc;
RF24 radio(PIN_CE_NRF24, PIN_CSN_NRF24);
AsyncWebServer server(80);
Adafruit_BME280 bmp;
EncButton2<EB_ENCBTN> enc(INPUT, 14, 27, 13);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600); // подключаемся к Wi-Fi сети
  radioSetup();
  pinMode(2, OUTPUT);
  lcd.init();
  rtc.begin();
  lcd.backlight();
  lcd.print("Power:OK");
  WiFi.begin(ssid, password);
  delay(5000);        
  if (WiFi.status() != WL_CONNECTED) {lcd.setCursor(0, 1);lcd.print("WIFI:DISCONNECTED");}
  else{lcd.setCursor(0, 1);lcd.print("WIFI:CONNECTED");}
  lcd.setCursor(0, 2); lcd.print("IP:"); lcd.print(WiFi.localIP());
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  create_async_server();
  getWeatherProcess();
  getBmpProcess();
  while (!enc.press()) {
    enc.tick();
    if (enc.press()) {
      lcd.clear();
      break;
    }
  }

}

void loop() {
  update_api_weather();
  encoder_process();
  time_upd();
  background_checker();
  update_fan_state();
}
