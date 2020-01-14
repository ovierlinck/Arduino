package com.iba.amaris;

public interface IParser<T>
{
   /** Feed data */
   void pushData(String pBuffer);

   /** ask for data extraction - can lead to 0, 1 or more calls to pReceiver.onNewData() (and onError()!) */
   void parse(IReceiver<T> pReceiver);
}
