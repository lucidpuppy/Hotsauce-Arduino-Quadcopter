boolean transmitter_is()      //returns true if transmitter is on
{
  cream=micros();
  if( ((cream-banana)* 0.000001)>0.04 )   //rx interrupt at 50 Hz frequency
      return false;
   else
    return true;
}
