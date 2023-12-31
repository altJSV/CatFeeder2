//Подключение библиотек
#include <lvgl.h> //библиотека пользовательского интерфейса
#include "secrets.h" //различные параметры авторизации
#include <FS.h> //Работа с файловой системой
#include <SPIFFS.h> //файловая система esp32
#include <TFT_eSPI.h> //драйвер дисплея
#include "images.c" //файл с графикой
#include <SPI.h> //драйвер spi
#include <WiFi.h> //библиотека для рабоы с wifi esp32
#include <WiFiManager.h> //Легкая настройка подключения к Wifi сети
#include <GyverNTP.h> //синхронизация с сервером точного времени
#include <GyverTimer.h>//подключение различных таймеров
#include <WiFiClient.h> //работа с wif соединением
#include <PubSubClient.h> //работа по протоколу mqtt
#include <WebServer.h> //веб интерфейс
#include <WiFiUdp.h>//поддержка широковещательных рассылок
#include <ArduinoJson.h>//библиотека для работы с файлами конфигурации
#include <GyverHX711.h> //работа с цифровыми весами
#include <FastBot.h> //Telegram бот
#include <GyverStepper.h> //,библиотека шагового двигателя
#include <Update.h> //OTA обновления
#include <WiFiServer.h> //веб интерфейс и OTA
#include <ESPmDNS.h>//локальный DNS сервер
#include "DHT.h"// библиотека для работы с dht сенсором
#include "webpages.h" //различные скрипты html код

#define TFT_BACKLIGHT 27 //пин подсветки экрана
// Определяем пин и тип датчика температуры DHT22
#define DHT_PIN 14
#define DHT_TYPE DHT22

//добавляем вывод новых символов
#define LV_SYMBOL_SANDCLOCK "\xEF\x82\xB3" //песочные часы
#define LV_SYMBOL_SCALES "\xEF\x82\xB4" //кухонные весы
#define LV_SYMBOL_TIMEOK "\xEF\x82\xB5" //часы ОК
#define LV_SYMBOL_CATFOOD "\xEF\x82\xB6"//кошачья еда
#define LV_SYMBOL_DISPLAY "\xEF\x82\xB7" //монитор
#define LV_SYMBOL_DCLOCK "\xEF\x82\xB8" //цифровые часы
#define LV_SYMBOL_BLUETOOTH "\xEF\x8A\x94" //bluetooth
#define LV_SYMBOL_ASCALES "\xEF\x89\x8E" //аналоговые весы
#define LV_SYMBOL_TELEGRAM "\xEF\x87\x98" //логотип Telegram
#define LV_SYMBOL_WEIGHT "\xEF\x97\x8D" //иконка гири
#define LV_SYMBOL_ACLOCK "\xEF\x80\x97" //аналоговые часы
#define LV_SYMBOL_TEMP "\xEF\x9D\xAB" //иконка температуры
#define LV_SYMBOL_HUMID"\xEF\x9D\xB3" //иконка влажности
#define FILEBUFSIZ 4096
//Выбор типа драйвера
#define STEPDRVTYPE 1 //0-MX1508,  1 - A4988

//файловая система
#define FORMAT_SPIFFS_IF_FAILED true //форматирование файловой системы при ошибке инициализации
#define CALIBRATION_FILE "/TouchCalData" 


//Объявление глобальных переменных и массивов
//массив будильников
uint8_t feedTime[4][4] = {
  {7, 0, 1,20},       // часы, минуты, флаг активности таймера, размер порции
  {12, 0, 1,30},
  {17, 0, 1,30},
  {21, 0, 1,20},
};

uint8_t feedAmountSet = 10; //размер порции на слайдере
uint8_t feedAmount = 10; //размер порции на слайдере
int8_t timezone = 3; //часовой пояс

//Яркость подсветки экрана
uint8_t bright_level=250;
uint8_t daybegin=7; //начало дня
uint8_t dayend=22; //конец дня
bool daytime=true; //отключение подсветки в ночное время

//переключатель цветовой темы оформления
bool theme = true;  //true темная тема, false светлая

//включение телеграм бота
bool tg_bot = true;

//флаг выполнения перезагрузки
bool espRes=false;

