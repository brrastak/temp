class TimeCounter
{
public:
  TimeCounter(void)
  {
    // сохранение текущего значения счётчика
    cnt = millis();
  }
  
  void reset(void)  // сброс счётчика
  {
    // сохранение текущего значения счётчика
    cnt = millis();
  }
  
  bool if_passed(unsigned long delay_ms)  // прошла ли пауза в мс?
  {
    if (millis() > cnt + delay_ms) {
      this->reset();  // сброс счётчика
      return true;
    }
    else
      return false;
  }

private:
  unsigned long cnt;  // счётчик мс
};



class Timeout
{
public:
  Timeout(void* start_func, void* stop_func)  // функции, которые вызовутся в начале и в конце
  {
    do_on_start = start_func;
    do_on_stop = stop_func;
    is_on = false;
  }

  void start(unsigned long delay_ms)  // начало отсчёта задержки в мс; выполнение стартовой функции
  {
    // сброс счётчика
    cnt.reset();
    // отсчёт включён
    is_on = true;
    // время задержки в мс
    timeout = delay_ms;
    
    // стартовая функция
    do_on_start();
  }

  bool proceed(void)  // если задержка прошла, выполняется конечная функция
  {
    // если отсчёт выключен, ничего не происходит
    if (is_on == false)
      return false;

    // если задержка прошла
    if (cnt.if_passed(timeout)) {

      // выключение отсчёта
      is_on = false;

      // выполнение конечной функции
      do_on_stop();

      return true;
    }

    return false;
  }

private:
  void (* do_on_start)();
  void (* do_on_stop)();
  unsigned long timeout;
  bool is_on;
  TimeCounter cnt;
};
