/*
Переименуйте данный файл в secrets.h и внесите изменения в соответсвии с вашими учетными данными
*/

//Настройки MQTT
String mqtt_login="login"; //логин
String mqtt_pass="pass"; //пароль
String clientID="CatFeeder2"; //ID клиента
String cmdTopic="CatFeeder/feed/"; //топик управления
String statusTopic="CatFeeder/status/"; //топик статуса
String mqtt_server = "192.168.1.1"; //ip или http адрес
int mqtt_port = 1883; //порт
//настройки telegram бота
String bot_token = "1234567890"; 
String chatID = "chat";