uint16_t lastFeed=0; //время последнего кормления

long tareWeight=0; //вес миски в граммах
long foodWeight=0; //вес еды в миске
float scales_param=191.7; //коэффициент взвешивания
uint16_t scales_control_weight=30;

//Параметры шагового двигателя
uint8_t fwd_steps=60; //шагов вперед
uint8_t bck_steps=20; //шагов назад
float step_speed = 100; //скорость вращения
uint8_t max_revs=16; //максимальное число оборотов до остановки мотора

//Температура и влажность
float temperature;
float humidity;
float temp_cal=25.0; //температура калибровки тензодатчика

String logStr = "Старт сессии:\n"; //лог действий в веб интерфейсе
char tempBuf[256]; //буфер для работы с файлами
File fsUploadFile; //буфер загрузки файла
bool fsFound = false; //флаг того что ФС найдена

void fsList(void);
bool initFS(bool format, bool force);

//Параметры экрана
static const uint16_t screenWidth = 320; //ширина экрана
static const uint16_t screenHeight = 240; //высота экрана


//MQTT настройки
bool usemqtt = true;

//Иконки статуса
String status_icons=LV_SYMBOL_WIFI;

//Файл конфигурации
const char * filename = "/config.json";  // имя файла конфигурации

//Объявление служебных переменных для LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 6];

//объекты интерфейса LVGL
    //Контейнеры
    static lv_obj_t * ui_feedwindow; // окно кормления
    static lv_obj_t * ui_stepwindow; //окно настроек шаговика
    static lv_obj_t * ui_scaleswindow; //окно настроек шаговика 
    static lv_obj_t * ui_tabview; // панель вкладок
    static lv_obj_t * ui_tabview_settings; //панель вкладок настроек
    //Панель состояния
    static lv_obj_t * ui_status_icons; //статус wifi
    static lv_obj_t * ui_status_ip; //ip адрес
    //Экранная клавиатура
    static lv_obj_t * kb; //клавиатура
    //Экранные объекты
      //Основной экран
      //Вкладка кормления
      static lv_obj_t * ui_clock; //часы
      static lv_obj_t * ui_label_feedAmount; //размер порции
      static lv_obj_t * ui_remain; //осталось времени до выдачи порции
      static lv_obj_t * ui_food_weight; //вес порции в миске
      static lv_obj_t * ui_temp_label; //температура
      static lv_obj_t * ui_humid_label; //влажность
      static lv_obj_t * ui_slider_feed_amount;//слайдер размера порции
       //Вкладка таймеры
      static lv_obj_t * ui_timer1_hour; //слайдер часов будильника 1
      static lv_obj_t * ui_timer1_minute; //слайдер минут будильника 1
      static lv_obj_t * ui_timer1_check; //активатор будильника 1
      static lv_obj_t * ui_timer1_amount;//размер порции будильника 1
      static lv_obj_t * ui_timer2_hour; //слайдер часов будильника 2
      static lv_obj_t * ui_timer2_minute; //слайдер минут будильника 2
      static lv_obj_t * ui_timer2_check; //активатор будильника 2
      static lv_obj_t * ui_timer2_amount;//размер порции будильника 2
      static lv_obj_t * ui_timer3_hour; //слайдер часов будильника 3
      static lv_obj_t * ui_timer3_minute; //слайдер минут будильника 3
      static lv_obj_t * ui_timer3_check; //активатор будильника 3
      static lv_obj_t * ui_timer3_amount;//размер порции будильника 3
      static lv_obj_t * ui_timer4_hour; //слайдер часов будильника 4
      static lv_obj_t * ui_timer4_minute; //слайдер минут будильника 4
      static lv_obj_t * ui_timer4_check; //активатор будильника 4
      static lv_obj_t * ui_timer4_amount;//размер порции будильника 4


      //Окно кормления
      static lv_obj_t * ui_feed_progress_bar; //полоса прогресса кормления
      static lv_obj_t * ui_feed_progress_bar_label; //текст на полосе прогресса кормления
      static lv_obj_t * ui_feed_label_max; //целевое значение корма
      static lv_obj_t * ui_feed_label_cur; //текущее значение корма
      


      //Окно настроек
      static lv_obj_t * ui_set_panel_display_bright_slider; //слайдер изменения ярккости подсветки экрана
      static lv_obj_t * ui_backlight_slider_label; //текст на слайдере яркости подсветки экрана
      static lv_obj_t * slider_daytime_label; //текст на слайдере подсветки в ночное время
      static lv_obj_t * ui_set_display_daytime_switch; //переключатель подсветки в ночное время
      static lv_obj_t * ui_slider_day_time; //слайдер установки дневного времени
      static lv_obj_t * ui_gmt_slider_label; //текст на слайдере изменения часового пояса
      static lv_obj_t * ui_set_panel_usemqtt_switch; //переключатель mqtt
      static lv_obj_t * ui_set_panel_mqtt_adress_ta; //адрес mqtt
      static lv_obj_t * ui_set_panel_mqtt_port_ta; //порт mqtt
      static lv_obj_t * ui_set_panel_mqtt_login_ta; //логин mqtt
      static lv_obj_t * ui_set_panel_mqtt_pass_ta; //пароль mqtt
      static lv_obj_t * ui_set_panel_mqtt_control_ta; //топик управления mqtt
      static lv_obj_t * ui_set_panel_mqtt_status_ta; //топик статуса mqtt
      static lv_obj_t * ui_set_panel_telegram_token_ta; //токен телеграм бота
      static lv_obj_t * ui_set_panel_telegram_chatid_ta; //чат ID телеграм бота
      static lv_obj_t * ui_set_panel_telegram_bot_switch;//переключатель бота
      static lv_obj_t * ui_set_panel_scales_coef_label; //коэффициент весов

      //Окно настроек шагового двигателя
      static lv_obj_t * ui_step_window_fwdstep_slider_label;//надаись на слайдере шагов вперед
      static lv_obj_t * ui_step_window_bckstep_slider_label;//надаись на слайдере шагов назад
      static lv_obj_t * ui_step_window_speed_slider_label;//надаись на слайдере скорость вращения
      static lv_obj_t * ui_step_window_maxrev_slider_label;//максимум оборотов до остановки


      //Окно калибровки весов
      static lv_obj_t * ui_scales_window_param_label; //калибровочный коэффициент
      static lv_obj_t * ui_scales_window_weight; //спинбокс калибровочного веса

