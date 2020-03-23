// ver.01 2019-11-08

//-------------------  Класс "Реле" -------------------

class Relay {
  public:
    Relay(uint8_t pin, bool active);
    //входные параметры --
    bool   modeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool   cmd;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool   state;               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    char  stateChar[2]={'0','\0'};
    char  cmdChar[2] = {'0','\0'};
    //--------------------
    uint8_t getPwm();
    bool   stateChanged();
    bool   cmdChanged();
    bool   pwmChanged();
    bool   getState();
    void   on();
    void   off();
    void   run();
    void   runAuto();
    void   setPwm(uint8_t val);
    void   runPwm();
  private:
    uint8_t _pin;
    bool   _active;
    bool   _on, _off;
    bool   _stateOld;
    bool   _cmdOld;
    bool   _pwmOld;
    bool   _pwmChanged;
    bool   _cmdChanged;
    bool   _stateChanged;
    uint32_t _timeLast = 0;
    uint8_t  _pwmValue = 0;  
    

};

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
Relay:: Relay(uint8_t pin, bool active) {
  // stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  _pwmChanged = true;
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
  setPwm(0);
}

///---------------------------------------------------------------///

void Relay:: on(){
  digitalWrite(_pin, _on);
}

void Relay:: off(){
  digitalWrite(_pin, _off);
}

bool Relay:: getState() {
  if (_active) {
    if (digitalRead(_pin) != 0)  {state=1; dtostrf((state), 1, 0, stateChar);}
    else                         {state=0; dtostrf((state), 1, 0, stateChar);}
    }
  else {
    if (digitalRead(_pin) != 0) {state=0; dtostrf((state), 1, 0, stateChar);}
    else                        {state=1; dtostrf((state), 1, 0, stateChar);}
  }
  return state;    
}

bool Relay:: cmdChanged(){
  if (_cmdOld != cmd){
    _cmdOld=cmd;
    _cmdChanged = true;
    dtostrf((cmd), 1, 0, cmdChar);
  }
  else _cmdChanged = false;
  return _cmdChanged;
}

