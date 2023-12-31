/*
Все что касается веб интерфейса и OTA обновлений
*/
//Инициализация веб интерфейса
void server_init()
{
   //обработчики http запросов
   server.on("/",handle_main);//главный экран
   server.on("/mqttsetting",handle_mqtt_setting);//сохранение настроек mqtt
   server.on("/tgsetting",handle_tg_setting);//сохранение настроек телеграм бота
   server.on("/stepsetting",handle_step_setting);//сохранение настроек шагового двигателя
   server.on("/brightsetting",handle_bright_setting);//сохранение настроек подсветки
   server.on("/alarmsetting",handle_alarm_setting);//сохранение настроек будильников
   server.on("/feed",handle_feed);//выдача корма
   server.on("/fileman", HTTP_GET, handleFileman);
   server.on("/esprestart", handle_esp_restart);
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
 
  server.on("/delete", HTTP_GET, handleFileDelete);
  server.on("/edit", HTTP_POST, []()
    {server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Файл загружен. <a href=/fileman>Вернуться к списку</a>"); }, handleFileUpload); 
  server.onNotFound([](){if(!handleFileRead(server.uri())) server.send(404, "text/plain", "404 Файл не найден");});
   //запускаем сервер
   logStr+="Запуск веб интерфейса... ";
   server.begin();
   logStr+="Ок\n";
}

//Главная страница веб интерфейса
void handle_main()
{
  //Заголовки html
  String page = "<!DOCTYPE HTML><html>";
  page+="<head>";
  page+=favincon;
  page+="<title>Панель управления Cat Feeder 2</title>";
  page+="<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page+="<meta charset='UTF-8'>";
  //стили
  page+=styleHTML;

  //скрипты
  //заголовок
  page+="<script type='text/javascript'>";
  //Отображение значения слайдера
  page+="function showSliderValue(param_id){";
  page+="var id_text = param_id+'_text';";
	page+="var sliderValue = document.getElementById(param_id).value;";
  page+=" document.getElementById(id_text).innerHTML = sliderValue; }";

  //конец блока
  page+="</script>";
  page+="</head>";
  
  //тело страницы
  page+="<body>";
  page+="<h1 align='center'>Cat Feeder 2</h1>";

  //Блоки данных
  page+="<div class='mainblock'>"; //основной контейнер
  
  
  //Выдача корма
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Выдача корма</center></h2>";
  page+="<form method='get' action='/feed'>";
  page+="&nbsp;&nbsp;Размер порции: <span id='feedamount_text'>"+String(feedAmountSet)+"</span> грамм<br><center><input name='feedamount' id='feedamount' type='range' class='slider' min='0' max='60' step='1' onchange='showSliderValue(id)' value='"+String(feedAmountSet)+"'></center><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit' value='Выдать корм'></form>";
  page+="</div>";

  //Параметры шагового двигателя
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры шагового двигателя</center></h2>";
  page+="<form method='get' action='/stepsetting'>";
  page+="&nbsp;&nbsp;Шагов вперед: <span id='fwd_steps_text'>"+String(fwd_steps)+"</span><br><center><input name='fwd_steps' id='fwd_steps' type='range' class='slider' min='0' max='200' step='1' onchange='showSliderValue(id)' value='"+String(fwd_steps)+"'></center><br>";
  page+="&nbsp;&nbsp;Шагов назад: <span id='bck_steps_text'>"+String(bck_steps)+"</span> <br><center><input name='bck_steps' id='bck_steps' type='range' class='slider' min='0' max='200' step='1' onchange='showSliderValue(id)' value='"+String(bck_steps)+"'></center><br>";
  page+="&nbsp;&nbsp;Скорость вращения: <span id='step_speed_text'>"+String(step_speed)+"</span><br><center><input id='step_speed' name='step_speed' type='range' class='slider' min='0' max='400' step='1'  onchange='showSliderValue(id)' value='"+String(step_speed)+"'></center><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";

  //Параметры подсветки экрана
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подсветки экрана</center></h2>";
  page+="<form method='get' action='/brightsetting'>";
  page+="&nbsp;&nbsp;Яркость подсветки: <span id='bright_level'>"+String(bright_level)+"</span><br><center><input name='bright' id='bright' type='range' class='slider' min='0' max='255' step='1' onchange='showSliderValue(id)' value='"+String(bright_level)+"'></center><br>";
  page+="&nbsp;&nbsp;Отключение подсветки ночью: <span class='checkbox-apple'><input class='yep' type='checkbox' id='daytime' name='daytime' ";
  if (daytime) page+="checked";
  page+="><label for='daytime'></label></span><br><br>";
  page+="&nbsp;&nbsp;Начало дня: &nbsp; <input class='inputs' name='daybegin' type='number' min='0' max='12' length='2' value='"+String(daybegin)+"'>&nbsp;";
  page+="Конец дня: &nbsp; <input class='inputs' name='dayend' type='number' min='13' max='23' length='2' value='"+String(dayend)+"'><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";

  //Параметры будильников
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры будильников</center></h2>";
  page+="<form method='get' action='/alarmsetting'>";
  for (byte i = 0; i < 4; i++) 
  {
    String alarmnumber=String(i);
    page+="&nbsp;&nbsp;Будильник "+alarmnumber+":&nbsp;&nbsp; <input class='inputs' type='time' name='alarm"+alarmnumber+"'  length='6' value='";
    if (feedTime[i][0]<10) page+="0";
    page+=String(feedTime[i][0])+":";
    if (feedTime[i][1]<10) page+="0";
    page+=String(feedTime[i][1])+"'>&nbsp;";
    page+="Размер порции: &nbsp; <input class='inputs' name='asize"+alarmnumber+"' type='number' min='1' max='60' length='2' value='"+String(feedTime[i][3])+"'>&nbsp;";
    page+="<span class='checkbox-apple'><input class='yep' type='checkbox' name='alarmcheck"+alarmnumber+"' id='alarmcheck"+alarmnumber+"' ";
    if (feedTime[i][2]==1) page+="checked";
    page+="><label for='alarmcheck"+alarmnumber+"'></label></span><br>";
  }
  
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";

  //MQTT
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к MQTT брокеру</center></h2>";
  page+="<form method='get' action='/mqttsetting'>";
  page+="<br>&nbsp;&nbsp;MQTT: <span class='checkbox-apple'><input class='yep' type='checkbox' id='usemqtt' name='usemqtt' ";
  if (usemqtt) page+="checked";
  page+="><label for='usemqtt'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Сервер: <input class='inputs' type='text' name='server'  length=32 value='"+mqtt_server+"'>&nbsp;</label>&nbsp;&nbsp;";
  page+="&nbsp;&nbsp;<label>Порт: <input class='inputs' name='port' type='number' length=6 value='"+String(mqtt_port)+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Логин: <input class='inputs' type='text' name='login'  length=32 value='"+mqtt_login+"'>&nbsp;</label>&nbsp;&nbsp;<label>Пароль: <input class='inputs' name='pass' type='password' length=64 value='"+mqtt_pass+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Топик управления: <input class='inputs' type='text' name='cmd'  length=100 value='"+cmdTopic+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Топик статуса: <input class='inputs' name='status' type='text' length=100 value='"+statusTopic+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";
  //telegram
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к Telegram боту</center></h2>";
  page+="<form method='get' action='/tgsetting'>";
  page+="<br>&nbsp;&nbsp;Бот: <span class='checkbox-apple'><input class='yep' type='checkbox' id='tg_bot' name='tg_bot' ";
  if (tg_bot) page+="checked";
  page+="><label for='tg_bot'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Токен: <input class='inputs' type='text' name='token'  length=40 value='"+bot_token+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>ID чата: <input class='inputs' name='chatid' type='number' length=10 value='"+chatID+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";
  //Температура и влажность
  page+="<div class='icon-block'>";
  page+="<svg width='24px' height='22px' fill='none' class='icon' xmlns='http://www.w3.org/2000/svg'>";
  page+="<path d='M12 15.9998C11.4477 15.9998 11 16.4475 11 16.9998C11 17.5521 11.4477 17.9998 12 17.9998C12.5523 17.9998 13 17.5521 13 16.9998C13 16.4475 12.5523 15.9998 12 15.9998ZM12 15.9998V6M12 16.9998L12.0071 17.0069M16 16.9998C16 19.209 14.2091 20.9998 12 20.9998C9.79086 20.9998 8 19.209 8 16.9998C8 15.9854 8.37764 15.0591 9 14.354L9 6C9 4.34315 10.3431 3 12 3C13.6569 3 15 4.34315 15 6V14.354C15.6224 15.0591 16 15.9854 16 16.9998Z' stroke='#000000'";
  page+="stroke-width='2' stroke-linecap='round' stroke-linejoin='round'/></svg>";
  page+=String(temperature)+"°С ";
  page+="<svg fill='#000000' width='24px' height='22px' class='icon' xmlns='http://www.w3.org/2000/svg'>";
  page+="<path d='M12,22c2.579,0,4-1.35,4-3.8,0-3.243-3.237-5.871-3.375-5.981a1,1,0,0,0-1.25,0C11.237,12.329,8,14.957,8,18.2,8,20.65,9.421,22,12,22Zm0-7.639A6.153,6.153,0,0,1,14,18.2c0,1.112-.335,1.8-2,1.8s-2-.688-2-1.8A6.153,6.153,0,0,1,12,14.361ZM6.625,2.219a1,1,0,0,0-1.25,0C5.237,2.329,2,4.957,2,8.2,2,10.65,3.421,12,6,12s4-1.35,4-3.8C10,4.957,6.763,2.329,6.625,2.219ZM6,10c-1.665,0-2-.688-2-1.8A6.153,6.153,0,0,1,6,4.361,6.153,6.153,0,0,1,8,8.2C8,";
  page+="9.312,7.665,10,6,10ZM18.625,2.219a1,1,0,0,0-1.25,0C17.237,2.329,14,4.957,14,8.2c0,2.45,1.421,3.8,4,3.8s4-1.35,4-3.8C22,4.957,18.763,2.329,18.625,2.219ZM18,10c-1.665,0-2-.688-2-1.8a6.153,6.153,0,0,1,2-3.839A6.153,6.153,0,0,1,20,8.2C20,9.312,19.665,10,18,10Z'/></svg>";
  page+=String(humidity)+"%";
  page+="</div>";
  page+="<a href='/fileman' class='links'>Файловый менеджер</a>";

  //Завершающий код
  page+="</div>";
  page+="</body>";
  page+="</html>";
server.send(200, "text/html", page);
} 

//Применение настроек MQTT
void handle_mqtt_setting()
{
  mqtt_server = server.arg("server");
  mqtt_port = server.arg("port").toInt();
  mqtt_login = server.arg("login");
  mqtt_pass = server.arg("pass");
  cmdTopic = server.arg("cmd");
  statusTopic = server.arg("status");
  if (server.arg("usemqtt")=="on")  {usemqtt=1;lv_obj_add_state(ui_set_panel_usemqtt_switch, LV_STATE_CHECKED);} else {usemqtt=0;lv_obj_clear_state(ui_set_panel_usemqtt_switch, LV_STATE_CHECKED);}
  lv_textarea_set_text(ui_set_panel_mqtt_adress_ta, mqtt_server.c_str());
  lv_textarea_set_text(ui_set_panel_mqtt_port_ta, String(mqtt_port).c_str());
  lv_textarea_set_text(ui_set_panel_mqtt_login_ta, mqtt_login.c_str());
  lv_textarea_set_text(ui_set_panel_mqtt_pass_ta, mqtt_pass.c_str());
  lv_textarea_set_text(ui_set_panel_mqtt_control_ta, cmdTopic.c_str());
  lv_textarea_set_text(ui_set_panel_mqtt_status_ta, statusTopic.c_str());
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
}

//Применение настроек телеграм бота
void handle_tg_setting()
{
  bot_token = server.arg("token");
  chatID = server.arg("chatid");
  if (server.arg("tg_bot")=="on")  {tg_bot=1; lv_obj_add_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED);} else {tg_bot=0; lv_obj_clear_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED);}
  lv_textarea_set_text(ui_set_panel_telegram_token_ta, bot_token.c_str());
  lv_textarea_set_text(ui_set_panel_telegram_chatid_ta, chatID.c_str());
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
       server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
    
}

