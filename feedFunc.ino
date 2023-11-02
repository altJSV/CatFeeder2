//отрисовка интерфейса кормления
void prefid()
{
  //Объявляем графические ресурсы
  LV_IMG_DECLARE(feed_cat_img); //гифка с едой
  //Отрисовываем интерфейс окна кормления и запускаем таймер
  //Окно контейнер
    ui_feedwindow = lv_win_create(lv_scr_act(), 0);
    lv_obj_t * feed_windows_cont = lv_win_get_content(ui_feedwindow);  //контейнер для содержимого окна
    //голодный кот
    lv_obj_t * img_feed_cat_gif = lv_gif_create(feed_windows_cont);
    lv_gif_set_src(img_feed_cat_gif, &feed_cat_img);
    lv_obj_align(img_feed_cat_gif, LV_ALIGN_CENTER,0,0);
    //Полоса прогресса кормления
    ui_feed_progress_bar = lv_bar_create(feed_windows_cont);
    lv_obj_set_size(ui_feed_progress_bar, lv_pct(100), 40);
    lv_obj_align(ui_feed_progress_bar, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_bar_set_value(ui_feed_progress_bar, 0, LV_ANIM_OFF);
    lv_bar_set_range(ui_feed_progress_bar, 0, feedAmount); //установка максимального значения шкалы прогресса
    //текст на полосе
    ui_feed_progress_bar_label = lv_label_create(ui_feed_progress_bar);
    lv_label_set_text_fmt(ui_feed_progress_bar_label,"%d", (feedAmount/100) * lv_bar_get_value(ui_feed_progress_bar));
    lv_obj_set_style_text_color(ui_feed_progress_bar_label,lv_palette_lighten(LV_PALETTE_GREY),0);
    lv_obj_align_to(ui_feed_progress_bar_label, ui_feed_progress_bar, LV_ALIGN_CENTER, 0, 0);
    reffeedtime.setInterval(100); //запускаем таймер для подгрузки окна кормления
    
}


//Запуск кормления
void feed() 
{ 
  for (int i = 0; i < feedAmount; i++) 
  {
    lv_bar_set_value(ui_feed_progress_bar, i, LV_ANIM_OFF); //заполняем шкалы прогресса
    lv_label_set_text_fmt(ui_feed_progress_bar_label,"%d", i*100/feedAmount);
    lv_event_send(ui_feed_progress_bar, LV_EVENT_REFRESH, NULL);
    lv_timer_handler();
    Serial.println(i);
    delay(20);
  }  
  //disableMotor();//выключаем мотор
  lastFeed=ntp.hour()*60 + ntp.minute();
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
      uint16_t alarmtime=1440; //время наиболе ближнего будильника
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
             if (remtime<mintime) 
             {
              mintime=remtime; 
              alarmtime=feedtimesum;
              }
           }
            
        } 
        lv_label_set_text_fmt(ui_remain, "Время до кормления: %dч. %dм.",(int)mintime/60, mintime%60); 
    //Движение котика по шкале
    if (lastFeed>alarmtime) {alarmtime=lastFeed+(1440-lastFeed)+alarmtime;}
    
    uint8_t x_cat=map(alarmtime-mintime,lastFeed,alarmtime,0,160);//вычисляем координату х котика на шкале
    Serial.println(x_cat);
    Serial.print("mintime: ");
    Serial.println(mintime);
    Serial.print("lastfeed: ");
    Serial.println(lastFeed);
    Serial.print("alarmtime: ");
    Serial.println(alarmtime);
    lv_obj_set_x(img_running_cat,x_cat);
}