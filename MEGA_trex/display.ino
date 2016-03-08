void display_rc()
{

  Serial.print("\nThrottle Yaw Pitch Roll BITS\t");
  Serial.print(del_throttle,DEC);
  Serial.print("\t");
  Serial.print(yaw,DEC);
  Serial.print("\t");
  Serial.print(pitch,DEC);
  Serial.print("\t");
  Serial.print(roll,DEC);
  Serial.print("\t");
  Serial.print(tx_buffer[4],BIN);
}

void display_transmitted_data()
{
  Serial.print("\nTransmitted Data\t");
  Serial.print(throttle);
  Serial.print("\t");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(roll);
  Serial.print("\t");
  Serial.print(tx_buffer[4],BIN);
}

void display_rx_buffer()
{
  Serial.print("\nRX_buffer\t");
  Serial.print((rx_buffer[0] & 0b11111111) | ((rx_buffer[1]<<8) & 0b1100000000),DEC);
}
