void calibrate_sensors()
{
  Serial.print("\nCalibrating sensors..");
  PORTB|= PORTB|=_BV(PB5);
  for (int i = beatles; i >= 0; i--)
  {
    get_ypr();
    //display_ypr();
    if (i == beatles / 2) {
      PORTB|= PORTB|=~_BV(PB5);
      Serial.print("\n\nAlmost done."); }
  }
  PORTB|= PORTB|=_BV(PB5);
  offset_yaw = yaw;                                   //Sets Initial value for Yaw to be current value
}

void initialize_mpu()
{
  Wire.begin();
  TWBR = 12;

  Serial.println(F("\nInitializing I2C devices..."));
  mpu.initialize();

  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));


  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(gyroX_offset);
  mpu.setYGyroOffset(gyroY_offset);
  mpu.setZGyroOffset(gyroZ_offset);
  mpu.setXAccelOffset(accelX_offset);
  mpu.setYAccelOffset(accelY_offset);
  mpu.setZAccelOffset(accelZ_offset);

  if (devStatus == 0)
  {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  }

  else
  {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void get_ypr()
{
prick:
  if (!dmpReady) return;

  while (!mpuInterrupt && fifoCount < packetSize){}
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    mpu.resetFIFO();
    Serial.println(F("******FIFO FUCK ALERT!!******"));
  }

  else if (mpuIntStatus & 0x02)
  {
    while (fifoCount < packetSize)
      fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);

    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    spudnut = micros();                                                                                            //calculating dt
    dt = (spudnut - donut) * 0.000001;
    donut = spudnut;

    yaw = ypr[0] * 180 / apple_pie;
    pitch = ypr[1] * 180 / apple_pie;
    roll = ypr[2] * 180 / apple_pie;
  }

  else
    goto prick;
}

void dmpDataReady() { mpuInterrupt = true; }