void handle_esp_restart()
  {
    server.sendHeader("Location", "/",true);   //редирект на главную
    server.send(302, "text/plane","");
    delay(1000);//задержка для перехода на главную страницу
    espRes=true; //устанавливаем флаг перезагрузки
  }


void handle_alarm_setting() 
  { 
  String Time="";              
  for (byte i=0; i<4; i++){
    Time=server.arg("alarm"+String (i));
     feedTime[i][0]=Time.substring(0,2).toInt();
     feedTime[i][1]=Time.substring(3,5).toInt();
     if (server.arg("alarmcheck"+String (i))=="on") feedTime[i][2]=1; else feedTime[i][2]=0;
     feedTime[i][3]=server.arg("asize"+String (i)).toInt(); 
  }
  //изменяем значения элементов на дисплее
  lv_roller_set_selected(ui_timer1_hour, feedTime[0][0], LV_ANIM_OFF);
  lv_roller_set_selected(ui_timer1_minute, feedTime[0][1], LV_ANIM_OFF);
  lv_spinbox_set_value(ui_timer1_amount,feedTime[0][3]);
  if (feedTime[0][2]==1) {lv_obj_add_state(ui_timer1_check, LV_STATE_CHECKED);} else {lv_obj_clear_state(ui_timer1_check, LV_STATE_CHECKED);}
  lv_roller_set_selected(ui_timer2_hour, feedTime[1][0], LV_ANIM_OFF);
  lv_roller_set_selected(ui_timer2_minute, feedTime[1][1], LV_ANIM_OFF);
  lv_spinbox_set_value(ui_timer2_amount,feedTime[1][3]);
  if (feedTime[1][2]==1) {lv_obj_add_state(ui_timer2_check, LV_STATE_CHECKED);} else {lv_obj_clear_state(ui_timer2_check, LV_STATE_CHECKED);}
  lv_roller_set_selected(ui_timer3_hour, feedTime[2][0], LV_ANIM_OFF);
  lv_roller_set_selected(ui_timer3_minute, feedTime[2][1], LV_ANIM_OFF);
  lv_spinbox_set_value(ui_timer3_amount,feedTime[2][3]);
  if (feedTime[2][2]==1) {lv_obj_add_state(ui_timer3_check, LV_STATE_CHECKED);} else {lv_obj_clear_state(ui_timer3_check, LV_STATE_CHECKED);}
  lv_roller_set_selected(ui_timer4_hour, feedTime[3][0], LV_ANIM_OFF);
  lv_roller_set_selected(ui_timer4_minute, feedTime[3][1], LV_ANIM_OFF);
  lv_spinbox_set_value(ui_timer4_amount,feedTime[3][3]);
  if (feedTime[3][2]==1) {lv_obj_add_state(ui_timer4_check, LV_STATE_CHECKED);} else {lv_obj_clear_state(ui_timer4_check, LV_STATE_CHECKED);}
//формируем ответ сервера
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
       server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
}

