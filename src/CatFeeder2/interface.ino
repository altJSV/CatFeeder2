/* В данном файле находятся функции работы с LVG.
Отрисовка интерфейса, обработчики событий и т.п.
*/
//Обработка изменения значения установки времени работы подсветки
  static void slider_daytime_event(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    daybegin = (int)lv_slider_get_left_value(slider);
    dayend = (int)lv_slider_get_value(slider);
    lv_label_set_text_fmt(slider_daytime_label, "%d - %d", daybegin, dayend);
    lv_obj_align_to(slider_daytime_label, slider, LV_ALIGN_CENTER, 0, 0);
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }  

//Переключение ночного режима подсветки
  static void daytime_switch_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          daytime=true;
        }
        else
        {
          daytime=false;
        }
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  } 

//Обработка изменения значения слайдера яркости экрана
  static void slider_brightness_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_backlight_slider_label, "%d%", (int)lv_slider_get_value(slider));
    lv_obj_align_to(ui_backlight_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    bright_level=(int)lv_slider_get_value(slider);
    analogWrite(TFT_BACKLIGHT,bright_level);
    //ledcWrite(0, bright_level);
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

//Универсальная функция обработки ввода текста в text_area
  static void ta_event_cb(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * tv_but = lv_tabview_get_tab_btns(ui_tabview);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_y(ta, 10, LV_ANIM_OFF);
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);

    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*сброс состояния текстового поля*/
    }
  }

 //Изменение значения токена в текстовом поле
 static void token_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  bot_token=lv_textarea_get_text(ta);
  bot.setToken(bot_token); // устанавливаем новый токен бота
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

   //Изменение значения chatid в текстовом поле
 static void chatid_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  chatID=lv_textarea_get_text(ta);
  bot.setChatID(chatID); // устанавливаем новый ID чата
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }


//Изменение значения адреса mqtt в текстовом поле
 static void mqttadress_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  mqtt_server=lv_textarea_get_text(ta);
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

   //Изменение значения mqtt порта в текстовом поле
 static void mqttport_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  String portnum=lv_textarea_get_text(ta);
  mqtt_port=portnum.toInt();
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

  //Изменение значения командного топика mqtt в текстовом поле
 static void mqttcommand_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  cmdTopic=lv_textarea_get_text(ta);
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

  //Изменение значения статусного топика mqtt в текстовом поле
 static void mqttstatus_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  statusTopic=lv_textarea_get_text(ta);
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

  //Изменение значения логина mqtt в текстовом поле
 static void mqttlogin_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  mqtt_login=lv_textarea_get_text(ta);
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

   //Изменение значения логина mqtt в текстовом поле
 static void mqttpass_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * ta = lv_event_get_target(e);
  mqtt_pass=(int)lv_textarea_get_text(ta);
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

//Изменения размеров порции в будильниках
static void timer1_amount_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ui_timer1_amount);
        feedTime[0][3]=lv_spinbox_get_value(ui_timer1_amount);
        refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer1_amount_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ui_timer1_amount);
         feedTime[0][3]=lv_spinbox_get_value(ui_timer1_amount);
         refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer2_amount_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ui_timer2_amount);
        feedTime[1][3]=lv_spinbox_get_value(ui_timer2_amount);
        refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer2_amount_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ui_timer2_amount);
         feedTime[1][3]=lv_spinbox_get_value(ui_timer2_amount);
         refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer3_amount_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ui_timer3_amount);
        feedTime[2][3]=lv_spinbox_get_value(ui_timer3_amount);
        refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer3_amount_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ui_timer3_amount);
       feedTime[2][3]=lv_spinbox_get_value(ui_timer3_amount);
         refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer4_amount_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ui_timer4_amount);
        feedTime[3][3]=lv_spinbox_get_value(ui_timer4_amount);
        refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

static void timer4_amount_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ui_timer4_amount);
         feedTime[3][3]=lv_spinbox_get_value(ui_timer4_amount);
         refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}
//Обработка изменения значения слайдера шагов вперед
  static void fwdstep_slider_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_step_window_fwdstep_slider_label, "%d шагов", (int8_t)lv_slider_get_value(slider));
    lv_obj_align_to(ui_step_window_fwdstep_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    fwd_steps=(int8_t)lv_slider_get_value(slider);
  }

//Обработка изменения значения слайдера шагов назад
  static void bckstep_slider_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_step_window_bckstep_slider_label, "%d шагов", (int8_t)lv_slider_get_value(slider));
    lv_obj_align_to(ui_step_window_bckstep_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    bck_steps=(int8_t)lv_slider_get_value(slider);
  }

  //Обработка изменения значения слайдера скорости двигателя
  static void speed_slider_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_step_window_speed_slider_label, "%d шагов в сек", (int16_t)lv_slider_get_value(slider));
    lv_obj_align_to(ui_step_window_speed_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    step_speed=(float)lv_slider_get_value(slider);
  } 

  //Обработка изменения значения слайдера скорости двигателя
  static void maxrev_slider_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_step_window_maxrev_slider_label, "%d оборотов", (int16_t)lv_slider_get_value(slider));
    lv_obj_align_to(ui_step_window_maxrev_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    max_revs=(float)lv_slider_get_value(slider);
  }       

//Обработка изменения значения слайдера часового пояса
  static void slider_gmt_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(ui_gmt_slider_label, "GMT: %d%", (int8_t)lv_slider_get_value(slider));
    lv_obj_align_to(ui_gmt_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    timezone=(int8_t)lv_slider_get_value(slider);
    ntp.setGMT(timezone);
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  }

//Переключение темы
  static void theme_switch_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          theme=true;
        }
        else
        {
          theme=false;
        }
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), theme, &mainfont14);  
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  } 

//Переключение состояния бота
  static void tg_bot_switch_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          tg_bot=true;
        }
        else
        {
          tg_bot=false;
        }
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  } 

  //Переключение mqtt
  static void usemqtt_switch_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          usemqtt=true;
          MQTT_init();
        }
        else
        {
          usemqtt=false;
        }
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  } 

