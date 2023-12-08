//Инициализация веб интерфейса
void server_init()
{
   //обработчики http запросов
   server.on("/",handle_main);//главный экран
   server.on("/mqttsetting",handle_mqtt_setting);//сохранение настроек mqtt
   server.on("/tgsetting",handle_tg_setting);//сохранение настроек телеграм бота
   server.on("/stepsetting",handle_step_setting);//сохранение настроек шагового двигателя
   server.on("/feed",handle_feed);//выдача корма
   server.on("/fileman", HTTP_GET, handleFileman);
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
  // second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []()
    {server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/fileman'>Файл загружен. <a href=/fileman>Вернуться к списку</a>"); }, handleFileUpload); 
  server.onNotFound([](){if(!handleFileRead(server.uri())) server.send(404, "text/plain", "404 Файл не найден");});
   //запускаем сервер
   server.begin();
}

//Главная страница веб интерфейса
void handle_main()
{
  //Заголовки html
  String page = "<!DOCTYPE HTML><html>";
  page+="<head>";
  page+="<title>Панель управления Cat Feeder 2</title>";
  page+="<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page+="<meta charset='UTF-8'>";
  //стили
  page+="<style>";
  page+=".icon-block {position: fixed; top: 1%; right: 2%;}";
  page+=".mainblock {display: flex; width: 60%; color: white; margin: 10px auto 10px auto; padding:20px; flex-direction: column; align-items: center; justify-content: center; background: Lightcyan;}";
  page+=".contentblock {width: 90%; margin: 10px auto; background-color: Skyblue; color:black; border-radius:10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}";
  page+=".title { background-color: Dodgerblue; color: white;margin: 0; padding: 10px; box-sizing: border-box; border-radius: 10px 10px 0 0}";
  page+=".infotitle { background-color: Forestgreen; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".warningtitle { background-color: Crimson; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".content { background-color: Lightcyan; color: black; margin: 0; padding: 10px; border-radius:10px;}";
  page+=".main {display:block; margin:auto; color: black; background: 	Skyblue; padding: 10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}"; 
  page+=".inputs {padding:10px; border-radius:10px; margin: 10px auto; background: #d3d3d3}";
  page+=".buttons {padding:10px;border-radius:10px; background: Dodgerblue; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 98%; border-style:hidden}";
  page+=".links {padding:10px;border-radius:10px; background: Sandybrown; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 88%; }";
  page+=".select-css {width: 100%; display: block; font-size: 16px; font-family: sans-serif; font-weight: 700; color: #444; line-height: 1.3; padding: .6em 1.4em .5em .8em; width: 100%; max-width: 100%;";
  page+="box-sizing: border-box; margin: 0; border: 1px solid #aaa; box-shadow: 0 1px 0 1px rgba(0,0,0,.04); border-radius: .5em;";
  page+="-moz-appearance: none; -webkit-appearance: none; appearance: none; background-color: #fff;"; 
  page+="background-image: url('data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22292.4%22%20height%3D%22292.4%22%3E%3Cpath%20fill%3D%22%23007CB2%22%20d%3D%22M287%2069.4a17.6%2017.6%200%200%200-13-5.4H18.4c-5%200-9.3%201.8-12.9%205.4A17.6%2017.6%200%200%200%200%2082.2c0%205%201.8%209.3%205.4%2012.9l128%20127.9c3.6%203.6%207.8%205.4%2012.8%205.4s9.2-1.8%2012.8-5.4L287%2095c3.5-3.5%205.4-7.8%205.4-12.8%200-5-1.9-9.2-5.5-12.8z%22%2F%3E%3C%2Fsvg%3E'), linear-gradient(to bottom, #ffffff 0%,#e5e5e5 100%);"; 
  page+="background-repeat: no-repeat, repeat; background-position: right .7em top 50%, 0 0; background-size: .65em auto, 100%;}";
  page+=".slider {-webkit-appearance: none; appearance: none; width: 98%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}";
  page+=".slider:hover { opacity: 1;}";
  page+=".slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}";
  page+=".slider::-moz-range-thumb {width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}";  
  page+=".icon{fill: #000000;}";
  page+=".checkbox-apple {position: relative; width: 50px; height: 25px; margin: 20px; webkit-user-select: none; -moz-user-select: none;-ms-user-select: none; user-select: none;}";
  page+=".checkbox-apple label {position: absolute; top: 0; left: 0; width: 50px; height: 25px; border-radius: 50px; background: linear-gradient(to bottom, #b3b3b3, #e6e6e6); cursor: pointer; transition: all 0.3s ease;}";
  page+=".checkbox-apple label:after {content: ''; position: absolute; top: 1px; left: 1px; width: 23px; height: 23px; border-radius: 50%; background-color: #fff; box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3); transition: all 0.3s ease;}";
  page+=".checkbox-apple input[type='checkbox']:checked + label {background: linear-gradient(to bottom, #005b98, #1e90ff);}";
  page+=".checkbox-apple input[type='checkbox']:checked + label:after {transform: translateX(25px);}";
  page+=".checkbox-apple label:hover {background: linear-gradient(to bottom, #b3b3b3, #e6e6e6);}";
  page+=".checkbox-apple label:hover:after {box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3);}";
  page+=".checkbox-apple input {opacity: 0;width: 0; height: 0;}";
  page+=".yep {position: absolute;top: 0;left: 0;width: 50px;height: 25px;}";
  page+="</style>";

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
  page+="&nbsp;&nbsp;Размер порции: <span id='feedamount_text'>"+String(feedAmountSet)+"</span> грамм<br><center><input name='feedamount' id='feedamount' type='range' class='slider' min='0' max='250' step='1' onchange='showSliderValue(id)' value='"+String(feedAmountSet)+"'></center><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit' value='Выдать корм'></form>";
  page+="</div>";
  //Параметры шагового двигателя
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры шагового двигателя</center></h2>";
  page+="<form method='get' action='/stepsetting'>";
  page+="&nbsp;&nbsp;Шагов вперед: <span id='fwd_steps_text'>"+String(fwd_steps)+"</span><br><center><input name='fwd_steps' id='fwd_steps' type='range' class='slider' min='0' max='200' step='1' onchange='showSliderValue(id)' value='"+String(fwd_steps)+"'></center><br>";
  page+="&nbsp;&nbsp;Шагов назад: <span id='bck_steps_text'>"+String(bck_steps)+"</span> <br><center><input name='bck_steps' id='bck_steps' type='range' class='slider' min='0' max='200' step='1' onchange='showSliderValue(id)' value='"+String(bck_steps)+"'></center><br>";
  page+="&nbsp;&nbsp;Скорость вращения: <span id='step_speed_text'>"+String(step_speed)+"</span><br><center><input id='step_speed' name='step_speed' type='range' class='slider' min='0' max='400' step='1'  onchange='showSliderValue(id)' value='"+String(step_speed)+"'></center><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";
  //MQTT
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к MQTT брокеру</center></h2>";
  page+="<form method='get' action='/mqttsetting'>";
  page+="<br>&nbsp;&nbsp;Использовать MQTT: <span class='checkbox-apple'><input class='yep' type='checkbox' id='usemqtt' name='usemqtt' ";
  if (usemqtt) page+="checked";
  page+="><label for='tgbot'></label></span><br><br>";
  page+=";&nbsp;&nbsp;<label>Сервер: <input class='inputs' type='text' name='server'  length=32 value='"+mqtt_server+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Порт: <input class='inputs' name='port' type='number' length=6 value='"+String(mqtt_port)+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Логин: <input class='inputs' type='text' name='login'  length=32 value='"+mqtt_login+"'>&nbsp;</label><br>&nbsp;&nbsp;<label>Пароль: <input class='inputs' name='pass' type='password' length=64 value='"+mqtt_pass+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";
  //telegram
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к Telegram боту</center></h2>";
  page+="<form method='get' action='/tgsetting'>";
  page+="<br>&nbsp;&nbsp;Использовать бота: <span class='checkbox-apple'><input class='yep' type='checkbox' id='tgbot' name='tg_bot' ";
  if (tg_bot) page+="checked";
  page+="><label for='tgbot'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Токен: <input class='inputs' type='text' name='token'  length=40 value='"+bot_token+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>ID чата: <input class='inputs' name='chatid' type='number' length=10 value='"+chatID+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";

  page+="<a href='#' class='links'>Статистика</a>";

  //Блок иконок
  page+="<div class='icon-block'><a href='/fileman'><svg class='icon' width='50' height='50' fill='none' xmlns='http://www.w3.org/2000/svg'>";
  page+="<g id='SVGRepo_bgCarrier' stroke-width='0'></g><g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'></g><g id='SVGRepo_iconCarrier'><path id='primary' d='M6,5H16a2,2,0,0,1,2,2v7' style='fill: none; stroke: #000000; stroke-linecap: round; stroke-linejoin: round; stroke-width: 2;'></path><path id='primary-2' data-name='primary' d='M18,19H8a2,2,0,0,1-2-2V10' style='fill: none; stroke: #000000; stroke-linecap: round; stroke-linejoin: round; stroke-width: 2;'></path><polyline id='primary-3' data-name='primary' points='15 11 18 14 21 11' style='fill: none; stroke: #000000; stroke-linecap: round; stroke-linejoin: round; stroke-width: 2;'></polyline><polyline id='primary-4' data-name='primary' points='9 13 6 10 3 13' style='fill: none; stroke: #000000; stroke-linecap: round; stroke-linejoin: round; stroke-width: 2;'></polyline></g></svg>";
  page+="</a> </div>";
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
  usemqtt = server.arg("usemqtt").toInt();
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      //page="{'Успешно':'Cохранено в память устройства.'}";
      //statusCode = 200;
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
  tg_bot = server.arg("tg_bot").toInt();
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
       server.sendHeader("Location", "/",true);   //редирект на главную
      server.send(302, "text/plane","");
      //page="{'Успешно':'Cохранено в память устройства.'}";
      //statusCode = 200;
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
      //page="{'Успешно':'Cохранено в память устройства.'}";
      //statusCode = 200;
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Отправляем 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", page);
    }
    //server.sendHeader("Access-Control-Allow-Origin", "*");
    //server.send(statusCode, "application/json", page);
    
}

//Выдача корма
void handle_feed()
{
  feedAmountSet = server.arg("feedamount").toInt();;
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      //page="{'Успешно':'Cохранено в память устройства.'}";
      //statusCode = 200;
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
    //server.sendHeader("Access-Control-Allow-Origin", "*");
    //server.send(statusCode, "application/json", page);
    
}

void handleFileSysFormat() {
	FILESYS.format();
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
  Serial.printf_P(PSTR("handleFileRead: %s\r\n"), path.c_str());
  if(path.endsWith("/")) path += "";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(FILESYS.exists(pathWithGz) || FILESYS.exists(path))
	{
    if(FILESYS.exists(pathWithGz))
      path += ".gz";
    File file = FILESYS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
		Serial.println("Read OK");
    return true;
  }
	Serial.printf("Read failed '%s', type '%s'\n", path.c_str(), contentType.c_str()) ;
  return false;
}

// uses edit mode uploader
void handleFileUpload(){
  bool OK = false;
  if(server.uri() != "/edit") return;
 
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.printf_P(PSTR("handleFileUpload Name: %s\r\n"), filename.c_str());
    fsUploadFile = FILESYS.open(filename, "w");
    filename = String();
  } 
  else if(upload.status == UPLOAD_FILE_WRITE)
  {
    Serial.printf_P(PSTR("handleFileUpload Data: %d\r\n"), upload.currentSize);
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } 
  else if(upload.status == UPLOAD_FILE_END)
  {
    if(fsUploadFile)
      fsUploadFile.close();    
    OK = true;
    Serial.printf_P(PSTR("handleFileUpload Size: %d\r\n"), upload.totalSize);
    sprintf(tempBuf,"File upload [%s] %s\n", upload.filename.c_str(), (OK)? "OK" : "failed");
    logStr += tempBuf;
  }
}

