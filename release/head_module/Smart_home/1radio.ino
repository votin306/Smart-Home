byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte transmit_data[3];  // массив, хранящий передаваемые данные
byte latest_data[3];    // массив, хранящий последние переданные данные
boolean flag;


void radioSetup() {             // настройка радио
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

void send_data(byte pin, bool status_pin) {
  transmit_data[0] = pin;
  transmit_data[1] = status_pin;
  radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио
}

void send_data_fan(bool on_fan) {
  transmit_data[2] = on_fan;
  radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио
}
