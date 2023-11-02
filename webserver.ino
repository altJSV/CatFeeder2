//Инициализация веб интерфейса
void server_init()
{
   //обработчики http запросов
   server.on("/",handle_main);
   server.on("/mqttsetting",handle_mqtt_setting);
   //запускаем сервер
   server.begin();
}

//Главная страница веб интерфейса
void handle_main()
{
  //Заголовки html
  String page = "<!DOCTYPE HTML><html>";
  page+="<head>";
  page+="<title>Панель управления RGB кольцом</title>";
  page+="<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page+="<meta charset='UTF-8'>";
  
  //стили
  page+="<style>";
  page+=".icon-block {position: fixed; top: 1%; right: 2%;}";
  page+=".headerblock {display: flex; width: 80%; color: white; margin: 10px auto 10px auto; flex-direction: column; align-items: center; justify-content: center; background: 	Dodgerblue;}";
  page+=".main {width: 100%; color: black; background: 	Skyblue; font-size: 16px; font-family: sans-serif; font-weight: 700;"; 
  page+=".select-css {width: 100%; display: block; font-size: 16px; font-family: sans-serif; font-weight: 700; color: #444; line-height: 1.3; padding: .6em 1.4em .5em .8em; width: 100%; max-width: 100%;";
  page+="box-sizing: border-box; margin: 0; border: 1px solid #aaa; box-shadow: 0 1px 0 1px rgba(0,0,0,.04); border-radius: .5em;";
  page+="-moz-appearance: none; -webkit-appearance: none; appearance: none; background-color: #fff;"; 
  page+="background-image: url('data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22292.4%22%20height%3D%22292.4%22%3E%3Cpath%20fill%3D%22%23007CB2%22%20d%3D%22M287%2069.4a17.6%2017.6%200%200%200-13-5.4H18.4c-5%200-9.3%201.8-12.9%205.4A17.6%2017.6%200%200%200%200%2082.2c0%205%201.8%209.3%205.4%2012.9l128%20127.9c3.6%203.6%207.8%205.4%2012.8%205.4s9.2-1.8%2012.8-5.4L287%2095c3.5-3.5%205.4-7.8%205.4-12.8%200-5-1.9-9.2-5.5-12.8z%22%2F%3E%3C%2Fsvg%3E'), linear-gradient(to bottom, #ffffff 0%,#e5e5e5 100%);"; 
  page+="background-repeat: no-repeat, repeat; background-position: right .7em top 50%, 0 0; background-size: .65em auto, 100%;}";
  page+=".slider {-webkit-appearance: none; appearance: none; width: 100%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}";
  page+=".slider:hover { opacity: 1;}";
  page+=".slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; background: Dodgerblue; cursor: pointer;}";
  page+=".slider::-moz-range-thumb {width: 25px; height: 25px; background: Dodgerblue; cursor: pointer;}";  
  page+=".icon{fill: #000000;}";
  page+="</style>";

  //скрипты

  //тело страницы
  page+="<body>";
  page+="<h1 align='center'>Cat Feeder 2</h1>";

  //Блоки данных
  //MQTT
  page+="<div class='headerblock'>";
  page+="<h2>Параметры подключения к MQTT брокеру</h2>";
  page+="<div class='main'>";
  page+="<p><form method='get' action='/mqttsetting'><label>Сервер: </label><input style='padding:10px;border-radius:10px; 15px' type='text' name='server'  length=32 value='"+mqtt_server+"'>&nbsp;";
  page+="<label>Порт: </label><input style='padding:10px;border-radius:10px;' name='port' type='number' length=64 value='"+String(mqtt_port)+"'>&nbsp;&nbsp;";
  page+="<label>Логин: </label><input style='padding:10px;border-radius:10px; 15px' type='text' name='login'  length=32 value='"+mqtt_login+"'>&nbsp;<label>Пароль: </label><input style='padding:10px;border-radius:10px;' name='pass' type='password' length=64 value='"+mqtt_pass+"'>&nbsp;&nbsp;";
  page+="<input style='padding:10px;border-radius:10px; background: 	Dodgerblue; color: white'  type='submit'></form></p>";
  page+="</div></div>";

  //Завершающий код
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
  /*String page;
  int statusCode;
  if (eeprom_write(qsid,qpass))
    {
      page="{'Успешно':'сохранено в EEPROM. Перезагружаемся для подключения к точке доступа'}";
      statusCode = 200;
      ESP.reset();
    }
    else
    {
    page = "{'Ошибка':'404 не найдено'}";
        statusCode = 404;
        Serial.println("Sending 404");
        server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", page);*/
  server.sendHeader("Location", "/",true);   //редирект на главную
  server.send(302, "text/plane","");

}