void handleFileDelete(){
	 if(!server.hasArg("file")) {server.send(500, "text/html", "<meta http-equiv='refresh' content='1;url=/main'>Bad arguments. <a href=/main>Back to list</a>"); return;}
  //if(server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");  
	String path = server.arg("file");
	//String path = server.arg(0);
  Serial.printf_P(PSTR("handleFileDelete: '%s'\r\n"),path.c_str());
  if(path == "/")
    return server.send(500, "text/html", "<meta http-equiv='refresh' content='1;url=/main>Can't delete root directory. <a href=/main>Back to list</a>");
  if(!FILESYS.exists(path))
    return server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/main'>File not found. <a href=/main>Back to list</a>");
  FILESYS.remove(path);
  server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/main'>File deleted. <a href=/main>Back to list</a>");
  logStr += "Deleted ";
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
  if(FILESYS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  if(!path.startsWith("/")) path = "/"+path;    // is this needed for LittleFS?
  File file = FILESYS.open(path, "w");
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
  server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/main'>File created. <a href=/main>Back to list</a>");
  logStr += "Created ";
  logStr += path;
  logStr +="\n";
  path = String();
}

// main page JS scripts 
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

// generate HTML for main page
void handleFileman() {
  bool foundText = false;
  bool foundName = false;
  bool foundMode = false;
  bool foundSaveBut = false;
  char filebuf[FILEBUFSIZ];
  char fileName[128];
  File file;
	String path = "", bText = "", bName = "", bMode ="";
  String page = "";
  // check arguments
  if(server.hasArg("mode"))
  {
    bMode = server.arg("mode");
    if(bMode.length() > 0) 
      foundMode = true;
    Serial.printf("Mode %s\n",bMode.c_str());
  }
  if(server.hasArg("dir"))// {server.send(500, "text/plain", "BAD ARGS"); return;}  
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

  // write
  if(foundName && foundText && bMode == "save")
  {
    Serial.println("something to save");
    file = FILESYS.open(bName, "w");
    if(file)
    {
      file.write((uint8_t *)bText.c_str(), bText.length());
      file.close();
      logStr += "Saved ";
      logStr += bName;
      logStr +="\n";
    }  
  }
  
  File dir = FILESYS.open(path.c_str());
  if(!dir)
	  Serial.printf("Directory [%s]not found", path.c_str());
	else if(!dir.isDirectory())
        Serial.println(" - not a directory");
	//path = String();
  
  page+="<title>Файловый менеджер Cat Feeder 2</title>";
  page+="<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page+="<meta charset='UTF-8'>";
  //стили
  page+="<style>";
  page+=".icon-block {position: fixed; top: 1%; right: 2%;}";
  page+=".mainblock {display: flex; width: 60%; color: white; margin: 10px auto 10px auto; padding:20px; flex-direction: column; align-items: center; justify-content: center; background: Lightcyan;}";
  page+=".contentblock {width: 90%; margin: 10px auto; background-color: Skyblue; color:black; border-radius:10px;}";
  page+=".title { background-color: Dodgerblue; color: white;margin: 0; padding: 10px; box-sizing: border-box; border-radius: 10px 10px 0 0}";
  page+=".infotitle { background-color: Forestgreen; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".warningtitle { background-color: Crimson; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".content { background-color: Lightcyan; color: black; margin: 0; padding: 10px; border-radius:10px;}";
  page+=".main {display:block; margin:auto; color: black; background: 	Skyblue; padding: 10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}"; 
  page+=".inputs {padding:10px; border-radius:10px; margin: 10px auto; background: #d3d3d3}";
  page+=".textareas {padding:10px; border-radius:10px; margin: 10px auto 10px auto; width: 90%; display: flex; background: #d3d3d3}";
  page+=".buttons {padding:10px;border-radius:10px; background: Dodgerblue; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; border-style:hidden}";
  page+=".links {padding:10px;border-radius:10px; background: Sandybrown; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 88%; }";
  page+=".select-css {width: 100%; display: block; font-size: 16px; font-family: sans-serif; font-weight: 700; color: #444; line-height: 1.3; padding: .6em 1.4em .5em .8em; width: 100%; max-width: 100%;";
  page+="box-sizing: border-box; margin: 0; border: 1px solid #aaa; box-shadow: 0 1px 0 1px rgba(0,0,0,.04); border-radius: .5em;";
  page+="-moz-appearance: none; -webkit-appearance: none; appearance: none; background-color: #fff;"; 
  page+="background-image: url('data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22292.4%22%20height%3D%22292.4%22%3E%3Cpath%20fill%3D%22%23007CB2%22%20d%3D%22M287%2069.4a17.6%2017.6%200%200%200-13-5.4H18.4c-5%200-9.3%201.8-12.9%205.4A17.6%2017.6%200%200%200%200%2082.2c0%205%201.8%209.3%205.4%2012.9l128%20127.9c3.6%203.6%207.8%205.4%2012.8%205.4s9.2-1.8%2012.8-5.4L287%2095c3.5-3.5%205.4-7.8%205.4-12.8%200-5-1.9-9.2-5.5-12.8z%22%2F%3E%3C%2Fsvg%3E'), linear-gradient(to bottom, #ffffff 0%,#e5e5e5 100%);"; 
  page+="background-repeat: no-repeat, repeat; background-position: right .7em top 50%, 0 0; background-size: .65em auto, 100%;}";
  page+=".slider {-webkit-appearance: none; appearance: none; width: 98%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}";
  page+=".slider:hover { opacity: 1;}";
  page+=".slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}";
  page+=".slider::-moz-range-thumb {width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}";  
  page+=".icon{fill: #000000;}";
  page+=".checkbox-apple {position: relative; width: 50px; height: 25px; margin: 20px; webkit-user-select: none; -moz-user-select: none;-ms-user-select: none; user-select: none;}";
  page+=".checkbox-apple label {position: absolute; top: 0; left: 0; width: 50px; height: 25px; border-radius: 50px; background: linear-gradient(to bottom, #b3b3b3, #e6e6e6); cursor: pointer; transition: all 0.3s ease;}";
  page+=".checkbox-apple label:after {content: ''; position: absolute; top: 1px; left: 1px; width: 23px; height: 23px; border-radius: 50%; background-color: #fff; box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3); transition: all 0.3s ease;}";
  page+=".checkbox-apple input[type='checkbox']:checked + label {background: linear-gradient(to bottom, #005b98, #1e90ff);}";
  page+=".checkbox-apple input[type='checkbox']:checked + label:after {transform: translateX(25px);}";
  page+=".checkbox-apple label:hover {background: linear-gradient(to bottom, #b3b3b3, #e6e6e6);}";
  page+=".checkbox-apple label:hover:after {box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3);}";
  page+=".checkbox-apple input {opacity: 0;width: 0; height: 0;}";
  page+=".yep {position: absolute;top: 0;left: 0;width: 50px;height: 25px;}";
  page+="</style>";
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
   page+= "Имя: <input class='buttons' type='file' name='data' required>&nbsp;&nbsp;";
   page+= "Путь: <input type='text' class='inputs' name='path' value='/'>";
   page+= "<input type='hidden' name='mode' value='upload'>";
   page+= "<button class='buttons'>Загрузить</button>";
   page+= "</form>";
   page+= "</div>";
  
   //Содержимое файловой системы
   page+="<div class='contentblock'>";
   page+= "<h2 class='title'><center>Файловая система</center></h2>";
	File entry;
  while(entry = dir.openNextFile())
  {  
	  bool isDir = entry.isDirectory(); // single level for SPIFFS, possibly not true for LittleFS
   // page += (isDir) ? "dir:" : "file: ";
   // page += "\t";
		if(isDir) 
		{
			page += "<a href=/main?dir=/" ;
			page +=  entry.name(); 
			page +=  ">";
		}
    strcpy(fileName, entry.name());
    page += String(entry.name());
		if(isDir) 
		  page += "</a>";
		page += " (";
    page += String(entry.size());
		page += ")&nbsp;&nbsp;";
    // edit
    page += "<a href=/fileman?mode=edit&nameSave="; 
    if(fileName[0] != '\\' && fileName[0] != '/') // avoid double \ or / in filename (on some OS)
   	 page += path;
    page += String(entry.name());
    page += ">Изменить</a>&nbsp;&nbsp;";
    // delete
    page += "<a href=/delete?file="; 
    if(fileName[0] != '\\' && fileName[0] != '/') // avoid double \ or / in filename (on some OS)
	page += path;
    page += String(entry.name());
    page += ">Удалить</a><BR>";
    entry.close();
  }	
   // edit form - text, filename and submit
   page += "<div><form action='/fileman' method='get'><textarea class='textareas' name=editBlock id=editBlock rows='30' cols='60'>";
  
   if(bMode = "edit")
   {
      // read file and insert content
      file = FILESYS.open(bName.c_str(), "r");
      if(file)
      {        
        Serial.printf("File read avail %i, ",file.available());
        int readlen = (file.available() < FILEBUFSIZ) ? file.available() : FILEBUFSIZ;   
        file.read((uint8_t *)filebuf, readlen);
        file.close();        
        filebuf[readlen] = '\0';
        page += filebuf;
        //Serial.printf("read len %i, text [%s]\n",readlen, filebuf);
        logStr += (foundSaveBut) ? "Сохранение " : "Редактирование ";
        logStr += bName;
        logStr +="\n";   
      }
   }
   page += "</textarea></div>\n";
   page += "<input type=hidden name='mode' value='save'>";
   page += "Имя файла: <input class='inputs' type=text value='";
   if(bMode = "edit")
     page += bName;        
   page += "' name=nameSave id=nameSave> <input type=submit class='buttons' name=saveBut onclick=saveFile() value='Сохранить'></form></div>\n";
   //Лог
   page+="<div class='contentblock'>";
   page+= "<h2 class='title'><center>Лог действий</center></h2>";
   page += "Лог: <form><textarea class='textareas' name=log id=log rows='5' cols='85'>";
   page += logStr;
   page += "</textarea></form>";
   page += "<BR><a class='links' href='/fileman'>Перезагрузить страницу</a>";
   page += "</div>";
   page += "<a class='links' href='/'>На главную</a>";
   //Закрывающие теги
   page += "</div></body>";
   page += edit_script;
   page+="</html>";
  //server.send(200, "text/json", page);
	server.send(200, "text/html", page);
}

bool initFS(bool format = false, bool force = false) 
{
 bool fsFound = FILESYS.begin();
   if(!fsFound) 
     Serial.println(F("No file system found. Please format it."));
     
   if(!format)
     return fsFound;
     
  // format
	if(!fsFound || force) 
	{      
    Serial.println(F("Formatting FS."));
	  if(FILESYS.format()) 
    {
			  if(FILESYS.begin())
        {
          Serial.println(F("Format complete."));
          return true;
        }
    }      
    Serial.println(F("Format failed."));
    return false;              
	}
	//fsList();			
  return false; // shouldn't get here
}
