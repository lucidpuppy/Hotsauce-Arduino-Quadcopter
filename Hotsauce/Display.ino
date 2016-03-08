void display_err()
{
  Serial.print("\nERR\tYPR");
  Serial.print("\t");
  Serial.print(error[0]);
  Serial.print("\t");
  Serial.print(error[1]);
  Serial.print("\t");
  Serial.print(error[2]);
}

void display_yaw_correction()
{
  Serial.print("\nTIMTOM YAW\tTEDTOD YAW\t");
  Serial.print(yaw_correction_tim_tom);
  Serial.print('\t');
  Serial.print(yaw_correction_ted_tod);
}

void display_errsumdt()
{
  Serial.print("\nErrsumdt YPR");
  Serial.print('\t');
  Serial.print(errsumdt[0]);
  Serial.print('\t');
  Serial.print(errsumdt[1]);
  Serial.print('\t');
  Serial.print(errsumdt[2]);
}
void display_ypr()
{
  Serial.print("\nYPR");
  Serial.print("\t");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(roll);
}

void display_deri()
{
  Serial.print("\nderrbydt YPR\t");
  Serial.print(derrbydt[0]);
  Serial.print('\t');
  Serial.print(derrbydt[1]);
  Serial.print('\t');
  Serial.print(derrbydt[2]);
}

void display_rc()
{
  Serial.print("\nSetpoint THROTTLE YPR\t");
  Serial.print(setpoint_throttle);
  Serial.print('\t');
  Serial.print(setpoint_yaw);
  Serial.print('\t');
  Serial.print(setpoint_pitch);
  Serial.print('\t');
  Serial.print(setpoint_roll);
  Serial.print('\t');
  Serial.print(bits,BIN);
  Serial.print("\tTransmitter is");
  if(transmitter_is==true) Serial.print(" ON");
  else Serial.print(" OFF");
}


void display_pitch_ax_pwm()
{
  Serial.print("\nTED TOD\t");
  Serial.print(pwm_ted);
  Serial.print('\t');
  Serial.print(pwm_tod);
}

void display_roll_ax_pwm()
{
  Serial.print("\nTIM TOM\t");
  Serial.print(pwm_tim);
  Serial.print('\t');
  Serial.print(pwm_tom);
}


void display_loop_freq()
{
  Serial.print("\nFreq\t");
  Serial.print((double)1 / dt);
  Serial.print("Hz");
}