//Инициализация библиотек
GyverNTP ntp(timezone); //инициализация работы с ntp, в параметрах часовой пояс
TFT_eSPI tft = TFT_eSPI(); // создаем экземпляр объекта TFT_eSPI
WiFiManager wm; //экземпляр объекта wifi manager
WiFiClient esp32Client; //обмен данными по wifi
PubSubClient client(esp32Client); //инициализируем библиотеку mqtt
WebServer server(80); //поднимаем веб сервер на 80 порту
GyverHX711 sensor(16, 13, HX_GAIN64_A); //data,clock, коэффициент усиления
FastBot bot (bot_token); //инициализация библиотеки ТГ бота
DHT dht(DHT_PIN, DHT_TYPE); // Датчик DHT

//Инициализируем библиотеку шагового двигателяв зависимости от типа драйвера
#if (STEPDRVTYPE==1)
GStepper<STEPPER2WIRE> stepper(200, 32,33,25); //шагов на полный оборот двигателя, step, dir, en (смотреть схему в документации)
#else
GStepper<STEPPER4WIRE> stepper(200, 26,25,32,33); //шагов на полный оборот двигателя, фаза 1, фаза 2, фаза 3, фаза 4 (смотреть схему в документации)
#endif

//Инициализация таймеров
GTimer reftime(MS);//часы
GTimer reftemp(MS);//взвешивание миски
GTimer reflvgl(MS); //обновление экранов LVGL 
GTimer refremain(MS); //таймер обновления времени до кормления 
GTimer reffeedtime(MS); //таймер времени до запуска кормления
GTimer refchecktime(MS);//проверка срабатывания таймеров кормления
GTimer refsaveconfigdelay(MS);//проверка срабатывания таймеров кормления 

/***** БЛОК СЛУЖЕБНЫХ ФУНКЦИЙ ****/
//Функция для вывода содержимого буфера экрана на дисплей
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
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

