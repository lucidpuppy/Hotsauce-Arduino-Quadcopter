void ignition()
{
  digitalWrite(LED_PIN, HIGH);

  tim.writeMicroseconds(1000);
  tom.writeMicroseconds(1000);
  ted.writeMicroseconds(1000);
  tod.writeMicroseconds(1000);

  delay(1000);
  Serial.print("\nArmed\n");

  PORTB|=~_BV(PB5);
  delay(300);
  PORTB|=_BV(PB5);
  delay(300);
  PORTB|=~_BV(PB5);
}

void calibrate_esc()
{
  tim.writeMicroseconds(throttle_max);
  tom.writeMicroseconds(throttle_max);
  ted.writeMicroseconds(throttle_max);
  tod.writeMicroseconds(throttle_max);

  delay(3500);

  tim.writeMicroseconds(throttle_min);
  tom.writeMicroseconds(throttle_min);
  ted.writeMicroseconds(throttle_min);
  tod.writeMicroseconds(throttle_min);

  delay(3500);
}
void program_esc()
{
  while (1)
  {
    tim.writeMicroseconds(setpoint_throttle);
    tom.writeMicroseconds(0);
    ted.writeMicroseconds(0);
    tod.writeMicroseconds(0);
  }
}
