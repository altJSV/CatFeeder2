//Инициализация веб интерфейса
void server_init()
{
   //обработчики http запросов
   server.on("/",handle_main);//главный экран
   server.on("/mqttsetting",handle_mqtt_setting);//сохранение настроек mqtt
   server.on("/tgsetting",handle_tg_setting);//сохранение настроек телеграм бота
   server.on("/stepsetting",handle_step_setting);//сохранение настроек шагового двигателя
   server.on("/alarmsetting",handle_alarm_setting);//сохранение настроек будильников
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
  page+="<br>&nbsp;&nbsp;Использовать MQTT: <span class='checkbox-apple'><input class='yep' type='checkbox' id='usemqtt' name='usemqtt' ";
  if (usemqtt) page+="checked";
  page+="><label for='usemqtt'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Сервер: <input class='inputs' type='text' name='server'  length=32 value='"+mqtt_server+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Порт: <input class='inputs' name='port' type='number' length=6 value='"+String(mqtt_port)+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Логин: <input class='inputs' type='text' name='login'  length=32 value='"+mqtt_login+"'>&nbsp;</label><br>&nbsp;&nbsp;<label>Пароль: <input class='inputs' name='pass' type='password' length=64 value='"+mqtt_pass+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
  page+="</div>";
  //telegram
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к Telegram боту</center></h2>";
  page+="<form method='get' action='/tgsetting'>";
  page+="<br>&nbsp;&nbsp;Использовать бота: <span class='checkbox-apple'><input class='yep' type='checkbox' id='tg_bot' name='tg_bot' ";
  if (tg_bot) page+="checked";
  page+="><label for='tg_bot'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Токен: <input class='inputs' type='text' name='token'  length=40 value='"+bot_token+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>ID чата: <input class='inputs' name='chatid' type='number' length=10 value='"+chatID+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='bigbuttons'  type='submit'></form>";
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
  if (server.arg("usemqtt")=="on")  usemqtt=1; else usemqtt=0;
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
  if (server.arg("tg_bot")=="on")  tg_bot=1; else tg_bot=0;
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

void handle_alarm_setting() { 
  String Time="";              
  for (byte i=0; i<4; i++){
    Time=server.arg("alarm"+String (i));
     feedTime[i][0]=Time.substring(0,2).toInt();
     feedTime[i][1]=Time.substring(3,5).toInt();
     if (server.arg("alarmcheck"+String (i))=="on") feedTime[i][2]=1; else feedTime[i][2]=0;
     feedTime[i][3]=server.arg("asize"+String (i)).toInt(); 
  }
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
