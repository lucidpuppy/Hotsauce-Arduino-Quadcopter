/*   Open Source Quadcopter project, that humans can understand.
 *   codename [HotSauce]
 *   This is a version with RC transmitter made from nRF24L01+ module.
 *   by Brian Boozebacon
 *   <hardik.mad@gmail.com>
 *   <brian.boozebacon@gmail.com>
 *   https://tachymoron.wordpress.com/
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"                                                                
#include "Wire.h"
#include <Servo.h>

/*                                                                                              //For reference while tuning
yaw_kp 1.5          1.58   1.3                                                                                                    YAW      0.674  0.231  0.4367     0.674  0.207  0.29 GOOOOOOOOD
yaw_kd 1.1          1.3  1.3   0.4   0.476   0.6  0.5        0.426666
yaw_ki 0            0.001   0.3  0.08     0.376   0.09  0.16   0.176    0.3  0.1    0.089   0.05  0.13

pitch_kp 1.4        0.89  0.063   0724  0.216  0.29                                                                               PITCH
pitch_kd 0.734      0.2546    0.09    0.12   0.015   0.22
pitch_ki 0.15       0.4367    0.4   0.63    0.000008   0.384

roll_kp 1.4         1.01  0.9    0.61   0.89  0.68   0.65  0.57                                                                   ROLL  Fuckking good-  0.86  0.093 0.16   0.786  0.093  0.16
roll_kd 0.734       0.207  0.198   0.083    0.075   0.09                                                            
roll_ki 0.15        0.2 0.29  0.315  0.56   0.026  0.029  IMP  0.002175    0.0105   0.06  b/w  0.05   to   0.01
*/

const struct tuning                                                                               //constant structure for Kp Ki and Kd of 2 axis 
{
  float kp,ki,kd;
} YAW={1.5,0,1.1},PITCH={1.4,0.15,0.734},ROLL={1.4,0.15,0.734};                       //Kp Ki Kd values for YAW PITCH and ROLL axis, which are actually used
                                                                                      //make sure while tuning Pitch or Roll individually, SET Kp Ki Kd for yaw to ZERO

#define kii 0.002                                                                     //variable to accumulate Throttle
#define apple_pie 3.14159

#define throttle_max 2000
#define throttle_min 1000
#define yaw_limit 160
#define pitch_roll_limit 30
                                                                                      //Error limits for PID's. PID loop will correct till 45 after that..error= constant=45
#define yaw_max 45                                                                    //Limits Yaw movements from remote to 45 degrees..change it desirably
#define yaw_min -45                                                                   //but once it gets closer to 180 there's a sudden shift from 180 to -180, which causes problem.

#define pitch_max 45
#define pitch_min -45

#define roll_max 45
#define roll_min -45

#define beatles 800                                                                    //No of MPU6050 readings which are throw away at the starting..
#define LED_PIN 13

#define gyroX_offset 55                                                                 //Gyro and Aceelerometer offsets..One must change these to the ones for your chip
#define gyroY_offset 4                                                                  // http://www.i2cdevlib.com/forums/topic/112-another-auto-offset-calibration-sketch/
#define gyroZ_offset 26

#define accelX_offset -4610
#define accelY_offset 559
#define accelZ_offset 1279

#define interval 50                                                                    //LED blink interval

//PID
float error[3] = {0, 0, 0}, last_error[3] = {0, 0, 0}, derrbydt[3] = {0, 0, 0}, errsumdt[3] = {0, 0, 0};

float yaw_correction_tim_tom, yaw_correction_ted_tod, last_yaw_correction_tim_tom, last_yaw_correction_ted_tod;

float pitch_correction_ted, pitch_correction_tod, last_pitch_correction_ted, last_pitch_correction_tod;

float roll_correction_tim, roll_correction_tom, last_roll_correction_tim, last_roll_correction_tom;

float pwm_tim, pwm_tom, pwm_ted, pwm_tod;


