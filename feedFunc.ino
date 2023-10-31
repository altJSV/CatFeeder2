//Запуск кормления
void feed() 
{ 
  for (int i = 0; i < feedAmount; i++) 
  {
    lv_bar_set_value(ui_feed_progress_bar, i, LV_ANIM_OFF); //заполняем шкалы прогресса
    lv_event_send(ui_feed_progress_bar, LV_EVENT_REFRESH, NULL);
    lv_timer_handler();
    Serial.println(i);
    delay(50);
  }  
  //disableMotor();//выключаем мотор
  lv_obj_del(ui_feedwindow);
  
}

// выключаем ток на мотор
void disableMotor() {
  for (byte i = 0; i < 4; i++) digitalWrite(drvPins[i], 0);
}

//крутим мотор
void oneRev() {
  for (int i = 0; i < STEPS_BKW; i++) runMotor(-1);
  for (int i = 0; i < STEPS_FRW; i++) runMotor(1);
}

const byte steps[] = {0b1010, 0b0110, 0b0101, 0b1001};
void runMotor(int8_t dir) {
  static byte step = 0;
  for (byte i = 0; i < 4; i++) digitalWrite(drvPins[i], bitRead(steps[step & 0b11], i));
  delayMicroseconds(FEED_SPEED);
  step += dir;
}

//Функция подсчета времени до ближайшего кормления
void feedRemain()
{
uint16_t curtime=ntp.hour()*60 + ntp.minute(); //Текущее время в минутах
      uint16_t mintime=1450; //Мнимальньное время между кормлениями в минутах
      uint16_t remtime=0; //Оставшееся время до кормления
      //расчет времени до кормления
      for (byte i = 0; i < 4; i++)    // проверяем массив с расписанием
        {
           if (feedTime[i][2]==1)
           {
            uint8_t hr=feedTime[i][0];
            uint8_t mn=feedTime[i][1];
            uint16_t feedtimesum=(hr * 60) + mn;
            if ( feedtimesum >= curtime) 
            {
              remtime=feedtimesum-curtime;
            }
            else 
              {
                remtime=1440-curtime + feedtimesum;
              }
             if (remtime<mintime) mintime=remtime;
           }
            
        } 
        lv_label_set_text_fmt(ui_remain, "Время до кормления: %d:%d",(int)mintime/60, mintime%60); 
}