


//io defs
const int sensorPin = 11;
const int alarmPin = 8;

//constants
const long sensorDistanceMin = 150;
const int alarmDuration = 100;
const int sensorThrottleMax = 25;


//globals
int alarmTicks;
int sensorThrottle;

///////////////////////////////////
void setup() 
{ 
  Serial.begin(9600);
  
  pinMode(sensorPin, INPUT);  // ping sensor
  pinMode(alarmPin, OUTPUT);  // alarm
  
  alarmTicks = 0;
  sensorThrottle = 0;
} 
 
///////////////////////////////////
void loop() 
{ 
  
  //check the sensor
  if (sensorReadPing(sensorPin) < sensorDistanceMin)
  {
    if(sensorThrottle <1)
    {
      out("s");
      
    }
    sensorThrottle = sensorThrottleMax;
  }
  
  if(sensorThrottle >0)
  {
    sensorThrottle--;
  }
  
  delay(5);
  
  //check for serial input
  int incomingByte = 0;
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    
    //turn on the alarm
    setAlarm(1);
  }
  
  delay(5);
  
  //check the alarm
  if(alarmTicks > 0)
  {
    alarmTicks--;
    
    //turn off the alarm
    if(alarmTicks < 1)
    {
      setAlarm(0);
    }
  }
  
  delay(5);
  
}

/////////////////////////////////
void setAlarm(int state)
{
  
  if(state == 1)
  {
      out("Alarm ON");
      digitalWrite(alarmPin, HIGH);
      alarmTicks = alarmDuration;
  }
  else
  {
    out("Alarm OFF");
    digitalWrite(alarmPin, LOW);
    alarmTicks == 0;
  }
}

/////////////////////////////////
// get ping sensor measurement
long sensorReadPing(int pingPin)
{
  //vars
  long duration;
  long cm;

  //sensor is triggered by a high pulse of more than 2 microseconds
  //send a short low pulse to ensure a clean high pulse
  pinMode(pingPin, OUTPUT); // maybe move this to setup for speed?
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(pingPin, LOW);

  //duration is the time in microseconds the sensor echo took
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  
  
  //Serial.println(duration);
  
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  
  //short delay to keep everything clean
  delay(50); //was 100
  
  //
  //Serial.println(cm);
  
  //return
  return cm;
}

/////////////////////////////////
long microsecondsToCentimeters(long microseconds)
{
  //speed of sound is 340 m/s
  //29 microseconds per centimeter
  return microseconds / 29 / 2;
}

/////////////////////////////////
void out(String output)
{
  Serial.print(output);
}
