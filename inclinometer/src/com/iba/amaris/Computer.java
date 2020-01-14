package com.iba.amaris;

import com.iba.amaris.IDisplay.Axis;
import com.iba.amaris.IDisplay.Nature;

public class Computer implements IReceiver<Integer>
{

   @Override
   public void onStarted()
   { // Nothing to do yet
   }

   @Override
   public void onStopped()
   {// Nothing to do yet
   }

   @Override
   public void onError(String pErrorMSg)
   {// Nothing to do yet
   }

   @Override
   public void onNewData(long pCurrentMillisec, Integer pAccelX, Integer pAccelY, Integer pAccelZ)
   {
      if (mDisplay==null)
      {
         return;
      }
      
      mAccelXAvg.addSample(pAccelX);
      mAccelYAvg.addSample(pAccelY);
      mAccelZAvg.addSample(pAccelZ);
      
      mAccelXfo.addSample(pAccelX);
      mAccelYfo.addSample(pAccelY);
      mAccelZfo.addSample(pAccelZ);

      double theta = 180.0 * Math.atan(pAccelX / Math.sqrt(pAccelY*pAccelY+pAccelZ*pAccelZ)) / Math.PI;
      double psi = 180.0 * Math.atan(pAccelY / Math.sqrt(pAccelX*pAccelX+pAccelZ*pAccelZ)) / Math.PI;
      double phi = 180.0 * Math.atan(pAccelZ / Math.sqrt(pAccelX*pAccelX+pAccelY*pAccelY)) / Math.PI;
      
      double accelXavg = mAccelXAvg.getAverage();
      double accelYavg = mAccelYAvg.getAverage();
      double accelZavg = mAccelZAvg.getAverage();
      
      double thetaAvg = 180.0 * Math.atan(accelXavg / Math.sqrt(accelYavg*accelYavg+accelZavg*accelZavg)) / Math.PI;
      double psiAvg = 180.0 * Math.atan(accelYavg / Math.sqrt(accelXavg*accelXavg+accelZavg*accelZavg)) / Math.PI;
      double phiAvg = 180.0 * Math.atan(accelZavg / Math.sqrt(accelXavg*accelXavg+accelYavg*accelYavg)) / Math.PI;

      double accelXfo = mAccelXfo.getValue();
      double accelYfo = mAccelYfo.getValue();
      double accelZfo = mAccelZfo.getValue();

      double thetaFo = 180.0 * Math.atan(accelXfo / Math.sqrt(accelYfo*accelYfo+accelZfo*accelZfo)) / Math.PI;
      double psiFo = 180.0 * Math.atan(accelYfo / Math.sqrt(accelXfo*accelXfo+accelZfo*accelZfo)) / Math.PI;
      double phiFo = 180.0 * Math.atan(accelZfo / Math.sqrt(accelXfo*accelXfo+accelYfo*accelYfo)) / Math.PI;

      mDisplay.setIntValue(Axis.accelX, Nature.raw, pCurrentMillisec, pAccelX);
      mDisplay.setIntValue(Axis.accelY, Nature.raw, pCurrentMillisec, pAccelY);
      mDisplay.setIntValue(Axis.accelZ, Nature.raw, pCurrentMillisec, pAccelZ);

      mDisplay.setDoubleValue(Axis.accelX, Nature.filtered1, pCurrentMillisec, accelXavg);
      mDisplay.setDoubleValue(Axis.accelY, Nature.filtered1, pCurrentMillisec, accelYavg);
      mDisplay.setDoubleValue(Axis.accelZ, Nature.filtered1, pCurrentMillisec, accelZavg);

      mDisplay.setDoubleValue(Axis.accelX, Nature.filtered2, pCurrentMillisec, accelXfo);
      mDisplay.setDoubleValue(Axis.accelY, Nature.filtered2, pCurrentMillisec, accelYfo);
      mDisplay.setDoubleValue(Axis.accelZ, Nature.filtered2, pCurrentMillisec, accelZfo);

      mDisplay.setDoubleValue(Axis.heel, Nature.raw, pCurrentMillisec, phi);
      mDisplay.setDoubleValue(Axis.psi, Nature.raw, pCurrentMillisec, psi);
      mDisplay.setDoubleValue(Axis.trim, Nature.raw, pCurrentMillisec, theta);

      mDisplay.setDoubleValue(Axis.heel, Nature.filtered1, pCurrentMillisec, phiAvg);
      mDisplay.setDoubleValue(Axis.psi, Nature.filtered1, pCurrentMillisec, psiAvg);
      mDisplay.setDoubleValue(Axis.trim, Nature.filtered1, pCurrentMillisec, thetaAvg);
      
      mDisplay.setDoubleValue(Axis.heel, Nature.filtered2, pCurrentMillisec, phiFo);
      mDisplay.setDoubleValue(Axis.psi, Nature.filtered2, pCurrentMillisec, psiFo);
      mDisplay.setDoubleValue(Axis.trim, Nature.filtered2, pCurrentMillisec, thetaFo);
}

   public void setDisplay(IDisplay pDisplay)
   {
      mDisplay = pDisplay;
   }

   public int getRAnbSample()
   {
      return mRAnbSample;
   }

   public void setRAnbSample(int pRAnbSample)
   {
      mRAnbSample = pRAnbSample;
      mAccelXAvg.setNbSample(pRAnbSample);
      mAccelYAvg.setNbSample(pRAnbSample);
      mAccelZAvg.setNbSample(pRAnbSample);
   }

   public double getFOAlpha()
   {
      return mAlpha;
   }

   public void setFOAlpha(double pAlpha)
   {
      mAlpha = pAlpha;
      mAccelXfo.setAlpha(mAlpha);
      mAccelYfo.setAlpha(mAlpha);
      mAccelZfo.setAlpha(mAlpha);
   }

   private IDisplay mDisplay;
   
   private int mRAnbSample = 100;

   private double mAlpha = 0.01;
   
   private RunningAverage mAccelXAvg = new RunningAverage(mRAnbSample);
   private RunningAverage mAccelYAvg = new RunningAverage(mRAnbSample);
   private RunningAverage mAccelZAvg = new RunningAverage(mRAnbSample);

   private FirstOrderLP mAccelXfo = new FirstOrderLP(mAlpha);
   private FirstOrderLP mAccelYfo = new FirstOrderLP(mAlpha);
   private FirstOrderLP mAccelZfo = new FirstOrderLP(mAlpha);
}
