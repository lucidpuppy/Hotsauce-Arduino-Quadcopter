void make_packets()        //total payload of 5bytes 4 analog joystics plus 9 bits to turn thing on and off
{
  //encrypt
  tx_buffer[0]= del_throttle & 0b11111111;                 //8 bits throttle
  tx_buffer[1]= ( (del_throttle>>8) & 0b00000001) | (bit_throttle<<1) | ((yaw <<2) & 0b11111100);   //2 bits throttle + 6 bits yaw
  tx_buffer[2]= ( (yaw>>6) & 0b00000011) | ((pitch<<2) & 0b01111100) | ((bit_yaw<<7) & 0b10000000);    //2 bits yaw + 5 bits pitch + 1 bit of yaw_sign
  tx_buffer[3]= ((roll) & 0b00011111) | ((bit_pitch<<5) & 0b00100000) | (bit_roll<<6) & 0b01000000 | ((ex_bit<<7) & 0b10000000); //5 bits roll + pitch sign + roll sign + 1 ex bit
  tx_buffer[4]= ex_bits[0] | (ex_bits[1]<<1) | (ex_bits[2]<<2) | (ex_bits[3]<<3) | (ex_bits[4]<<4) | (ex_bits[5]<<5) | (ex_bits[6]<<6) | (ex_bits[7]<<7);     //1 byte of extra data
  //Serial.print("\nPackets_ready");
}

