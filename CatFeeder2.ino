//Подключение библиотек
#include <lvgl.h> //библиотека пользовательского интерфейса
#include "touch.h" //работа с тачем
#include "interface.h"//'экранный интерфейс
#include <TFT_eSPI.h> //драйвер дисплея
#include <SPI.h> //драйвер spi
#include <WiFi.h> //библиотека для рабоы с wifi esp32
#include <WiFiManager.h> //Легкая настройка подключения к Wifi сети
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

//Настройка шагового двигателя
const byte drvPins[] = {5, 17, 16, 22};  // драйвер (фазаА1, фазаА2, фазаВ1, фазаВ2)

//Параметры экрана
static const uint16_t screenWidth = 240; //ширина экрана
static const uint16_t screenHeight = 320; //высота экрана

//Объявление служебных переменных для LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 6];



//Инициализация библиотек
GyverNTP ntp(timezone); //инициализация работы с ntp, в параметрах часовой пояс
TFT_eSPI tft = TFT_eSPI(); // создаем экземпляр объекта TFT_eSPI
WiFiManager wm; //экземпляр объекта wifi manager


/***** БЛОК СЛУЖЕБНЫХ ФУНКЦИЙ ****/
//Функция для вывода содержимого буфера на экран
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
  {
 uint32_t w = ( area->x2 - area->x1 + 1 );
 uint32_t h = ( area->y2 - area->y1 + 1 );

 tft.startWrite();
 tft.setAddrWindow( area->x1, area->y1, w, h );
 tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
 tft.endWrite();

 lv_disp_flush_ready( disp );
  }
// Вычисление координат касания
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
  {
  if (touch_has_signal()) //если есть касание
  {
  if (touch_touched())
  {
  data->state = LV_INDEV_STATE_PR; //сообщаем библиотеке, что есть касание и удерживается

  //Отправка координат
  data->point.x = touch_last_x; //координата касания по X
  data->point.y = touch_last_y; //координата касания по Y
  }
  else if (touch_released())
  {
  data->state = LV_INDEV_STATE_REL; //сообщаем библиотеке, что касания больше нет
  }
  }
  else
  {
  data->state = LV_INDEV_STATE_REL;
  }
  }


/***** КОНЕЦ БЛОКА СЛУЖЕБНЫХ ФУНКЦИЙ ****/

void setup() 
{
  Serial.begin( 115200 ); //открытие серийного порта
  //Инициализация wifi
  WiFi.mode(WIFI_AP_STA);
  if(!wm.autoConnect("CatFeeder2","12345678")) {
        Serial.println("Не удалось подкключиться к сети");
    } 
    else {  
        Serial.println("Подключение успешно");
    }

  //Настройки экрана  
  touch_init(); //иницилизация тача 
  lv_init();//инициализация LVGL
  //Далее идут функции настройки LVGL 
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 6 ); //создаем буфер для вывода информации на экран
 
  //далее идет настройка параметров экрана
  static lv_disp_drv_t disp_drv; //объявляем переменную для хранения драйвера дисплея
  lv_disp_drv_init( &disp_drv ); //базовая инициализация драйвера
  disp_drv.hor_res = screenWidth; //ширина экрана
  disp_drv.ver_res = screenHeight; //высота экрана
  disp_drv.flush_cb = my_disp_flush; //функция которая выводит содержимое буфера в заданное место экрана. Указываем имя функции которую мы написали выше
  disp_drv.draw_buf = &draw_buf; //объявляем библиотеке, что содержимое буфера экрана находится в переменной draw_buf
  lv_disp_drv_register( &disp_drv ); //регистрируем драйвер дисплея и сохранем его настройки
  // Инициализируем драйвер тачскрина
  static lv_indev_drv_t indev_drv; //объявляем переменные для хранения драйвера тачскрина
  lv_indev_drv_init( &indev_drv ); // базовая инициализация драйвера
  indev_drv.type = LV_INDEV_TYPE_POINTER; //указываем тип драйвера. В данном случае это тачскрин
  indev_drv.read_cb = my_touchpad_read; //указываем имя функции обработчика нажатий на тачскрин, которую мы создали
  lv_indev_drv_register( &indev_drv ); //регистрация драйвера тачскрина и сохранение его настроек
  tft.init(); // инициализируем дисплей
  tft.setRotation (2);
  
  //настраиваем пины для шагового двигателя
  for (byte i = 0; i < 4; i++) pinMode(drvPins[i], OUTPUT);   // пины выходы
    
  
  //запуск сервисов
  ntp.begin(); //сервис синхронизации времени

  //Отрисовка интерфейса
  load_interface();
}

void loop() 
{
  //функция обновления экрана и параметров LVGL 
  lv_timer_handler(); 
  delay( 10 );
  ntp.tick(); //синхронизируем время
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


