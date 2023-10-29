
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
  }