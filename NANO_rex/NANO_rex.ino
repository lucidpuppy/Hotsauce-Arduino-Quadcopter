/*  RX Node Recieves 5 bytes payload to TX node.
 *  10 bits for throttle..
 *  6 bits for pitch and roll..max and min -30 to 30
 *  9 bits Yaw
 *  1 extra bit
 *  1 byte for switches and stuff
 *  Enhanced Shockburst is been enabled, one can disable it in the config_nrf_rx()
 *  one can modify the payload size in RX config of the RX node
 *  comments and queries <brian.boozebacon@outlook.com>
 *  https://tachymoron.wordpress.com
 */

#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13
#define SS_ARD_PIN 10
#define SS_PIN 9    //PB1
#define CE_PIN 8  //CE HIGH for RX  //PB0
#define IRQ 2

#define flush_tx 0b11100001
#define flush_rx 0b11100010

#define rd_rx_payload 0b01100001
#define wt_tx_payload 0b10100000

#define set_mode() SPCR&=0xf3
#define set_data_order() SPCR&=0xdf
#define begin_spi() SPCR|=0x40

#define nrf_config     0x00
#define en_AA 0x01
#define en_rx_addr     0x02
#define setup_aw       0x03
#define setup_retr     0x04
#define rf_ch          0x05
#define rf_setup       0x06
#define nrf_status     0x07
#define observe_tx     0x08
#define rpd            0x09
#define rx_addr_p0     0x0a
#define rx_addr_p1     0x0b
#define rx_addr_p2     0x0c
#define rx_addr_p3     0x0d
#define rx_addr_p4     0x0e
#define rx_addr_p5     0x0f
#define tx_addr        0x10
#define rx_pw_p0       0x11
#define rx_pw_p1       0x12
#define rx_pw_p2       0x13
#define rx_pw_p3       0x14
#define rx_pw_p4       0x15
#define rx_pw_p5       0x16
#define fifo_status    0x17
#define dyn_payload    0x1c
#define feature_reg    0x1d

#include <Wire.h>
#define ki 0.002
int del_throttle, yaw, pitch, roll;
boolean bit_throttle,bit_yaw,bit_pitch,bit_roll,ex_bit,ex_bits[8];
float throttle;

boolean demake;
uint32_t spudnut,donut;
float dt;

byte data_in[2],rx_buffer[5],tx_buffer[2],rx_tx_addr[3] = {0x1a, 0x2b, 0x3c},bits;
byte twi_buffer[5];
boolean nrf=false,nrf_mode;                     //nrf_mode -1 for rx and 0 for tx for RX node

uint32_t banana,cream;

void setup()
{
  cli();
  Serial.begin(250000);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  init_spi();                     //intialize SPI on arduino
  set_sck_mode_order();           //set clock rate mode and data order
  begin_spi();                    //set SPI enable bit in SPCR
  Serial.print("\nnRF_test\t");
  Serial.print(test_nrf());
  config_nrf_rx();
  clear_interrupts();
  sei();
}

void loop()
{
    //display_rx_buffer();
    display_rc();
    update_twi_buffer();
    delay(10);
}

void requestEvent()
{
  Wire.write(twi_buffer,5);
}

void update_twi_buffer()                                    //This buffer is sent to host microcontroller which is Arduino Nano..
{ 
  twi_buffer[0]=rx_buffer[0];
  twi_buffer[1]=rx_buffer[1];
  twi_buffer[2]=rx_buffer[2];
  twi_buffer[3]=rx_buffer[3] | transmitter_is()<<7;         //Packet is modified so as to add staus of the RC if it is on or Off so as to intialize automatic landing.
  twi_buffer[4]=rx_buffer[4] & 0b11111111;
}


