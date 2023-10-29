//Подключение библиотек
#include <TFT_eSPI.h> //драйвер дисплея
#include <SPI.h> //драйвер spi
#include <WiFi.h> //библиотека для рабоы с wifi esp32
#include <GyverNTP.h> //синхронизация с сервером точного времени

//Объявление глобальных переменных и массивов
const byte feedTime[][2] = {
  {7, 0},       // часы, минуты. НЕ НАЧИНАТЬ ЧИСЛО С НУЛЯ
  {12, 0},
  {17, 0},
  {21, 0},
};

int feedAmount = 100; //размер порции

//различные параметры и настройки
#define FEED_SPEED 3000     // задержка между шагами мотора (мкс)
#define STEPS_FRW 19        // шаги вперёд
#define STEPS_BKW 12        // шаги назад
#define timezone 3          // часовой пояс

const byte drvPins[] = {5, 17, 16, 22};  // драйвер (фазаА1, фазаА2, фазаВ1, фазаВ2)

//Инициализация библиотек
GyverNTP ntp(timezone); //инициализация работы с ntp, в параметрах часовой пояс
TFT_eSPI tft = TFT_eSPI(); // создаем экземпляр объекта TFT_eSPI

void setup() {
  //настраиваем пины для шагового двигателя
  for (byte i = 0; i < 4; i++) pinMode(drvPins[i], OUTPUT);   // пины выходы
  //Инициализация дисплея
  tft.init(); // инициализируем дисплей
  tft.setRotation (2);
  tft.fillScreen(TFT_BLACK);
  tft.println("Done!");
  //запуск сервисов
  //ntp.begin(); //сервис синхронизации времени
}

void loop() {
  //ntp.tick(); //синхронизируем время
  //Проверка таймера кормления 2 раза в секунду
  /*static uint32_t tmr = 0;
  if (millis() - tmr > 500) 
  {           // два раза в секунду
      static byte prevMin = 0;
      tmr = millis();
      if (ntp.status()==0)
      {
        if (prevMin != ntp.minute()) 
          {//исключаем двойной запуск кормления при срабатывании таймера
            prevMin = ntp.minute();
            for (byte i = 0; i < sizeof(feedTime) / 2; i++)    // проверяем массив с расписанием
            if (feedTime[i][0] == ntp.hour() && feedTime[i][1] == ntp.minute()) feed();//при совпадении включаем кормушку
          }
      }
  
  }

  btn.tick(); //проверка кнопки
  if (btn.click()) feed();

  if (btn.hold()) {
    int newAmount = 0;
    while (btn.isHold()) {
      btn.tick();
      oneRev();
      newAmount++;
    }
    disableMotor();
    feedAmount = newAmount;
  }
 */ 
}


