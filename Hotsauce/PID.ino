void compute_pwm()
{
  /*                                                                                                                                //PID equations in simpler way
  yaw_correction_tim_tom = (YAW.kp) * (error[0]) + (YAW.kd) * (derrbydt[0]) +  (YAW.ki) * (errsumdt[0]);
  
  yaw_correction_ted_tod = -1 * yaw_correction_tim_tom;
  
  pitch_correction_ted = (PITCH.kp) * (error[1]) + (PITCH.kd) * (derrbydt[1]) + (PITCH.ki) * (errsumdt[1]);
  
  pitch_correction_tod = -1 * pitch_correction_ted;
  
  roll_correction_tom = (ROLL.kp) * (error[2]) + (ROLL.kd) * (derrbydt[2]) + (ROLL.ki) * (errsumdt[2]);
  
  roll_correction_tim = -1 * roll_correction_tom;
  */
  
  pwm_tom = setpoint_throttle + compute_PID(YAW.kp,YAW.ki,YAW.kd,error[0],derrbydt[0],errsumdt[0],&yaw_correction_tim_tom) + compute_PID(ROLL.kp,ROLL.ki,ROLL.kd,error[2],derrbydt[2],errsumdt[2],&roll_correction_tom) ;
  pwm_tim = setpoint_throttle + yaw_correction_tim_tom - roll_correction_tom;

  pwm_ted = setpoint_throttle - yaw_correction_tim_tom + compute_PID(PITCH.kp,PITCH.ki,PITCH.kd,error[1],derrbydt[1],errsumdt[1],&pitch_correction_ted);
  pwm_tod = setpoint_throttle - yaw_correction_tim_tom - pitch_correction_ted;

  pwm_tim = constrain(pwm_tim, 1000, 2000);
  pwm_tom = constrain(pwm_tom, 1000, 2000);
  pwm_ted = constrain(pwm_ted, 1000, 2000);
  pwm_tod = constrain(pwm_tod, 1000, 2000);
}

float compute_PID(float kp,float ki, float kd, float error, float derrbydt, float errsumdt,float *correction)
{
  *correction= kp * error + kd * derrbydt +  ki * errsumdt;
  return *correction;
}

void compute_error()
{
  error[0] = yaw - offset_yaw - setpoint_yaw;
  error[1] = pitch - setpoint_pitch;
  error[2] = roll - setpoint_roll;

  error[0] = constrain(error[0], yaw_min, yaw_max);
  error[1] = constrain(error[1], pitch_min, pitch_max);
  error[2] = constrain(error[2], roll_min, roll_max);

  derrbydt[0] = (error[0] - last_error[0]) / dt;
  derrbydt[1] = (error[1] - last_error[1]) / dt;
  derrbydt[2] = (error[2] - last_error[2]) / dt;

  derrbydt[0] = constrain(derrbydt[0], -80, 80);
  derrbydt[1] = constrain(derrbydt[1], -80, 80);
  derrbydt[2] = constrain(derrbydt[2], -80, 80);

  errsumdt[0] += (error[0]+last_error[0]) * 0.5 * dt;                         //Trapezoidal integration..its fairly simple
  errsumdt[1] += (error[1]+last_error[1]) * 0.5 * dt;
  errsumdt[2] += (error[2]+last_error[2]) * 0.5 * dt;
}

void update_last_values()
{
  last_error[0] = error[0];
  last_error[1] = error[1];
  last_error[2] = error[2];
  
  last_yaw_correction_tim_tom = yaw_correction_tim_tom;
  last_yaw_correction_ted_tod = yaw_correction_tim_tom;

  last_pitch_correction_ted = pitch_correction_ted;
  last_pitch_correction_tod = pitch_correction_tod;

  last_roll_correction_tim = roll_correction_tim;
  last_roll_correction_tom = roll_correction_tom;
}

void update_motors()
{
  tim.writeMicroseconds(pwm_tim);
  tom.writeMicroseconds(pwm_tom);

  ted.writeMicroseconds(pwm_ted);
  tod.writeMicroseconds(pwm_tod);
  are_motors_cutoff=false;
}
