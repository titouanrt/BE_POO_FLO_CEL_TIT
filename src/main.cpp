#include "Arduino.h"
#include "Wire.h"
#include "SHT31.h"
#include "Ultrasonic.h"
#include "rgb_lcd.h"
#include "Servo.h"
#include <ESP8266WiFi.h>
#include <exception>
#include "Web_serv.h"
#include <vector>

//définition du nom et du mot de passe du point d'accès wifi
#ifndef APSSID
#define APSSID "ESPap"
#define APPSK "thereisnospoon"
#endif


const char *ssid = APSSID;
const char *password = APPSK;

bool etape_1 = false;
bool etape_2 = false;
bool etape_3 = false;
bool etape_4 = false;
bool etape_5 = false;



//---------DEVICE---------------------
class Device{
protected:
  int8_t pin;
public:
  Device(){};
  Device(int8_t pin):pin(pin){}
  ~Device(){}

  Device& operator=(int8_t pin_ext) {
    this->pin = pin_ext; 
    return (*this);
  }
};

//---------SENSOR---------------------
class Sensor: public Device{
public:
  Sensor():Device(){}
  Sensor(int8_t pin):Device(pin){
    pinMode(pin, INPUT);
  }
  ~Sensor(){}
};

class Actuator: public Device
{
  public:
    Actuator(int8_t pin):Device(pin){
      pinMode(pin, OUTPUT);
    };
    ~Actuator(){};
};

class Light_sensor : public Sensor
{
  private:
    float light_value;
  public :

  Light_sensor(int8_t pin):Sensor(pin),light_value(){};
  ~Light_sensor(){};
  void ReadCurrentLight()
  {
    light_value = analogRead(pin);
  };

  int GetLight()
  {
    return light_value;
  };

};

class Button_sensor : public Sensor
{
  private:
    int button_state;
  
  public:
    Button_sensor(int8_t pin):Sensor(pin), button_state(){};
    ~Button_sensor(){};

    void ReadCurrentButton()
    {
      button_state = digitalRead(pin);
    };

    int GetButton()
    {
      return button_state;
    };
};

class Buzzer_actuator : public Actuator
{
  private:

  public:
    Buzzer_actuator(int8_t pin):Actuator(pin){};
    ~Buzzer_actuator(){};

  void SetBuzzerHigh()
  {
    digitalWrite(pin, HIGH);
  }

  void SetBuzzerLow()
  {
    digitalWrite(pin, LOW);
  }

};

//---------TEMPERATURE SENSOR---------------------
class Temp_sensor: public Sensor {
private:
  float temperature;  //  °C
  float humidity;     //  % 
  SHT31 sht31;

public:
  
  Temp_sensor():Sensor(),temperature(),humidity()
  {
    sht31 = SHT31();
    sht31.begin();
  }
  
  ~Temp_sensor(){}

  void ReadCurrentTemp(){
    temperature = sht31.getTemperature(false);
  }

  void ReadCurrentHumidity(){
    humidity = sht31.getHumidity();
  }

  float GetHumidity(){
    return humidity;
  }

  float GetTemperature(){
    return temperature;
  }
};

//---------SOUND SENSOR---------------------
class Sound_sensor: public Sensor{
private:
  long sound_level;
public:
  Sound_sensor(int pin):Sensor(pin),sound_level(){}

  ~Sound_sensor(){}

  void GetCurrentSound(){
    for(int i=0; i<32; i++)
    {
        sound_level += analogRead(pin);
    }
    sound_level >>= 5;
  }
};

//---------POTENTIOMETER SENSOR---------------------
class Potentiometer_sensor: public Sensor{
  private:
  float voltage;
  float resistor;
  const int adc_ref = 5;
  const float current = 0.0005;
  public:
  Potentiometer_sensor(uint8_t pin):Sensor(pin),voltage(),resistor(){}
  void ReadCurrentVoltage(){
    int value = analogRead(this->pin);
    voltage = (float)value*adc_ref/1023;
  }
  float GetCurrentVoltage(){
    return voltage;
  }
  void ReadCurrentResistance(){
    ReadCurrentVoltage();
    resistor = this->voltage/current; 
  }
  float GetCurrentResistance(){
    return resistor;
  }
};

//---------TOUCH SENSOR---------------------
class Touch_sensor : public Sensor{
private:
  int sensorValue;
public:
  Touch_sensor(int8_t pin):Sensor(pin),sensorValue(){};
  ~Touch_sensor(){};
  void ReadTouch(){
    sensorValue = digitalRead(pin);
  }
  int GetTouchStatus(){
    return sensorValue;
  }
};