//MPU6050
MPU6050 mpu;                                                                                            //MPU6050 object
bool dmpReady = false;                                                                                  // set true if DMP init was successful
uint8_t mpuIntStatus;                                                                                   // holds actual interrupt status byte from MPU
uint8_t devStatus;                                                                                      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;                                                                                    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;                                                                                     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];                                                                                 // FIFO storage buffer
float ypr[3] = {0, 0, 0};                                                                               // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
Quaternion q;
VectorFloat gravity;

volatile bool mpuInterrupt = false;                                                                     // indicates whether MPU interrupt pin has gone high
float yaw, pitch, roll;

uint32_t spudnut, donut;                                                                                 //spudnut stores micros() and donut stores spudnut
double dt;
float offset_yaw = 0;


//Radio
float setpoint_throttle;
int setpoint_yaw, setpoint_pitch, setpoint_roll;
int del_throttle;
byte rx_buffer[5],bits;
boolean bit_throttle,bit_yaw,bit_roll,bit_pitch,exbit,rc_state=true;
boolean is_dead_stop=false;

//Servo objects
Servo tim, tom;                                                                                         //servo objects
Servo ted, tod;                                                                                          // four objects for 4 Motors..group of two.

//jick shit
unsigned long tea, last_tea;
bool led_state, transmitter_is = false, is_cutoff_clear=false,are_motors_cutoff=false;


void setup()
{
  Serial.begin(250000);
  
  Serial.print("START\n");
  
  pinMode(LED_PIN, OUTPUT);
  
  ground_it();                                                //Analog pins are used to provide Arduino's ground to ESCs
  
  attach_timtom();                                            //Tim and Tom on Roll axis
  
  attach_tedtod();                                            //Ted and Tod on Pitch axis
  
  ignition();                                                 //Arms ESC
  
  //program_esc();
  
  //calibrate_esc();
  
  wdt_enable(WDTO_500MS);                                     //Watchdog incase connection is unsuccessfull
  
  initialize_mpu();
  
  wdt_disable();
  
  calibrate_sensors();                                        //Reads Yaw Pitch Roll from MPU6050..and throws them aways since for first few seconds values drifts

  Serial.print("\n\nMain control loop started");
}


void loop()
{
  if(is_dead_stop==false)                                             //Dead stop is what I configured with Switches from my nR24L01+ based RC transmitter
  {
    keep_busy();
    
    get_ypr();                                                        //Gets YPR from MPU6050
    
    gett_rc();                                                        //Reads RC from Arduino Nano..via I2C
    
    compute_error();                                                  //Computes Error
    
    compute_pwm();                                                    //Computes PWM and PID
    
    update_motors();                                                  //Writes new PWM values to motor ESCs
    
    update_last_values();                                             //Puts current values into Last values for future calculations
    
    blink_led();                                                      //Blinks LEDs
    
    //display_ypr();
    
    //display_err();
    
    //display_pitch_ax_pwm();
    
    //display_roll_ax_pwm();
    
    //display_errsumdt();
    
    //display_deri();
    
    //log_eeprom();
    
    //display_loop_freq();
    
    //display_yaw_correction();
    
    //display_rc();
    
    //display_throttle_yaw();
    
    Serial.print("\n\nFucked\n\n");
  }

  else
  { 
    while(is_dead_stop)
    {
      if(are_motors_cutoff==false) dead_stop();
      PORTB|= PORTB|=_BV(PB5);
    }
  }
  
}                                                                                                     //loop ends


void attach_timtom()
{
  tim.attach(4);                                                                                     //clock
  tom.attach(5);                                                                                     //clock
}

void attach_tedtod()
{
  ted.attach(6);                                                                                      //anti clock
  tod.attach(7);                                                                                      //anti clock
}

void ground_it()
{
  analogWrite(A0, LOW);

  analogWrite(A1, LOW);

  analogWrite(A2, LOW);

  analogWrite(A3, LOW);
}

void blink_led()
{
  tea = millis();
  if (tea - last_tea >= interval)
  {
    last_tea = tea;
    led_state=!led_state;
    if(led_state==1) PORTB|=_BV(PB5);                                     //PB5= 13 on arduino
    else PORTB&= ~_BV(PB5);
  }
}

inline void reset_arduino()
{
  asm volatile("jmp 0");
}
