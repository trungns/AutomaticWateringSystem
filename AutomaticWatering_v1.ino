/*
 Automatic watering system
 
 created 28 Jul. 2016
 by TrungNS

 Scope
 - Apply for small plants located in the pots of your beautiful balcony
 
 Main functions
 - Measure soil moisture periodically
 - Automatically supply the water if the soil is dry detected by the Moisture Sensor 

 What you can learn
 - Basic programming for Arduino
 - Display information & Debuging using your computer
 - Using Timer
 - Interact with Moisture Sensor
 - How to control water pumper or other devices using Relay
 
 */

//Define the moisture sensor pins
const int SENSOR_MOIS_A_IN_PIN0 = A1;   
const int  SENSOR_MOIS_SAMPLE_NO = 30;                       //the sum of the 30 times sampling

//Define the water pump contorl pins
const int RELAY_OUT_PIN =  6;                                //pin 6 connect to the anode of the pump

//Define the parameters of the flower1
//we need to sampling the moisture 30 times and get its average value, 
//this variable is used to store the sum of the 30 times sampled value   
long int moistureValue0 = 0;                                 //the value readed from each moisture sensor
long int moistureSum0 = 0;
long int waterDryValue0 = 400;
long int waterDryStep = 50;

unsigned long intervalTime = 3600000;                        // 30 m
unsigned long intervalStep =   600000;                       // 10 minute
unsigned long wateringTime = 10 * 1000;                      // ms
unsigned long wateringTimeStep = 10000;                      //s

// Timer
unsigned long loopTime = 0;
unsigned long currentMillis = 0;

void setup() 
{ 
 
  Serial.begin(9600); 

  delay(500);
  pinMode(RELAY_OUT_PIN, OUTPUT);

  // initial timer
  currentMillis = millis();
  loopTime = currentMillis;
} 


void loop() 
{
  currentMillis = millis();
  if ((unsigned long)(currentMillis - loopTime) >= intervalTime) {
    moistureSensorSampling();
    loopTime = currentMillis;  // Updates loopTime

    if(moistureValue0 > waterDryValue0)
    {
      watering(wateringTime);
      return;
     } 
  }
}

// read the value of the soil moisture
void moistureSensorSampling()
{
  for(int i = 0; i < SENSOR_MOIS_SAMPLE_NO; i++)
  {
    moistureSum0 = moistureSum0 + analogRead(SENSOR_MOIS_A_IN_PIN0);  
    delay(100);
  }
  moistureValue0 = moistureSum0 / SENSOR_MOIS_SAMPLE_NO;//get the average value

  // print the results to the serial monitor:
  Serial.print("Moisture0 = " );                       
  Serial.print(moistureValue0);      
  Serial.println(); 
  
  moistureSum0 = 0;//reset the variable
  delay(4000);     //delay 4 seconds 
}


void watering(int wateringTime)
{
  Serial.println("Watering ON");
  delay(1000);
  digitalWrite(RELAY_OUT_PIN, HIGH);
  delay(wateringTime);
  Serial.println("Watering OFF");
  digitalWrite(RELAY_OUT_PIN, LOW);
  delay(1000);
}
