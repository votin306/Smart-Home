/*void clear_screen() {
  if (do_clear == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    do_clear = 0;
  }
  }*/

unsigned long timing_upd_page;
void api_page_weather() {
  if (millis() - timing_upd_page > 600) { // Простой таймер обновления инфы на экране
    timing_upd_page = millis();
    lcd.setCursor(0, 0); lcd.print(api_city);
    lcd.setCursor(1, 1); lcd.print(api_temp); lcd.print("*C");
    lcd.setCursor(8, 1); lcd.print(api_weather);
    lcd.setCursor(0, 2); lcd.print("Humidity: "); lcd.print(api_hud); lcd.print("%");
    lcd.setCursor(0, 3); lcd.print("Wind: "); lcd.print(api_speed_wind); lcd.print(" m/sec");
  }
}

void bmp_data() {
  if (millis() - timing_upd_page > 1000) { 
    timing_upd_page = millis();
    lcd.setCursor(0, 0); lcd.print("Bmp data");
    lcd.setCursor(0, 1); lcd.print("Pressure: "); lcd.print(bmp_press); lcd.print(" mmgh");
    lcd.setCursor(0, 2); lcd.print("Temerature: "); lcd.print(bmp_temp); lcd.print(" *C");
    lcd.setCursor(0, 3); lcd.print("Pressure: "); lcd.print(bmp_hud); lcd.print(" %");
  }
}
