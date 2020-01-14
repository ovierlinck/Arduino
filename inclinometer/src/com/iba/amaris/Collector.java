package com.iba.amaris;

import jssc.*;

/** Connect to sensor and retrieve X,Y,Z acceleration measure, and notify associated IReceiver (if any) as soon as data is ready */
public class Collector<T> implements ICollector<T>
{

   public void setPortName(String pPortName)
   {
      mSerialPort = new SerialPort(pPortName);
   }

   public void setBaudRate(int pBaudRate)
   {
      mBaudRate = pBaudRate;
   }
   
   public void setParser(IParser<T> pParser)
   {
      mParser = pParser;
   }

   /** Receiver can be null and can be changed will Collector is running */
   @Override
   public void setReceiver(IReceiver<T> pReceiver)
   {
      mReceiver = pReceiver;
   }

   @Override
   public void start()
   {
      System.out.println("Start handling data from " + mSerialPort.getPortName());
      
      if (mSerialPort==null)
      {
         reportError("No serial port defined - configure first using Collector.setPortName()");
         return;
      }
      try
      {
         mSerialPort.openPort();
         mSerialPort.setParams(mBaudRate, 8, 1, 0);
         int mask = SerialPort.MASK_RXCHAR;// + SerialPort.MASK_CTS + SerialPort.MASK_DSR;
         mSerialPort.setEventsMask(mask);
         mSerialPort.addEventListener(new SerialPortEventListener()
         {

            @Override
            public void serialEvent(SerialPortEvent pEvent)
            {
               {
                  try
                  {
                     String buffer = mSerialPort.readString();
                     if (buffer != null)
                     {
                        processNewBuffer(buffer);
                     }
                  }
                  catch (SerialPortException e)
                  {
                     reportError(e);
                  }
               }
            }
         });
      }
      catch (SerialPortException e)
      {
         reportError(e);
         return;
      }

      if (mReceiver != null)
      {
         mReceiver.onStarted();
      }
   }

   protected void processNewBuffer(String pBuffer)
   {
      mParser.pushData(pBuffer);
      mParser.parse(mReceiver); // even if mReceiver is null, to avoid buffer overflow
   }

   private void reportError(Exception pExc)
   {
      reportError(pExc.toString());
   }
   
   private void reportError(String pMsg)
   {
      if (mReceiver != null)
      {
         mReceiver.onError(pMsg);
      }
      else
      {
         System.err.println("COLLECTOR ERROR :" + pMsg);
      }
   }

   @Override
   public void stop()
   {
      if (mReceiver != null)
      {
         mReceiver.onStopped();
      }
   }

   private SerialPort mSerialPort;
   private int mBaudRate;
   private IParser<T> mParser;
   private IReceiver<T> mReceiver;
   
   /** Test code*/
   public static void main(String[] pArgs)
   {
      IParser<Integer> parser = new Parser();
      IReceiver<Integer> receiver = new LoggerReceiverDecorator<>();
      Collector<Integer> collector = new Collector<>();
      
      collector.setParser(parser);
      collector.setReceiver(receiver);
      
      collector.setPortName("COM9");
      collector.start();
   }

}
