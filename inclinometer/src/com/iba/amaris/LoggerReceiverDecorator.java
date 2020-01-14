package com.iba.amaris;

/** Trace all calls to decorated Receiver. Receiver can be null and changed at any time*/
public class LoggerReceiverDecorator<T> implements IReceiver<T>
{
   
   public LoggerReceiverDecorator()
   {
      super();
   }

   public LoggerReceiverDecorator(IReceiver<T> pReceiver)
   {
      super();
      mReceiver = pReceiver;
   }

   public void setReceiver(IReceiver<T> pReceiver)
   {
      mReceiver = pReceiver;
   }
   
   @Override
   public void onStarted()
   {
      System.out.println("onStarted");
      if (mReceiver!=null)
      {
         mReceiver.onStarted();
      }
   }

   @Override
   public void onStopped()
   {
      System.out.println("onStopped");
      if (mReceiver!=null)
      {
         mReceiver.onStopped();
      }
   }

   @Override
   public void onError(String pErrorMSg)
   {
      System.out.println("onError(" + pErrorMSg + ")");
      if (mReceiver!=null)
      {
         mReceiver.onError(pErrorMSg);
      }
   }

   @Override
   public void onNewData(long pCurrentMillisec, T pAccelX, T pAccelY, T pAccelZ)
   {
      System.out.println("onNewData(" + pCurrentMillisec + ", " + pAccelX + ", " + pAccelY + ", " + pAccelZ + ")");
      if (mReceiver!=null)
      {
         mReceiver.onNewData(pCurrentMillisec, pAccelX, pAccelY, pAccelZ);
      }
   }

   private IReceiver<T> mReceiver;

}
