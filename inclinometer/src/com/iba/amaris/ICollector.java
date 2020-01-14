package com.iba.amaris;

/** T is the type for acceleration (Integer, Double,...)*/
public interface ICollector<T>
{
   void setReceiver(IReceiver<T> pReceiver);

   void start();
   void stop();
}
