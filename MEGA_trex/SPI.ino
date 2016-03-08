void set_sck_mode_order()
{
  set_sck();
  set_mode();
  set_data_order();
}

void set_sck()
{
  SPCR &= 0xFC;
  SPSR |= 0x01;
}


ISR(SPI_STC_vect)
{
  Serial.print("\nArduino ISR CALLED");
}

void ss_low()
{
  PORTL= PORTL & 0b11111101;
}

void ss_high()
{
  PORTL= PORTL | 0b00000010;
}

void init_spi()
{
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(SS_ARD_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  pinMode(IRQ, INPUT);
  SPCR = 0x00;
  SPCR |= 0x10; //Interrupt disabled,Arduino as Master.
  delay(2);
}

byte send_byte(byte data)           //send byte via spi
{
  ss_low();
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));
  data_in[0] = SPDR;
  ss_high();
  return data_in[0];
}

byte send_byte_ns(byte data)      //send byte via SPI non stop..just like spi.transfer
{
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}
