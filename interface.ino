//Перключение темы
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
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), theme, &fira);  
  refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
  } 

//событие измениния значения таймеров кормления
static void alarm_roll_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) {savealarm=true;}
    refsaveconfigdelay.setInterval(10000); //запускаем планировщик сохранения настроек
}

//Функция обработки нажатия кнопки кормления
static void event_feed(lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
   if(code == LV_EVENT_CLICKED) 
   {
    prefid();
    }
 }

//Смена активной вкладки
 static void change_tab_event(lv_event_t * e)
  {
    uint16_t acttab=lv_tabview_get_tab_act(ui_tabview);
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) 
    {
       if (acttab!=1 && savealarm==true) //Если изменяется активная вкладка и стоит флаг, что значение одного из элементов вкладки будильники изменилось
        {
          savealarm=false;//отключаем флаг
          //Опрашиваем значения всех элементов и заносим в массив
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
        }
    }
  }

//Изменение значения размера порции на экране
   static void slider_feedamount_event_cb(lv_event_t * e)
    {
      lv_obj_t * slider = lv_event_get_target(e);
      feedAmount=(int)lv_slider_get_value(slider);
      lv_label_set_text_fmt(ui_label_feedAmount,"%d", feedAmount);
      refsaveconfigdelay.setInterval(10000);
    }

