#include <math.h>
#include <SimpleDHT.h>

#define therm_pin A0

double V_0 = 3.291;
double R_1 = 100000.0; VCC - R_1 - A0 + Thermistor - GND

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(therm_pin,INPUT);
  analogReference(EXTERNAL);
}

double C0 = 0.0009621996628; //0.9621996628 e-3
double C1 = 0.0001800586745; //1.800586745 e-4
double C2 = 0.000000197132282; //1.971322820 e-7 

double factor = 10000000.0
double CM0 = 9621.996628; //0.9621996628 e-3
double CM1 = 1800.586745; //1.800586745 e-4
double CM2 = 1.97132282; //1.971322820 e-7 


// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  double sensorValue = analogRead(therm_pin);

  // Convert the analog reading (which goes from 0 - 1023) to voltage reference (3.3V or 5V or other):
  double voltage = sensorValue * V_0 /1024.0;

  double rThermistor = R_1 * voltage/(V_0 - voltage); // voltage to resistance
  double lnR = log(rThermistor);

  // T(K) = 1/(C0 + C1Ln(R) + C2(Ln(R))^3)
  double temp  = factor /(CM0 + lnR * CM1 + CM2 * lnR * lnR * lnR);
  temp  = T_sum - 273.15; // K -> C
    
  Serial.print("Thermistor: ");

  Serial.print("A0: ");
  Serial.println(sensorValue);

  Serial.print("RT: ");
  Serial.println(rThermistor);

  Serial.print(T_sum);
  Serial.print(" (");
  Serial.print((T_sum*9.0/5.0)+32.0);
  Serial.println(" F)");

  delay(1500);
}
