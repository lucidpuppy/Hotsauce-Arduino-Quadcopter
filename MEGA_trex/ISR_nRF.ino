void ISR_NRF()
{
  byte status_nrf;
  status_nrf = read_reg(nrf_status);
  
  if ((status_nrf >> 6) & 0x01)
    rx_interrupt();
  if ((status_nrf >> 5) & 0x01)
    tx_interrupt();
  if ((status_nrf >> 4) & 0x01)
    max_rt();
}

void tx_interrupt()
{
  Serial.print("\nTX_interrupt");
  write_reg_bit(nrf_status,5,1);                  //Clear Interrupt flag in STATUS reg of nRF very essential..because flag isn't automatically cleared when ISR is called
}

void max_rt()
{
  Serial.print("\nMAX_RT");
  write_reg_bit(nrf_status, 4, 1);
}

void rx_interrupt()                              //ISR for reciever
{
  Serial.print("\nRX_interrupt");
  ss_low();
  data_in[0] = send_byte_ns(rd_rx_payload);   //Instruction to read payload
  rx_buffer[0] = send_byte_ns(0);             //Read
  rx_buffer[1] = send_byte_ns(0);             //Read 
  ss_high();
  delay(2);
  write_reg_bit(0x07, 6, 1);                //Clear Interrupt flag in STATUS reg of nRF very essential..because flag isn't automatically cleared when ISR is called
}


