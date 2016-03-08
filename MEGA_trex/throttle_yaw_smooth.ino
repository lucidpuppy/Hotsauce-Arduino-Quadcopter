void smooth_throttle_yaw()                                                                //this function is to increase throttle value smoothly when its up and decrease when its down
{                                                                                         //same ways for the yaw
  if(throttle>=510 && throttle<=1000) del_throttle=map(throttle,510,1000,0,500);
  
  else if(throttle>490 && throttle<510) del_throttle=0;

  else del_throttle=map(throttle,490,0,0,-250);

  if(del_throttle<0){ bit_throttle=true; del_throttle=-del_throttle;}
  else {bit_throttle=false;}
  
  target_yaw=yaw;
  drag_yaw=drag_yaw + ki_y * (target_yaw-drag_yaw) * dt + (target_yaw-drag_yaw)* 0.006;
  drag_yaw=constrain(drag_yaw,-180,180);
  
  if(drag_yaw<0){ bit_yaw=HIGH; yaw=-drag_yaw;}
  else {bit_yaw=LOW; yaw=drag_yaw;}
}

