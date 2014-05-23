/*
  Dumper
  Sends dumped message times
 */

int led = LED_BUILTIN;

int dumpPin = 3;
//Time of start 
unsigned long startMsgTime = 0;
unsigned long lastPulseTime = 0;
//Time to separate messages
unsigned long periodTime = 500000;  // 0.5 sec
int curPulseVal = 0;
bool vals[256];
unsigned long times[256];
int pulseCount = 0;


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(dumpPin, INPUT);
  pulseCount = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:

  int pinValue = digitalRead(dumpPin);  
  if(pinValue != curPulseVal){
    unsigned long curMicros = micros();
    lastPulseTime = curMicros;
    if(pulseCount == 0){  // Start of the message
      startMsgTime = curMicros;
      vals[0] = pinValue;
      pulseCount = 1;
    }else{
      times[pulseCount-1] = curMicros - startMsgTime;
      vals[pulseCount] = pinValue;
      pulseCount++;
      
      if(pulseCount > 255){
        Serial.print("Overflow");
      }
        
    }
    curPulseVal = pinValue;
  }else{  //Check if we need to send message
    if(pulseCount > 0){
      if((micros() - lastPulseTime) > periodTime){
        sendDump();
      }
    }
  }}

//Sendig accumulated values of cod
void sendDump() {
  Serial.print("Dump: ");
  Serial.println(pulseCount);
  int maxL = pulseCount -1;
  int i;
  for (i = 0; i < maxL; i = i + 1) {
    Serial.print(vals[i]);
    Serial.print(",");
  }
  Serial.println();
  for (i = 0; i < maxL; i = i + 1) {
    Serial.print(times[i]);
    Serial.print(",");
  }  
  Serial.println();
  unsigned long lastTime = 0;
  double p200 = 0;
  int p200c = 0;
  double p700 = 0;
  int p700c = 0;
  for (i = 0; i < maxL; i = i + 1) {
    int period = times[i] - lastTime;
    Serial.print(period);
    lastTime = times[i];
    Serial.print(",");
  }  
  Serial.println();
  lastTime = 0;
  for (i = 0; i < maxL; i = i + 1) {
    int period = times[i] - lastTime;
    if(period > 200 && period < 300){
      p200 += period;
      p200c++;
      Serial.print("s");
    }else if(period > 700 && period < 1000){
      p700 += period;
      p700c++;
      Serial.print("l");
    }else if(period > 1000 && period < 2000){
      Serial.print("n");
    }else if(period > 3000){
      Serial.print("p");
    }else{
      Serial.print("?");
    }
    lastTime = times[i];
    Serial.print(",");
  }  
  Serial.println();
  Serial.print("s: ");
  Serial.print(p200c);
  Serial.print(", ");
  Serial.println(p200/float(p200c));
  Serial.print("l: ");
  Serial.print(p700c);
  Serial.print(", ");
  Serial.println(p700/float(p700c));
  pulseCount = 0;
  Serial.println("===");
}

