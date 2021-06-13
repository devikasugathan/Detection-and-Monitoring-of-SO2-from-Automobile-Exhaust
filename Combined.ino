int gas_sensor = A0; //SO2 sensor MQ-136
float m = -0.8294; //Slope 
float b = 1.358; //Y-Intercept 
float R0 = 4.27; //From prev code

int CO_sensor = A1; //Sensor pin Mq-7
float mco = -0.7527; //Slope 
float bco = 1.30; //Y-Intercept 
float R0co = 5.22; //Prev code

String apiKey = "NO8MZ75WNDWGV1JD";  
#include <dht.h>
dht DHT;
#define DHT11_PIN A2
void setup() {
  
 
  Serial.begin(9600);      
  Serial1.begin(115200);   
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);
  pinMode(A2,INPUT); //For DHT Sensor
 } 

void loop() { 
  float sensor_volt; 
  float RS_gas; 
  float ratio; 
  float sensorValue = analogRead(so_sensor); 
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
    RS_gas = ((5.0*10.0)/sensor_volt)-10.0; 
  ratio = RS_gas/R0;  //in air
  double ppm_log = (log10(ratio)-b)/m; 
  double ppm = pow(10, ppm_log); 
  Serial.print("SO2 in PPM = ");
  Serial.println(ppm);
  

  float sensor_voltco; 
  float RS_gasco;
  float ratioco; 
  float sensorValueco = analogRead(CO_sensor); 
  sensor_voltco = sensorValueco*(5.0/1023.0); 
  RS_gasco = ((5.0*10.0)/sensor_voltco)-10.0; 
  ratioco = RS_gasco/R0co;  
  double ppm_logco = (log10(ratioco)-bco)/mco;
  double ppmco = pow(10, ppm_logco); 
  Serial.print("CO in PPM = ");
  Serial.println(ppmco);


  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  float temp = DHT.temperature;
  Serial.println(temp);
  float hum = DHT.humidity;
  Serial.print("Humidity = ");
  Serial.println(hum);



  Serial1.println("AT+CIPMUX=0\r\n");      
  delay(2000);                             // 2 sec

  // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\"";  
  cmd += "184.106.153.149";                
  cmd += "\",80\r\n\r\n";                 

  Serial1.println(cmd);                    
  Serial.println(cmd);                     

  delay(1000);                         

  if(Serial1.find("ERROR"))                
  { 
    Serial.println("AT+CIPSTART error");   
    //return; 
  }

  // prepare GET string 
  String getStr = "GET /update?api_key=";   
  getStr += apiKey;
  getStr +="&field1=";
  getStr += ppm;
  getStr +="&field2=";
  getStr += ppmco;
  getStr +="&field3=";
  getStr += temp;
  getStr +="&field4=";
  getStr += hum;
  getStr += "\r\n\r\n"; 

  Serial.println(getStr);                 

  cmd = "AT+CIPSEND=";                    
  cmd += String(getStr.length());
  cmd+="\r\n";

  Serial.println(cmd);                  
  Serial1.println(cmd);                 
  if(Serial1.find(">"))                  
  {
    Serial.println("Data sent to CLOUD");  
    Serial1.print(getStr);                 
  }
  else
  { 
    Serial1.println("AT+CIPCLOSE\r\n");    
    Serial.println("AT+CIPCLOSE");         
  } 
 
  delay(20000);                         
 }