//событие измениния значения таймеров кормления
static void alarm_roll_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
      //Опрашиваем значение всех элементов
      //Таймер 0
          feedTime[0][0]=lv_roller_get_selected(ui_timer1_hour);//часы
          feedTime[0][1]=lv_roller_get_selected(ui_timer1_minute);//минуты
          feedTime[0][2]=lv_obj_has_state(ui_timer1_check, LV_STATE_CHECKED);//включен или выключен
          //Таймер 1
          feedTime[1][0]=lv_roller_get_selected(ui_timer2_hour);//часы
          feedTime[1][1]=lv_roller_get_selected(ui_timer2_minute);//минуты
          feedTime[1][2]=lv_obj_has_state(ui_timer2_check, LV_STATE_CHECKED);//включен или выключен
          //Таймер 2
          feedTime[2][0]=lv_roller_get_selected(ui_timer3_hour);//часы
          feedTime[2][1]=lv_roller_get_selected(ui_timer3_minute);//минуты
          feedTime[2][2]=lv_obj_has_state(ui_timer3_check, LV_STATE_CHECKED);//включен или выключен
          //Таймер 3
          feedTime[3][0]=lv_roller_get_selected(ui_timer4_hour);//часы
          feedTime[3][1]=lv_roller_get_selected(ui_timer4_minute);//минуты
          feedTime[3][2]=lv_obj_has_state(ui_timer4_check, LV_STATE_CHECKED);//включен или выключен
    
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    }
}

//Функция обработки нажатия кнопки кормления
static void event_feed(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
   {
    prefid(feedAmountSet);
    }
 }


//Функция обработки нажатия кнопки тест двигателя
static void event_test_motor(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
    {
    stepper.setMaxSpeed(step_speed);
    stepper.enable();
    oneRev();
    stepper.disable();
    }
 }

//Закрытие окна настроек шагового двигателя
static void ui_stepwindow_btn_close_event_handler(lv_event_t * e)
{
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    lv_obj_del(ui_stepwindow);//Закрываем окно
}

//Закрытие окна настроек калибровки весов
static void ui_scaleswindow_btn_close_event_handler(lv_event_t * e)
{
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
    lv_obj_del(ui_scaleswindow);//Закрываем окно
}

static void scales_weight_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ui_scales_window_weight);
        scales_control_weight=lv_spinbox_get_value(ui_scales_window_weight);
    }
}

static void scales_weight_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ui_scales_window_weight);
         scales_control_weight=lv_spinbox_get_value(ui_scales_window_weight);
    }
}

 //Функция обработки нажатия кнопки настроек шагового двигателя
static void step_motor_setup_window_open(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
   {
    window_setup_motor();
    }
 }

 //Функция калибровки коэффициента взвешивания
static void event_scales_calibrate_param(lv_event_t * e)
  {
  long curWeight;
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
    {
      if (sensor.available()) 
      {
        curWeight=sensor.read();
        curWeight-=tareWeight;
        Serial.println(curWeight);
        scales_param=curWeight/scales_control_weight;
        lv_label_set_text_fmt (ui_scales_window_param_label,"К: %.2f   Температура: %.1f ºС",scales_param,temp_cal);//Пишем текст метки
      }
    }
  }

// Определяем функцию для вычисления коэффициента коррекции
float getCorrectionFactor(float T) {
  // Коэффициенты для полинома третьей степени, подобранного по данным из статьи
  float a = -0.000000014;
  float b = 0.000003;
  float c = -0.0002;
  float d = 0.9998;
  // Вычисляем коэффициент коррекции по формуле
  float correctionFactor = a * pow(T - temp_cal, 3) + b * pow(T - temp_cal, 2) + c * (T - temp_cal) + d;
  return correctionFactor;
}

 //отрисовка интерфейса окна настрорек шаговика
