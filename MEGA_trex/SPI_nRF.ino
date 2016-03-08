boolean test_nrf()
{
  write_reg(0x00, 0x0A);
  if(read_reg(0x00)==0x0a)
  {
    write_reg(0x00, 0x1A);
    if(read_reg(0x00)==0x1a) nrf=true;
  }
  return nrf;
}

void clear_interrupts()
{
  write_reg_bit(0x07, 4, 1);                                //clear all interrupt flags before starting
  write_reg_bit(0x07, 5, 1);
  write_reg_bit(0x07, 6, 1);
}

void write_reg(byte address, byte value)
{
  ss_low();
  SPDR = 32 + address;
  while (!(SPSR & (1 << SPIF)));
  data_in[0] = SPDR;

  SPDR = value;
  while (!(SPSR & (1 << SPIF)));
  data_in[1] = SPDR;
  ss_high();
}

void write_reg_bit(byte address, byte bit_no, boolean value)
{
  data_in[1] = read_reg(address);
  if (value == 1)
    bitSet(data_in[1], bit_no);
  else if (value == 0)
    bitClear(data_in[1], bit_no);

  write_reg(address, data_in[1]);
}

void write_addr_reg(byte address, byte value[3])
{
  ss_low();
  SPDR = 32 + address;
  while (!(SPSR & (1 << SPIF)));
  SPDR = value[0];
  while (!(SPSR & (1 << SPIF)));
  SPDR = value[1];
  while (!(SPSR & (1 << SPIF)));
  SPDR = value[2];
  while (!(SPSR & (1 << SPIF)));
  ss_high();
}

byte read_reg(byte address)
{
  ss_low();
  SPDR = address;
  while (!(SPSR & (1 << SPIF)));
  data_in[0] = SPDR;

  SPDR = 0b00000000;
  while (!(SPSR & (1 << SPIF)));
  data_in[1] = SPDR;
  ss_high();
  return data_in[1];
}

boolean read_reg_bit(byte address, byte bit_no)
{
  data_in[1] = read_reg(address);
  return ((data_in[1] >> bit_no) & 0x01);
}
