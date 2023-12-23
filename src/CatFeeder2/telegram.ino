/*
Все для работы Телеграм бота
*/
//Обработчик команд telegram бота
void newMsg(FB_msg& msg) 
{
  String param;//параметр
  String command;//команда
  uint8_t val,val2; //вспомогательные значения
  
  int8_t paramstart=msg.text.indexOf(" "); //ищем символ пробела
  if (paramstart>0) {command=msg.text.substring(0,paramstart); param=msg.text.substring(paramstart+1);} else {command=msg.text;} //если есть параметры записываем их в строку param
  if (command=="/feed") {//выдать корм
                          if (paramstart>0) //если был указан параметр
                              {
                              val=param.toInt();
                              if (val>0 && val<=60) prefid(val); else bot.sendMessage("Укажите значение в интервале от 1 до 60!"); //при корректном значении параметра выдаем установленное значение корма
                              } else prefid(feedAmountSet); //выдаем заранее установленное значение корма
                              
                        } 
  if (command=="/info") {tg_send_info();} //информация о системе
  if (command=="/mqtt") {usemqtt=!usemqtt; if (usemqtt) bot.sendMessage("MQTT включен"); else bot.sendMessage("MQTT отключен"); refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем mqtt
  if (command=="/lastfeed") {bot.sendMessage("Время последнего кормления: "+ String((uint16_t)lastFeed/60)+":"+String((uint8_t)lastFeed%60)+" Размер порции: "+String(feedAmount)+ " грамм");}
  if (command=="/temp") {bot.sendMessage("Температура: "+ String(temperature)+"°С Влажность: "+String(humidity)+"%");}
  if (command=="/commandlist") {tg_send_cmdhelp();}
  if (command=="/restart") {espRes=true;}//перезагрузка устройства
  //Установка значений таймеров
  if (paramstart>1) //если был указан параметр
    {
      int8_t paramtype=param.indexOf(":");
      if (paramtype>0)
        {
          val=param.substring(0,paramtype).toInt();
          val2=param.substring(paramtype+1).toInt();
          if (val>=0 && val2>=0 && val<24 && val2<60)
          {
            if (command=="/alarm0") {feedTime[0][0]=val; feedTime[0][1]=val2; bot.sendMessage("Будильник 0 установлен на: "+String(feedTime[0][0])+":"+String(feedTime[0][1])); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm1") {feedTime[1][0]=val; feedTime[1][1]=val2; bot.sendMessage("Будильник 1 установлен на: "+String(feedTime[1][0])+":"+String(feedTime[1][1])); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm2") {feedTime[2][0]=val; feedTime[2][1]=val2; bot.sendMessage("Будильник 2 установлен на: "+String(feedTime[2][0])+":"+String(feedTime[2][1])); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm3") {feedTime[3][0]=val; feedTime[3][1]=val2; bot.sendMessage("Будильник 3 установлен на: "+String(feedTime[3][0])+":"+String(feedTime[3][1])); refsaveconfigdelay.setInterval(10000);}
          } else {bot.sendMessage("Установите значение в пределах 0:00 - 23:59!");}  
        }
        else
        {
          val = param.toInt(); 
          if (val>1 && val<=60)
          {
          if (command=="/alarm0") {feedTime[0][3]=val; bot.sendMessage("Колличество корма будильника 0 установлено на: "+String(feedTime[0][3])+" грамм"); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm1") {feedTime[1][3]=val; bot.sendMessage("Колличество корма будильника 1 установлено на: "+String(feedTime[1][3])+" грамм"); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm2") {feedTime[2][3]=val; bot.sendMessage("Колличество корма будильника 2 установлено на: "+String(feedTime[2][3])+" грамм"); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm3") {feedTime[3][3]=val; bot.sendMessage("Колличество корма будильника 3 установлено на: "+String(feedTime[3][3])+" грамм"); refsaveconfigdelay.setInterval(10000);}
          } else {bot.sendMessage("Укажите значение в интервале от 1 до 60!");}
        }
    }
    else
    {  
      if (command=="/alarm0") {bool astate=feedTime[0][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 0 включен"); else bot.sendMessage("Таймер 0 отключен"); feedTime[0][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 0
      if (command=="/alarm1") {bool astate=feedTime[1][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 1 включен"); else bot.sendMessage("Таймер 1 отключен"); feedTime[1][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 1
      if (command=="/alarm2") {bool astate=feedTime[2][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 2 включен"); else bot.sendMessage("Таймер 2 отключен"); feedTime[2][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 2
      if (command=="/alarm3") {bool astate=feedTime[3][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 3 включен"); else bot.sendMessage("Таймер 3 отключен"); feedTime[3][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 3
    }
  
  
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
   //OTA оновление
   if (msg.OTA) {
    if (msg.fileName.indexOf("mklittlefs") > 0 ||
        msg.fileName.indexOf("spiffs") > 0) {
      bot.updateFS(); // обновить spiffs
    } else {
      bot.update();   // обновить прошивку
    }
  }
}

void tg_send_info()
  {
    String info="Размер порции: ";
    info+=String(feedAmountSet)+" грамм\n";
    info+="Время до следующего кормления: "+String(lv_label_get_text(ui_remain))+"\n";
    info+="Активные таймеры:\n";
    for (byte i = 0; i < 4; i++)    // проверяем массив с расписанием
        {
          if (feedTime[i][2]==1)
           {
            info+=String(feedTime[i][0])+":"+String(feedTime[i][1])+" Порция: "+String(feedTime[i][3])+" грамм\n";
           }
        }
    if (usemqtt) info+="MQTT активен\n"; else info+="MQTT не активен\n";
    bot.sendMessage(info); //отправляем сообщение о готовности     
  }

void tg_send_cmdhelp()
{
  const char* help=
"Команда без параметра включает или отключает функцию. С параметром - устанавливает значение или выполняет действие \n"
"Список команд бота:\n"
"/feed - выдать корм (параметр колличество 1-60 гр)\n"
"/info - вывод информации о работе устройства\n"
"/mqtt - включение и отключение mqtt\n"
"/lastfeed - вывод времени последнего кормления\n"
"/temp - вывод информации с DHT22\n"
"/alarmN - управление таймером (N - номер таймера от 0 до 3. Параметры hh:mm - время срабатывания или val -  колличество корма\n"
"/restart - перезагрузка устройства\n"
"/commandlist - справочник по командам)";
bot.sendMessage(help);
}
  