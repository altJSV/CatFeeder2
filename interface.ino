
//Функция обработки нажатия экранной кнопки
static void event_handler_btn(lv_event_t * event)
  {
  static uint32_t cnt = 1;
  lv_obj_t * btn = lv_event_get_target(event);
  lv_obj_t * label = lv_obj_get_child(btn, 0);
   lv_label_set_text_fmt(label, "%"LV_PRIu32, cnt);
   cnt++;
 }

//Смена активной вкладки
 static void change_tab_event(lv_event_t * e)
  {
    uint16_t acttab=lv_tabview_get_tab_act(ui_tabview);
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) 
    {
       
    }
  }

//Измеение значения размера порции на экране
   static void slider_feedamount_event_cb(lv_event_t * e)
    {
      lv_obj_t * slider = lv_event_get_target(e);
      feedAmount=(int)lv_slider_get_value(slider);
      lv_label_set_text_fmt(ui_label_feedAmount,"%d", feedAmount);
    }

void draw_interface()
  {
  //Создаем экранные объекты
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
  //Вкладка кормления (ui_tab1)
    ui_clock = lv_label_create(ui_tab1); //часы
    lv_obj_align(ui_clock, LV_ALIGN_TOP_LEFT, 0, 0); //положение на экране
    lv_obj_set_size(ui_clock, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(ui_clock, &digital, 0);
    //Кнопка кормления
    lv_obj_t * ui_feed_button = lv_btn_create(ui_tab1); // кнопка кормления  
    lv_obj_set_size(ui_feed_button, lv_pct(100), 40);
    lv_obj_align(ui_feed_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
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

  }