// можно использовать вместо getState() //
bool Relay:: stateChanged(){ 
  bool statetemp = getState();
  if  (statetemp != _stateOld) {
    _stateOld = statetemp;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

void Relay:: runAuto(){
  if (modeAuto){
    if (cmd == 1) {
      }
    else {
    }
  }
  else{
  }
  
};

void Relay:: run() {
  if (cmd) {
    on();
  }
  else {
    off();
  }
}

void Relay:: setPwm(uint8_t val){
  _pwmValue  = val;
  if (_pwmValue != _pwmOld){
    _pwmOld = _pwmValue;
    _pwmChanged = true;
  } 
  else {
    _pwmChanged = false;
  }
}

uint8_t Relay:: getPwm(){
  return _pwmValue;
}

bool Relay:: pwmChanged(){
  if (_pwmChanged) return true;
  else return false;
}


void Relay:: runPwm() {
  uint16_t pwmMap;
  if (_active){
    pwmMap = map(_pwmValue, 0, 20, 0, 1023);
  }
  else {
    pwmMap = map(_pwmValue, 0, 20, 1023, 0);
  }
    if (cmd) {
    //analogWrite(_pin, pwmMap);
  }
  else {
    off();
  }
}


//******************************************************************************************************************************

//-------------------  Класс "Термостат" -------------------

class Thermostat {
  public:
    Thermostat(uint8_t pin, bool active);
    Thermostat(uint8_t pin, bool active, bool _cooler);
    Thermostat(uint8_t pin, bool active, int8_t min, int8_t max, int8_t thres);
    Thermostat(uint8_t pin, bool active, int8_t min, int8_t max, int8_t thres, bool _cooler);
    //входные параметры --
    int8_t min;
    int8_t max;
    int8_t thres;
    float  paramCurrent;
    bool   modeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool   cmd;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool   state;               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    bool   cooler;
    char  stateChar[2]={'2','\0'};
    char  cmdChar[2] = {'2','\0'};
    //--------------------
    bool   stateChanged();
    bool   cmdChanged();
    bool   getState();
    void   on();
    void   off();
    void   runMin();
    void   runMax();
    void   runMinMax();
    void   runAuto();
  private:
    uint8_t _pin;
    bool   _active;
    bool   _on, _off;
    bool   _stateOld;
    bool   _cmdOld;
    bool   _cmdChanged;
    bool   _stateChanged;
    uint32_t _timeLast = 0;

};

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
Thermostat:: Thermostat(uint8_t pin, bool active) {
  //stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  cooler = 0;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
//              _cooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, bool _cooler) {
  //stateChar = "2";
  modeAuto = 1;
  _pin    = pin;
  _active = active;
  cooler = _cooler;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _min, _max, _thres - минимум значения (температуры), максимум, гистерезис 
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _min, int8_t _max, int8_t _thres) {
  //stateChar = "2";
  modeAuto = 1;
  _pin =    pin;
  _active=  active;
  cooler = 0;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  min = _min;
  max = _max;
  thres = _thres;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _min, _max, _thres - минимум значения (температуры), максимум, гистерезис 
//              _cooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _min, int8_t _max, int8_t _thres, bool _cooler) {
  //stateChar = "2";
  modeAuto = 1;
  _pin    = pin;
  _active = active;
  cooler = _cooler;
  if (_active) {
    _on =   1;
    _off =  0; }
  else {
    _on =   0;
    _off =  1; 
  }
  min = _min;
  max = _max;
  thres = _thres;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  off();
  delay(100);
  _stateOld = getState();
  cmdChanged();
}

///---------------------------------------------------------------///

void Thermostat:: on(){
  digitalWrite(_pin, _on);
}

void Thermostat:: off(){
  digitalWrite(_pin, _off);
}

bool Thermostat:: getState() {
  if (_active) {
    if (digitalRead(_pin) != 0)  {state=1; dtostrf((state), 1, 0, stateChar);}
    else                         {state=0; dtostrf((state), 1, 0, stateChar);}
    }
  else {
    if (digitalRead(_pin) != 0) {state=0; dtostrf((state), 1, 0, stateChar);}
    else                        {state=1; dtostrf((state), 1, 0, stateChar);}
  }
  return state;    
}

bool Thermostat:: cmdChanged(){
  if (_cmdOld != cmd){
    _cmdOld=cmd;
    _cmdChanged = true;
  }
  else _cmdChanged = false;
  return _cmdChanged;
}

// можно использовать вместо getState()
bool Thermostat:: stateChanged(){ 
  bool i = getState(); 
  if (i != _stateOld) {
    _stateOld = i;
    _stateChanged = true;
  }
  else _stateChanged = false;
  return _stateChanged;
}

// для нагревателя использовать обе функции: runMin(); runMax(); либо только runMin() для "Анти-замерзания"
// runMinMax() для нагревателя включает "Анти-замерзание" свм по себе. 

// Только для нагревателя. Включ если темп < min, min+thres - отключается 
void Thermostat:: runMin(){       
  if (cmd != 1) {
    if (paramCurrent <= (float)(min)) on();
    if (paramCurrent >= (float)(min+thres)) off(); 
  }
};

// нагрев:      включ. если темп <= max; отключ. темп >= max-thres
// кондиционер: включ. если темп >= max; отключ. темп <= max-thres
void Thermostat:: runMax(){       
  if (cmd == 1) {
    if (!cooler){
      if (paramCurrent <= (float)(max-thres)) on();
      if (paramCurrent >= (float)(max)) off(); 
    }
    else {
      if (paramCurrent <= (float)(max-thres)) off();
      if (paramCurrent >= (float)(max)) on();      
    }
  }
};

// используется, в основном, для кондиционера и вентилятора
// нагрев:      от min до max
// кондиционер: включ. если темп >= max; отключ. темп <= min
void Thermostat:: runMinMax(){
  if (cmd == 1) {
    if (!cooler){
      if (paramCurrent >= (float)max) off();
      if (paramCurrent <= (float)min) on();  
    }
    else {
      if (paramCurrent >= (float)max) on();
      if (paramCurrent <= (float)min) off(); 
    }
  }
  else {
    if (!cooler) runMin();
    else off();
  }
};

void Thermostat:: runAuto(){
  if (modeAuto){
    if (cmd == 1) {
      if (!cooler){
        if (paramCurrent >= (float)max) off();
        if (paramCurrent <= (float)min) on();  
      }
      else {
        if (paramCurrent >= (float)max) on();
        if (paramCurrent <= (float)min) off(); 
      }
    }
    else {
      if (!cooler) runMin();
      else off();
    }
  }
  else{
  }
};