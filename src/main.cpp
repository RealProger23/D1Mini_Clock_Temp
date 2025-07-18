// дефайн для изменения скорости SPI, по умолч 1000000
// #define MAX_SPI_SPEED 500000
#include <GyverBME280.h>
GyverBME280 bme;

#include <GyverMAX7219.h>
// MAX7219 < W, H, CS > mtrx;    // подключение к аппаратному SPI
//  пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

// MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам
//  W и H - количество МАТРИЦ по горизонтали и вертикали
//  CS, DATA, CLK - номера пинов

MAX7219<1, 1, 5> mtrx; // одна матрица (1х1), пин CS на D1//5

void setup()
{
  Serial.begin(9600);                                   // Запуск последовательного порта
  bme.setMode(FORCED_MODE);                             // Перед инициализацией говорим датчику работать в принудительном режиме
  bme.begin();                                          // Больше настройки не нужны  - инициализируем датчик

  while (millis() < 10000) {                            // Работа с датчиком в принудительном режиме в течении первых 10 секунд
    bme.oneMeasurement();                               // Просим датчик проснуться и сделать одно преобразование
    while (bme.isMeasuring());                          // Ждем окончания преобразования

    Serial.print("Temperature: ");
    Serial.print(bme.readTemperature());                // Читаем и выводим температуру
    Serial.println(" *C");

    Serial.print("Humidity: ");
    Serial.print(bme.readHumidity());                   // Читаем и выводим влажность
    Serial.println(" %");

    Serial.print("Pressure: ");
    Serial.print(pressureToMmHg(bme.readPressure()));   // Читаем и выводим давление
    Serial.println(" mm Hg");
    Serial.println("");
    delay(1000);
  }

  bme.setMode(NORMAL_MODE);                             // Спустя 10 секунд переключаем датчик в обычный режим
  bme.begin();               // Переинициализируем датчик после изменения настроек - обязательная процедура

  mtrx.begin();      // запускаем
  mtrx.setBright(5); // яркость 0..15
  // mtrx.setRotation(1);   // можно повернуть 0..3, по 90 град по часовой стрелке

  mtrx.dot(0, 0); // пиксель на координатах 0,0
  mtrx.update();  // показать
  delay(1000);
  mtrx.clear();

  // линии крест накрест
  mtrx.line(0, 0, 7, 7); // (x0, y0, x1, y1)
  mtrx.line(7, 0, 0, 7);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // круг
  mtrx.circle(3, 3, 3, GFX_FILL); // х, у, радиус, заливка
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // окружность
  mtrx.circle(3, 3, 3, GFX_STROKE);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // остальную геометрию смотри в документации
}

void loop()
{
  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature());                  // Читаем и выводим температуру
  Serial.println(" *C");

  Serial.print("Humidity: "); 
  Serial.print(bme.readHumidity());                     // Читаем и выводим влажность
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressureToMmHg(bme.readPressure()));     // Читаем и выводим давление
  Serial.println(" mm Hg");
  Serial.println("");
  delay(1000);
}
