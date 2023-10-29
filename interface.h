//объекты интерфейса
static lv_obj_t * btn1;
static lv_obj_t * screenMain;
static lv_obj_t * label;

//Функция обработки нажатия экранной кнопки
static void event_handler_btn(lv_event_t * event)
  {
  static uint32_t cnt = 1;
  lv_obj_t * btn = lv_event_get_target(event);
  lv_obj_t * label = lv_obj_get_child(btn, 0);
   lv_label_set_text_fmt(label, "%"LV_PRIu32, cnt);
   cnt++;
 }

void load_interface()
  {
    //Создаем экранные объекты
 lv_obj_t * screenMain = lv_obj_create(NULL); //создаем экранный объект, который будет содержать все другие объекты
//Создадим объект надпись и опишем его свойства
 label = lv_label_create(screenMain); //создаем объект Надпись как дочерний объект screenMain
 lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP); //текст можно переносить по строкам если не вмещается
 lv_label_set_text(label, "Press the button!"); //сам текст для надписи
 lv_obj_set_size(label, 240, 40); //размеры надписи
 lv_obj_set_pos(label, 0, 15); //положение на экране
//создадим объект Кнопка
 btn1 = lv_btn_create(screenMain); //создаем объект Кнопка как дочерний объект screenMain
 lv_obj_add_event_cb(btn1, event_handler_btn, LV_EVENT_CLICKED, NULL); //функция, которая вызывается при нажатии на кнопку
 lv_obj_set_width(btn1, 120); //ширина
 lv_obj_set_height(btn1, 30); //высота
 lv_obj_set_pos(btn1,10, 40); //положение
 //далее создадим надпись на кнопке
 lv_obj_t * label1 = lv_label_create(btn1); //создаем объект Надпись как дочерний объект созданной ранее кнопки
 lv_label_set_text(label1, "Press me!"); //Надпись на кнопке
// далее выводим все на экран
 lv_scr_load(screenMain);
  }