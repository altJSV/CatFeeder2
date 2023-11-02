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
        //Serial.println("Чтение файла конфигурации...");
        //loadConfiguration(filename);
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
  /*
  // копируем значения 
  const char * jspc_server_path = doc1["pcadress"] ;
  //const char * jstoken = doc1["yatoken"];
  const char * jsapi_key= doc1["weatherapi"];
  const char * jsqLocation= doc1["weathercity"];
  const char * jsSSID=doc1["ssid"];
  const char * jsPASS=doc1["pass"];
  usesensor= (bool)doc1["usesensor"];
  gmt= (int8_t)doc1["gmt"];
  refpcinterval= (uint32_t)doc1["refpcinterval"];
  refweatherinterval= (uint32_t)doc1["refweatherinterval"];
  refsensorinterval= (uint32_t)doc1["refsensorinterval"];
  bright_level = (uint8_t)doc1["bright_level"];
  daybegin=(uint8_t)doc1["day_begin"];
  dayend=(uint8_t)doc1["day_end"];
  photosensor= (bool)doc1["photosensor"];
  pc_server_path = jspc_server_path;
  ledindicator = (bool)doc1["ledindicator"];
  darktheme = (bool)doc1["darktheme"] | true;
  api_key = jsapi_key;
  qLocation  = jsqLocation;
  SSID=jsSSID;
  PASS=jsPASS;
  // Закрываем файл
  */
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

  // Пишем значения в документ
  /*doc1["pcadress"] = pc_server_path; //адрес сервера пк монитора
  doc1["refpcinterval"] = refpcinterval; //период обновления пк монитора
  doc1["weatherapi"] = api_key;//api ключ погоды
  doc1["weathercity"] = qLocation; //Местоположение
  doc1["refweatherinterval"] = refweatherinterval;//период обновления погоды
  doc1["gmt"] = gmt; //часовой пояс
  doc1["ledindicator"] = ledindicator;//состояние led индикатора
  doc1["darktheme"] = darktheme;//темная тема
  doc1["day_begin"] = daybegin;//начало дневного времени
  doc1["day_end"] = dayend;//конец дневного времени
  doc1["usesensor"] = usesensor; //использование сенсора bme
  doc1["refsensorinterval"] = refsensorinterval; //период обновления сенсора
  doc1["bright_level"] = bright_level;// яркость подсветки экрана
  doc1["ssid"] = SSID; //часовой пояс
  doc1["pass"] = PASS;//состояние led индикатора
  doc1["photosensor"] = photosensor; //автоматическая регулировка яркости подсветки
  */
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