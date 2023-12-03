# CatFeeder 2
Полная перереработка проекта [CatFeeder](https://github.com/altJSV/CatFeeder) под ESP32.

## Оглавление
* [1. Особенности проекта](#Особенности-проекта)
*  [1.1. Отличия от CatFeeder](#Отличия-от-CatFeeder)
* [2. Сборка проекта](#Сборка-проекта)
*  [2.1 Необходимые компоненты](#Необходимые-компоненты)
*  [2.2 Сборка на макетной плате](#Сборка-на-макетной-плате)
*  [2.3 Подключение шагового двигателя](#Подключение-шагового-двигателя)
*  [2.4 Подключение весов](#Подключение-весов)
* [3. Прошивка](#Прошивка)
*  [3.1. Сборка из исходников](#Сборка-из-исходников)
*  [3.1.1 Необходимые библиотеки](#Необходимые-библиотеки)
*  [3.1.1 Особенности использования библиотек](#Особенности-использования-библиотек)
*  [3.2. Прошивка бинарных файлов](#Прошивка-бинарных-файлов)
* [4. Особенности использования](#Особенности-использования)
*  [4.1. Настройка весов](#Настройка-весов)
*  [4.2. Управление через Telegram](#Управление-через-Telegram)
*  [4.3. Управление по MQTT](#Управление-по-MQTT)
	

## Особенности проекта
* Цветной TFT экран с сенсорным управлением
* Часы с синхронизацией времени по NTP протоколу
* Настройки размера порции и времени кормления с 4 независимыми будильниками
* Точная настройка размера порции с помощью цифровых весов
* Проверка наличия корма в бункере и отслеживание забивания подающего лотка
* Уведомления в Telegram
* Управление по MQTT протколу для интеграции в систему умного дома
* Веб интерфейс
* Сохранение и запись настроек в файлы конфигурации

### Отличия от CatFeeder
* Кормушка построена на базе ESP32, что позволяет существенно расширить ее функционал.
* Для управления используется цветной сенсорный дисплей ILI9341.
* Интерфейс реализован с помощью библиотеки LVGL.
* Все управление и настройки теперь осуществляется с помощью сенсорного экран, что намного повышает удобство работы.
* Также был полностью переработан веб интерфейс и добавлено управление через телеграмм бота.
* Работа через MQTT пока изменениям не подвергалась
* Все основные модули распаяны на макетной плате с колодками, для легкой замены.
* Шаговый двигатель теперь используется NEMA 17HS4023. По сравнению с предыдущим вариантом он более компактный и занимает гораздо меньше в корпусе. Мощность у него, конечно поменьше, но со своей задачей вращения шнека он полностью справляется.
* Полностью был переделан корпус. Как мне кажется, он стал более симпатичным и, надеюсь, поможет избавиться от проблемы застревания корма в подающем лотке.
* Также была полностью изменена логика выдачи корма. Теперь это не абстрактные цифры, а вполне конкретный вес в граммах. Для этого в корпус были интегрированы цифровые весы. Это позволяет не только точно отслеживать количество выдаваемого корма, но и своевременно реагировать на различные внештатные ситуации. Например "холостой ход двигателя" из-за застревания или окончания корма. Или же наоборот, подача корма в переполненную миску.

## Сборка проекта

### Необходимые компоненты
Для сборки проекта вам понадобятся следующие компоненты:
* Любая отладочная плата на базе ESP32. Я использовал ESP32 WROOM 38 PIN
* Драйвер мотора MX1508
* Шаговый двигатель NEMA17 17HS4023
* Сенсорный дисплей ILI9341 с разрешением 320x240 и драйвером тача XPT2046
* Тензодатчик 5кг + драйвер HX711
* Набор проводов для соединений
* Макетная плата 50х70 мм
* Несколько разъемов hx2.54 или похожих для подключения проводов
* Источник питания минимум 5В 2А. Подойдет любая мощная USB зарядка от телефона

Далее, в соответствии со схемой, подключаем все компоненты.

### Сборка на макетной плате
Если кто-то решит собирать на макетной плате как я, то ниже несколько более подробное описание процесса сборки с фото.
Решил не распаивать все на одной плате и сделать проект модульным, чтобы в случае каких либо проблем, просто заменить неисправный модуль на новый.
Для начала, на макетке распаял все разъемы для подключения ESP32 и домполнительных модулей. Сразу же припаял USB разъем для подключения зарядного устройства.
![Распайка разъемов](/docs/solder1.jpg)
Со подключенным ESP32 управляющая плата выглядит следующим образом.
![Подключение ESP32](/docs/solder2.jpg)
С обратной стороны платы, соединил проводами из витой пары контакты разъема ESP32 с разъемами на плате. Ниже одно из ранних фото процесса. Еще не все контакты распаяны.
![Вид сзади](/docs/solder3.jpg)
Для удобства, при пайке, можете сверяться со схемой ниже
![Схема пайки](/docs/board_pinout_ref.png)
Шлейфы, для подключения дополнительных модулей, делал из той же витой пары. На фото ниже шлейф дисплея.
![Шлейф дисплея](/docs/solder4.jpg)
После пайки платы, обязательно прозвоните все контакты мультиметром и заизолируйте все места пайки. Паяльной маской, термоклеем или тем, что у вас есть под рукой.
Два разъема питания из четырех, пока никак не задействованы и были оставлены для возможных будущих модификаций.

### Подключение шагового двигателя
Для привода шнека кормушки в движение используется шаговый двигаетель Nema17 17HS4023. Крутящего момента хватает, но лучше все же использовать Nema17 42HS34 или подобный.
Общая схема подключения к драйверу и микроконтроллеру на схеме ниже:
![Подключение шагового двигателя](/docs/stepper.png)
Потребляемый ток при максимальной нагрузке может достигать до 1.5А, поэтому необходимо подключение качественного блока питания. USB зарядка на 2.1А с этим неплохо справляется. 

### Подключение весов
Подключение тензодатчика осуществляется через модуль HX711 по следующей схеме:
![Подключение тензодатчика](/docs/loadcell.png)


## Прошивка
Установка прошивки возможно как с помощью сборки из исходников так и с помощью предварительно скомпиллированных бинарных файлов.

### Сборка из исходников
Сборка и установка прошивки осуществляется в среде разработки Arduino IDE

#### Необходимые библиотеки
Для сборки проекта вам необходимо установить дополнительные библиотеки для Arduino IDE:
* LVGL - 8.3.10
* TFT_eSPI - 2.5.0 и выше
* GyverNTP - 1.3.1
* GyverTimer - 3.2
* PubSubClient от Nick O'Leary - 2.8
* ArduinoJSON - 6.21.3
* FastBot - 2.25
* GyverHX711 - 1.2

#### Особенности использования библиотек
В проекте используется стандартная библиотека **LVGL - 8.3.10**, но с некоторыми модификациями исходного кода. В папке проекта **extra** находится модифицированная версия библиотеки. Просто скопируйте содержимое архива **lvgl.7z** в папку с библиотеками Arduino, при необходимости подтвердив замену файлов.
Также в папке extra находится конфигурационный файл для библиотеки **TFT_eSPI** - *User_Setup.h*. Скопируйте его в папку **TFT_eSPI**, заменив исходный файл при необходимости.

### Прошивка бинарных файлов
Для прошивки вам понадобится официальная программа от Espresif [Flash Download Tools](https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.5.zip).
После запуска программы, на первом экране установите следующие настройки:

![Настройки Flash Download Tools](/docs/fdt1.png)

На втором экране укажите путь к бинарным файлам из архива проекта и их адреса во флеш памяти. Скорость SPI - *40Mhz*, режим записи - *DIO*, установить флажок -  *DoNotChgBin*. Для удобства можете сверяться со скриншотом ниже:

![Настройки Flash Download Tools 2](/docs/fdt2.png)

Значение COM выбирайте исходя из того, какой виртуальны порт присвоил драйвер USB-TTL вашему устройству.
Теперь необходимо полностью очистить соедержимое флеш памяти ESP32. Нажмите на кнопку *ERASE* и подключите ваш  ESP32 к компьютеру по USB с зажатой кнопкой BOOT.
Через некоторое время прогресс бар внизу окна прошивальщика начнет заполняться. После окончания процесса вам будет выведено уведомление **Finish**
Теперь можно установить и саму прошивку. Нажмите кнопку START и повторите весь процесс с подключением ESP32 по USB к компьютеру с зажатой кнопкой BOOT на модуле.
После полного заполнения шкалы прогресса и вывода уведомления **Finish** можно закрыть прошивальщик и отключить ESP32 от компьютера.

## Особенности использования
Данный раздел посвящен особенностям использования Cat Feeder 2. 

### Настройка весов
Перед первым использованием весы кормушки необходимо откалибровать. Для этого поставьте на поддон весов пустую миску для корма и перейдите на вкладку **Настройки**.
Далее откройте раздел **Весы**. Вверху страницы находится информация о настроенном весе. Нажмите кнопку **Калибровать**. Будет произведен контрольный замер веса пустой тары и установлен оффсет для последующих замеров. В дальнейшем вес тары учитываться при замерах не будет.

### Управление через Telegram
Для управления кормушкой через Telegram бота прежде всего необходимо его создать воспользовашись [инструкцией](https://projectalt.ru/publ/arduino_esp8266_i_esp32/programmirovanie/upravlenie_esp8266_cherez_telegram_bota/11-1-0-38) и получить API ключ и ваш ChatID.
Далее, если вы собираете прошивку из исходников, измените значения переменныйх *String bot_token = "1234567890" и String chatID = "chat"* в файле secrets.h на полученные вами учетные данные.
Если вы прошили устройство с помощью предварительно скомпилированных бинарных файлов, то данные параметры можно изменить на вкладке "Настройки" в разделе Telegram.
Если все настроено корректно, то кормушка  при старте отправляет боту сообщение: "Cat Feeder 2 готов к работе!".
Управление устройством осуществляется отправкой в чат с ботом управляющих команд вида */команда параметр 1 параметр2 ...*

#### Полный список управляющих команд
**/feed** - выдача одной порции корма с предустановленными параметрами
**/info** - получение статусной информации о работе кормушки

### Управление по MQTT
Для управления кормушкой по протоколe MQTT необходимо выполнить ряд настроек. В файле CatFeeder.ino изменить значения следующих переменных:
*String mqtt_server = "192.168.1.1"; //ip или http адрес
int mqtt_port = 1883; //порт*
Подставьте в них адрес и порт используемого вами MQTT брокера.
В secrets.h хранятся учетные данные для подключения.
String mqtt_login="login"; //логин
String mqtt_pass="pass"; //пароль
Измените значения на ваши.
Также настройка MQTT подключения возможна и через веб интерфейс.
Если все настройки верны, то при успешном подключении к брокеру в статусной панели появится соответствующий значок.

#### Полный список управляющих команд
Для управления используется топик CatFeeder/feed

**feed** - выдача одной порции корма с предустановленными параметрами
