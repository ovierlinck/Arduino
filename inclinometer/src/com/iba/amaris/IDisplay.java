package com.iba.amaris;

/**Interface offered to acquisition/computer to publish data to UI */
public interface IDisplay
{
   enum Axis {accelX, accelY, accelZ, trim, psi, heel} ;
   enum Nature {raw, callibrated, filtered1, filtered2, filtered3};
   
   void setIntValue(Axis pAxis, Nature pNature, long pMilli, int pValue);
   void setIntValue(String pName, long pMilli, int pValue);
   
   void setDoubleValue(Axis pAxis, Nature pNature, long pMilli, double pValue);
   void setDoubleValue(String pName, long pMilli, double pValue);
   
}