//калибровка тач скрина
void touch_calibrate(bool rewrite)
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  if (rewrite) {SPIFFS.remove(CALIBRATION_FILE); rewrite=false;}
  // Проверка наличия файла калибровки и его целостности
  if (SPIFFS.exists(CALIBRATION_FILE)) {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    
  }

  if (calDataOK && !rewrite) {
    // настройки калибровки верны
    tft.setTouch(calData);
  } else {
    // выводим интерфейс калибровки экрана
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch a points to calibrate...");
    tft.println();


    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Screen calibration complete!");

    // сохраняем данные
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
      ESP.restart();//перезагрузка esp32
    }
  }
}

/***** КОНЕЦ БЛОКА СЛУЖЕБНЫХ ФУНКЦИЙ ****/

void setup() 
{
  Serial.begin( 115200 ); //открытие серийного порта
  stepper.setRunMode(FOLLOW_POS);//устанвливаем режим работы двигателя Следовать к позиции
  stepper.setAcceleration(0);//отключаем ускорения двигателя
  //Настройки экрана  
  tft.init(); // инициализируем дисплей
  tft.setRotation (1); //ориентация эрана горизонтальная

  //Инициализация файловой системы
 if (fs_init())
  {
        Serial.println("Чтение файла конфигурации...");
        logStr+="Чтение файла конфигурации...";
        loadConfiguration("/config.json"); 
        readFile(SPIFFS, "/config.json");
        touch_calibrate(false);
  }
  logStr+="Запуск интерфейса... ";

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
  
  
//Отрисовка интерфейса
  draw_interface();
  logStr+="Ок...\n";
 
 //Инициализация wifi
 logStr+="Инициализация WiFi...\n";
  WiFi.mode(WIFI_AP_STA);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  if(!wm.autoConnect("CatFeeder2","12345678")) {
        Serial.println("Не удалось подключиться к сети");
        logStr+="Не удалось подключиться к сети...\n";
        lv_obj_add_flag(ui_status_icons, LV_OBJ_FLAG_HIDDEN); 
        lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN); 
    } 
    else {  
        Serial.println("Подключение успешно");
        lv_obj_clear_flag(ui_status_icons, LV_OBJ_FLAG_HIDDEN); //Отображение иконки wifi
        lv_obj_clear_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN); //Показать IP адрес
        ntp.begin(); //сервис синхронизации времени
       
        //подключаем Telegram бота
        logStr+="Телеграм бот инициализация... ";
        bot.setChatID(chatID); // открываем чат
        bot.attach(newMsg); //подключаем функцию обработки сообщений
        bot.sendMessage("Cat Feeder 2 готов к работе!"); //отправляем сообщение о готовности
        logStr+="Ок\n";
    } 
  
  //запуск сервисов
  // DNS
  if (!MDNS.begin("catfeeder2")) {Serial.println("Ошибка открытия MDNS!");logStr+="Ошибка открытия MDNS!\n";}   
  else
    Serial.println("mDNS запущен. имя хоста = http://catfeeder2.local");
    logStr+="mDNS запущен. имя хоста = http://catfeeder2.local\n";
  server_init();//запуск веб сервера
  
  //Запуск сервиса DHT
  pinMode(DHT_PIN, INPUT);
  logStr+="Запуск DHT... ";
  Serial.println("Запуск DHT...");
  dht.begin();
  logStr+="Ок\n";

  logStr+="Запуск Таймеров... ";
  
  //Установка значений таймеров
  reftime.setInterval(1000);//обновление времени на экране 1000 мс или 1 секунда
  refremain.setInterval(10000);//обновление времени на экране 30000 мс или 30 секунд
  reflvgl.setInterval(30);//обновление экрана LVGL 30 мс
  refchecktime.setInterval(500);//раз в полсекунды
  reftemp.setInterval(10000);//взвешивание миски разв 10 секунд
  refsaveconfigdelay.stop();
  logStr+="Ок\n";
  logStr+="Настройка подсветки экрана... ";
  pinMode(TFT_BACKLIGHT,OUTPUT);//Переключаем пин подсветки на передачу данных
  analogWrite(TFT_BACKLIGHT,bright_level);
  logStr+="Ок\n";
  logStr+="Устройство готово к работе\n";
  }

