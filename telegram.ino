//Обработчик команд telegram бота
void newMsg(FB_msg& msg) 
{
  String command=msg.text;
  if (command=="/feed") {prefid(feedAmountSet);} //информация о системе
  if (command=="/info") {tg_send_info();} //информация о системе
  if (command=="/mqtt") {usemqtt=!usemqtt; if (usemqtt) bot.sendMessage("MQTT включен"); else bot.sendMessage("MQTT отключен"); refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем mqtt
  if (command=="/alarm0") {bool astate=feedTime[0][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 0 включен"); else bot.sendMessage("Таймер 0 отключен"); feedTime[0][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 0
  if (command=="/alarm1") {bool astate=feedTime[1][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 1 включен"); else bot.sendMessage("Таймер 1 отключен"); feedTime[1][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 1
  if (command=="/alarm2") {bool astate=feedTime[2][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 2 включен"); else bot.sendMessage("Таймер 2 отключен"); feedTime[2][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 2
  if (command=="/alarm3") {bool astate=feedTime[3][2]; astate=!astate; if (astate) bot.sendMessage("Таймер 3 включен"); else bot.sendMessage("Таймер 3 отключен"); feedTime[3][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 3
  if (command=="/lastfeed") {bot.sendMessage("Время последнего кормления: "+ String((uint16_t)lastFeed/60)+":"+String((uint8_t)lastFeed%60)+" Размер порции: "+String(feedAmount)+ " грамм");}
  if (command=="/temp") {bot.sendMessage("Температура: "+ String(temperature)+"°С Влажность: "+String(humidity)+"%");}
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
    info+="В миске: "+String((foodWeight-tareWeight)/1000)+ " грамм корма\n";
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
  