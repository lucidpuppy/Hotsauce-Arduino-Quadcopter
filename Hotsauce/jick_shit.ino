void dead_stop()
{
    Serial.print("\nDEAD STOP");
    tim.writeMicroseconds(1000);
    tom.writeMicroseconds(1000);

    ted.writeMicroseconds(1000);
    tod.writeMicroseconds(1000);
    are_motors_cutoff=true;
    Serial.print("\nAre motors cutoff true");
}

void keep_busy()                                      //Keep busy when throttle is less than some particular value
{
  while((!is_cutoff_clear))
    {
      Serial.print("\nKeep busy");
      get_ypr();                                      //Just so when Quad is back on ground..it doesn't accumulates integral error whilethe circuit is still on
      offset_yaw=yaw;
      gett_rc();
      if(are_motors_cutoff==false) dead_stop();
      blink_led();
    }
}

