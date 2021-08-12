//Wire colors refer to a standard Teknic ClearPath controller cable 
const int EnableX = 2; // ClearPath ~enable input; +enable = BLU wire; -enable = ORN wire
const int InputAX = 3; // ClearPath Input A; +InputA = WHT wire; -InputA is BRN wire
const int InputBX = 4; // ClearPath Input B; +InputB = BLK wire; -InputB = YEL wire
const int HLFBX = 5; // ClearPath HLFB Output; +HLFB = GRN wire; -HLFB = RED wire
const int EnableY = 6; // ClearPath ~enable input; +enable = BLU wire; -enable = ORN wire
const int InputAY = 7; // ClearPath Input A; +InputA = WHT wire; -InputA is BRN wire
const int InputBY = 8; // ClearPath Input B; +InputB = BLK wire; -InputB = YEL wire
const int HLFBY = 9; // ClearPath HLFB Output; +HLFB = GRN wire; -HLFB = RED wire
const int Ducerping = 10; // Signal to transducer box
const int myLED = 13; // Arduino on-board LED     //Set Up high speed reads on pin 11  
                           

void setup() 
{
 
 
  Serial.begin(19200);
  while (!Serial){}
  pinMode(EnableX, OUTPUT);
  pinMode(InputAX, OUTPUT);
  pinMode(InputBX, OUTPUT);
  pinMode(HLFBX, INPUT_PULLUP);
  pinMode(myLED, OUTPUT);
  pinMode(EnableY, OUTPUT);
  pinMode(InputAY, OUTPUT);
  pinMode(InputBY, OUTPUT);
  pinMode(HLFBY, INPUT_PULLUP);
  pinMode(Ducerping,OUTPUT);
  //pinMode(Ducerback,INPUT);
  digitalWrite(Ducerping,LOW);
  
 
  // start off by ensuring thet the motor is disabled before proceeding
  digitalWrite(EnableX, LOW);
  digitalWrite(EnableY, LOW);
  delay(10);

  // set the two outputs to their initial states
  digitalWrite(InputAX, LOW);
  digitalWrite(InputBX, LOW);
  digitalWrite(myLED, LOW);
  
  digitalWrite(InputAY, LOW);
  digitalWrite(InputBY, LOW);
  
  // toggle ~enable, read HLFB & set on-board LEDs - this clears any safety shutdowns & activates motor's power stage
  digitalWrite(EnableX, HIGH);
  delay(15);
  while(digitalRead(HLFBX) == !LOW) {
    delay(20);
  }
  if (digitalRead(HLFBX) == LOW) {
    digitalWrite(myLED, HIGH);
    delay(100);
  }


  
  digitalWrite(EnableY, HIGH);
  delay(15);
  while(digitalRead(HLFBY) == !LOW) {
    delay(20);
  }
  if (digitalRead(HLFBY) == LOW) {
    digitalWrite(myLED, HIGH);
    delay(100);
  }

}


// end of setup code



void loop(){
while (Serial.available()==0){};
String com = Serial.readString();
  if (com=="i"){                    //Initalization response
  Serial.print("i");
  }

  else if (com=="hxy"){                      //Homing command
  digitalWrite(EnableX,LOW);
  delay(100);
  digitalWrite(EnableX,HIGH);
  while (digitalRead(HLFBX)==HIGH){}
  digitalWrite(EnableY,LOW);
  delay(100);
  digitalWrite(EnableY,HIGH);
   Serial.print("h");  
  }

  else if (com=="hx"){
  digitalWrite(EnableX,LOW);
  delay(100);
  digitalWrite(EnableX,HIGH);
  while (digitalRead(HLFBX)==HIGH){}
  Serial.print("h");  
  }


  else if (com=="hy"){
  digitalWrite(EnableY,LOW);
  delay(100);
  digitalWrite(EnableY,HIGH);
  while (digitalRead(HLFBY)==HIGH){}
  Serial.print("h");  
  }


  
 else if (com=="d"){ //Collect data command
   uint8_t INPUTPIN_mask = digitalPinToBitMask(11);             //Set Up high speed reads on pin 11      
   volatile uint8_t *INPUTPIN_port = portInputRegister(digitalPinToPort(11));    //Set Up high speed reads on pin 11
 digitalWrite(Ducerping,HIGH);
 delay(10);
 digitalWrite(Ducerping,LOW);
 while (true){
    uint8_t pinValue = (*INPUTPIN_port & INPUTPIN_mask)!= 0;
    if (pinValue == LOW){
      break;
    }
 }
 Serial.print("d");
 }

 

  else if (abs(com.toFloat())>=10000){                 //Y positioning command
  movey(com.toFloat()/1000000);
  delay(25);
  Serial.print("r");  
  }

  else{                            
  //X Positioning command
  movex(com.toFloat());
  delay(25);
  Serial.print("r");
  }
}




//Move Commands
void movex(float rot){
  //clockwise
  if (rot<0){
       digitalWrite(InputAX,LOW);
  }
  //counterclockwise
   if (rot>=0){
       digitalWrite(InputAX,HIGH);
   }
  for (long i=0; i<(1000*abs(rot)*6.29); i++){ 
  digitalWrite(InputBX,HIGH);
  delay(0.033);
  digitalWrite(InputBX,LOW);
  delay(0.033);
  }
  while (digitalRead(HLFBX)==HIGH){
    //Do Nothing
  }
  }

  void movey(float rot){
  //clockwise
  if (rot<0){
       digitalWrite(InputAY,LOW);
  }
  //counterclockwise
   if (rot>=0){
       digitalWrite(InputAY,HIGH);
   }
  for (long i=0; i<(1000*abs(rot)*6.29); i++){
  digitalWrite(InputBY,HIGH);
  delay(0.033);
  digitalWrite(InputBY,LOW);
  delay(0.033);
  }
  while (digitalRead(HLFBY)==HIGH){
    //Do Nothing
  }
  }