//Применение настроек шагового двигателя
void handle_step_setting()
{
  fwd_steps = server.arg("fwd_steps").toInt();;
  bck_steps = server.arg("bck_steps").toInt();
  step_speed = server.arg("step_speed").toInt();
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
}

//Применение настроек подсветки
void handle_bright_setting()
{
  bright_level = server.arg("bright").toInt();;
  daybegin = server.arg("daybegin").toInt();
  dayend = server.arg("dayend").toInt();
  lv_slider_set_value(ui_set_panel_display_bright_slider, bright_level, LV_ANIM_OFF); 
  lv_label_set_text_fmt(ui_backlight_slider_label, "%d%", (int)lv_slider_get_value(ui_set_panel_display_bright_slider));
  if (server.arg("daytime")=="on")  daytime=true; else daytime=false;
  if (daytime){lv_obj_add_state(ui_set_display_daytime_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_display_daytime_switch, LV_STATE_CHECKED);}
  lv_slider_set_left_value(ui_slider_day_time, daybegin, LV_ANIM_OFF);
  lv_slider_set_value(ui_slider_day_time, dayend, LV_ANIM_OFF);
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
}

//Выдача корма
void handle_feed()
{
  feedAmountSet = server.arg("feedamount").toInt();;
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      lv_slider_set_value(ui_slider_feed_amount, feedAmountSet, LV_ANIM_OFF);
      lv_label_set_text_fmt(ui_label_feedAmount,"%d грамм", feedAmountSet);
      prefid(feedAmountSet);
      server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
    
}

