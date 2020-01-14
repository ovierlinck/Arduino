package com.iba.amaris;

import java.util.Arrays;

public class RunningAverage
{
   public RunningAverage(int pNbSample)
   {
      mData = new int[pNbSample];
      Arrays.fill(mData, 0);
   }

   public void setNbSample(int pNbSample)
   {
      int[] oldData = mData;
      mData = new int[pNbSample];
      Arrays.fill(mData, 0);
      mCurrentTotal = 0;
      for (int i= 0; i < Math.min(pNbSample, oldData.length); i++)  // copy N most recent samples, N = min(newLength, oldLength)
      {
         int oldValue = oldData[(mIndex-1-i+oldData.length) % oldData.length];
         mData[(mIndex-1-i+mData.length) % mData.length] = oldValue;  // add legnth to ibdex, to make sure it is positive ( -1 % x = -1, NOT x-1)
         mCurrentTotal += oldValue;
      }
      
      mIndex = mIndex % mData.length;
   }
   
   public void addSample(int pSample)
   {
      mCurrentTotal -= mData[mIndex];
      mData[mIndex] = pSample;
      mCurrentTotal += pSample;
      mIndex = (mIndex + 1) % mData.length;
   }

   public double getAverage()
   {
      return mCurrentTotal / mData.length;
   }

   private int[] mData;

   /** Next slot for new value */
   private int mIndex = 0;
   private int mCurrentTotal = 0;

   /** For testing */
   public static void main(String[] pArgs)
   {
      RunningAverage avg = new RunningAverage(10);
      for (int i = 0; i < 100; i++)
      {
         avg.addSample(i);
         System.out.println(i + ":" + avg.getAverage());
      }
   }
}