void draw_interface()
  {
  //Установка цветового оформления
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), theme, &fira);   
  //Создаем экранные объекты
  //графические ресурсы
  LV_IMG_DECLARE(runingcat_img); //гифка с котиком
  LV_IMG_DECLARE(food_img); //гифка с едой
  //Контейнеры
    ui_tabview = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, 30); //создаем панель вкладок
      //Создаем вкладки и присваиваем им отображаемые имена
      lv_obj_t * ui_tab1 = lv_tabview_add_tab(ui_tabview, "Кормление");
      lv_obj_t * ui_tab2 = lv_tabview_add_tab(ui_tabview, "Таймеры");
      lv_obj_t * ui_tab3 = lv_tabview_add_tab(ui_tabview, "Настройки");
      //обработчики событий для вкладок
      lv_obj_add_event_cb(ui_tabview, change_tab_event, LV_EVENT_ALL, NULL);//смена активной вкладки
  //Панель состояния
    //Иконка Wifi
    ui_wifistatus = lv_label_create(ui_tabview);
    lv_obj_align(ui_wifistatus, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(ui_wifistatus, LV_SYMBOL_WIFI);
    lv_obj_add_flag(ui_wifistatus, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_wifistatus, LV_OBJ_FLAG_HIDDEN);
    //IP адрес
    ui_status_ip = lv_label_create(ui_tabview);
    lv_obj_align(ui_status_ip, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(ui_status_ip, " ");
    lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_status_ip, LV_OBJ_FLAG_HIDDEN);
    //Иконка MQTT
    ui_mqttstatus = lv_label_create(ui_tabview);
    lv_obj_align(ui_mqttstatus, LV_ALIGN_TOP_RIGHT, -25, 0);
    lv_label_set_text(ui_mqttstatus, LV_SYMBOL_LOOP);
    lv_obj_add_flag(ui_mqttstatus, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_mqttstatus, LV_OBJ_FLAG_HIDDEN);
  //Вкладка кормления (ui_tab1)
    //Часы
    ui_clock = lv_label_create(ui_tab1); //часы
    lv_obj_align(ui_clock, LV_ALIGN_TOP_LEFT, 0, 5); //положение на экране
    lv_obj_set_size(ui_clock, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(ui_clock, &digital60, 0);
    //графика
    //бегущий кот
    img_running_cat = lv_gif_create(ui_tab1);
    lv_gif_set_src(img_running_cat, &runingcat_img);
    lv_obj_set_pos(img_running_cat, 0, 105);
    //миска с едой
    lv_obj_t * img_food_gif = lv_gif_create(ui_tab1);
    lv_gif_set_src(img_food_gif, &food_img);
    lv_obj_set_pos(img_food_gif, 195, 115);
    lv_obj_t * img_line;
    img_line = lv_line_create(ui_tab1);
    static lv_point_t line_points[] = { {0, 130}, {220, 130}};
    lv_line_set_points(img_line, line_points, 2);     /*Set the points*/

    //Кнопка кормления
    lv_obj_t * ui_feed_button = lv_btn_create(ui_tab1); // кнопка кормления  
    lv_obj_set_size(ui_feed_button, lv_pct(100), 40);
    lv_obj_align(ui_feed_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(ui_feed_button, event_feed, LV_EVENT_ALL, NULL); //обработчик нажатия кнопки
    lv_obj_t * ui_feed_btn_label=lv_label_create(ui_feed_button);//Надпись на кнопке
    lv_label_set_text(ui_feed_btn_label, "Выдать корм");
    lv_obj_center(ui_feed_btn_label);
    //Размер порции
    lv_obj_t * ui_slider_feed_amount = lv_slider_create(ui_tab1);
    lv_obj_set_size(ui_slider_feed_amount, lv_pct(90), 20);
    lv_slider_set_range(ui_slider_feed_amount, 50 , 250);
    lv_slider_set_value(ui_slider_feed_amount, feedAmount, LV_ANIM_OFF);
    lv_obj_align_to(ui_slider_feed_amount, ui_feed_button, LV_ALIGN_TOP_LEFT, 0, -45);
    lv_obj_add_event_cb(ui_slider_feed_amount, slider_feedamount_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
    ui_label_feedAmount = lv_label_create(ui_tab1);
    lv_label_set_text_fmt(ui_label_feedAmount,"%d", feedAmount);
    lv_obj_align_to(ui_label_feedAmount, ui_slider_feed_amount, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * ui_label_feed_text = lv_label_create(ui_tab1);
    lv_label_set_text(ui_label_feed_text, "Размер порции (50-250):");
    lv_obj_align_to(ui_label_feed_text, ui_slider_feed_amount, LV_ALIGN_TOP_LEFT, 0, -25);

    ui_remain = lv_label_create(ui_tab1);
    lv_label_set_text(ui_remain, " ");
    lv_obj_align_to(ui_remain, ui_label_feed_text, LV_ALIGN_TOP_LEFT, 0, -25);


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
      lv_obj_set_size(ui_timer1_hour,70,60);
      lv_obj_add_event_cb(ui_timer1_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
       ui_timer1_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer1_minute, 2);
      lv_roller_set_options(ui_timer1_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer1_minute, feedTime[0][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer1_minute,70,60);
      lv_obj_align_to(ui_timer1_minute, ui_timer1_hour, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer1_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer1_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer1_check, ui_timer1_minute, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
      lv_checkbox_set_text(ui_timer1_check, "");
      if (feedTime[0][2]==1) lv_obj_add_state(ui_timer1_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer1_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 2
      ui_timer2_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer2_hour, 2);
      lv_roller_set_options(ui_timer2_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer2_hour, feedTime[1][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer2_hour,70,60);
      lv_obj_align_to(ui_timer2_hour, ui_timer1_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer2_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer2_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer2_minute, 2);
      lv_roller_set_options(ui_timer2_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer2_minute, feedTime[1][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer2_minute,70,60);
      lv_obj_align_to(ui_timer2_minute,ui_timer2_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer2_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer2_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer2_check, ui_timer2_minute, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
      lv_checkbox_set_text(ui_timer2_check, "");
      if (feedTime[1][2]==1) lv_obj_add_state(ui_timer2_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer2_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 3
      ui_timer3_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer3_hour, 2);
      lv_roller_set_options(ui_timer3_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer3_hour, feedTime[2][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer3_hour,70,60);
      lv_obj_align_to(ui_timer3_hour, ui_timer2_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer3_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer3_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer3_minute, 2);
      lv_roller_set_options(ui_timer3_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer3_minute, feedTime[2][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer3_minute,70,60);
      lv_obj_align_to(ui_timer3_minute,ui_timer3_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer3_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer3_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer3_check, ui_timer3_minute, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
      lv_checkbox_set_text(ui_timer3_check, "");
      if (feedTime[2][2]==1) lv_obj_add_state(ui_timer3_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer3_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

      //Таймер 4
      ui_timer4_hour=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer4_hour, 2);
      lv_roller_set_options(ui_timer4_hour, rollhours, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer4_hour, feedTime[3][0], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer4_hour,70,60);
      lv_obj_align_to(ui_timer4_hour, ui_timer3_hour,  LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
      lv_obj_add_event_cb(ui_timer4_hour, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer4_minute=lv_roller_create(ui_tab2);
      lv_roller_set_visible_row_count(ui_timer4_minute, 2);
      lv_roller_set_options(ui_timer4_minute, rollminutes, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_selected(ui_timer4_minute, feedTime[3][1], LV_ANIM_OFF);
      lv_obj_set_size(ui_timer4_minute,70,60);
      lv_obj_align_to(ui_timer4_minute,ui_timer4_hour,  LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(ui_timer4_minute, alarm_roll_event_handler, LV_EVENT_ALL, NULL);
      ui_timer4_check = lv_checkbox_create(ui_tab2);
      lv_obj_align_to(ui_timer4_check, ui_timer4_minute, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
      lv_checkbox_set_text(ui_timer4_check, "");
      if (feedTime[3][2]==1) lv_obj_add_state(ui_timer4_check, LV_STATE_CHECKED);
      lv_obj_add_event_cb(ui_timer4_check, alarm_roll_event_handler, LV_EVENT_ALL, NULL);

  //Вкладка настроек
    //Груупируем настройки. Создаем панель для каждой группы
    //Панель настроек дисплея
    lv_obj_t * ui_set_panel_display = lv_obj_create(ui_tab3);
      //Название панели
      lv_obj_t * ui_set_panel_display_label = lv_label_create(ui_set_panel_display);//метка названия панели
      lv_obj_align(ui_set_panel_display_label, LV_ALIGN_TOP_MID, 0, 5); //Выравниваем по центру вверху
      lv_label_set_text (ui_set_panel_display_label,"Настройки дисплея");//Пишем текст метки
      //Надпись настройка яркости
      lv_obj_t * ui_set_panel_display_bright_label = lv_label_create(ui_set_panel_display);//метка названия панели
      lv_obj_align(ui_set_panel_display_bright_label, LV_ALIGN_TOP_LEFT, 0, 20); //Выравниваем по левому краю
      lv_label_set_text (ui_set_panel_display_bright_label,"Яркость дисплея");//Пишем текст метки
      //Слайдер изменения яркости
      lv_obj_t * ui_set_panel_display_bright_slider = lv_slider_create(ui_set_panel_display);
      lv_obj_set_width(ui_set_panel_display_bright_slider,lv_pct(100));
      lv_obj_align_to(ui_set_panel_display_bright_slider, ui_set_panel_display_bright_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
      lv_slider_set_range(ui_set_panel_display_bright_slider, 1 , 255);
      //Переключение светлой и темной темы
      lv_obj_t  * ui_set_display_theme_label = lv_label_create(ui_set_panel_display); //создаем объект заголовок
      lv_label_set_text(ui_set_display_theme_label, "Темная тема"); //текст
      lv_obj_align_to(ui_set_display_theme_label,ui_set_panel_display_bright_slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); //положение на экране
      //Переключатель светлой и тёмной темы
      lv_obj_t * ui_set_display_theme_switch = lv_switch_create(ui_set_panel_display);
      lv_obj_add_event_cb(ui_set_display_theme_switch, theme_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
      lv_obj_set_size(ui_set_display_theme_switch,32,16);
      lv_obj_align_to(ui_set_display_theme_switch, ui_set_display_theme_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0); //положение на экране
      
      if (theme){lv_obj_add_state(ui_set_display_theme_switch, LV_STATE_CHECKED); } else{lv_obj_clear_state(ui_set_display_theme_switch, LV_STATE_CHECKED);}
      
      lv_obj_set_size(ui_set_panel_display, lv_pct(100),LV_SIZE_CONTENT); 
    
    //Панель настроек часов
    lv_obj_t * ui_set_panel_time = lv_obj_create(ui_tab3);
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
      lv_obj_set_width(ui_set_panel_time_gmt_slider,lv_pct(100));
      lv_obj_align_to(ui_set_panel_time_gmt_slider, ui_set_panel_time_gmt_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
      lv_slider_set_range(ui_set_panel_time_gmt_slider, -12 , 14);
      lv_slider_set_value(ui_set_panel_time_gmt_slider, timezone, LV_ANIM_OFF);
    lv_obj_set_size(ui_set_panel_time, lv_pct(100),LV_SIZE_CONTENT);
    lv_obj_align_to(ui_set_panel_time, ui_set_panel_display, LV_ALIGN_OUT_BOTTOM_LEFT,0,5); 
  }