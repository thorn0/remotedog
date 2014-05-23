const int BUFF_LENGTH = 252;
const int IN_LENGTH = 1552;
const int S_LENGTH = 258;
const int L_LENGTH = 779;
const int P_LENGTH = 9516;
int ledPin = LED_BUILTIN;
int sendPin = 2;
char msg[BUFF_LENGTH];
int msgI = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(ledPin, OUTPUT); 
  pinMode(sendPin, OUTPUT); 
  msgI = 0;
}

void loop() {
  char ch;
  while (Serial.available() > 0) {
    ch=(char)Serial.read();
    if (ch == '\n') {
      sendMsg(msgI);
      msgI = 0;
    } else if(ch != ',' && msgI < BUFF_LENGTH){
      msg[msgI] = ch;
      msgI++;
    }
  }
}

void sendMsg(int length) {
  Serial.println("Message!");
  Serial.println(length);
  digitalWrite(ledPin, LOW);
  digitalWrite(sendPin, LOW);

  int curPinValue = HIGH;  
  int nextPinValue = LOW;  
  int i = 0;
  unsigned long pulseLength;

  unsigned long nextTime;
  unsigned long curTime = micros();
  digitalWrite(sendPin, curPinValue);
  for(i = 0; i < length; i++){
    if(curPinValue == HIGH){
      nextPinValue = LOW;
    }else{
      nextPinValue = HIGH;
    }
    char pulseCode = msg[i];
    if(pulseCode == 'n'){
      //Serial.println("n");
      pulseLength = IN_LENGTH;
    }else if(pulseCode == 's'){
      //Serial.println("s");
      pulseLength = S_LENGTH;
    }else if(pulseCode == 'l'){
      //Serial.println("l");
      pulseLength = L_LENGTH;
    }else{
      //Serial.println("p");
      pulseLength = P_LENGTH;
    }
    nextTime = curTime + pulseLength;
    
    //while(micros() <= nextTime){
      //delayMicroseconds(1);
    //}
    delayMicroseconds(nextTime - micros());
    digitalWrite(sendPin, nextPinValue);
    digitalWrite(ledPin, nextPinValue);
    curPinValue = nextPinValue;
    curTime = nextTime;
  }
  digitalWrite(sendPin, LOW);
  digitalWrite(ledPin, LOW);
  Serial.println("Sent.");
}
