//Обработчик команд telegram бота
void newMsg(FB_msg& msg) 
{
  if (msg.text=="/feed") {prefid(feedAmountSet);} //информация о системе
  if (msg.text=="/info") {tg_send_info();} //информация о системе
  
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
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