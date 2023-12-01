//Инициализация веб интерфейса
void server_init()
{
   //обработчики http запросов
   server.on("/",handle_main);
   server.on("/mqttsetting",handle_mqtt_setting);
   server.on("/tgsetting",handle_tg_setting);
   server.on("//stepsetting",handle_step_setting);
   
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
  page+=".mainblock {display: flex; width: 60%; color: white; margin: 10px auto 10px auto; flex-direction: column; align-items: center; justify-content: center; background: Lightcyan;}";
  page+=".contentblock {width: 90%; margin: 10px auto; background-color: Skyblue; color:black; border-radius:10px;}";
  page+=".title { background-color: Dodgerblue; color: white;margin: 0; padding: 10px; box-sizing: border-box; border-radius: 10px 10px 0 0}";
  page+=".infotitle { background-color: Forestgreen; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".warningtitle { background-color: Crimson; color: white; margin: 0; padding: 10px; box-sizing: border-box;}";
  page+=".content { background-color: Lightcyan; color: black; margin: 0; padding: 10px; border-radius:10px;}";
  page+=".main {display:block; margin:auto; color: black; background: 	Skyblue; padding: 10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}"; 
  page+=".inputs {padding:10px; border-radius:10px; margin: 10px auto;}";
  page+=".buttons {padding:10px;border-radius:10px; background: Dodgerblue; color: white; margin: 10px auto;}";
  page+=".links {padding:10px;border-radius:10px; background: Sandybrown; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 88%; }";
  page+=".select-css {width: 100%; display: block; font-size: 16px; font-family: sans-serif; font-weight: 700; color: #444; line-height: 1.3; padding: .6em 1.4em .5em .8em; width: 100%; max-width: 100%;";
  page+="box-sizing: border-box; margin: 0; border: 1px solid #aaa; box-shadow: 0 1px 0 1px rgba(0,0,0,.04); border-radius: .5em;";
  page+="-moz-appearance: none; -webkit-appearance: none; appearance: none; background-color: #fff;"; 
  page+="background-image: url('data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22292.4%22%20height%3D%22292.4%22%3E%3Cpath%20fill%3D%22%23007CB2%22%20d%3D%22M287%2069.4a17.6%2017.6%200%200%200-13-5.4H18.4c-5%200-9.3%201.8-12.9%205.4A17.6%2017.6%200%200%200%200%2082.2c0%205%201.8%209.3%205.4%2012.9l128%20127.9c3.6%203.6%207.8%205.4%2012.8%205.4s9.2-1.8%2012.8-5.4L287%2095c3.5-3.5%205.4-7.8%205.4-12.8%200-5-1.9-9.2-5.5-12.8z%22%2F%3E%3C%2Fsvg%3E'), linear-gradient(to bottom, #ffffff 0%,#e5e5e5 100%);"; 
  page+="background-repeat: no-repeat, repeat; background-position: right .7em top 50%, 0 0; background-size: .65em auto, 100%;}";
  page+=".slider {-webkit-appearance: none; appearance: none; width: 90%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}";
  page+=".slider:hover { opacity: 1;}";
  page+=".slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; background: Dodgerblue; cursor: pointer;}";
  page+=".slider::-moz-range-thumb {width: 25px; height: 25px; background: Dodgerblue; cursor: pointer;}";  
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
  //Изменение яркости дисплея
  page+="<script type='text/javascript'>";


  //конец блока
  page+="</script>";
  page+="</head>";
  
  //тело страницы
  page+="<body>";
  page+="<h1 align='center'>Cat Feeder 2</h1>";

  //Блоки данных
  page+="<div class='mainblock'>"; //основной контейнер
  
  //Параметры шагового двигателя
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры шагового двигателя</center></h2>";
  page+="<form method='get' action='/stepsetting'>";
  page+="&nbsp;&nbsp;Шагов вперед:<br><input name='fwd_steps' type='range' class='slider' min='0' max='200' step='1' width='100%' align='center' value='"+String(fwd_steps)+"'><br>";
  page+="&nbsp;&nbsp;Шагов назад<br><input name='bck_steps' type='range' class='slider' min='0' max='200' step='1' width='100%' align='center' value='"+String(bck_steps)+"'><br>";
  page+="&nbsp;&nbsp;Скорость вращения<br><input name='step_speed' type='range' class='slider' min='0' max='400' step='1' width='100%' align='center' value='"+String(step_speed)+"'><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";
  //MQTT
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к MQTT брокеру</center></h2>";
  page+="<form method='get' action='/mqttsetting'>&nbsp;&nbsp;<label>Сервер: <input class='inputs' type='text' name='server'  length=32 value='"+mqtt_server+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Порт: <input class='inputs' name='port' type='number' length=6 value='"+String(mqtt_port)+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>Логин: <input class='inputs' type='text' name='login'  length=32 value='"+mqtt_login+"'>&nbsp;</label><br>&nbsp;&nbsp;<label>Пароль: <input class='inputs' name='pass' type='password' length=64 value='"+mqtt_pass+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";
  //telegram
  page+="<div class='contentblock'>";
  page+="<h2 class='title'><center>Параметры подключения к Telegram боту</center></h2>";
  page+="<form method='get' action='/tgsetting'>";
  page+="<br>&nbsp;&nbsp;Использовать бота: <span class='checkbox-apple'><input class='yep' type='checkbox' id='tgbot' name='tg_bot' value='"+String(tg_bot)+"'><label for='tgbot'></label></span><br><br>";
  page+="&nbsp;&nbsp;<label>Токен: <input class='inputs' type='text' name='token'  length=40 value='"+bot_token+"'>&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<label>ID чата: <input class='inputs' name='chatid' type='number' length=10 value='"+chatID+"'>&nbsp;&nbsp;</label><br>";
  page+="&nbsp;&nbsp;<input class='buttons'  type='submit'></form>";
  page+="</div>";

  page+="<a href='#' class='links'>Статистика</a>";
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
  String page;
  int statusCode;
  if (saveConfiguration("/config.json"))
    {
      page="{'Успешно':'Cохранено в память устройства.'}";
      statusCode = 200;
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
      page="{'Успешно':'Cохранено в память устройства.'}";
      statusCode = 200;
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
      page="{'Успешно':'Cохранено в память устройства.'}";
      statusCode = 200;
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
