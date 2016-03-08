void gett_rc()
{
  //Serial.print("\nGetting radio");
  Wire.requestFrom(8, 5);    // request 5 bytes from slave device #8
  for(uint8_t i=0; i<5; rx_buffer[i]=Wire.read(),i++);
  demake_packets();
}

void demake_packets()
{
  transmitter_is= (rx_buffer[3]>>7) &0b00000001;
  
  if(transmitter_is==true)
  {
    del_throttle= (rx_buffer[0] & 0b11111111) | ((rx_buffer[1]<<8) & 0b100000000);
    bit_throttle=((rx_buffer[1]>>1) & 0b00000001);
    if(bit_throttle==HIGH) del_throttle=-del_throttle;
    
    setpoint_yaw= ((rx_buffer[1]>>2) & 0b00111111) | ((rx_buffer[2] <<6) & 0b11000000);
    bit_yaw= ((rx_buffer[2]>>7) & 0b00000001);
    if(bit_yaw==HIGH) setpoint_yaw=-setpoint_yaw;
    setpoint_yaw=constrain(setpoint_yaw,-yaw_limit,yaw_limit);
    
    setpoint_pitch= (rx_buffer[2]>>2) & 0b00011111;
    bit_pitch= ((rx_buffer[3]>>5) & 0b00000001);
    if(bit_pitch==HIGH) setpoint_pitch=-setpoint_pitch;
    setpoint_pitch=constrain(setpoint_pitch,-pitch_roll_limit,pitch_roll_limit);
    
    setpoint_roll= rx_buffer[3] & 0b00011111;
    bit_roll= ((rx_buffer[3]>>6) & 0b00000001);
    if(bit_roll==HIGH) setpoint_roll=-setpoint_roll;
    setpoint_roll=constrain(setpoint_roll,-pitch_roll_limit,pitch_roll_limit);
    
    bits= rx_buffer[4];
  }
  
  else
  {
    Serial.print("\nInitiate landing");
    del_throttle=-70;
    setpoint_pitch=0;
    setpoint_roll=0;
  }

  setpoint_throttle=setpoint_throttle + del_throttle*kii;                    //del_throttle is a value b/w -250 to 1000..multiply it bu kii and add it over to itself gives us throttle
  setpoint_throttle=constrain(setpoint_throttle,throttle_min,throttle_max);  //kii decided how fas the throttle increases or decreases
  
  if(setpoint_throttle>=1150) is_cutoff_clear=true;
  else is_cutoff_clear=false;
  
  if(del_throttle<-200 && bits==0x01) is_dead_stop=true;                     //if throttle joystick[left] is down and right joysticks switch is pressed ..it stops all mototrs.
    //if( del_throttle<-200 && setpoint_roll<=-24 && setpoint_pitch<=-24) is_dead_stop=false;
}

void display_rx_buffer()
{
  Serial.print("\nRX_buffer\t");
  Serial.print(rx_buffer[0],DEC);
  Serial.print("\t");
  Serial.print(rx_buffer[1],DEC);
  Serial.print("\t");
  Serial.print(rx_buffer[2],DEC);
  Serial.print("\t");
  Serial.print(rx_buffer[3],DEC);
  Serial.print("\t");
  Serial.print(rx_buffer[4],BIN);
  Serial.print("\t");
}
