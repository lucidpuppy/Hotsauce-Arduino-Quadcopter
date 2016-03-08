void setup_joysticks()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(4,INPUT);           //joystick switch 1
  pinMode(5,INPUT);           //jostick switch 2
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(4,HIGH);       //enable pullups to read switch
  digitalWrite(5,HIGH);       //enable pullups to read switch
  digitalWrite(6, HIGH);      //vcc for joysticks
  digitalWrite(7, LOW);      //vcc for joysticks
  digitalWrite(8, HIGH);      //gnd for joysticks
  digitalWrite(9, LOW);      //gnd for joysticks
}

void read_joysticks()
{
  if(!correction) get_correction();                           // to correct the mean positon of all the joysticks to 500
  
  throttle=map(analogRead(A0),0,1023,0,1000)-thrtle;
  
  yaw=map(analogRead(A1),0,1023,-180,180)-yw;
  
  smooth_throttle_yaw();
  
  pitch=map(analogRead(A2),0,1023,-30,30)-pt;
  if(pitch<0) { bit_pitch=HIGH; pitch=-pitch;}
  else bit_pitch=LOW;
  
  roll=map(analogRead(A3),0,1023,-30,30)-rl;
  if(roll<0) { bit_roll=HIGH; roll=-roll;}
  else bit_roll=LOW;
  
  ex_bits[0]=digitalRead(4);
  ex_bits[1]=digitalRead(5);
  //Serial.print("\nReading Joysticks");
}

void get_correction()
{
  thrtle=map(analogRead(A0),0,1023,0,1000)-500;
  yw=map(analogRead(A1),0,1023,-180,180)-0;
  pt=map(analogRead(A2),0,1023,-30,30)-0;
  rl=map(analogRead(A3),0,1023,-30,30)-0;
  correction=true;
}
