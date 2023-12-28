/*
Здесь расположены функции для работы по протоколу MQTT
*/
//Инициализация MQTT
void MQTT_init(){  
  static uint32_t tmr = 0;
  client.setServer(mqtt_server.c_str(), mqtt_port);
  client.setCallback(MQTTcallback);
  byte tries=10;
  while (--tries && !client.connected()) 
  {   
     Serial.println("Подключение к MQTT брокеру...");
     logStr+="Подключение к MQTT брокеру... ";
     if (client.connect(clientID.c_str(), mqtt_login.c_str(),mqtt_pass.c_str(),statusTopic.c_str(),1,true,"Клиент отключен"))
       {
      Serial.println("Подключено");
      logStr+="Подключено\n";
      client.publish(statusTopic.c_str(),"Готов к работе");
      client.subscribe(cmdTopic.c_str());
      }
    else
    {
      Serial.print("");
      logStr+="Ошибка подключения: "+String(client.state())+"\n";
      Serial.println(client.state());
      }
  }
  
   delay(2000);
   }

//Парсинг сообшений в MQTT топике по подписке
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Получено сообщение в топике: ");
  Serial.println(topic);
  Serial.print("Сообщение:");
  String message;//сообщение
  String param;//параметр
  String command;//команда
  String out; //вывод в топик
  uint8_t val,val2; //вспомогательные значения
  
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  int8_t paramstart=message.indexOf(" "); //ищем символ пробела
  if (paramstart>0) {command=message.substring(0,paramstart); param=message.substring(paramstart+1);} else {command=message;} //если есть параметры записываем их в строку param
  if (command=="/feed") {//выдать корм
                          if (paramstart>0) //если был указан параметр
                              {
                              val=param.toInt();
                              if (val>0 && val<=60) prefid(val); else client.publish(statusTopic.c_str(),"Укажите значение в интервале от 1 до 60!"); //при корректном значении параметра выдаем установленное значение корма
                              } else prefid(feedAmountSet); //выдаем заранее установленное значение корма
                              
                        }
  if (command=="/brightlevel") {//установить яркость подсветки
                          if (paramstart>0) //если был указан параметр
                              {
                              val=param.toInt();
                              if (val>0 && val<=255) {bright_level=val; lv_slider_set_value(ui_set_panel_display_bright_slider, bright_level, LV_ANIM_OFF); lv_label_set_text_fmt(ui_backlight_slider_label, "%d%", (int)lv_slider_get_value(ui_set_panel_display_bright_slider));} 
                              else {client.publish(statusTopic.c_str(),"Укажите значение в интервале от 1 до 255!");} //при корректном значении устанавливаем яркость подсветки
                              } else client.publish(statusTopic.c_str(),"Укажите в качестве параметра значение от 1 до 255!");; //выдаем предупреждение в чат
                        }
  if (command=="/daytime") {//отключение подсветки экрана в ночное время
                          if (paramstart>0) //если был указан параметр
                              {
                              int8_t paramtype=param.indexOf(":");
                              if (paramtype>0)
                                {
                                  val=param.substring(0,paramtype).toInt();
                                  val2=param.substring(paramtype+1).toInt();
                                  if (val>=0 && val2>=0 && val<24 && val2<24 && val<val2)
                                    {
                                      daybegin=val;//начало дня
                                      dayend=val2;//конец дня
                                      lv_slider_set_left_value(ui_slider_day_time, daybegin, LV_ANIM_OFF);
                                      lv_slider_set_value(ui_slider_day_time, dayend, LV_ANIM_OFF);
                                      out="Интервал дневного время установлен: с "+String(daybegin)+" до "+String(dayend)+" часов"; client.publish(statusTopic.c_str(),out.c_str());refsaveconfigdelay.setInterval(10000);
                                    } else {client.publish(statusTopic.c_str(),"Неверные параметры команды");} 
                              } else {
                                daytime=!daytime;
                                if (daytime) {client.publish(statusTopic.c_str(),"Отключение подсветки экрана ночью включено"); lv_obj_add_state(ui_set_display_daytime_switch, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Отключение подсветки экрана ночью выключено");lv_obj_clear_state(ui_set_display_daytime_switch, LV_STATE_CHECKED);} refsaveconfigdelay.setInterval(10000);} 
                              }
                        }                         
  if (command=="/tgbot") {tg_bot=!tg_bot; if (tg_bot) {client.publish(statusTopic.c_str(),"Бот включен"); lv_obj_add_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Бот отключен");  lv_obj_clear_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED);} refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем бота
  if (command=="/lastfeed") {out="Время последнего кормления: "+ String((uint16_t)lastFeed/60)+":"+String((uint8_t)lastFeed%60)+" Размер порции: "+String(feedAmount)+ " грамм";client.publish(statusTopic.c_str(),out.c_str());}
  if (command=="/temp") {out="Температура: "+ String(temperature)+"°С Влажность: "+String(humidity)+"%"; client.publish(statusTopic.c_str(),out.c_str());}
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
            if (command=="/alarm0") {feedTime[0][0]=val; feedTime[0][1]=val2; out="Будильник 0 установлен на: "+String(feedTime[0][0])+":"+String(feedTime[0][1]); client.publish(statusTopic.c_str(),out.c_str()); lv_roller_set_selected(ui_timer1_hour, feedTime[0][0], LV_ANIM_OFF); lv_roller_set_selected(ui_timer1_minute, feedTime[0][1], LV_ANIM_OFF); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm1") {feedTime[1][0]=val; feedTime[1][1]=val2; out="Будильник 1 установлен на: "+String(feedTime[1][0])+":"+String(feedTime[1][1]); client.publish(statusTopic.c_str(),out.c_str()); lv_roller_set_selected(ui_timer2_hour, feedTime[1][0], LV_ANIM_OFF); lv_roller_set_selected(ui_timer2_minute, feedTime[1][1], LV_ANIM_OFF); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm2") {feedTime[2][0]=val; feedTime[2][1]=val2; out="Будильник 2 установлен на: "+String(feedTime[2][0])+":"+String(feedTime[2][1]); client.publish(statusTopic.c_str(),out.c_str()); lv_roller_set_selected(ui_timer3_hour, feedTime[2][0], LV_ANIM_OFF); lv_roller_set_selected(ui_timer3_minute, feedTime[2][1], LV_ANIM_OFF); refsaveconfigdelay.setInterval(10000);}
            if (command=="/alarm3") {feedTime[3][0]=val; feedTime[3][1]=val2; out="Будильник 3 установлен на: "+String(feedTime[3][0])+":"+String(feedTime[3][1]); client.publish(statusTopic.c_str(),out.c_str()); lv_roller_set_selected(ui_timer4_hour, feedTime[3][0], LV_ANIM_OFF); lv_roller_set_selected(ui_timer4_minute, feedTime[3][1], LV_ANIM_OFF); refsaveconfigdelay.setInterval(10000);}
          } else {client.publish(statusTopic.c_str(),"Установите значение в пределах 0:00 - 23:59!");}  
        }
        else
        {
          val = param.toInt(); 
          if (val>1 && val<=60)
          {
          if (command=="/alarm0") {feedTime[0][3]=val; out="Колличество корма будильника 0 установлено на: "+String(feedTime[0][3])+" грамм"; client.publish(statusTopic.c_str(),out.c_str()); lv_spinbox_set_value(ui_timer1_amount,feedTime[0][3]); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm1") {feedTime[1][3]=val; out="Колличество корма будильника 1 установлено на: "+String(feedTime[1][3])+" грамм"; client.publish(statusTopic.c_str(),out.c_str()); lv_spinbox_set_value(ui_timer2_amount,feedTime[1][3]); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm2") {feedTime[2][3]=val; out="Колличество корма будильника 2 установлено на: "+String(feedTime[2][3])+" грамм"; client.publish(statusTopic.c_str(),out.c_str()); lv_spinbox_set_value(ui_timer3_amount,feedTime[2][3]); refsaveconfigdelay.setInterval(10000);}
          if (command=="/alarm3") {feedTime[3][3]=val; out="Колличество корма будильника 3 установлено на: "+String(feedTime[3][3])+" грамм"; client.publish(statusTopic.c_str(),out.c_str()); lv_spinbox_set_value(ui_timer4_amount,feedTime[3][3]); refsaveconfigdelay.setInterval(10000);}
          } else {client.publish(statusTopic.c_str(),"Укажите значение в интервале от 1 до 60!");}
        }
    }
    else
    {  
      if (command=="/alarm0") {bool astate=feedTime[0][2]; astate=!astate; if (astate) {client.publish(statusTopic.c_str(),"Таймер 0 включен"); lv_obj_add_state(ui_timer1_check, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Таймер 0 отключен"); lv_obj_clear_state(ui_timer1_check, LV_STATE_CHECKED);} feedTime[0][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 0
      if (command=="/alarm1") {bool astate=feedTime[1][2]; astate=!astate; if (astate) {client.publish(statusTopic.c_str(),"Таймер 1 включен"); lv_obj_add_state(ui_timer2_check, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Таймер 1 отключен"); lv_obj_clear_state(ui_timer2_check, LV_STATE_CHECKED);} feedTime[1][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 1
      if (command=="/alarm2") {bool astate=feedTime[2][2]; astate=!astate; if (astate) {client.publish(statusTopic.c_str(),"Таймер 2 включен"); lv_obj_add_state(ui_timer3_check, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Таймер 2 отключен"); lv_obj_clear_state(ui_timer3_check, LV_STATE_CHECKED);} feedTime[2][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 2
      if (command=="/alarm3") {bool astate=feedTime[3][2]; astate=!astate; if (astate) {client.publish(statusTopic.c_str(),"Таймер 3 включен"); lv_obj_add_state(ui_timer4_check, LV_STATE_CHECKED);} else {client.publish(statusTopic.c_str(),"Таймер 3 отключен"); lv_obj_clear_state(ui_timer4_check, LV_STATE_CHECKED);} feedTime[3][2]=astate; refsaveconfigdelay.setInterval(10000);} //Включаем и отключаем таймер 3
    }
 
}                   