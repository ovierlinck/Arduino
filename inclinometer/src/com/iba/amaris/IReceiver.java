package com.iba.amaris;

public interface IReceiver<Type>
{
   void onStarted();
   void onStopped();
   void onError(String pErrorMSg);
   void onNewData(long pCurrentMillisec, Type pAccelX, Type pAccelY, Type pAccelZ);
}
