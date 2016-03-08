void display_rc()
{
  if(demake==false) demake_packets();
  Serial.print("\nThrottle Yaw Pitch Roll BITS\t");
  Serial.print((int)throttle,DEC);
  Serial.print("\t");
  Serial.print(yaw,DEC);
  Serial.print("\t");
  Serial.print(pitch,DEC);
  Serial.print("\t");
  Serial.print(roll,DEC);
  Serial.print("\t");
  Serial.print(bits,BIN);
  
  Serial.print("\tTransmitter ");
  if(transmitter_is()==true) Serial.print("ON");
  else Serial.print("OFF");
  Serial.print("\tFrequency ");
  Serial.print(1/dt);
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

