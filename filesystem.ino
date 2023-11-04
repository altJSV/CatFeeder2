bool fs_init()
{
  //Инициализация файловой системы
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("Ошибка монтирования SPIFFS");
        Serial.println("Форматирование SPIFFS...");
        bool formatted = SPIFFS.format();
        if(formatted){
        Serial.println("\n\nФорматирование окончено");
        }else{
       Serial.println("\n\nОшибка форматирования");
        }
        return false;
    }
    else 
      {
        Serial.println("Файловая система инициализирована");
        listDir(SPIFFS, "/", 0);
        return true;
      }
}

//чтение файла конфигурации
bool loadConfiguration(const char *filename) {
  //Открываем файл конфигурации
  File file = SPIFFS.open(filename);
  if(!file){
        Serial.println("Ошибка открытия файла");
        return false;
        }
  //выделяем память под json
  StaticJsonDocument<1024> doc1;

  // читаем json
  DeserializationError error = deserializeJson(doc1, file);
  if (error) {
              Serial.println(F("Ошибка чтения JSON. Загружена конфигурация по умолчанию"));
              file.close();
              return false;
              }
 
  // чтение значений 
  const char * mqtt_server_p= doc1["mqtt_server"];
  mqtt_port= (uint16_t)doc1["mqtt_port"];
  const char * mqtt_login_p= doc1["mqtt_login"];
  const char * mqtt_pass_p= doc1["mqtt_pass"];
  feedAmount = (uint8_t)doc1["feed_amount"];
  mqtt_server=mqtt_server_p;
  mqtt_login=mqtt_login_p;
  mqtt_pass=mqtt_pass_p;
  //массив данных будильника
  for (byte i = 0; i < 4; i++) 
  {
    feedTime[i][0]=(uint8_t)doc1["houralarm"+String(i)];
    feedTime[i][1]=(uint8_t)doc1["minutesalarm"+String(i)];
    feedTime[i][2]=(uint8_t)doc1["activealarm"+String(i)];
  }
  // Закрываем файл
  file.close();
  return true;
}

// сохраняем настройки в файл
bool saveConfiguration(const char *filename) 
{
  // Удаляем предыдущий файл конфигурации
  SPIFFS.remove(filename);

  // Открываем файл для чтения
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Ошибка создания файла"));
    return false;
  }

  //Выделяем память под JSON
  StaticJsonDocument<1024> doc1;
  doc1["mqtt_server"] = mqtt_server; //адрес брокера mqtt
  doc1["mqtt_port"] = mqtt_port; //порт mqtt брокера
  doc1["mqtt_login"] = mqtt_login;//mqtt логин
  doc1["mqtt_pass"] = mqtt_pass; //mqtt пароль
  doc1["feed_amount"] = feedAmount;// размер порции
  doc1["timezone"] = timezone; //часовой пояс
  //массив данных будильника
  for (byte i = 0; i < 4; i++) 
  {
    doc1["houralarm"+String(i)]=feedTime[i][0];
    doc1["minutesalarm"+String(i)]=feedTime[i][1];
    doc1["activealarm"+String(i)]=feedTime[i][2];
  }
    
  // Сохраняем JSON в файл
  if (serializeJson(doc1, file) == 0) {
    Serial.println(F("Ошибка записи в файл"));
    file.close();
    return false;
  }
  else
  {
    Serial.println("Файл записан!");
    file.close();
    return true;
  }  
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Содержимое папки: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Ошибка открытия папки");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("это не папка");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  ПАПКА : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  ФАЙЛ: ");
      Serial.print(file.name());
      Serial.print("  РАЗМЕР: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Создание папки: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Папка создана");
  } else {
    Serial.println("Ошибка создания папки");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Удаление папки: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Папка удалена");
  } else {
    Serial.println("Ошибка удаления папки");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Чтение файла: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Ошибка открытия файла для чтения");
    return;
  }

  Serial.print("Чтение из файла: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Запись файла: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Ошибка открытия файла для записи");
    return;
  }
  if(file.print(message)){
    Serial.println("Файл записан");
  } else {
    Serial.println("Ошибка записи файла");
  }
  file.close();
}