/**** ВТОРОЙ БЛОК ФУНКЦИЙ ****/

//Подключение к Wifi успешно. Получен ip адрес
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi подключен");
  logStr +="WiFi подключен. IP: ";
  IPAddress ip = WiFi.localIP();
  String ipString =String(ip[0]);
  for (byte octet = 1; octet < 4; ++octet) {
    ipString += '.' + String(ip[octet]);
  }
  lv_label_set_text(ui_status_ip,ipString.c_str());
  Serial.println("IP address: ");
  logStr+=ipString;
  logStr+="\n";
  Serial.println(ip);
  if (usemqtt) MQTT_init();//Подключение к MQTT брокеру
  
}
/**** КОНЕЦ БЛОКА ФУНКЦИЙ ****/


void loop() 
{
  //Проверка статуса wifi
  if (WiFi.status() == WL_CONNECTED)
    { 
      status_icons="";
      if (tg_bot) status_icons+=LV_SYMBOL_TELEGRAM" ";
      if (client.connected()) status_icons+=LV_SYMBOL_LOOP" ";
      status_icons+=LV_SYMBOL_WIFI;
      lv_label_set_text(ui_status_icons, status_icons.c_str());
      lv_obj_clear_flag(ui_status_icons, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
      ntp.tick(); //синхронизируем время
      if (ntp.synced() && lastFeed==0) lastFeed=ntp.hour()*60 + ntp.minute(); //заполняем время последнего кормления при первом запуске. По умолчанию ставим время первой синхронизации с с ервером времени
    }
  else
    {
      lv_obj_add_flag(ui_status_icons, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
    }
  
  //Проверяем значения таймеров
  if (reflvgl.isReady()) { lv_timer_handler();} //Обновляем экран
  if (reftime.isReady()) {if (lv_tabview_get_tab_act(ui_tabview)==0) {lv_label_set_text(ui_clock, ntp.timeString().c_str());}} //обновляем занчение часов на экране
  if (refremain.isReady()){feedRemain();} //Отображение времени оставшегося до кормления
  if (reffeedtime.isReady()) {reffeedtime.stop();feed(feedAmount);}//ожидание загрузки экрана кормления и запуск функции
  //Измерение температуры
  if (reftemp.isReady()) 
    {
      temperature = dht.readTemperature();
      lv_label_set_text_fmt(ui_temp_label, LV_SYMBOL_TEMP" %.1f °С",temperature);
      humidity = dht.readHumidity();
      lv_label_set_text_fmt(ui_humid_label, LV_SYMBOL_HUMID" %.1f%%",humidity);
    }
  if (refsaveconfigdelay.isReady()) {refsaveconfigdelay.stop();logStr+="Сохранение настроек..."; saveConfiguration("/config.json") ;} //сохраняем настройки
  if (refchecktime.isReady()) //проверка таймера кормления
      {
        static byte prevMin = 0;
          if (ntp.status()==0)
            {
              if (prevMin != ntp.minute()) 
                {//исключаем двойной запуск кормления при срабатывании таймера
                  prevMin = ntp.minute();
                  for (byte i = 0; i < sizeof(feedTime) / 2; i++)    // проверяем массив с расписанием
                  if (feedTime[i][0] == ntp.hour() && feedTime[i][1] == ntp.minute() &&feedTime[i][2] == 1) prefid(feedTime[i][3]);
                  //проверка включен ли ночной режим
                  if (daytime) 
                  {
                    if (ntp.hour()>=daybegin && ntp.hour()<dayend) {analogWrite(TFT_BACKLIGHT,bright_level);} else {analogWrite(TFT_BACKLIGHT,20);} 
                  } else {analogWrite(TFT_BACKLIGHT,bright_level);}
                  
                }
            }
  }
  if (usemqtt) client.loop(); //чтение состояния топиков MQQT
  if (tg_bot) bot.tick(); //поддерживаем соединение с telegram ботом
  server.handleClient(); //обработка запросов web интерфейса
  if (espRes)
    {
      bot.tickManual();
      ESP.restart();
    }
}


