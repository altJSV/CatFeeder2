//Инициализация MQTT
void MQTT_init(){  
  static uint32_t tmr = 0;
  client.setServer(mqtt_server.c_str(), mqtt_port);
  client.setCallback(MQTTcallback);
  byte tries=10;
  while (--tries && !client.connected()) 
  {   
     Serial.println("Подключение к MQTT брокеру...");
     if (client.connect("CatFeeder", mqtt_login.c_str(),mqtt_pass.c_str()))
       {
      Serial.println("Подключено");
      client.publish("CatFeeder/status","Ok");
      client.subscribe("CatFeeder/feed");
      lv_obj_clear_flag(ui_mqttstatus, LV_OBJ_FLAG_HIDDEN);
      }
    else
    {
      Serial.print("Ошибка подключения: ");
      Serial.println(client.state());
      lv_obj_add_flag(ui_mqttstatus, LV_OBJ_FLAG_HIDDEN);
      }
  }
  
   delay(2000);
   }

//Парсинг сообшений в MQTT топике по подписке
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Получено сообщение в топике: ");
  Serial.println(topic);
  Serial.print("Сообщение:");
  String message;
  String command;
  int param,param2;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  command=message.substring(0,4);
  
 
 if (command=="feed"){ feed();
                  client.publish("CatFeeder/feed","CMD OK");
 }
 if (command=="feam"){  param=message.substring(4).toInt();
                  feedAmount=param;
                  client.publish("CatFeeder/feed","CMD OK");
 }
 /*                 
 if (command=="alsw"){  param=message.substring(5,6).toInt();
                  param2=message.substring(7).toInt();
                  if (param>=0 and param<4 and param2>=0 and param2<2) feedTime[param][2]=param2;
                  client.publish("CatFeeder/feed","CMD OK");
                  } 
  */
}                   