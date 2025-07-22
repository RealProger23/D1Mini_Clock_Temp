/*
   Пример работы датчика в двух режимах - обычном и принудительном , а так же перезапись настроек во время работы
   О режимах:
   NORMAL_MODE - Обычный - датчик делает измерения циклически , между измерениями спит , время сна настраивается  с помощью .setStandbyTime(); , см пример low_power
   FORCED_MODE - Принудительный - датчик спит , после вызова .oneMeasurement(); просыпается и делает ОДНО преобразование , после чего уходит в сон
   По умолчанию - NORMAL_MODE - Время сна между преобразованиями - 250мс
   См. константы в GyverBME280.h , стандартные настройки можно изменить там же в классе GyverMBE280
*/

// #include <TimeLib.h>
#include <GyverMAX7219.h>
#include <RunningGFX.h>
#include <GyverHub.h>
// MAX7219 < W, H, CS > mtrx;    // подключение к аппаратному SPI
//  пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

// MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам
//  W и H - количество МАТРИЦ по горизонтали и вертикали
//  CS, DATA, CLK - номера пинов

MAX7219<4, 1, 15> mtrx; // одна матрица (1х1), пин CS на D1//5
RunningGFX run(&mtrx);
const char pstr_g[] PROGMEM = "Global pgm string";

#include <GyverBME280.h> // Подключение библиотеки
GyverBME280 bme;         // Создание обьекта bme
int Case_view = 0;

/// @brief
void setup()
{
  Serial.begin(9600); // Запуск последовательного порта

  mtrx.begin();      // запускаем
  mtrx.setBright(5); // яркость 0..15
  // mtrx.setRotation(1);   // можно повернуть 0..3, по 90 град по часовой стрелке

  // линии крест накрест
  mtrx.line(0, 0, 7, 7); // (x0, y0, x1, y1)
  mtrx.line(7, 0, 0, 7);
  mtrx.update();
  delay(1000);
  mtrx.clear();
  mtrx.update();

  bme.setMode(NORMAL_MODE); // Спустя 10 секунд переключаем датчик в обычный режим
  bme.begin();              // Переинициализируем датчик после изменения настроек - обязательная процедура
}

void view_tempr(int m_tempr)
{
  // Отображаем данные на матрице
  mtrx.clearDisplay();  // очищаем экран
  mtrx.setCursor(0, 0); // устанавливаем позицию текста
  mtrx.print("T:");
  mtrx.print(m_tempr, 0); // вывод температуры с одной цифрой после запятой
                          // mtrx.print(" C");
  mtrx.update();
}
void view_pressure(int m_pres)
{
  // Отображаем данные на матрице
  mtrx.clearDisplay();  // очищаем экран
  mtrx.setCursor(0, 0); // устанавливаем позицию текста
  mtrx.print("P:");
  mtrx.print(m_pres, 0); // вывод температуры с одной цифрой после запятой
  // mtrx.print(" ");
  mtrx.update();
}
void loop()
{ // Работа с датчиком в обычном режиме , преобразования идут в цикличном режиме
  // static gh::Timer tmr(1000);    // период 1 секунда
  static gh::Timer tmr10(10000); // период 60 секунда
  // static gh::Timer tmr60(60000); // период 60 секунда

  // каждые 10 секунд будем обновлять заголовок
  if (tmr10)
  {
    float temp = bme.readTemperature();
    float press = bme.readPressure();

    if (Case_view > 0)
    {
      Case_view = 0;
      view_tempr(temp);
    }
    else
    {
      Case_view += 1;
      view_pressure(pressureToMmHg(press));
    }
  }
}
