package com.iba.amaris;


public class FirstOrderLP
{
   public FirstOrderLP(double pAlpha)
   {
      mAlpha = pAlpha;
   }

   public void setAlpha(double pAlpha)
   {
      mAlpha = pAlpha;
   }
   
   public void addSample(int pSample)
   {
      mValue = mValue + mAlpha * (pSample-mValue); 
   }

   public double getValue()
   {
      return mValue;
   }

   private double mValue;
   private double mAlpha;

}
