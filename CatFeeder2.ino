//Подключение библиотек
#include <lvgl.h> //библиотека пользовательского интерфейса
#include "touch.h" //работа с тачем
#include <TFT_eSPI.h> //драйвер дисплея
#include <SPI.h> //драйвер spi
#include <WiFi.h> //библиотека для рабоы с wifi esp32
#include <WiFiManager.h> //Легкая настройка подключения к Wifi сети
#include <GyverNTP.h> //синхронизация с сервером точного времени
#include <GyverTimer.h>//подключение различных таймеров

//Объявление глобальных переменных и массивов
uint8_t feedTime[4][3] = {
  {7, 0, 1},       // часы, минуты. НЕ НАЧИНАТЬ ЧИСЛО С НУЛЯ
  {12, 0, 1},
  {17, 0, 1},
  {21, 0, 1},
};

int feedAmount = 250; //размер порции
uint16_t timefeed;

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

//объекты интерфейса LVGL
    //Контейнеры
    static lv_obj_t * ui_feedwindow; // окно кормления
    static lv_obj_t * ui_tabview; // панель вкладок
    //Панель состояния
    static lv_obj_t * ui_wifistatus; //статус wifi
    static lv_obj_t * ui_status_ip; //ip адрес
    //Экранные объекты
      //Основной экран
      //Вкладка кормления
      static lv_obj_t * ui_clock; //часы
      static lv_obj_t * ui_label_feedAmount; //размер порции
      static lv_obj_t * ui_remain; //осталось
      //Вкладка таймеры
      static lv_obj_t * ui_timer1_hour; //слайдер часов будильника 1
      static lv_obj_t * ui_timer1_minute; //слайдер минут будильника 1
      static lv_obj_t * ui_timer1_check; //активатор будильника 1
      static lv_obj_t * ui_timer2_hour; //слайдер часов будильника 2
      static lv_obj_t * ui_timer2_minute; //слайдер минут будильника 2
      static lv_obj_t * ui_timer2_check; //активатор будильника 2
      static lv_obj_t * ui_timer3_hour; //слайдер часов будильника 3
      static lv_obj_t * ui_timer3_minute; //слайдер минут будильника 3
      static lv_obj_t * ui_timer3_check; //активатор будильника 3
      static lv_obj_t * ui_timer4_hour; //слайдер часов будильника 4
      static lv_obj_t * ui_timer4_minute; //слайдер минут будильника 4
      static lv_obj_t * ui_timer4_check; //активатор будильника 4

      //Окно кормления
      static lv_obj_t * ui_feed_progress_bar; //полоса прогресса кормления

//Инициализация библиотек
GyverNTP ntp(timezone); //инициализация работы с ntp, в параметрах часовой пояс
TFT_eSPI tft = TFT_eSPI(); // создаем экземпляр объекта TFT_eSPI
WiFiManager wm; //экземпляр объекта wifi manager

//Инициализация таймеров
GTimer reftime(MS);//часы
GTimer reflvgl(MS); //обновление экранов LVGL 
GTimer refremain(MS); //таймер обновления времени до кормления 
GTimer reffeedtime(MS); //таймер времени до запуска кормления 
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
  lv_disp_drv_register( &disp_drv ); //регистрируем драйвер дисплея и сохраняем его настройки
  // Инициализируем драйвер тачскрина
  static lv_indev_drv_t indev_drv; //объявляем переменные для хранения драйвера тачскрина
  lv_indev_drv_init( &indev_drv ); // базовая инициализация драйвера
  indev_drv.type = LV_INDEV_TYPE_POINTER; //указываем тип драйвера. В данном случае это тачскрин
  indev_drv.read_cb = my_touchpad_read; //указываем имя функции обработчика нажатий на тачскрин, которую мы создали
  lv_indev_drv_register( &indev_drv ); //регистрация драйвера тачскрина и сохранение его настроек
  tft.init(); // инициализируем дисплей
  tft.setRotation (2);
  
//Отрисовка интерфейса
  draw_interface();

 //Инициализация wifi
 
  WiFi.mode(WIFI_AP_STA);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  if(!wm.autoConnect("CatFeeder2","12345678")) {
        Serial.println("Не удалось подкключиться к сети");
        lv_obj_add_flag(ui_wifistatus, LV_OBJ_FLAG_HIDDEN); 
        lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN); 
    } 
    else {  
        Serial.println("Подключение успешно");
        lv_obj_clear_flag(ui_wifistatus, LV_OBJ_FLAG_HIDDEN); //Отображение иконки wifi
        lv_obj_clear_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN); //Показать IP адрес
    } 

  //настраиваем пины для шагового двигателя
  for (byte i = 0; i < 4; i++) pinMode(drvPins[i], OUTPUT);   // пины выходы
    
  
  //запуск сервисов
  ntp.begin(); //сервис синхронизации времени

  //Установка значений таймеров
  reftime.setInterval(1000);//обновление времени на экране 1000 мс или 1 секунда
  refremain.setInterval(30000);//обновление времени на экране 30000 мс или 30 секунд
  reflvgl.setInterval(30);//обновление экрана LVGL 30 мс
  }

/**** ВТОРОЙ БЛОК ФУНКЦИЙ ****/
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  IPAddress ip = WiFi.localIP();
  String ipString =String(ip[0]);
  for (byte octet = 1; octet < 4; ++octet) {
    ipString += '.' + String(ip[octet]);
  }
  lv_label_set_text(ui_status_ip,ipString.c_str());
  Serial.println("IP address: ");
  Serial.println(ip);
}
/**** КОНЕЦ БЛОКА ФУНКЦИЙ ****/


void loop() 
{
  //Проверка статуса wifi
  if (WiFi.status() == WL_CONNECTED)
    { 
      lv_obj_clear_flag(ui_wifistatus, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
      ntp.tick(); //синхронизируем время
    }
  else
    {
      lv_obj_add_flag(ui_wifistatus, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
    }
  
  //Проверяем значения таймеров
  if (reflvgl.isReady()) { lv_timer_handler();} //Обновляем экран
  if (reftime.isReady()) {if (lv_tabview_get_tab_act(ui_tabview)==0) {lv_label_set_text(ui_clock, ntp.timeString().c_str());}} //обновляем занчение часов на экране
  if (refremain.isReady()){feedRemain();} //Отображение времени оставшегося до кормления
  if (reffeedtime.isReady()) {reffeedtime.stop();feed();}

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


