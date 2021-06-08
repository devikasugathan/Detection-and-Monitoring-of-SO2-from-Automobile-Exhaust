
int gas_sensor = A0; //Sensor pin 
float m = -0.270; //Slope 
float b = 1.157; //Y-Intercept 
float R0 = 5.169; //Sensor Resistance in fresh air from previous code

int CO_sensor = A1; //Sensor pin 
float m1 = -0.6527; //Slope 
float b1 = 1.30; //Y-Intercept 
float R01 = 7.22; //Sensor Resistance
#include <dht.h>
dht DHT;
#define DHT11_PIN A2
void setup() {
  
 
  Serial.begin(9600);      // PC to Arduino Serial Monitor
  Serial1.begin(115200);   // Arduino to ESP01 Communication
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);
  pinMode(A2,INPUT); //For DHT Sensor
 } 

void loop() { 
  // put your main code here, to run repeatedly:
 float sensor_volt; //Define variable for sensor voltage 
  float RS_gas; //Define variable for sensor resistance  
  float ratio; //Define variable for ratio
  int sensorValue = analogRead(gas_sensor); //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0; //Get value of RS in a gas
  ratio = RS_gas/R0;  // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale 
  Serial.print("Our desired PPM = ");
  Serial.println(ppm);
  delay(2000);

  float sensor_volt1; //Define variable for sensor voltage 
  float RS_gas1; //Define variable for sensor resistance  
  float ratio1; //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor); //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0; //Get value of RS in a gas
  ratio1 = RS_gas1/R01;  // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-b1)/m1; //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1); //Convert ppm value to log scale 
  Serial.print("CO PPM = ");
  Serial.println(ppm1);


  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  float temp = DHT.temperature;
  Serial.println(temp);
  float hum = DHT.humidity;
  Serial.print("Humidity = ");
  Serial.println(hum);


}
