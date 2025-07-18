/*
   Пример работы датчика в двух режимах - обычном и принудительном , а так же перезапись настроек во время работы
   О режимах:
   NORMAL_MODE - Обычный - датчик делает измерения циклически , между измерениями спит , время сна настраивается  с помощью .setStandbyTime(); , см пример low_power
   FORCED_MODE - Принудительный - датчик спит , после вызова .oneMeasurement(); просыпается и делает ОДНО преобразование , после чего уходит в сон
   По умолчанию - NORMAL_MODE - Время сна между преобразованиями - 250мс
   См. константы в GyverBME280.h , стандартные настройки можно изменить там же в классе GyverMBE280
*/
#include <GyverMAX7219.h>
// MAX7219 < W, H, CS > mtrx;    // подключение к аппаратному SPI
//  пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

// MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам
//  W и H - количество МАТРИЦ по горизонтали и вертикали
//  CS, DATA, CLK - номера пинов

MAX7219<1, 1, 15> mtrx; // одна матрица (1х1), пин CS на D1//5

#include <GyverBME280.h> // Подключение библиотеки
GyverBME280 bme;         // Создание обьекта bme

/// @brief
void setup()
{
  Serial.begin(9600); // Запуск последовательного порта

  mtrx.begin();      // запускаем
  mtrx.setBright(5); // яркость 0..15
  // mtrx.setRotation(1);   // можно повернуть 0..3, по 90 град по часовой стрелке

  mtrx.dot(0, 0); // пиксель на координатах 0,0
  mtrx.update();  // показать
  delay(1000);
  mtrx.clear();

  //
  bme.setMode(FORCED_MODE); // Перед инициализацией говорим датчику работать в принудительном режиме
  bme.begin();              // Больше настройки не нужны  - инициализируем датчик

  while (millis() < 10000)
  {                       // Работа с датчиком в принудительном режиме в течении первых 10 секунд
    bme.oneMeasurement(); // Просим датчик проснуться и сделать одно преобразование
    while (bme.isMeasuring())
      ; // Ждем окончания преобразования

    Serial.print("Temperature: ");
    Serial.print(bme.readTemperature()); // Читаем и выводим температуру
    Serial.println(" *C");

    Serial.print("Humidity: ");
    Serial.print(bme.readHumidity()); // Читаем и выводим влажность
    Serial.println(" %");

    Serial.print("Pressure: ");
    Serial.print(pressureToMmHg(bme.readPressure())); // Читаем и выводим давление
    Serial.println(" mm Hg");
    Serial.println("");
    delay(1000);
  }

  bme.setMode(NORMAL_MODE); // Спустя 10 секунд переключаем датчик в обычный режим
  bme.begin();              // Переинициализируем датчик после изменения настроек - обязательная процедура
}

void loop()
{ // Работа с датчиком в обычном режиме , преобразования идут в цикличном режиме

  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature()); // Читаем и выводим температуру
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(bme.readHumidity()); // Читаем и выводим влажность
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressureToMmHg(bme.readPressure())); // Читаем и выводим давление
  Serial.println(" mm Hg");
  Serial.println("");
  delay(1000);
}
