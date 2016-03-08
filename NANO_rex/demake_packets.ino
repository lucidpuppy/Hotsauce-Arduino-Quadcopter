void demake_packets()
{
    del_throttle= (rx_buffer[0] & 0b11111111) | ((rx_buffer[1]<<8) & 0b100000000);
    bit_throttle=((rx_buffer[1]>>1) & 0b00000001);
    if(bit_throttle==HIGH) del_throttle=-del_throttle;
    
    throttle= throttle + del_throttle*ki;
    throttle=constrain(throttle,0,1000);
    
    yaw= ((rx_buffer[1]>>2) & 0b00111111) | ((rx_buffer[2] <<6) & 0b11000000);
    bit_yaw= ((rx_buffer[2]>>7) & 0b00000001);
    if(bit_yaw==HIGH) yaw=-yaw;
    
    pitch= (rx_buffer[2]>>2) & 0b00011111;
    bit_pitch= ((rx_buffer[3]>>5) & 0b00000001);
    if(bit_pitch==HIGH) pitch=-pitch;
    
    roll= rx_buffer[3] & 0b00011111;
    bit_roll= ((rx_buffer[3]>>6) & 0b00000001);
    if(bit_roll==HIGH) roll=-roll;
    
    ex_bit= ((rx_buffer[3]>>7) & 0b00000001);
    bits= rx_buffer[4];
    
    demake=true;
}
