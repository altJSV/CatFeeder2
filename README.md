# CatFeeder 2
![Лого](/docs/logo.png)

Полная переработка проекта [CatFeeder](https://github.com/altJSV/CatFeeder) под ESP32.

## Оглавление
* [1. Особенности проекта](#Особенности-проекта)
*  [1.1. Отличия от CatFeeder](#Отличия-от-CatFeeder)
* [2. Сборка проекта](#Сборка-проекта)
*  [2.1. Необходимые компоненты](#Необходимые-компоненты)
*  [2.2. Сборка на макетной плате](#Сборка-на-макетной-плате)
*  [2.3. Сборка основного модуля управления](#Сборка-основного-модуля-управления)
*  [2.4. Подключение шагового двигателя](#Подключение-шагового-двигателя)
*   [2.4.1. Подключение драйвера MX1508](#Подключение-драйвера-MX1508)
*   [2.4.2. Подключение драйвера A4988](#Подключение-драйвера-A4988)
*  [2.5. Подключение весов](#Подключение-весов)
* [3. Прошивка](#Прошивка)
*  [3.1. Сборка из исходников](#Сборка-из-исходников)
*  [3.1.1. Необходимые библиотеки](#Необходимые-библиотеки)
*  [3.1.2. Особенности использования библиотек](#Особенности-использования-библиотек)
*  [3.2. Прошивка бинарных файлов](#Прошивка-бинарных-файлов)
* [4. Сборка-корпуса](#Сборка-корпуса)
* [4.1. Основная часть](#Основная-часть)
*  [4.2. Шнек](#Шнек)
*  [4.3. Миска](#Миска)
*  [4.4. Крышка с бункером для корма](#Крышка-с-бункером-для-корма)
*  [4.5. Регулируемые стойки](#Регулируемые-стойки)
* [5. Особенности использования](#Особенности-использования)
* [5.1. Первый запуск](#Первый-запуск)
*  [5.2. Управление с сенсорного экрана](#Управление-с-сенсорного-экрана)
*  [5.3. Веб интерфейс](#Веб-интерфейс)
*  [5.4. Настройка весов](#Настройка-весов)
*  [5.5. Управление через Telegram](#Управление-через-Telegram)
*  [5.6. Управление по MQTT](#Управление-по-MQTT)
	

## Особенности проекта
* Цветной TFT экран с сенсорным управлением
* Часы с синхронизацией времени по NTP протоколу
* Отображение температуры и влажности в помещении
* Настройки размера порции и времени кормления с 4 независимыми будильниками
* Точная настройка размера порции с помощью цифровых весов
* Калибровка цифровых весов в зависимости от температуры в помещении
* Проверка наличия корма в бункере и отслеживание забивания подающего лотка
* Уведомления и настройка некоторых параметров в Telegram
* OTA обновление прошивки как через веб интерфейс, так и через Телеграм бота
* Управление по MQTT протколу для интеграции в систему умного дома
* Веб интерфейс
* Сохранение и запись настроек в файлы конфигурации

### Отличия от CatFeeder
* Кормушка построена на базе ESP32, что позволяет существенно расширить ее функционал.
* Для управления используется цветной сенсорный дисплей ILI9341.
* Интерфейс реализован с помощью библиотеки LVGL.
* Все управление и настройки теперь осуществляется с помощью сенсорного экрана, что намного повышает удобство работы.
* Также был полностью переработан веб интерфейс и добавлено управление через телеграмм бота.
* Полностью переписано OTA обновление прошивки
* Работа через MQTT пока изменениям не подвергалась
* Все основные модули распаяны на макетной плате с колодками, для легкой замены.
* Шаговый двигатель теперь используется NEMA 17HS4023. По сравнению с предыдущим вариантом он более компактный и занимает гораздо меньше в корпусе. Мощность у него, конечно поменьше, но со своей задачей вращения шнека он полностью справляется.
* Полностью был переделан корпус. Как мне кажется, он стал более симпатичным и, надеюсь, поможет избавиться от проблемы застревания корма в подающем лотке.
* Также была полностью изменена логика выдачи корма. Теперь это не абстрактные цифры, а вполне конкретный вес в граммах. Для этого в корпус были интегрированы цифровые весы. Это позволяет не только точно отслеживать количество выдаваемого корма, но и своевременно реагировать на различные внештатные ситуации. Например "холостой ход двигателя" из-за застревания или окончания корма. Или же наоборот, подача корма в переполненную миску.

## Сборка проекта

### Необходимые компоненты
Для сборки проекта вам понадобятся следующие компоненты:
* Любая отладочная плата на базе ESP32. Я использовал [ESP32 WROOM 38 PIN](https://aliexpress.ru/item/1005004697343896.html?sku_id=12000032059705286&spm=a2g2w.productlist.search_results.0.47c4476dJO2bWr)
* Драйвер мотора MX1508. Например [такой](https://aliexpress.ru/item/1005001636421978.html?sku_id=12000016918368460&spm=a2g2w.productlist.search_results.0.392416baQ2WoLT) или [драйвер шагового двигателя A4988](https://www.ozon.ru/product/a4988-stepper-motor-driver-with-ext-board-drayver-shagovogo-dvigatelya-s-platoy-rasshireniya-688838720/)
* [Повышающий преобразователь напряжения MT3608](https://www.ozon.ru/product/povyshayushchiy-preobrazovatel-napryazheniya-dc-dc-gsmin-mt3608-2a-vhod-2-24v-vyhod-5-28v-siniy-799440534/) - только при использовании драйвера A4988
* Шаговый двигатель [NEMA17 17HS4023](https://aliexpress.ru/item/10000232580902.html?sku_id=12000024948240502&spm=a2g2w.productlist.search_results.0.275638f2m4ucuK)
* Сенсорный дисплей [ILI9341 2.8'](https://aliexpress.ru/item/4000219159401.html?spm=a2g2w.cart.cart_split.2.2bbe4aa65r1ryO&sku_id=12000036224126576&_ga=2.99068254.1986242529.1701799552-786537644.1642186627) с разрешением 320x240 и драйвером тача XPT2046
* [Тензодатчик 5кг + драйвер HX711](https://aliexpress.ru/item/33046037411.html?spm=a2g2w.cart.cart_split.2.4d0d4aa6adNlcT&sku_id=67466940815&_ga=2.94864988.1986242529.1701799552-786537644.1642186627)
* Набор проводов для соединений
* [Макетная плата 50х70 мм](https://aliexpress.ru/item/32945724064.html?sku_id=66216201600&spm=a2g2w.productlist.search_results.0.a1b9405ff7bEMO)
* [Несколько разъемов hx2.54](https://aliexpress.ru/item/1005001479020815.html?sku_id=12000016290856136&spm=a2g2w.productlist.search_results.0.1dcd7338dVcV9S) или похожих для подключения проводов
* [2 безголовых винта M3x2mm](https://aliexpress.ru/item/1005005451331078.html?sku_id=12000033135491590&spm=a2g2w.productlist.search_results.3.24bb5c860kHUHe) 
* [Модуль DHT22](https://aliexpress.ru/item/33037061522.html?spm=a2g2w.cart.cart_split.2.146a4aa6KI8d1V&sku_id=12000032670222589&_ga=2.182953190.1322727573.1702322662-786537644.1642186627)
* Качественный блок питания 5В 3А. Например [такой](https://www.ozon.ru/product/blok-pitaniya-5v-3a-micro-usb-dlya-raspberry-pi-3-i-pi-2-novaya-plata-422415562/) 
* Пластиковый квадратный пищевой контейнер с шириной основания 96 мм. Например [такой](https://www.ozon.ru/product/konteyner-vakuumnyy-dlya-edy-dd-style-hranenie-1200-ml-1-sht-994520444/?avtc=1&avte=2&avts=1702385033)

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

Модуль DHT22 распаян прямо на плате.
Шлейфы, для подключения дополнительных модулей, делал из той же витой пары. На фото ниже шлейф дисплея.
![Шлейф дисплея](/docs/solder4.jpg)

После пайки платы, обязательно прозвоните все контакты мультиметром и изолируйте все места пайки. Паяльной маской, термоклеем или тем, что у вас есть под рукой.
Два разъема питания из четырех, пока никак не задействованы и были оставлены для возможных будущих модификаций.


### Подключение шагового двигателя

Для привода шнека кормушки в движение используется шаговый двигаетель Nema17 17HS4023. Крутящего момента хватает, но лучше все же использовать Nema17 42HS34 или подобный.

Потребляемый ток при максимальной нагрузке может достигать до 1.5А, поэтому необходимо подключение качественного блока питания. Я использовал блок питания 5В 3А. 

Для управления шаговым двигателем используется специальная плата-драйвер. Прошивка поддерживает следующие типы драйверов:
* MX1508 - самый дешевый, но вместе с тем самый ненадежный вариант. Его главное достоинство - дешевизна. Также драйвер и мотор можно питать от 5 вольт. Из недостатков мотор шумит, иногда довольно ощутимо греется, снижен курутящий момент
* А4988 - чуть более дорогой вариант, но гораздо более лучший по всем параметрам. Крутящий момент максимальный, мотор практически не греется, возможна точная настройка шагов, регулировка опорного напряжения двигателя и многое другое. Из недостатков это необходимость установки дополнительного повышающего трансформатора на 9 вольт для питания мотора, гораздо более высокая цена. И двигатель при работе все еще ощутимо шумит. Как вариант можно установить драйвер TMC2208. С ним мотор работает ощутимо тише, но и драйвер сильно дороже.

Выбор способа подключения драйвера осуществляется в прошивке в файле CatFeeder2.ino заменой значения дефайна:
```
//Выбор типа драйвера
#define STEPDRVTYPE 1 //0-MX1508,  1 - A4988
```
По умолчанию используется драйвер А4988.

#### Подключение драйвера MX1508
Для подключения драйвера MX1508 просто припаиваем к нему несколько разъемов JST 2.0. HX2.54, к сожалению, не подойдет из-за раздличий в расстоянии между монтажными отверстиями. Общий вид платы на фото:

![MX1508](/docs/MX1508.jpg)

Общая схема подключения к драйверу и микроконтроллеру на схеме ниже:
![Подключение шагового двигателя](/docs/stepper.png)

Также следует учитывать, что при использовании этого драйвера, шаговый двигатель очень чувствителен к силе тока подаваемой на обмотки. При использовании 17HS4401 ток, выдаваемый драйвером (около 1.5А на обмотку), вполне достаточен. Но для 17HS4023 это все же многовато. Двигатель может работать неправильно. Пропуски шагов, нагрев и все в таком духе. Помогает установка резисторов 3.3 Ома в разрыв проводов идущих к двигателю. Номинал во многом еще зависит от типа и толщины используемых проводов, но на комплектных проводах шаговика этот номинал вполне достаточен.
Как вариант можно использовать 17HS08-1004S. По размерам он немного меньше, но крутящий момент почти такой же. И номинальный ток несколько выше, так что должно работать и без резисторов.

#### Подключение драйвера A4988

Для более удобного подключения, рекомендую использовать плату расширения. Общий вид платы и драйвера на фото ниже:
![A4988](/docs/a4988.png)

Диаграмма подключения на схеме ниже:
![A4988connection](/docs/a4988connect.png)

Значение всех переключателей на плате расширения установить в положение OFF. Питание драйвера 5 вольт - можно взять с разъема на основной плате. Питание двигателя, также берется из разъема основной платы 5в и подключается через MT3608, установленным на 9 вольт.
Пины на плате расширения подключать к ESP32 по следующей схеме:
* **Step - GPIO35**
* **Dir - GPIO36**
* **EN - GPIO25**

Также при использовании шагового двигателя 17HS4023, необходимо установить опорное напряжение 0.7В с помощью подстроечного резистора на плате драйвера. Для этого включите устройство в сеть и установите плюсовой щуп мультиметра прямо на винт подстроечного резистора, минусовой на массу, и измерьте напряжение. Далее, с помощью мелкой крестовой отвертки, немного покрутите винт резистора до установки требуемого напряжения. Для 17HS4023 опорное напряжение должно быть около 1.5 вольта.


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

Также необходимо установить следующие настройки компилятора:
![Настройки компилятора](/docs/compiletset.png)

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
Все основновные части корпуса распечатаны на 3Д принтере. Модели для печати расположены в папке **3dmodel**. Также в ней расположены исходники для Fusion360.

Корпус состоит из 5 частей.
1. Основная часть
2. Шнек
3. Миска
4. Крышка с бункером для корма
5. Регулируемые стойки

Общий рендер корпуса с установленными компонентами на скриншоте ниже:
![Общий вид](/docs/mainview.png)

### Основная часть
Служит для размещения всех компонентов устройства. Схема расположения электронных компонентов в корпусе на рендере ниже. 
![Крепление электронных компонентов](/docs/bodycomp.png)

Большая часть всех компонентов крепится винтами m3x3мм. Шаговый двигатель крепится двумя винтами М4x8мм.
**Основные параметры печати:**
* Пластик: PETG
* Высота слоя: 0.2 мм
* Температура сопла: 230
* Температура стола: 75
* Плотность заполнения 20%
* Поддержки: везде

### Шнек
Служит для подачи корма в миску. Одевается на вал шагового двигателя с небольшим натягом. 
![Шнек](/docs/screwrend.png)

Для дополнительной фиксации используется 2 безголовых винта M3x2мм.
![Крепление шнека](/docs/screw.png)

**Основные параметры печати:**
* Пластик: PETG
* Высота слоя: 0.1 мм
* Температура сопла: 230
* Температура стола: 75
* Плотность заполнения 100%
* Поддержки: нет

### Миска
![Миска](/docs/plow.png)

Соединяется с тензодатчиком с помощью 2 винтов М4x16. Затем в сборе через технологическое отверстие вставляется в корпус. После чего другая сторона тензодатчика фиксируется двумя винтами М5х16, вставленными через отверстия внизу корпуса.
**Основные параметры печати:**
* Пластик: PETG
* Высота слоя: 0.2 мм
* Температура сопла: 230
* Температура стола: 75
* Плотность заполнения 20%
* Поддержки: от стола

### Крышка с бункером для корма
![Крышка](/docs/cover.png)

На ней размещен бункер для корма. Также обеспечивает доставку корма к подающему шнеку. В качестве бункера для корма используется квадратный пищевой контейнер из оргстекла с основанием 96 мм из списка компонентов. В нижней его части выерезается отверстие для подачи корма. Для установки в крышку, в ней предусмотрено отверстие с упорами по углам. Потом контейнер дополнительно фиксируется термоклеем по углам.
Для установки крышки в корпус на ней предумострены направляющиев в врехней части. Дополнительная фиксация не требуется.
**Основные параметры печати:**
* Пластик: PETG
* Высота слоя: 0.2 мм
* Температура сопла: 230
* Температура стола: 75
* Плотность заполнения 20%
* Поддержки: от стола


### Регулируемые стойки
![Стойки](/docs/stands.png)

Представляют собой 4 ножки с резьбой М8х1.25 для регулировки кормушки по высоте и выравнивания ее на плоскости.В нижней части корпуса под них предусмотрены соответствующие отверстия с резьбой.
**Основные параметры печати:**
* Пластик: PETG
* Высота слоя: 0.1 мм
* Температура сопла: 230
* Температура стола: 75
* Плотность заполнения 100%
* Поддержки: нет

Фото первого рабочего прототипа представлено на фото ниже. На нем еще присутствуют некоторые недостатки, которые были исправлены в финальной версии.
![Общий вид](/docs/fullview.jpeg)

## Особенности использования
Данный раздел посвящен особенностям использования Cat Feeder 2. 

### Первый запуск
После сборки и загрузки прошивки, при первом запуске устройства, происходит процесс инициализации. Условно его можно разделить на несколько этапов:

1. Форматирование файловой системы и создание конфигурационных файлов;
2. Калибровка сенсорного экрана;
3. Настройка подключения к WiFi;
4. Запуск всех основных сервисов и настройка параметров работы;
5. Переход в рабочий режим;

Разберем каждый из этапов более подробно. На первом этапе проверяется наличие раздела ФС для хранения конфигурационных файлов. Если раздел обнаружен, то производится его форматирование в файловую систему SPIFFS и устройство перезагружается.

На следующем этапе проверяется наличие в файловой системе устройства файла TouchCalData, с данными калибровки сенсорного дисплея. При его отсутсвии или чтении некорректных значений, инициализируется процесс калибровки экрана. В разных углах дисплея последовательно будут появляться стрелки. Вам необходимо касаться стилусом острия каждой из них. Как только процесс будет закончен, в файловой системе создается файл TouchCalData, с данными калибровки сенсорного дисплея, и устройство перезагружается.

Далее происходит настройка подключения к WiFi. Проверяется наличие учетных данных для подключения к сети в специальном разделе памяти устройства. Если они отсутсвуют, то создается точка доступа "CatFeeder2", к которой вам необходимо подключится по Wifi.На экране при этом не будет отображено никакой информации. После подключения к точке доступа по WiFi, перейдите в браузере по адресу http://192.168.4.1 и, в открывшемся веб интерфейсе, введите ваши учетные данные. После нажатия кнопи **Save**, устройство в очередной раз перезагрузится.

Далее, при следующем запуске, производится попытка запуска всех основных сервисов, таких как: веб интерфейс, mqtt клиент, телеграм бот, служба синхронизации времени, сенсорный интерфейс экрана. Если вы собирали прошивку из исходников, то учетные данные для mqtt и telegram, автоматически загрузятся из конфигурационного файла secrets.h. Если же вы загружали прошивку через предварительно скомпилированный бинарный файл, то вам необходимо настроить эти параметры через веб или сенсорный интерфейс. Также необходимо откалибровать платформу весов, настроить яркость дисплея, таймеры, шаги двигателя и прочие параметры. Все изменения автоматически сохраняются в конфигурационном файле config.json в файловой системе устройства. Далее они будут применяться при каждом перезапуске.

И, наконец, кормушка переходит в рабочий режим. Можно попробовать выдать порцию корма, нажав кнопку **Покормить** на главном экране. Если все прошло успешно, то устройство готово к работе и далее будет действовать в атоматическом режиме.

### Управление с сенсорного экрана
Сенсорный интерфейс управления устройством разделен на 3 основные вкладки:
* Главная
* Вкладка будильников
* Вкладка настроек
Перемещение по ним осуществляется либо тапом по имени вкладки в нижней части экрана, либо свайпами влево или вправо.
Фото главного экрана представлено на фото ниже:
![MainScreen](/docs/main.png)

В верхней части экрана расположена статусная строка. В ней отображаются: IP адрес, присвоенный устройству, а также индикаторы состояния подключения к телеграм боту, mqtt брокеру и сети wifi.
Ниже расположена информационная панель с отображением текущего времени, полученного c ntp сервера, время оставшееся до начала кормления, температура и влажность воздуха в помещении.
Еще ниже расположен слайдер размера порции. Он может принимать значение от 1 до 60 грамм. Под ним находится кнопка для немедленной выдачи установленного колличества корма. Тапом по иконке шагового мотора, справа от часов, осуществляется переход в окно настроек шагового двигателя.
![Stepmotor](/docs/stepmotor.png)

В данном окне производится тонкая настройка двигателя.
Для предотвращения застревания корма, двигатель вращает шнек на несколько шагов вперед, потом немного назад. В окне настраивается колличество этих шагов, а также скорость вращения двигателя. Полный оборот двигатель совершает за 200 шагов. Поэтому слайдеры работают в этом пределе. Также в нижней части окна, расположена кнопка тестового запуска двигателя для проверки настроек.
Тапом по вкладке **Таймеры** можно перейти в меню настройки будильников.
![alarms](/docs/alarms.png)

Всего для редактирования доступны 4 будильника. В каждом из которых вы можете установить время срабатывания и колличество выдаваемого корма. Также вы можете включить или выключить любой из таймеров.
Тап по вкладке **Настройки** открывает доступ к различным параметрам устройства. Для удобства они разделены на несколько категорий.
В категории **Настройки дисплея** находится все что касается экрана устройства.
![screenset](/docs/screenset.png)

В частности здесь вы можете откалибровать сенсорную панель, настроить яркость подсветки и изменить тему оформления.
Светлая тема оформления представлена на фото ниже:
![lighttheme](/docs/lighttheme.png)

Следующая категория **Настройки времени**.
![timeset](/docs/timeset.png)

В ней пока присутствует лишт один параметр. Настройка часового пояса для синхронизации времени.
Далее идет **Калибровка весов**.
![scaleset](/docs/scaleset.png)

В ней можно выполнить калибровку весовой платформы. Подробнее об этом в главе [Настройка весов](#Настройка-весов).
Следующая категория **Настройки Telegram бота**.
![tg](/docs/tg.png)

В ней вы можете включить или отключить использование бота и настроить различные параметры подключения.
И последняя категория **Настройки MQTT**.
![mqtt](/docs/mqtt.png)

В ней устанавливаются различные параметры подключения к MQTT брокеру.



### Веб интерфейс
Для доступа к вебинтерфейсу кормушки просто введите в адресной строке браузера следующий текст: **https://catfeeder2.local**. Если по какой-либо причине данный способ не работает, то перейти в веб интерфейс можно введя IP адрес кормушки в адресную строку браузера. Узнать его можно посмотрев в левый верхний угол экрана устройства.
Общий вид веб интерфейса представлен на скриншоте ниже:

![Web1](/docs/web1.png)

На данном экране вы можете:
* Выдать порцию корма заданного веса;
* Настроить параметры шагового двигателя;
* Посмотреть и отредактировать все установленные будильники;
* Настроить параметры подключения к MQTT брокеру;
* Настроить параметры подключения к Telegram боту;
* Перейти на экран работы с файловой системой;

**Обратите внимание**, что значения всех параметров сохраняются только после нажатия кнопки **Отправить**!

При нажатии на кнопку **Файловый менеджер** откроется новый экран работы с файловой системой устройства.

![Web2](/docs/web2.png)

На данном экране вы можете:
* Загрузить прошивку в устройство;
* Загрузить файл в файловую систему;
* Просмотреть содержимое памяти устройства и отредактировать/удалить файлы;
* Просмотреть лог работы устройства;
* Перезагрузить активную страницу;
* Вернуться на главную страницу;

### OTA обновление прошивки
Устройство поддерживает обновление прошивки "По воздуху". Для его выполнения зайдите в веб интерфейс устройства. В нижней части страницы нажмите на кнопку "**Файловый менеджер**".
![OTA1](/docs/ota1.png)

Далее в разделе Обновление прошивки с помощью кнопки **Выберите файл** укажите путь к файлу прошивки и нажмите **Обновить** для его загрузки в устройство.
![OTA2](/docs/ota2.png).

Также поддерживается обновление прошивки и через Телеграм бота. Достаточно, лишь в диалоге с ним отправить ему скомпилированный бинарный файл.

### Настройка весов
Перед первым использованием весы кормушки необходимо откалибровать. Перед калибровкой желательно дать поработать кормушке 5-10 минут в дежурном режиме. Это связано с тем, что HX711, сразу после запуска выдает несколько завышенные показания. Из-за этого показатели веса на платформе постоянно плавают. Примерно минут через 5, все приходит в норму и выдаются примерно одинаковые данные. С чем это связано, я не знаю. Возможно брак конкретно моего модуля. 
Далее для калибровки подготовьте груз с заранее известным весом. После чего перейдите на вкладку **Настройки**.
Далее откройте раздел **Весы**. Вверху страницы находится информация о настроенном калибровочном коэффициенте. 
![Калибровка весов 1](/docs/scales_cal.jpg)

Нажмите кнопку **Калибровка весов**. 
Откроется новое окно.
![Калибровка весов 2](/docs/scales_cal2.jpg)

В нем установите вес груза и положите его на платформу весов. После чего нажмите кнопку **Калибровать**. Новый коэффициент будет расчитан и отобразится в нижней части окна. 
При калибровке также запоминается значение текущей температуры в помещении. Далее значения полученные с весов будут корректироваться в зависимости от температуры в помещении.
Закройте окно, для сохранения изменений.

### Управление через Telegram
Для управления кормушкой через Telegram бота прежде всего необходимо его создать воспользовашись [инструкцией](https://projectalt.ru/publ/arduino_esp8266_i_esp32/programmirovanie/upravlenie_esp8266_cherez_telegram_bota/11-1-0-38) и получить API ключ и ваш ChatID.
Далее, если вы собираете прошивку из исходников, измените значения переменныйх *String bot_token = "1234567890" и String chatID = "chat"* в файле secrets.h на полученные вами учетные данные.
Если вы прошили устройство с помощью предварительно скомпилированных бинарных файлов, то данные параметры можно изменить на вкладке "Настройки" в разделе Telegram.
Если все настроено корректно, то кормушка  при старте отправляет боту сообщение: "Cat Feeder 2 готов к работе!".
Также поддерживается OTA обновление прошивки через бота. Достаточно в диалоге отправить ему через вложение скомпилированный бинарный файл с прошивкой.

#### Полный список управляющих команд
Команда без параметра включает или отключает функцию. С параметром - устанавливает значение или выполняет действие. 
* **/feed** - выдача одной порции корма. (Если указан параметр, то выдать определенное колличество 1-60 гр). Например **/feed 40**.
* **/info** - получение статусной информации о работе кормушки
* **/lastfeed** - время последнего кормления и размер порции
* **/temp** - температура и влажность
* **/mqtt** - включение/отключение MQTT
* **/brightlevel** - установить яркость подсветки экрана (параметр 0-255)
* **/daytime** - без параметра, включение и отключение снижения яркости экрана. Параметр вида hn:he установка дневного времени. hn-начало, he-конец
* **/restart** - перезагрузка устройства
* **/commandlist** - справка по командам

Следующие команды без параметра включают или отключают будильники. С параметром устанавливают его значение и сохраняют в память
Параметр вида **hh:mm** устанавливает время. Например, **/alarm0 11:20**. Параметр val размер порции в граммах. Например, **/alarm0 40**.
* **/alarm0** - Будильник 0
* **/alarm1** - Будильник  1
* **/alarm2** - Будильник  2
* **/alarm3** - Будильник  3

### Управление по MQTT
Для управления кормушкой по протоколу MQTT необходимо выполнить ряд настроек. В файле secrets.h изменить значения следующих переменных:

`String mqtt_server = "192.168.1.1"; //ip или http адрес
int mqtt_port = 1883; //порт`

Подставьте в них адрес и порт используемого вами MQTT брокера.
Также вы можете изменить учетные данные для подключения:

`String mqtt_login="login"; //логин
String mqtt_pass="pass"; //пароль`

Для управления, по умолчанию используется топик: **CatFeeder/cmd/**.
Для отображения статусной информации: **CatFeeder/status/**.
За них отвечают переменные:

`String cmdTopic="CatFeeder/feed/"; //топик управления
String statusTopic="CatFeeder/status/"; //топик статуса`

Тут же вы можете изменить и ID клиента:

`String clientID="CatFeeder2"; //ID клиента`

Также настройка MQTT подключения возможна и через веб интерфейс.
Если все настройки верны, то при успешном подключении к брокеру в статусной панели появится соответствующий значок.

#### Полный список управляющих команд
Команда без параметра включает или отключает функцию. С параметром - устанавливает значение или выполняет действие. 

* **feed** - выдача одной порции корма. (Если указан параметр, то выдать определенное колличество 1-60 гр). Например **/feed 40**.
* **info** - получение статусной информации о работе кормушки
* **lastfeed** - время последнего кормления и размер порции
* **temp** - температура и влажность
* **tgbot** - включение/отключение телеграм бота
* **brightlevel** - установить яркость подсветки экрана (параметр 0-255)
* **daytime** - без параметра, включение и отключение снижения яркости экрана. Параметр вида hn:he установка дневного времени. hn-начало, he-конец
* **restart** - перезагрузка устройства

Следующие команды без параметра включают или отключают будильники. С параметром устанавливают его значение и сохраняют в память.
Параметр вида **hh:mm** устанавливает время. Например, **/alarm0 11:20**. Параметр val размер порции в граммах. Например, **/alarm0 40**.
* **alarm0** - Будильник 0
* **alarm1** - Будильник  1
* **alarm2** - Будильник  2
* **alarm3** - Будильник  3

#### Статусные MQTT топики
* **CatFeeder/temp/** - топик температуры с DHT датчика
* **CatFeeder/humid/** - топик влажности с DHT датчика
* **CatFeeder/feed/** - время последнего кормления

