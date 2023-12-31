//отрисовка интерфейса кормления
void prefid(uint8_t feedA)
{
  feedAmount=feedA;
  //Объявляем графические ресурсы
  LV_IMG_DECLARE(feed_cat_img); //гифка с едой
  //Отрисовываем интерфейс окна кормления и запускаем таймер
  //Окно контейнер
    ui_feedwindow = lv_win_create(lv_scr_act(), 0);
    lv_obj_t * feed_windows_cont = lv_win_get_content(ui_feedwindow);  //контейнер для содержимого окна
    //голодный кот
    lv_obj_t * img_feed_cat = lv_img_create(feed_windows_cont);
    lv_img_set_src(img_feed_cat, &feed_cat_img);
    lv_obj_align(img_feed_cat, LV_ALIGN_CENTER,0,0);
    //Полоса прогресса кормления
    ui_feed_progress_bar = lv_bar_create(feed_windows_cont);
    lv_obj_set_size(ui_feed_progress_bar, lv_pct(100), 40);
    lv_obj_align(ui_feed_progress_bar, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_bar_set_value(ui_feed_progress_bar, 0, LV_ANIM_OFF);
    lv_bar_set_range(ui_feed_progress_bar, 0, feedAmount); //установка максимального значения шкалы прогресса
    //текст на полосе
    ui_feed_progress_bar_label = lv_label_create(ui_feed_progress_bar);
    lv_label_set_text_fmt(ui_feed_progress_bar_label,"%d", (feedAmount/100) * lv_bar_get_value(ui_feed_progress_bar));
    lv_obj_set_style_text_color(ui_feed_progress_bar_label,lv_palette_main(LV_PALETTE_CYAN),0);
    lv_obj_align_to(ui_feed_progress_bar_label, ui_feed_progress_bar, LV_ALIGN_CENTER, 0, 0);
    
    //Целевое значение корма
    ui_feed_label_max = lv_label_create(feed_windows_cont);//метка названия панели
    lv_obj_align(ui_feed_label_max,LV_ALIGN_TOP_RIGHT, 0, 20); //положение на экране
    lv_label_set_text (ui_feed_label_max,"Цель:");//Пишем текст метки

    //Выданное колличество корма
    ui_feed_label_cur = lv_label_create(feed_windows_cont);//метка названия панели
    lv_obj_align(ui_feed_label_cur,LV_ALIGN_TOP_LEFT, 0, 20); //положение на экране
    lv_label_set_text (ui_feed_label_cur,"Выдано:");//Пишем текст метки

    reffeedtime.setInterval(100); //запускаем таймер для подгрузки окна кормления
}


//Запуск кормления
void feed(uint16_t amount) 
{ 
  while (sensor.available()==false)
    {
      Serial.println("Waiting...");
      delay(10);
    }
  long  curWeight=sensor.read(); //вычисляем текущий вес
  stepper.setMaxSpeed(step_speed);
  stepper.enable();
  long maxWeight=curWeight+(amount*scales_param);//максимальный вес
  long minWeight=curWeight;
  long lastWeight=0; //последнее значение веса
  uint8_t i=0; //значение на прогресс баре
  uint8_t movements=0; //Отключаем движок через 10 оборотов
  uint8_t errorCount=0;//счетчик ошибок
  lv_label_set_text_fmt(ui_feed_label_max, "Цель: %d грамм", maxWeight/scales_param);
  while (curWeight<maxWeight)//основной цикл
    {
      i=map(curWeight,minWeight,maxWeight,0,100);
      lv_bar_set_value(ui_feed_progress_bar, i, LV_ANIM_OFF); //заполняем шкалы прогресса
      lv_label_set_text_fmt(ui_feed_progress_bar_label,"%d%%", i);
      lv_event_send(ui_feed_progress_bar, LV_EVENT_REFRESH, NULL);
      oneRev();
      movements+=1;
      lastWeight=curWeight;//запоминаем предыдущее значение веса
      while (sensor.available()==false)
      {
      Serial.println("Waiting...");
      delay(1);
      } 
      curWeight=sensor.read(); //вычисляем текущий вес
      //lv_label_set_text_fmt(ui_feed_label_cur, "Выдано: %d грамм", curWeight/scales_param);
      if (curWeight<=lastWeight) errorCount+=1; else errorCount=0;//если текущий вес не изменился увеличиваем счетчик ошибок
      Serial.printf("last=%d, cur=%d, error=%d /n",lastWeight,curWeight,errorCount);
      if (errorCount>3 || movements>max_revs) //счетчик ошибок превысил пороговое значение
        {
          if (errorCount>3)
          { 
            logStr+="Корм не подается! Проверьте наличие корма в бункере и вращение шнека!\n";
            if (tg_bot) bot.sendMessage("Корм не подается! Проверьте наличие корма в бункере и вращение шнека!");
          }
          else
            {
            logStr+="Достигнуто максимальное число оборотов шнека\n";
            if (tg_bot) bot.sendMessage("Достигуто максимальное число оборотов шнека! Возможна проблема с весовой платформой или застреванием корма");
            }  
          lastFeed=ntp.hour()*60 + ntp.minute();
          lv_obj_del(ui_feedwindow);
          stepper.disable();//выключаем двигатель
          return; //выход из цикла
        }
    }
  
  lastFeed=ntp.hour()*60 + ntp.minute();
  lv_obj_del(ui_feedwindow);
  stepper.disable();//выключаем двигатель
}

//крутим мотор
void oneRev() {
    stepper.setTarget(-1*bck_steps,RELATIVE);//крутим назад
  while (stepper.tick())
    {
      //ждем пока мотор придет к заданной позиции
    }
    lv_timer_handler();//обновление содержимого экрана
    stepper.setTarget(fwd_steps,RELATIVE);//крутим вперед
  while (stepper.tick())
    {
      //ждем пока мотор придет к заданной позиции
    }
    lv_timer_handler();//обновление содержимого экрана
  
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
        lv_label_set_text_fmt(ui_remain, LV_SYMBOL_SANDCLOCK" %dч. %dм.",(int)mintime/60, mintime%60); 
}