//---------ULTRASONIC SENSOR---------------------
class Ultrasonic_ranger: public Sensor{
private:
  long measure_inch;
  long measure_centimeters;
  Ultrasonic ultrasonic;
public:
 Ultrasonic_ranger(int pin):Sensor(pin),ultrasonic(pin),measure_centimeters(),measure_inch(){
 }
 ~Ultrasonic_ranger(){}

 void ReadDistanceInches(){
  measure_inch = ultrasonic.MeasureInInches();
 
 }

 long GetDistanceInches(){
  return measure_inch;
 }
 
 void ReadDistanceCentimeters(){
  measure_centimeters = ultrasonic.MeasureInCentimeters();
  delay(250);
 }

 long GetDistanceCentimeters(){
  return measure_centimeters;
 }
};

class Servo_actuator
{
  private:
    Servo myservo;
  public:
    Servo_actuator(int8_t pin){
      myservo.attach(pin);
    };
    ~Servo_actuator(){};

    void UpServo()
    {
      myservo.write(180);
    };

    void DownServo()
    {
      myservo.write(0);
    };
};

class LCD_Display{
private:
int R;
int G;
int B;
rgb_lcd lcd;
public:
  LCD_Display(int R, int G, int B):R(),G(),B(){
    lcd.begin(16,2);
    lcd.setRGB(R,G,B);
  }
  ~LCD_Display(){}

  void LCD_PrintLower(String message){
    lcd.setCursor(0, 1);
    lcd.print(message);
    //delay(200);
  }
    void LCD_PrintUpper(String message){
    lcd.setCursor(0,0);
    lcd.print(message);
    //delay(200);
  }
  void LCD_Clear(){
    lcd.clear();
  }
};

//instanciation des capteurs et actionneurs 
Button_sensor button = Button_sensor(D7);
LCD_Display lcd_disp = LCD_Display(255,75,155);
Potentiometer_sensor potar = Potentiometer_sensor(A0);
Servo_actuator servo = Servo_actuator(D6);
Touch_sensor touch = Touch_sensor(D5);
Temp_sensor temp_sens = Temp_sensor();

long lasttime = 0;
bool state = false;


void setup() {
  Serial.begin(9600); //configuration du port serie du 9600 bauds
  try { //tentative de mise en place du point d'accès wifi
    WebServ::ConnectWiFi("ESPap", "thereisnospoon");
  } catch (WiFiExceptions e) {
      Serial.print(e.what());
  }
  WebServ::begin();
}

void loop() {
  WebServ::update(); //actualisation des pages web

  //lecture des valeurs renvoyées par les capteurs
  potar.ReadCurrentResistance();
  potar.ReadCurrentVoltage();
  button.ReadCurrentButton();
  touch.ReadTouch();
  temp_sens.ReadCurrentTemp();

  //mise à jour de l'affichage sur l'écran LCD toutes les 500ms
  auto now = millis();
  if(now - lasttime > 500){
    lasttime = now;
    lcd_disp.LCD_Clear();
    lcd_disp.LCD_PrintUpper(String(potar.GetCurrentVoltage()) + " V");
    lcd_disp.LCD_PrintLower(String(temp_sens.GetTemperature()));
  }
  
  //1ere sécurité : bouton poussoir et capteur de touché
  if ( (button.GetButton() == 1 && touch.GetTouchStatus() == 1) || etape_1)
  {
    etape_1 = true;
    if ( (button.GetButton() == 0 && touch.GetTouchStatus() == 1) || etape_2 )
    {
      etape_2 = true;
      if ( (button.GetButton() == 1 && touch.GetTouchStatus() == 1) && !etape_3)
      {
        etape_3 = true;
        Serial.print("etape_3 ok\n");
        //mise à jour du tableau de la page web
        WebServ::SetStep3(etape_3);
        WebServ::updateStates();
      } 
    }
  }

  //2e sécurité : détection de température
  if (temp_sens.GetTemperature() > 30 && !etape_4)
  {
    etape_4 = true;
    Serial.print("etape_4 ok\n");
    //mise à jour du tableau de la page web
    WebServ::SetStep4(etape_4);
    WebServ::updateStates();
  }

  //3e sécurité : configuration du potentiomètre
  if (button.GetButton() == 1 && ( potar.GetCurrentVoltage() > 1.5 && potar.GetCurrentVoltage() < 2 ) && !etape_5)
  {
    etape_5 = true;
    Serial.print("etape_4 ok\n");
    //mise à jour du tableau de la page web
    WebServ::SetStep5(etape_5);
    WebServ::updateStates();
  }
  
  //Détection de l'ouverture du coffre
  if (etape_3 && etape_4 && etape_5)
  {
    Serial.print("Unlocked\n");
    etape_1 = etape_2 = etape_3 = etape_4 = etape_5 = false;
    servo.DownServo();
  }
  
}