void window_setup_motor()
{
  //Отрисовываем интерфейс окна настроек
  //Окно контейнер
    ui_stepwindow = lv_win_create(lv_scr_act(), 30);
    lv_win_add_title(ui_stepwindow, "Настройка шагового мотора");
    lv_obj_t * ui_stepwindow_btn_close = lv_win_add_btn(ui_stepwindow, LV_SYMBOL_CLOSE, 30);
    lv_obj_add_event_cb(ui_stepwindow_btn_close, ui_stepwindow_btn_close_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * step_windows_cont = lv_win_get_content(ui_stepwindow);  //контейнер для содержимого окна
    //содержимое окна
    //Надпись Шагов вперед
      lv_obj_t * ui_step_window_fwdsteps_label = lv_label_create(step_windows_cont);//метка названия панели
      lv_obj_align(ui_step_window_fwdsteps_label, LV_ALIGN_TOP_LEFT, 0, 5); //Выравниваем по левому краю
      lv_label_set_text (ui_step_window_fwdsteps_label,"Шагов вперед:");//Пишем текст метки
      //Слайдер изменения колличества шагов вперед
      lv_obj_t * ui_step_window_fwdstep_slider = lv_slider_create(step_windows_cont);
      lv_obj_set_size(ui_step_window_fwdstep_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_step_window_fwdstep_slider, ui_step_window_fwdsteps_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
      lv_slider_set_range(ui_step_window_fwdstep_slider, 0 , 200);
      lv_slider_set_value(ui_step_window_fwdstep_slider, fwd_steps, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_step_window_fwdstep_slider, fwdstep_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //Надпись на слайдере шагов вперед
      ui_step_window_fwdstep_slider_label = lv_label_create(step_windows_cont);
      lv_label_set_text_fmt(ui_step_window_fwdstep_slider_label, "%d шагов", (int)lv_slider_get_value(ui_step_window_fwdstep_slider));
      lv_obj_align_to(ui_step_window_fwdstep_slider_label, ui_step_window_fwdstep_slider, LV_ALIGN_CENTER, 0, 0);
      //Надпись Шагов назад
      lv_obj_t * ui_step_window_bcksteps_label = lv_label_create(step_windows_cont);//метка названия панели
      lv_obj_align_to(ui_step_window_bcksteps_label, ui_step_window_fwdstep_slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15); //Выравниваем по левому краю
      lv_label_set_text (ui_step_window_bcksteps_label,"Шагов назад:");//Пишем текст метки
      //Слайдер изменения колличества шагов назад
      lv_obj_t * ui_step_window_bckstep_slider = lv_slider_create(step_windows_cont);
      lv_obj_set_size(ui_step_window_bckstep_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_step_window_bckstep_slider, ui_step_window_bcksteps_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
      lv_slider_set_range(ui_step_window_bckstep_slider, 0 , 200);
      lv_slider_set_value(ui_step_window_bckstep_slider, bck_steps, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_step_window_bckstep_slider, bckstep_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //Надпись на слайдере шагов назад
      ui_step_window_bckstep_slider_label = lv_label_create(step_windows_cont);
      lv_label_set_text_fmt(ui_step_window_bckstep_slider_label, "%d шагов", (int)lv_slider_get_value(ui_step_window_bckstep_slider));
      lv_obj_align_to(ui_step_window_bckstep_slider_label, ui_step_window_bckstep_slider, LV_ALIGN_CENTER, 0, 0);
      //Надпись Скорость
      lv_obj_t * ui_step_window_speed_label = lv_label_create(step_windows_cont);//метка названия панели
      lv_obj_align_to(ui_step_window_speed_label, ui_step_window_bckstep_slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15); //Выравниваем по левому краю
      lv_label_set_text (ui_step_window_speed_label,"Скорость вращения:");//Пишем текст метки
      //Слайдер изменения скорости
      lv_obj_t * ui_step_window_speed_slider = lv_slider_create(step_windows_cont);
      lv_obj_set_size(ui_step_window_speed_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_step_window_speed_slider, ui_step_window_speed_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
      lv_slider_set_range(ui_step_window_speed_slider, 0 , 400);
      lv_slider_set_value(ui_step_window_speed_slider, step_speed, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_step_window_speed_slider, speed_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //Надпись на слайдере скорости
      ui_step_window_speed_slider_label = lv_label_create(step_windows_cont);
      lv_label_set_text_fmt(ui_step_window_speed_slider_label, "%d шагов в сек", (int)lv_slider_get_value(ui_step_window_speed_slider));
      lv_obj_align_to(ui_step_window_speed_slider_label, ui_step_window_speed_slider, LV_ALIGN_CENTER, 0, 0);

      //Надпись максимум оборотов до остановки
      lv_obj_t * ui_step_window_maxrev_label = lv_label_create(step_windows_cont);//метка названия панели
      lv_obj_align_to(ui_step_window_maxrev_label, ui_step_window_speed_slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15); //Выравниваем по левому краю
      lv_label_set_text (ui_step_window_maxrev_label,"Оборотов до остановки:");//Пишем текст метки
      //Слайдер изменения максимума оборотов
      lv_obj_t * ui_step_window_maxrev_slider = lv_slider_create(step_windows_cont);
      lv_obj_set_size(ui_step_window_maxrev_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_step_window_maxrev_slider, ui_step_window_maxrev_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
      lv_slider_set_range(ui_step_window_maxrev_slider, 0 , 30);
      lv_slider_set_value(ui_step_window_maxrev_slider, max_revs, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_step_window_maxrev_slider, maxrev_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //Надпись на слайдере максимума оборотов
      ui_step_window_maxrev_slider_label = lv_label_create(step_windows_cont);
      lv_label_set_text_fmt(ui_step_window_maxrev_slider_label, "%d оборотов", (int)lv_slider_get_value(ui_step_window_maxrev_slider));
      lv_obj_align_to(ui_step_window_maxrev_slider_label, ui_step_window_maxrev_slider, LV_ALIGN_CENTER, 0, 0);
      //Кнопка тест
      lv_obj_t * ui_step_window_test_button = lv_btn_create(step_windows_cont); // кнопка кормления  
      lv_obj_set_size(ui_step_window_test_button, lv_pct(100), 40);
      lv_obj_align_to(ui_step_window_test_button, ui_step_window_maxrev_slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15); //Выравниваем по левому краю
      lv_obj_add_event_cb(ui_step_window_test_button, event_test_motor, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
      lv_obj_t * ui_step_window_test_button_label=lv_label_create(ui_step_window_test_button);//Надпись на кнопке
      lv_label_set_text(ui_step_window_test_button_label, "Тест двигателя");
      lv_obj_center(ui_step_window_test_button_label);

}

//Окно калибровки весов
void window_scales_calibrate()
{
  //Отрисовываем интерфейс окна калибровки
  //Окно контейнер
    ui_scaleswindow = lv_win_create(lv_scr_act(), 30);
    lv_win_add_title(ui_scaleswindow, "Калибровка весов");
    lv_obj_t * ui_scaleswindow_btn_close = lv_win_add_btn(ui_scaleswindow, LV_SYMBOL_CLOSE, 30);
    lv_obj_add_event_cb(ui_scaleswindow_btn_close, ui_scaleswindow_btn_close_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * scales_windows_cont = lv_win_get_content(ui_scaleswindow);  //контейнер для содержимого окна
    //содержимое окна
    //Надпись 1 шаг
      lv_obj_t * ui_scales_window_weight_label = lv_label_create(scales_windows_cont);//метка названия панели
      lv_obj_align(ui_scales_window_weight_label, LV_ALIGN_TOP_LEFT, 0, 5); //Выравниваем по левому краю
      lv_label_set_text (ui_scales_window_weight_label,"1. Установите вес для калибровки:");//Пишем текст метки
      ui_scales_window_weight = lv_spinbox_create(scales_windows_cont); //Размер порции
      lv_spinbox_set_range(ui_scales_window_weight, 0, 1000);
      lv_spinbox_set_rollover(ui_scales_window_weight,true);
      lv_spinbox_set_digit_format(ui_scales_window_weight, 4, 4);
      lv_obj_set_size(ui_scales_window_weight, 100,40);
      lv_obj_align(ui_scales_window_weight,  LV_ALIGN_TOP_LEFT, 40, 25);
      lv_spinbox_set_value(ui_scales_window_weight, scales_control_weight);
      lv_obj_set_style_bg_opa(ui_timer4_amount, 0, LV_PART_CURSOR | LV_PART_SELECTED);
      lv_obj_t * btn = lv_btn_create(scales_windows_cont);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_scales_window_weight, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
      lv_obj_add_event_cb(btn, scales_weight_increment_event_cb, LV_EVENT_ALL,  NULL);
      btn = lv_btn_create(scales_windows_cont);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_scales_window_weight, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
      lv_obj_add_event_cb(btn, scales_weight_decrement_event_cb, LV_EVENT_ALL, NULL);
      //Шаг второй
      lv_obj_t * ui_scales_window_calibrate_label = lv_label_create(scales_windows_cont);//метка названия панели
      lv_obj_align(ui_scales_window_calibrate_label, LV_ALIGN_TOP_LEFT, 0, 75); //Выравниваем по левому краю
      lv_label_set_text (ui_scales_window_calibrate_label,"2. Положите вес и нажмите кнопку:");//Пишем текст метки
      //Кнопка калибровать
      lv_obj_t * ui_scales_window_calibrate_button = lv_btn_create(scales_windows_cont); // кнопка кормления  
      lv_obj_set_size(ui_scales_window_calibrate_button, lv_pct(100), 40);
      lv_obj_align_to(ui_scales_window_calibrate_button, ui_scales_window_calibrate_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15); //Выравниваем по левому краю
      lv_obj_add_event_cb(ui_scales_window_calibrate_button, event_scales_calibrate_param, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
      lv_obj_t * ui_scales_window_calibrate_button_label=lv_label_create(ui_scales_window_calibrate_button);//Надпись на кнопке
      lv_label_set_text(ui_scales_window_calibrate_button_label, "Калибровать");
      lv_obj_center(ui_scales_window_calibrate_button_label);
      ui_scales_window_param_label = lv_label_create(scales_windows_cont);//метка коэффициента
      lv_obj_align_to(ui_scales_window_param_label, ui_scales_window_calibrate_button, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text_fmt (ui_scales_window_param_label,"К: %.2f   Температура: %.1f ºС",scales_param,temp_cal);//Пишем текст метки
}



//Функция обработки нажатия кнопки калибровки весов
static void event_scales_calibrate(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
   {
    while (sensor.available()==false)
    {
      Serial.println("Waiting...");
      delay(10);
    }
    tareWeight=sensor.read();
    temp_cal = dht.readTemperature();
    window_scales_calibrate();
    }
 }

//Функция обработки нажатия кнопки калибровки экрана 
static void event_calibrate(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
   {
    touch_calibrate(true);
    }
 }

//Изменение значения размера порции на экране
   static void slider_feedamount_event_cb(lv_event_t * e)
    {
      lv_obj_t * slider = lv_event_get_target(e);
      feedAmountSet=(int)lv_slider_get_value(slider);
      lv_label_set_text_fmt(ui_label_feedAmount,"%d грам", feedAmountSet);
      refsaveconfigdelay.setInterval(10000);
    }

void draw_interface()
  {
    LV_IMG_DECLARE(step_icon); //иконка шагового двигателя
  //Установка цветового оформления
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), theme, &mainfont14);   
  //Создаем экранные объекты
  
  //Контейнеры
    ui_tabview = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, 30); //создаем панель вкладок
      //Создаем вкладки и присваиваем им отображаемые имена
      lv_obj_t * ui_tab1 = lv_tabview_add_tab(ui_tabview, "Основной");
      lv_obj_t * ui_tab2 = lv_tabview_add_tab(ui_tabview, "Таймеры");
      lv_obj_t * ui_tab3 = lv_tabview_add_tab(ui_tabview, "Настройки");
    //вкладки на экране настроек
    ui_tabview_settings = lv_tabview_create(ui_tab3, LV_DIR_LEFT, 30);
      lv_obj_t * ui_set_tab1 = lv_tabview_add_tab(ui_tabview_settings, LV_SYMBOL_DISPLAY);
      lv_obj_t * ui_set_tab2 = lv_tabview_add_tab(ui_tabview_settings, LV_SYMBOL_ACLOCK);
      lv_obj_t * ui_set_tab3 = lv_tabview_add_tab(ui_tabview_settings, LV_SYMBOL_ASCALES);
      lv_obj_t * ui_set_tab4 = lv_tabview_add_tab(ui_tabview_settings, LV_SYMBOL_TELEGRAM);
      lv_obj_t * ui_set_tab5 = lv_tabview_add_tab(ui_tabview_settings, LV_SYMBOL_LOOP);
  //Создаем экранную клавиатуру 
  kb = lv_keyboard_create(lv_scr_act());//создаем экранную клавиатуру
  lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0); //положение на экране
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);     
  //Панель состояния
    //Иконка Wifi
    ui_status_icons = lv_label_create(ui_tabview);
    lv_obj_align(ui_status_icons, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(ui_status_icons, LV_SYMBOL_WIFI);
    lv_obj_add_flag(ui_status_icons, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_status_icons, LV_OBJ_FLAG_HIDDEN);
    //IP адрес
    ui_status_ip = lv_label_create(ui_tabview);
    lv_obj_align(ui_status_ip, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(ui_status_ip, " ");
    lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
   
  //Вкладка кормления (ui_tab1)
    //Часы
    ui_clock = lv_label_create(ui_tab1); //часы
    lv_obj_align(ui_clock, LV_ALIGN_TOP_LEFT, 0, 5); //положение на экране
    lv_obj_set_size(ui_clock, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(ui_clock, &digital, 0);

    //кнопка настройки шагового двигателя
    lv_obj_t * ui_stepmotor_setup_btn = lv_imgbtn_create(ui_tab1);
    lv_imgbtn_set_src(ui_stepmotor_setup_btn, LV_IMGBTN_STATE_RELEASED, NULL, &step_icon, NULL);
    lv_obj_align(ui_stepmotor_setup_btn, LV_ALIGN_TOP_RIGHT, 0, 12);
    lv_obj_set_size(ui_stepmotor_setup_btn, 48, 48);
    lv_obj_add_event_cb(ui_stepmotor_setup_btn, step_motor_setup_window_open, LV_EVENT_CLICKED, NULL);

    //Кнопка кормления
    lv_obj_t * ui_feed_button = lv_btn_create(ui_tab1); // кнопка кормления  
    lv_obj_set_size(ui_feed_button, lv_pct(100), 40);
    lv_obj_align(ui_feed_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(ui_feed_button, event_feed, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
    lv_obj_t * ui_feed_btn_label=lv_label_create(ui_feed_button);//Надпись на кнопке
    lv_label_set_text(ui_feed_btn_label, "Покормить "LV_SYMBOL_CATFOOD );
    lv_obj_center(ui_feed_btn_label);
    //Размер порции
    ui_slider_feed_amount = lv_slider_create(ui_tab1);
    lv_obj_set_size(ui_slider_feed_amount, lv_pct(90), 20);
    lv_slider_set_range(ui_slider_feed_amount, 1 , 60);
    lv_slider_set_value(ui_slider_feed_amount, feedAmountSet, LV_ANIM_OFF);
    lv_obj_align_to(ui_slider_feed_amount, ui_feed_button, LV_ALIGN_TOP_RIGHT, 0, -40);
    lv_obj_add_event_cb(ui_slider_feed_amount, slider_feedamount_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
    ui_label_feedAmount = lv_label_create(ui_tab1);
    lv_label_set_text_fmt(ui_label_feedAmount,"%d гр.", feedAmountSet);
    lv_obj_align_to(ui_label_feedAmount, ui_slider_feed_amount, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * ui_label_feed_text = lv_label_create(ui_tab1);
    lv_label_set_text(ui_label_feed_text, LV_SYMBOL_SCALES);
    lv_obj_align_to(ui_label_feed_text, ui_slider_feed_amount, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    ui_remain = lv_label_create(ui_tab1); //осталось времени до срабатывания таймера
    lv_label_set_text(ui_remain, LV_SYMBOL_SANDCLOCK);
    lv_obj_align(ui_remain, LV_ALIGN_TOP_LEFT, 0, 70);
    //вес корма
    ui_food_weight = lv_label_create(ui_tab1);
    lv_label_set_text_fmt(ui_food_weight, LV_SYMBOL_WEIGHT" %d грамм",foodWeight); 
    lv_obj_align_to(ui_food_weight, ui_remain, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_obj_add_flag(ui_food_weight, LV_OBJ_FLAG_HIDDEN);
    //Температура и влажность
    ui_temp_label = lv_label_create(ui_tab1); //температура
    lv_label_set_text_fmt(ui_temp_label, LV_SYMBOL_TEMP" %.1f °С",temperature);
    lv_obj_align_to(ui_temp_label, ui_remain, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_obj_align(ui_temp_label, LV_ALIGN_TOP_RIGHT, 0, 70);

    ui_humid_label = lv_label_create(ui_tab1); //осталось времени до срабатывания таймера
    lv_label_set_text_fmt(ui_humid_label, LV_SYMBOL_HUMID" %.1f%%",humidity);
    lv_obj_align_to(ui_humid_label, ui_temp_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);


  //Вкладка таймеров
    //Список констант для роллеров
    const char * rollhours = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
    const char * rollminutes = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\m50\n51\m52\n53\n54\n55\n56\n57\n58\n59";
      //Таймер 1
      ui_timer1_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer1_hour, 2);
      lv_roller_set_options(ui_timer1_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer1_hour, feedTime[0][0], LV_ANIM_OFF);
      lv_obj_align(ui_timer1_hour, LV_ALIGN_TOP_LEFT, 0, 5);
      lv_obj_set_size(ui_timer1_hour,50,60);
      lv_obj_add_event_cb(ui_timer1_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
       ui_timer1_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer1_minute, 2);
      lv_roller_set_options(ui_timer1_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer1_minute, feedTime[0][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer1_minute,50,60);
      lv_obj_align_to(ui_timer1_minute, ui_timer1_hour, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer1_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer1_amount = lv_spinbox_create(ui_tab2); //Размер порции
      lv_spinbox_set_range(ui_timer1_amount, 1, 60);
      lv_spinbox_set_digit_format(ui_timer1_amount, 3, 3);
      lv_spinbox_set_rollover(ui_timer1_amount,true);
      lv_obj_set_size(ui_timer1_amount, 40,40);
      lv_obj_align_to(ui_timer1_amount, ui_timer1_minute, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_spinbox_set_value(ui_timer1_amount,feedTime[0][3]);
      lv_obj_set_style_bg_opa(ui_timer1_amount, 0, LV_PART_CURSOR | LV_PART_SELECTED);
      lv_obj_t * btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer1_amount, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
      lv_obj_add_event_cb(btn, timer1_amount_increment_event_cb, LV_EVENT_ALL,  NULL);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer1_amount, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
      lv_obj_add_event_cb(btn, timer1_amount_decrement_event_cb, LV_EVENT_ALL, NULL);
      ui_timer1_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer1_check, ui_timer1_amount, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_checkbox_set_text(ui_timer1_check, "");
      if (feedTime[0][2]==1) lv_obj_add_state(ui_timer1_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer1_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 2
      ui_timer2_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer2_hour, 2);
      lv_roller_set_options(ui_timer2_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer2_hour, feedTime[1][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer2_hour,50,60);
      lv_obj_align_to(ui_timer2_hour, ui_timer1_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer2_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer2_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer2_minute, 2);
      lv_roller_set_options(ui_timer2_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer2_minute, feedTime[1][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer2_minute,50,60);
      lv_obj_align_to(ui_timer2_minute,ui_timer2_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer2_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer2_amount = lv_spinbox_create(ui_tab2); //Размер порции
      lv_spinbox_set_range(ui_timer2_amount, 1, 60);
      lv_spinbox_set_digit_format(ui_timer2_amount, 3, 3);
      lv_spinbox_set_rollover(ui_timer2_amount,true);
      lv_obj_set_size(ui_timer2_amount, 40,40);
      lv_obj_align_to(ui_timer2_amount, ui_timer2_minute, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_spinbox_set_value(ui_timer2_amount,feedTime[1][3]);
      lv_obj_set_style_bg_opa(ui_timer2_amount, 0, LV_PART_SELECTED | LV_PART_CURSOR);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer2_amount, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
      lv_obj_add_event_cb(btn, timer2_amount_increment_event_cb, LV_EVENT_ALL,  NULL);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer2_amount, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
      lv_obj_add_event_cb(btn, timer2_amount_decrement_event_cb, LV_EVENT_ALL, NULL);
      ui_timer2_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer2_check, ui_timer2_amount, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_checkbox_set_text(ui_timer2_check, "");
      if (feedTime[1][2]==1) lv_obj_add_state(ui_timer2_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer2_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 3
      ui_timer3_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer3_hour, 2);
      lv_roller_set_options(ui_timer3_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer3_hour, feedTime[2][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer3_hour,50,60);
      lv_obj_align_to(ui_timer3_hour, ui_timer2_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer3_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer3_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer3_minute, 2);
      lv_roller_set_options(ui_timer3_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer3_minute, feedTime[2][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer3_minute,50,60);
      lv_obj_align_to(ui_timer3_minute,ui_timer3_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer3_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer3_amount = lv_spinbox_create(ui_tab2); //Размер порции
      lv_spinbox_set_range(ui_timer3_amount, 1, 60);
      lv_spinbox_set_digit_format(ui_timer3_amount, 3, 3);
      lv_spinbox_set_rollover(ui_timer3_amount,true);
      lv_obj_set_size(ui_timer3_amount, 40,40);
      lv_obj_align_to(ui_timer3_amount, ui_timer3_minute, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_spinbox_set_value(ui_timer3_amount,feedTime[2][3]);
      lv_obj_set_style_bg_opa(ui_timer3_amount, 0, LV_PART_CURSOR | LV_PART_SELECTED);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer3_amount, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
      lv_obj_add_event_cb(btn, timer3_amount_increment_event_cb, LV_EVENT_ALL,  NULL);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer3_amount, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
      lv_obj_add_event_cb(btn, timer3_amount_decrement_event_cb, LV_EVENT_ALL, NULL);
      ui_timer3_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer3_check, ui_timer3_amount, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_checkbox_set_text(ui_timer3_check, "");
      if (feedTime[2][2]==1) lv_obj_add_state(ui_timer3_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer3_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 4
      ui_timer4_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer4_hour, 2);
      lv_roller_set_options(ui_timer4_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer4_hour, feedTime[3][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer4_hour,50,60);
      lv_obj_align_to(ui_timer4_hour, ui_timer3_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer4_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer4_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer4_minute, 2);
      lv_roller_set_options(ui_timer4_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer4_minute, feedTime[3][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer4_minute,50,60);
      lv_obj_align_to(ui_timer4_minute,ui_timer4_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer4_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer4_amount = lv_spinbox_create(ui_tab2); //Размер порции
      lv_spinbox_set_range(ui_timer4_amount, 1, 60);
      lv_spinbox_set_digit_format(ui_timer4_amount, 3, 3);
      lv_spinbox_set_rollover(ui_timer4_amount,true);
      lv_obj_set_size(ui_timer4_amount, 40,40);
      lv_obj_align_to(ui_timer4_amount, ui_timer4_minute, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_spinbox_set_value(ui_timer4_amount,feedTime[3][3]);
      lv_obj_set_style_bg_opa(ui_timer4_amount, 0, LV_PART_CURSOR | LV_PART_SELECTED);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer4_amount, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
      lv_obj_add_event_cb(btn, timer4_amount_increment_event_cb, LV_EVENT_ALL,  NULL);
      btn = lv_btn_create(ui_tab2);
      lv_obj_set_size(btn, 25, 40);
      lv_obj_align_to(btn, ui_timer4_amount, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
      lv_obj_add_event_cb(btn, timer4_amount_decrement_event_cb, LV_EVENT_ALL, NULL);
      ui_timer4_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer4_check, ui_timer4_amount, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
      lv_checkbox_set_text(ui_timer4_check, "");
      if (feedTime[3][2]==1) lv_obj_add_state(ui_timer4_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer4_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

  //Вкладка настроек
    //Группируем настройки. Создаем панель для каждой группы
    //Панель настроек дисплея на вкладке 1
    lv_obj_t * ui_set_panel_display = lv_obj_create(ui_set_tab1);
      //Название панели
      lv_obj_t * ui_set_panel_display_label = lv_label_create(ui_set_panel_display);//метка названия панели
      lv_obj_align(ui_set_panel_display_label, LV_ALIGN_TOP_MID, 0, 5); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_display_label,"Настройки дисплея");//Пишем текст метки
      
      //Кнопка калибровки экрана
      lv_obj_t * ui_calibrate_button = lv_btn_create(ui_set_panel_display); // кнопка кормления  
      lv_obj_set_size(ui_calibrate_button, lv_pct(100), 30);
      lv_obj_align(ui_calibrate_button, LV_ALIGN_TOP_LEFT, 0, 25);
      lv_obj_add_event_cb(ui_calibrate_button,  event_calibrate, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
      
      //текст на кнопке калибровки экрана
      lv_obj_t * ui_calibrate_button_text = lv_label_create(ui_calibrate_button);
      lv_label_set_text(ui_calibrate_button_text, "Калибровка экрана");
      lv_obj_center(ui_calibrate_button_text);
      
      //Надпись настройка яркости
      lv_obj_t * ui_set_panel_display_bright_label = lv_label_create(ui_set_panel_display);//метка названия панели
      lv_obj_align_to(ui_set_panel_display_bright_label, ui_calibrate_button, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text (ui_set_panel_display_bright_label,"Яркость дисплея");//Пишем текст метки
      //Слайдер изменения яркости
      ui_set_panel_display_bright_slider = lv_slider_create(ui_set_panel_display);
      lv_obj_set_size(ui_set_panel_display_bright_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_set_panel_display_bright_slider, ui_set_panel_display_bright_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
      lv_slider_set_range(ui_set_panel_display_bright_slider, 1 , 255);
      lv_slider_set_value(ui_set_panel_display_bright_slider, bright_level, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_set_panel_display_bright_slider, slider_brightness_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //текст на слайдере изменения яркости
      ui_backlight_slider_label = lv_label_create(ui_set_panel_display);
      lv_label_set_text_fmt(ui_backlight_slider_label, "%d%", (int)lv_slider_get_value(ui_set_panel_display_bright_slider));
      lv_obj_align_to(ui_backlight_slider_label,ui_set_panel_display_bright_slider, LV_ALIGN_CENTER, 0, 0);

      //Надпись установки дневного времени
    lv_obj_t  * ui_label_daytime = lv_label_create(ui_set_panel_display); //создаем объект заголовок
    lv_label_set_text(ui_label_daytime, "Дневная подсветка"); //сам текст для надписи
    lv_obj_align_to(ui_label_daytime, ui_set_panel_display_bright_slider,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране

    //Переключатель подсветки в ночное время
      ui_set_display_daytime_switch = lv_switch_create(ui_set_panel_display);
      lv_obj_add_event_cb(ui_set_display_daytime_switch, daytime_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
      lv_obj_set_size(ui_set_display_daytime_switch,32,22);
      lv_obj_align_to(ui_set_display_daytime_switch, ui_label_daytime, LV_ALIGN_OUT_RIGHT_MID, 30, 0); //положение на экране
      
      if (daytime){lv_obj_add_state(ui_set_display_daytime_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_display_daytime_switch, LV_STATE_CHECKED);}
    
    ui_slider_day_time = lv_slider_create(ui_set_panel_display);
    lv_obj_align_to(ui_slider_day_time, ui_label_daytime, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_slider_set_range(ui_slider_day_time, 0 , 23);
    lv_obj_set_size(ui_slider_day_time, lv_pct(100), 20);
    lv_slider_set_mode(ui_slider_day_time, LV_SLIDER_MODE_RANGE);
    lv_slider_set_left_value(ui_slider_day_time, daybegin, LV_ANIM_OFF);
    lv_slider_set_value(ui_slider_day_time, dayend, LV_ANIM_OFF);
    lv_obj_refresh_ext_draw_size(ui_slider_day_time);

    lv_obj_add_event_cb(ui_slider_day_time, slider_daytime_event, LV_EVENT_VALUE_CHANGED, NULL);
    slider_daytime_label = lv_label_create(ui_set_panel_display);
    lv_label_set_text_fmt(slider_daytime_label, "%d - %d", daybegin, dayend);
    lv_obj_align_to(slider_daytime_label, ui_slider_day_time, LV_ALIGN_CENTER, 0, 0);

      //Переключение светлой и темной темы
      lv_obj_t  * ui_set_display_theme_label = lv_label_create(ui_set_panel_display); //создаем объект заголовок
      lv_label_set_text(ui_set_display_theme_label, "Темная тема"); //текст
      lv_obj_align_to(ui_set_display_theme_label, ui_slider_day_time, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      //Переключатель светлой и тёмной темы
      lv_obj_t * ui_set_display_theme_switch = lv_switch_create(ui_set_panel_display);
      lv_obj_add_event_cb(ui_set_display_theme_switch, theme_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
      lv_obj_set_size(ui_set_display_theme_switch,32,22);
      lv_obj_align_to(ui_set_display_theme_switch, ui_set_display_theme_label, LV_ALIGN_OUT_RIGHT_MID, 30, 0); //положение на экране
      
      if (theme){lv_obj_add_state(ui_set_display_theme_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_display_theme_switch, LV_STATE_CHECKED);}
      
      
      lv_obj_set_size(ui_set_panel_display, lv_pct(100),LV_SIZE_CONTENT); 
    
    //Панель настроек часов на вкладке 2
    lv_obj_t * ui_set_panel_time = lv_obj_create(ui_set_tab2);
      //Название панели
      lv_obj_t * ui_set_panel_time_label = lv_label_create(ui_set_panel_time);//метка названия панели
      lv_obj_align(ui_set_panel_time_label, LV_ALIGN_TOP_MID, 0, 0); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_time_label,"Настройки времени");//Пишем текст метки
      //Надпись часовой пояс
      lv_obj_t * ui_set_panel_time_gmt_label = lv_label_create(ui_set_panel_time);//метка названия панели
      lv_obj_align(ui_set_panel_time_gmt_label, LV_ALIGN_TOP_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text (ui_set_panel_time_gmt_label,"Часовой пояс");//Пишем текст метки
      //Слайдер изменения часового пояса
      lv_obj_t * ui_set_panel_time_gmt_slider = lv_slider_create(ui_set_panel_time);
      lv_obj_set_size(ui_set_panel_time_gmt_slider, lv_pct(100), 20);
      lv_obj_align_to(ui_set_panel_time_gmt_slider, ui_set_panel_time_gmt_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
      lv_slider_set_range(ui_set_panel_time_gmt_slider, -12 , 14);
      lv_slider_set_value(ui_set_panel_time_gmt_slider, timezone, LV_ANIM_OFF);
      lv_obj_add_event_cb(ui_set_panel_time_gmt_slider, slider_gmt_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
      //Надпись на слайдере изменения часового пояса
      ui_gmt_slider_label = lv_label_create(ui_set_panel_time);
      lv_label_set_text_fmt(ui_gmt_slider_label, "GMT: %d", (int)lv_slider_get_value(ui_set_panel_time_gmt_slider));
      lv_obj_align_to(ui_gmt_slider_label, ui_set_panel_time_gmt_slider, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_size(ui_set_panel_time, lv_pct(100),LV_SIZE_CONTENT);
    //lv_obj_align_to(ui_set_panel_time, ui_set_panel_display, LV_ALIGN_OUT_BOTTOM_LEFT,0,5); 

    //Панель настроек весов на вкладке 3
    lv_obj_t * ui_set_panel_scales = lv_obj_create(ui_set_tab3);
      //Название панели
      lv_obj_t * ui_set_panel_scales_label = lv_label_create(ui_set_panel_scales);//метка названия панели
      lv_obj_align(ui_set_panel_scales_label, LV_ALIGN_TOP_MID, 0, 0); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_scales_label,"Калибровка весов");//Пишем текст метки
      //Надпись вес пустой тары
      ui_set_panel_scales_coef_label = lv_label_create(ui_set_panel_scales);//метка названия панели
      lv_obj_align(ui_set_panel_scales_coef_label, LV_ALIGN_TOP_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text_fmt (ui_set_panel_scales_coef_label,"Коэффициент: %.2f", scales_param);//Пишем текст метки
      //Кнопка калибровки весов
      lv_obj_t * ui_scales_calibrate_button = lv_btn_create(ui_set_panel_scales); // кнопка кормления  
      lv_obj_set_size(ui_scales_calibrate_button, lv_pct(100), 30);
      lv_obj_align(ui_scales_calibrate_button, LV_ALIGN_TOP_LEFT, 0, 45);
      lv_obj_add_event_cb(ui_scales_calibrate_button,  event_scales_calibrate, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
      //текст на кнопке калибровки весов
      lv_obj_t * ui_calibrate_scales_button_text = lv_label_create(ui_scales_calibrate_button);
      lv_label_set_text(ui_calibrate_scales_button_text, "Калибровка весов");
      lv_obj_center(ui_calibrate_scales_button_text);
      lv_obj_set_size(ui_set_panel_scales, lv_pct(100),LV_SIZE_CONTENT);

    //Панель настроек телеграм бота на вкладке 4
    lv_obj_t * ui_set_panel_telegram = lv_obj_create(ui_set_tab4);
      //Название панели
      lv_obj_t * ui_set_panel_telegram_label = lv_label_create(ui_set_panel_telegram);//метка названия панели
      lv_obj_align(ui_set_panel_telegram_label, LV_ALIGN_TOP_MID, 0, 0); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_telegram_label,"Настройки Telegram бота");//Пишем текст метки
      
      //Переключатель бота заголовок
      lv_obj_t  * ui_set_panel_telegram_bot_label = lv_label_create(ui_set_panel_telegram); //создаем объект заголовок
      lv_label_set_text(ui_set_panel_telegram_bot_label, "Использовать бота"); //текст
      lv_obj_align(ui_set_panel_telegram_bot_label, LV_ALIGN_TOP_LEFT, 0, 20); //положение на экране
      //Переключатель бота
      ui_set_panel_telegram_bot_switch = lv_switch_create(ui_set_panel_telegram);
      lv_obj_add_event_cb(ui_set_panel_telegram_bot_switch, tg_bot_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
      lv_obj_set_size(ui_set_panel_telegram_bot_switch,32,22);
      lv_obj_align_to(ui_set_panel_telegram_bot_switch, ui_set_panel_telegram_bot_label, LV_ALIGN_OUT_RIGHT_MID, 30, 0); //положение на экране
      if (tg_bot){lv_obj_add_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_panel_telegram_bot_switch, LV_STATE_CHECKED);}

      //Надпись токен
      lv_obj_t * ui_set_panel_telegram_token_label = lv_label_create(ui_set_panel_telegram);//метка названия панели
      lv_obj_align_to(ui_set_panel_telegram_token_label, ui_set_panel_telegram_bot_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_telegram_token_label,"Токен:");//Пишем текст метки
      //Поле ввода токена
      ui_set_panel_telegram_token_ta = lv_textarea_create(ui_set_panel_telegram);
      lv_textarea_set_one_line(ui_set_panel_telegram_token_ta, true);
      lv_textarea_set_text(ui_set_panel_telegram_token_ta, bot_token.c_str());
      lv_obj_set_width(ui_set_panel_telegram_token_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_telegram_token_ta,ui_set_panel_telegram_token_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_telegram_token_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_telegram_token_ta, token_ta_event_cb, LV_EVENT_READY, NULL);
       //Надпись ChatID
      lv_obj_t * ui_set_panel_telegram_chatid_label = lv_label_create(ui_set_panel_telegram);//метка названия панели
      lv_obj_align(ui_set_panel_telegram_chatid_label, LV_ALIGN_TOP_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text (ui_set_panel_telegram_chatid_label,"ID чата:");//Пишем текст метки
      lv_obj_align_to(ui_set_panel_telegram_chatid_label,ui_set_panel_telegram_token_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      //Поле ввода ID
      ui_set_panel_telegram_chatid_ta = lv_textarea_create(ui_set_panel_telegram);
      lv_textarea_set_one_line(ui_set_panel_telegram_chatid_ta, true);
      lv_textarea_set_text(ui_set_panel_telegram_chatid_ta, chatID.c_str());
      lv_obj_set_width(ui_set_panel_telegram_chatid_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_telegram_chatid_ta,ui_set_panel_telegram_chatid_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_telegram_chatid_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_telegram_chatid_ta, chatid_ta_event_cb, LV_EVENT_READY, NULL);

    lv_obj_set_size(ui_set_panel_telegram, lv_pct(100),LV_SIZE_CONTENT);
    //lv_obj_align_to(ui_set_panel_time, ui_set_panel_display, LV_ALIGN_OUT_BOTTOM_LEFT,0,5); 

    //Панель настроек mqtt на вкладке 5
    lv_obj_t * ui_set_panel_mqtt = lv_obj_create(ui_set_tab5);
      //Название панели
      lv_obj_t * ui_set_panel_mqtt_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align(ui_set_panel_mqtt_label, LV_ALIGN_TOP_MID, 0, 0); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_mqtt_label,"Настройки MQTT");//Пишем текст метки
      
      //Переключатель mqtt заголовок
      lv_obj_t  * ui_set_panel_usemqtt_label = lv_label_create(ui_set_panel_mqtt); //создаем объект заголовок
      lv_label_set_text(ui_set_panel_usemqtt_label, "Использовать MQTT"); //текст
      lv_obj_align(ui_set_panel_usemqtt_label,LV_ALIGN_TOP_LEFT, 0, 20); //положение на экране
      //Переключатель MQTT
      lv_obj_t * ui_set_panel_usemqtt_switch = lv_switch_create(ui_set_panel_mqtt);
      lv_obj_add_event_cb(ui_set_panel_usemqtt_switch, usemqtt_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
      lv_obj_set_size(ui_set_panel_usemqtt_switch,32,22);
      lv_obj_align_to(ui_set_panel_usemqtt_switch, ui_set_panel_usemqtt_label, LV_ALIGN_OUT_RIGHT_MID, 30, 0); //положение на экране
      if (usemqtt){lv_obj_add_state(ui_set_panel_usemqtt_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_panel_usemqtt_switch, LV_STATE_CHECKED);}
      //Надпись адрес
      lv_obj_t * ui_set_panel_mqtt_adress_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align_to(ui_set_panel_mqtt_adress_label, ui_set_panel_usemqtt_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_mqtt_adress_label,"Адрес брокера:");//Пишем текст метки
      //Поле ввода адреса
      ui_set_panel_mqtt_adress_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_adress_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_adress_ta, mqtt_server.c_str());
      lv_obj_set_width(ui_set_panel_mqtt_adress_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_adress_ta,ui_set_panel_mqtt_adress_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_adress_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_adress_ta, mqttadress_ta_event_cb, LV_EVENT_READY, NULL);
       //Надпись порт
      lv_obj_t * ui_set_panel_mqtt_port_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align(ui_set_panel_mqtt_port_label, LV_ALIGN_TOP_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text (ui_set_panel_mqtt_port_label,"Порт:");//Пишем текст метки
      lv_obj_align_to(ui_set_panel_mqtt_port_label,ui_set_panel_mqtt_adress_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      //Поле ввода порта
      ui_set_panel_mqtt_port_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_port_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_port_ta, String(mqtt_port).c_str());
      lv_textarea_set_accepted_chars(ui_set_panel_mqtt_port_ta, "0123456789");
      lv_textarea_set_max_length(ui_set_panel_mqtt_port_ta, 5);
      lv_obj_set_width(ui_set_panel_mqtt_port_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_port_ta,ui_set_panel_mqtt_port_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_port_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_port_ta, mqttport_ta_event_cb, LV_EVENT_READY, NULL);
      //Надпись логин
      lv_obj_t * ui_set_panel_mqtt_login_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align_to(ui_set_panel_mqtt_login_label, ui_set_panel_mqtt_port_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_mqtt_login_label,"Логин:");//Пишем текст метки
      //Поле ввода логина
      ui_set_panel_mqtt_login_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_login_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_login_ta, mqtt_login.c_str());
      lv_obj_set_width(ui_set_panel_mqtt_login_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_login_ta,ui_set_panel_mqtt_login_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_login_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_login_ta, mqttlogin_ta_event_cb, LV_EVENT_READY, NULL);
      //Надпись пароль
      lv_obj_t * ui_set_panel_mqtt_pass_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align_to(ui_set_panel_mqtt_pass_label, ui_set_panel_mqtt_login_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_mqtt_pass_label,"Пароль:");//Пишем текст метки
      //Поле ввода пароля
      ui_set_panel_mqtt_pass_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_pass_ta, true);
      lv_textarea_set_password_mode(ui_set_panel_mqtt_pass_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_pass_ta, mqtt_pass.c_str());
      lv_obj_set_width(ui_set_panel_mqtt_pass_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_pass_ta,ui_set_panel_mqtt_pass_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_pass_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_pass_ta, mqttpass_ta_event_cb, LV_EVENT_READY, NULL);
      //поле ввода топика управления
      //Надпись Топик управления
      lv_obj_t * ui_set_panel_mqtt_control_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align_to(ui_set_panel_mqtt_control_label, ui_set_panel_mqtt_pass_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_mqtt_control_label,"Топик управления:");//Пишем текст метки
      //Поле ввода топика
      ui_set_panel_mqtt_control_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_control_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_control_ta, cmdTopic.c_str());
      lv_obj_set_width(ui_set_panel_mqtt_control_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_control_ta,ui_set_panel_mqtt_control_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_control_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_control_ta, mqttcommand_ta_event_cb, LV_EVENT_READY, NULL);
      //поле ввода топика статуса
      //Надпись Топик статуса
      lv_obj_t * ui_set_panel_mqtt_status_label = lv_label_create(ui_set_panel_mqtt);//метка названия панели
      lv_obj_align_to(ui_set_panel_mqtt_status_label, ui_set_panel_mqtt_control_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      lv_label_set_text (ui_set_panel_mqtt_status_label,"Топик статуса:");//Пишем текст метки
      //Поле ввода топика
      ui_set_panel_mqtt_status_ta = lv_textarea_create(ui_set_panel_mqtt);
      lv_textarea_set_one_line(ui_set_panel_mqtt_status_ta, true);
      lv_textarea_set_text(ui_set_panel_mqtt_status_ta, statusTopic.c_str());
      lv_obj_set_width(ui_set_panel_mqtt_status_ta, lv_pct(100));
      lv_obj_align_to(ui_set_panel_mqtt_status_ta,ui_set_panel_mqtt_status_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); //положение на экране
      lv_obj_add_event_cb(ui_set_panel_mqtt_status_ta, ta_event_cb, LV_EVENT_ALL, kb);
      lv_obj_add_event_cb(ui_set_panel_mqtt_status_ta, mqttstatus_ta_event_cb, LV_EVENT_READY, NULL);

    lv_obj_set_size(ui_set_panel_mqtt, lv_pct(100),LV_SIZE_CONTENT);
  }