void handleFileSysFormat() {
	SPIFFS.format();
	server.send(200, "text/json", "format complete");
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  Serial.printf_P(PSTR("Чтение файла: %s\r\n"), path.c_str());
  if(path.endsWith("/")) path += "";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
	{
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
		Serial.println("Успешно");
    return true;
  }
	Serial.printf("Ошибка чтения '%s', тип '%s'\n", path.c_str(), contentType.c_str()) ;
  return false;
}

// загрузка файла в ФС
void handleFileUpload(){
  bool OK = false;
  if(server.uri() != "/edit") return;
 
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.printf_P(PSTR("Загружаем файл: %s\r\n"), filename.c_str());
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } 
  else if(upload.status == UPLOAD_FILE_WRITE)
  {
    Serial.printf_P(PSTR("Загружено: %d\r\n"), upload.currentSize);
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } 
  else if(upload.status == UPLOAD_FILE_END)
  {
    if(fsUploadFile)
      fsUploadFile.close();    
    OK = true;
    Serial.printf_P(PSTR("Всего: %d\r\n"), upload.totalSize);
    sprintf(tempBuf,"Файл загружен [%s] %s\n", upload.filename.c_str(), (OK)? "OK" : "failed");
    logStr += tempBuf;
  }
}

void handleFileDelete(){
	 if(!server.hasArg("file")) {server.send(500, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Неверный аргумент. <a href=/fileman>Вернуться к списку</a>"); return;}
	String path = server.arg("file");
  Serial.printf_P(PSTR("Удаление файла: '%s'\r\n"),path.c_str());
  if(path == "/")
    return server.send(500, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman>Невозможно удалить корневую папку. <a href=/fileman>Вернуться к списку</a>");
  if(!SPIFFS.exists(path))
    return server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Файл не найден. <a href=/fileman>Вернуться к списку</a>");
  SPIFFS.remove(path);
  server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Файл удален. <a href=/fileman>Вернуться к списку</a>");
  logStr += "Удален ";
  logStr += path;
  logStr +="\n";
  path = String();
}

void handleFileCreate(){
  if(server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  Serial.printf_P(PSTR("handleFileCreate: %s\r\n"),path.c_str());
  if(path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if(SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  if(!path.startsWith("/")) path = "/"+path;    // is this needed for LittleFS?
  File file = SPIFFS.open(path, "w");
  if(file)
	{
    file.close();
		Serial.printf("Created file [%s]\n",path.c_str());
	}
  else
	{
		Serial.printf("Create file [%s] failed\n",path.c_str());
    return server.send(500, "text/plain", "CREATE FAILED");
	}
  server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Файл создан. <a href=/fileman>Вернуться к списку</a>");
  logStr += "Создан ";
  logStr += path;
  logStr +="\n";
  path = String();
}

// скрипты файлового менеджера
const char edit_script[] PROGMEM = R"rawliteral(
<script>
  var fName; 
  function doEdit(item) 
  {
    console.log('clicked', item); 
    fName = item;
    var fe = document.getElementById("nameSave");
    fe.value = fName;
  } 
  function saveFile()
  {
    console.log('Save', fName);
  }
  function scrollToBottom(el) // log window
  {
    //console.log("scrolling", el);
    var element = document.getElementById(el);
    element.scrollTop = element.scrollHeight;   
  }
</script>
)rawliteral";

// Генерация страницы файлового менеджера
void handleFileman() {
  bool foundText = false; //найден текст
  bool foundName = false; //найдено имя
  bool foundMode = false; //режим поиска
  bool foundSaveBut = false;
  char filebuf[FILEBUFSIZ];
  char fileName[128];
  File file;
	String path = "", bText = "", bName = "", bMode ="";
  String page = "<!DOCTYPE HTML><html>";
  page+="<head>";
  page+=favincon;
  // Проверка аргументов функции
  if(server.hasArg("mode"))
  {
    bMode = server.arg("mode");
    if(bMode.length() > 0) 
      foundMode = true;
    Serial.printf("Режим %s\n",bMode.c_str());
  }
  if(server.hasArg("dir"))
		path = server.arg("dir");
	else
		path ="/";
  if(server.hasArg("editBlock"))
  {
    bText = server.arg("editBlock");
    if(bText.length() > 0) 
      foundText = true;
  }
  
  if(server.hasArg("nameSave"))
  {
    bName = server.arg("nameSave");
    if(bName.length() > 0) 
      foundName = true;
    if(!bName.startsWith("/")) bName = "/"+ bName;    // is this needed for LittleFS?   
  }
  
  if(server.hasArg("saveBut"))
  {
    foundSaveBut = true;
  }

  // запись
  if(foundName && foundText && bMode == "save")
  {
    Serial.println("Сохраняем");
    file = SPIFFS.open(bName, "w");
    if(file)
    {
      file.write((uint8_t *)bText.c_str(), bText.length());
      file.close();
      logStr += "Сохранено";
      logStr += bName;
      logStr +="\n";
    }  
  }
  
  File dir = SPIFFS.open(path.c_str());
  if(!dir)
	  Serial.printf("Папка [%s]не найдена", path.c_str());
	else if(!dir.isDirectory())
        Serial.println(" - не папка");
  
  page+="<title>Файловый менеджер Cat Feeder 2</title>";
  page+="<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page+="<meta charset='UTF-8'>";
  //стили
  page+=styleHTML;
  //конец блока
  page+="</head>";
  
  //тело страницы
  page+="<body>";
  page+="<h1 align='center'>Файловый менеджер Cat Feeder 2</h1>";

  //Блоки данных
  page+="<div class='mainblock'>"; //основной контейнер
  //OTA
  page+="<div class='contentblock'>";
  page+= otaHTML;
  page+= "</div>";
    
   //Загрузка файлов 
   page+="<div class='contentblock'>";
   page+= "<h2 class='title'><center>Загрузка файлов</center></h2>";
   page+="<form action='/edit' method='post' enctype='multipart/form-data'><BR>"; // use post for PUT /edit server handler
   page+= "&nbsp;&nbsp;Имя: <input class='buttons' type='file' name='data' required>&nbsp;&nbsp;";
   page+= "Путь: <input type='text' class='inputs' name='path' value='/'>";
   page+= "<input type='hidden' name='mode' value='upload'>";
   page+= "<button class='buttons'>Загрузить</button>";
   page+= "</form>";
   page+= "</div>";
  
   //Содержимое файловой системы
   page+="<div class='contentblock'>";
   page+= "<h2 class='title'><center>Файловая система</center></h2>";
	File entry;
  page+="&nbsp;&nbsp;Содержимое файловой системы:<br>";
  page+="<table border='1'><tr>";
  while(entry = dir.openNextFile())
  {  
	  page+="<td>";
    bool isDir = entry.isDirectory(); // проверяем является ли текущий элемент папкой
		if(isDir) 
		{
			page += "<a href=/fileman?dir=/" ;
			page +=  entry.name(); 
			page +=  ">";
		}
    strcpy(fileName, entry.name());
    page += String(entry.name());
		if(isDir) 
		  page += "</a>";
		page += " (";
    page += String(entry.size());
		page += ")</td>";
    // редактирование
    page += "<td><a href=/fileman?mode=edit&nameSave="; 
    if(fileName[0] != '\\' && fileName[0] != '/') // avoid double \ or / in filename (on some OS)
   	 page += path;
    page += String(entry.name());
    page += ">Изменить</a></td>";
    // удаление
    page += "<td><a href=/delete?file="; 
    if(fileName[0] != '\\' && fileName[0] != '/') // avoid double \ or / in filename (on some OS)
	page += path;
    page += String(entry.name());
    page += ">Удалить</a></td></tr>";
    entry.close();
  }
  page+="</table>";	
   // форма редактирования
   page += "<div><form action='/fileman' method='get'><textarea class='textareas' name=editBlock id=editBlock rows='15' cols='60'>";
  
   if(bMode = "edit")
   {
      // читаем файл и вставляем содержимое
      file = SPIFFS.open(bName.c_str(), "r");
      if(file)
      {        
        Serial.printf("File read avail %i, ",file.available());
        int readlen = (file.available() < FILEBUFSIZ) ? file.available() : FILEBUFSIZ;   
        file.read((uint8_t *)filebuf, readlen);
        file.close();        
        filebuf[readlen] = '\0';
        page += filebuf;
        logStr += (foundSaveBut) ? "Сохранение " : "Редактирование ";
        logStr += bName;
        logStr +="\n";   
      }
   }
   page += "</textarea></div>\n";
   page += "<input type=hidden name='mode' value='save'>";
   page += "&nbsp;&nbsp;Имя файла: <input class='inputs' type=text value='";
   if(bMode = "edit")
     page += bName;        
   page += "' name=nameSave id=nameSave> <input type=submit class='buttons' name=saveBut onclick=saveFile() value='Сохранить'></form></div>\n";
   //Лог
   page+="<div class='contentblock'>";
   page+= "<h2 class='title'><center>Лог действий</center></h2>";
   page += "&nbsp;&nbsp;Лог: <form><textarea class='textareas' name=log id=log rows='5' cols='85'>";
   page += logStr;
   page += "</textarea></form>";
   page += "<BR><a class='links' href='/esprestart'>Перезагрузить устройство</a>";
   page += "</div>";
   page += "<a class='links' href='/'>На главную</a>";
   //Закрывающие теги
   page += "</div></body>";
   page += edit_script;
   page+="</html>";
	server.send(200, "text/html", page);
}

bool initFS(bool format = false, bool force = false) 
{
 bool fsFound = SPIFFS.begin();
   if(!fsFound) 
     Serial.println(F("No file system found. Please format it."));
     
   if(!format)
     return fsFound;
     
  // Форматирование
	if(!fsFound || force) 
	{      
    Serial.println(F("Форматирование ФС."));
	  if(SPIFFS.format()) 
    {
			  if(SPIFFS.begin())
        {
          Serial.println(F("Форматирование окончено."));
          return true;
        }
    }      
    Serial.println(F("Ошибка форматирования."));
    return false;              
	}	
  return false; 
}
