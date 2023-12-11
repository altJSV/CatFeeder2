# CatFeeder 2
Полная переработка проекта [CatFeeder](https://github.com/altJSV/CatFeeder) под ESP32.

## Оглавление
* [1. Особенности проекта](#Особенности-проекта)
*  [1.1. Отличия от CatFeeder](#Отличия-от-CatFeeder)
* [2. Сборка проекта](#Сборка-проекта)
*  [2.1. Необходимые компоненты](#Необходимые-компоненты)
*  [2.2. Сборка на макетной плате](#Сборка-на-макетной-плате)
*  [2.3. Сборка основного модуля управления](#Сборка-основного-модуля-управления)
*  [2.4. Подключение шагового двигателя](#Подключение-шагового-двигателя)
*  [2.5. Подключение весов](#Подключение-весов)
* [3. Прошивка](#Прошивка)
*  [3.1. Сборка из исходников](#Сборка-из-исходников)
*  [3.1.1. Необходимые библиотеки](#Необходимые-библиотеки)
*  [3.1.2. Особенности использования библиотек](#Особенности-использования-библиотек)
*  [3.2. Прошивка бинарных файлов](#Прошивка-бинарных-файлов)
* [4. Сборка-корпуса](#Сборка-корпуса)
* [4.1. Основная часть](#Основная-часть)
*  [4.2. Шнек](#Шнек)
*  [4.3. Платформа весов](#Платформа-весов)
*  [4.4. Крышка с бункером для корма](#Крышка-с-бункером-для-корма)
*  [4.5. Регулируемые стойки](#Регулируемые-стойки)
* [5. Особенности использования](#Особенности-использования)
*  [5.1. Настройка весов](#Настройка-весов)
*  [5.2. Управление через Telegram](#Управление-через-Telegram)
*  [5.3. Управление по MQTT](#Управление-по-MQTT)
	

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
* Все управление и настройки теперь осуществляется с помощью сенсорного экрана, что намного повышает удобство работы.
* Также был полностью переработан веб интерфейс и добавлено управление через телеграмм бота.
* Работа через MQTT пока изменениям не подвергалась
* Все основные модули распаяны на макетной плате с колодками, для легкой замены.
* Шаговый двигатель теперь используется NEMA 17HS4023. По сравнению с предыдущим вариантом он более компактный и занимает гораздо меньше в корпусе. Мощность у него, конечно поменьше, но со своей задачей вращения шнека он полностью справляется.
* Полностью был переделан корпус. Как мне кажется, он стал более симпатичным и, надеюсь, поможет избавиться от проблемы застревания корма в подающем лотке.
* Также была полностью изменена логика выдачи корма. Теперь это не абстрактные цифры, а вполне конкретный вес в граммах. Для этого в корпус были интегрированы цифровые весы. Это позволяет не только точно отслеживать количество выдаваемого корма, но и своевременно реагировать на различные внештатные ситуации. Например "холостой ход двигателя" из-за застревания или окончания корма. Или же наоборот, подача корма в переполненную миску.

## Сборка проекта

### Необходимые компоненты
Для сборки проекта вам понадобятся следующие компоненты:
* Любая отладочная плата на базе ESP32. Я использовал [ESP32 WROOM 38 PIN](https://aliexpress.ru/item/1005004697343896.html?sku_id=12000032059705286&spm=a2g2w.productlist.search_results.0.47c4476dJO2bWr)
* Драйвер мотора MX1508. Например [такой](https://aliexpress.ru/item/1005001636421978.html?sku_id=12000016918368460&spm=a2g2w.productlist.search_results.0.392416baQ2WoLT)
* Шаговый двигатель [NEMA17 17HS4023](https://aliexpress.ru/item/10000232580902.html?sku_id=12000024948240502&spm=a2g2w.productlist.search_results.0.275638f2m4ucuK)
* Сенсорный дисплей [ILI9341 2.8'](https://aliexpress.ru/item/4000219159401.html?spm=a2g2w.cart.cart_split.2.2bbe4aa65r1ryO&sku_id=12000036224126576&_ga=2.99068254.1986242529.1701799552-786537644.1642186627) с разрешением 320x240 и драйвером тача XPT2046
* [Тензодатчик 5кг + драйвер HX711](https://aliexpress.ru/item/33046037411.html?spm=a2g2w.cart.cart_split.2.4d0d4aa6adNlcT&sku_id=67466940815&_ga=2.94864988.1986242529.1701799552-786537644.1642186627)
* Набор проводов для соединений
* [Макетная плата 50х70 мм](https://aliexpress.ru/item/32945724064.html?sku_id=66216201600&spm=a2g2w.productlist.search_results.0.a1b9405ff7bEMO)
* [Несколько разъемов hx2.54](https://aliexpress.ru/item/1005001479020815.html?sku_id=12000016290856136&spm=a2g2w.productlist.search_results.0.1dcd7338dVcV9S) или похожих для подключения проводов
* [2 безголовых винта M3x2mm](https://aliexpress.ru/item/1005005451331078.html?sku_id=12000033135491590&spm=a2g2w.productlist.search_results.3.24bb5c860kHUHe) 
* Качественный блок питания 5В 3А. Например [такой](https://www.ozon.ru/product/blok-pitaniya-5v-3a-micro-usb-dlya-raspberry-pi-3-i-pi-2-novaya-plata-422415562/) 

Далее, в соответствии со схемой, подключаем все компоненты.

### Сборка на макетной плате
Для сборки использовалась макетная плата 50х70 мм. Ниже описание процесса сборки с фото.
Для удобства сборки проект состоит из нескольких модулей.
* Основной модуль управления. На нем находится плата ESP32, Micro USB разъем для питания и разъемы для подключения остальных модулей.
* Модуль управления шаговым двигателем. Представляет собой плату MX1508 с распаянными колодками для подключения.
* Модуль контроллера весов. Представляет собой плату HX711 с распаянными колодками для подключения тензодатчика и ESP32.

#### Сборка основного модуля управления
На макетной плате распаиваются колодки для подключения ESP32 и разъемы hx2.54 для подключения дополнительных модулей. Также припаивается и Micro USB разъем для подключения питания.
![Распайка разъемов](/docs/solder1.jpg)
С подключенным ESP32 управляющая плата выглядит следующим образом.
![Подключение ESP32](/docs/solder2.jpg)
С обратной стороны платы, соединяются проводами контакты разъема ESP32 с разъемами на плате. Провода использовал из обрезка витой пары. Ниже одно из ранних фото процесса. Еще не все контакты распаяны.
![Вид сзади](/docs/solder3.jpg)
Для удобства, при пайке, можете сверяться со схемой ниже
![Схема пайки](/docs/board_pinout_ref.png)
Шлейфы, для подключения дополнительных модулей, делал из той же витой пары. На фото ниже шлейф дисплея.
![Шлейф дисплея](/docs/solder4.jpg)
После пайки платы, обязательно прозвоните все контакты мультиметром и изолируйте все места пайки. Паяльной маской, термоклеем или тем, что у вас есть под рукой.
Два разъема питания из четырех, пока никак не задействованы и были оставлены для возможных будущих модификаций.


### Подключение шагового двигателя

В качестве модуля управления шагового двигателя используется драйвер MX1508. Просто припаиваем к нему несколько разъемов JST 2.0. HX2.54, к сожалению, не подойдет из-за раздличий в расстоянии между монтажными отверстиями. Общий вид платы на фото:

![MX1508](/docs/MX1508.jpg)

Для привода шнека кормушки в движение используется шаговый двигаетель Nema17 17HS4023. Крутящего момента хватает, но лучше все же использовать Nema17 42HS34 или подобный.
Общая схема подключения к драйверу и микроконтроллеру на схеме ниже:
![Подключение шагового двигателя](/docs/stepper.png)
Потребляемый ток при максимальной нагрузке может достигать до 1.5А, поэтому необходимо подключение качественного блока питания. Я использовал блок питания 5В 3А. 

Также следует учитывать, что шаговый двигатель очень чувствителен к силе тока подаваемой на обмотки. При использовании 17HS4401 ток, выдаваемый драйвером (около 1.5А на обмотку), вполне достаточен. Но для 17HS4023 это все же многовато. Двигатель может работать неправильно. Пропуски шагов, нагрев и все в таком духе. Помогает установка резисторов 3.3 Ома в разрыв проводов идущих к двигателю. Номинал во многом еще зависит от типа и толщины используемых проводов, но на комплектных проводах шаговика этот номинал вполне достаточен.
Как вариант можно использовать 17HS08-1004S. По размерам он немного меньше, но крутящий момент почти такой же. И номинальный ток несколько выше, так что должно работать и без резисторов.

### Подключение весов
Подключение тензодатчика осуществляется через модуль HX711 по следующей схеме:
![Подключение тензодатчика](/docs/loadcell.png)


## Прошивка
Установка прошивки возможна двумя способами. С помощью сборки из исходников и с помощью предварительно скомпиллированных бинарных файлов.

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

## Сборка корпуса
Все основновные части корпуса распечатаны на 3Д принтере. 
Корпус состоит из 5 частей.
1. Основная часть
2. Шнек
3. Платформа весов
4. Крышка с бункером для корма
5. Регулируемые стойки

Общий рендер корпуса с установленными компонентами на скриншоте ниже:
![Общий вид](/docs/mainview.png)

### Основная часть
Служит для размещения всех компонентов устройства. Схема расположения электронных компонентов в корпусе на рендере ниже. 
![Крепление электронных компонентов](/docs/bodycomp.png)
Большая часть всех компонентов крепится винтами m3x3мм. Шаговый двигатель крепится двумя винтами М4x8мм

### Шнек
Служит для подачи корма в миску. Одевается на вал шагового двигателя с небольшим натягом. Для дополнительной фиксации используется 2 безголовых винта M3x2мм.
![Крепление шнека](/docs/screw.png)

### Платформа весов
Напрямую соединена с тензодатчиком и имеет крепления для размещения миски

### Крышка с бункером для корма
На ней размещен бункер для корма. Также обеспечивает доставку корма к подающему шнеку.

### Регулируемые стойки
Представляют собой 4 ножки с резьбой М8х1.25 для регулировки кормушки по высоте и выравнивания ее на плоскости

## Особенности использования
Данный раздел посвящен особенностям использования Cat Feeder 2. 

### OTA обновление прошивки
Устройство поддерживает обновление прошивки "По воздуху". Для его выполнения зайдите в веб интерфейс устройства. В нижней части страницы нажмите на кнопку "**Файловый менеджер**".
![OTA1](/docs/ota1.png)
Далее в разделе Обновление прошивки с помощью кнопки **Выберите файл** укажите путь к файлу прошивки и нажмите **Обновить** для его загрузки в устройство.
![OTA2](/docs/ota2.png).

### Настройка весов
Перед первым использованием весы кормушки необходимо откалибровать. Для этого подготовьте груз с заранее известным весом. После чего перейдите на вкладку **Настройки**.
Далее откройте раздел **Весы**. Вверху страницы находится информация о настроенном калибровочном коэффициенте. 
![Калибровка весов 1](/docs/scales_cal.jpg)
Нажмите кнопку **Калибровка весов**. 
Откроется новое окно.
![Калибровка весов 2](/docs/scales_cal2.jpg)
В нем установите вес груза и положите его на платформу весов. После чего нажмите кнопку **Калибровать**. Новый коэффициент будет расчитан и отобразится в нижней части окна. Закройте окно, для сохранения изменений.

### Управление через Telegram
Для управления кормушкой через Telegram бота прежде всего необходимо его создать воспользовашись [инструкцией](https://projectalt.ru/publ/arduino_esp8266_i_esp32/programmirovanie/upravlenie_esp8266_cherez_telegram_bota/11-1-0-38) и получить API ключ и ваш ChatID.
Далее, если вы собираете прошивку из исходников, измените значения переменныйх *String bot_token = "1234567890" и String chatID = "chat"* в файле secrets.h на полученные вами учетные данные.
Если вы прошили устройство с помощью предварительно скомпилированных бинарных файлов, то данные параметры можно изменить на вкладке "Настройки" в разделе Telegram.
Если все настроено корректно, то кормушка  при старте отправляет боту сообщение: "Cat Feeder 2 готов к работе!".
Управление устройством осуществляется отправкой в чат с ботом управляющих команд вида */команда параметр 1 параметр2 ...*

#### Полный список управляющих команд
> **/feed** - выдача одной порции корма с предустановленными параметрами
> **/info** - получение статусной информации о работе кормушки

### Управление по MQTT
Для управления кормушкой по протоколe MQTT необходимо выполнить ряд настроек. В файле CatFeeder.ino изменить значения следующих переменных:
`*String mqtt_server = "192.168.1.1"; //ip или http адрес
int mqtt_port = 1883; //порт*`
Подставьте в них адрес и порт используемого вами MQTT брокера.
В secrets.h хранятся учетные данные для подключения.

`String mqtt_login="login"; //логин
String mqtt_pass="pass"; //пароль`

Измените значения на ваши.
Также настройка MQTT подключения возможна и через веб интерфейс.
Если все настройки верны, то при успешном подключении к брокеру в статусной панели появится соответствующий значок.

#### Полный список управляющих команд
Для управления используется топик CatFeeder/feed

**feed** - выдача одной порции корма с предустановленными параметрами
