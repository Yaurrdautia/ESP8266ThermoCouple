#include <LiquidCrystal_I2C.h>

#include <math.h>

#define therm_pin A0

double V_0 = 3.291;

// WeMos D1 Mini internally contains voltage divider for A0 input in order to expand the voltage
// range of ESP8266 which is limited to 1.0V
// R_2 of the divider (between A0 and GND) is fixed 100k ohm
double R_2 = 100000.0; //VCC - R_1 - A0 - R_2 - GND (R_1 = 230 + rThermistor)

// D1 Mini also has 220k ohm on the VCC side of the divider
double R_1d = 220000.0;

// I'm adding additional 10k register on VCC side of the divider, along with the sensor
// to make sure input voltage to the A0 pin never exceed 1V.
double R_1Adj = 10000.0;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // initialize serial communication at 9600 bits per second:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  Serial.begin(9600);
  pinMode(therm_pin,INPUT);
 
//  analogReference(EXTERNAL);
}

double C0 = 0.0009621996628; //0.9621996628 e-3
double C1 = 0.0001800586745; //1.800586745 e-4
double C2 = 0.000000197132282; //1.971322820 e-7 

double factor = 10000000.0;
double CM0 = 9621.996628; //0.9621996628 e-3
double CM1 = 1800.586745; //1.800586745 e-4
double CM2 = 1.97132282; //1.971322820 e-7 

double sensorMax = 1.0;


// the loop routine runs over and over again forever:
void loop() {

  // read the pin value 10 times over 100 ms span and take average
  double sensorValue = 0;
  for (int i = 0; i < 10; i++)
  {
    // read the input on analog pin 0:
    sensorValue += analogRead(therm_pin);
    delay(10);
  }

  sensorValue = sensorValue / 10;

  // Convert the analog reading (which goes from 0 - 1023) to voltage reference (3.3V or 5V or other):
  double voltage = sensorValue * sensorMax /1023.0;

  // i'm adjusting the misterious temperature error (for now treating it as constant error)
  voltage = voltage - 0.028;

  double R_1 = R_2 * (V_0 - voltage)/voltage; // voltage to resistance

  // remove the fixed 230k ohm on the vcc side of the voltage dividor
  double rThermistor = R_1 - R_1d - R_1Adj;

  //rThermistor = 133000;
  double lnR = log(rThermistor);

  // T(K) = 1/(C0 + C1Ln(R) + C2(Ln(R))^3)
  double temp  = factor /(CM0 + lnR * CM1 + CM2 * lnR * lnR * lnR);
  temp  = temp - 273.15; // K -> C
    
  Serial.print("Thermistor: ");

  Serial.print("A0: ");
  Serial.println(sensorValue);

  Serial.print("Voltage: ");
  Serial.println(voltage);
  
  Serial.print("RT (ohm): ");
  Serial.println(rThermistor);

  Serial.print("TEMP (C): ");
  Serial.print(temp);
  Serial.print(" (");
  Serial.print((temp*9.0/5.0)+32.0);
  Serial.println(" F)");
  lcd.print("the temp is ");
  lcd.print(temp);
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
}
