package com.iba.amaris;

/** Receives data (String) and extract accelerations - expected syntax: "[accel  intAccelX intAccelY intAccelZ]" */
public class Parser implements IParser<Integer>
{

   private boolean mDebug = false;

   public void setDebug(boolean pDebug)
   {
      mDebug = pDebug;
   }

   @Override
   public void pushData(String pBuffer)
   {
      mIncomingData.append(pBuffer);
   }

   @Override
   public void parse(IReceiver<Integer> pReceiver)
   {
      debug("New buffer: /" + mIncomingData + "/");

      while (true)
      {
         int firstEnd = mIncomingData.indexOf(END_MARKER);
         debug("    firstEnd " + firstEnd);
         if (firstEnd == -1) // No end of sentence reached yet
         {
            debug("   not complete");
            return;
         }

         int firstStart = mIncomingData.indexOf(BEGIN_MARKER);
         debug("    firstStart " + firstStart);
         if (firstStart == -1 || firstStart > firstEnd) // We have a end without a start before
         {
            mIncomingData.delete(0, firstEnd + END_MARKER.length()); // Drop end of incomplete sentence
            debug("    drop incomplete sentence. Now is:");
            debug("    " + mIncomingData);
            continue; // and restart processing
         }

         String sentence = mIncomingData.substring(firstStart + BEGIN_MARKER.length(), firstEnd); // Sentence
                                                                                                  // does NOT
                                                                                                  // include
                                                                                                  // markers
         parseSentence(sentence, pReceiver);
         mIncomingData.delete(0, firstEnd + END_MARKER.length()); // Drop end of incomplete sentence

         debug("    Buffer is now /" + mIncomingData + "/");
      }
   }

   private void debug(String pString)
   {
      if (mDebug )
      {
         System.out.println(pString);
      }
   }

   private void parseSentence(String pSentence, IReceiver<Integer> pReceiver)
   {
      debug("        PARSE /" + pSentence + "/");

      String[] words = pSentence.trim().split(" +");
      if (words.length != 3)
      {
         pReceiver.onError("INCOMPLETE SENTENCE " + pSentence + " : found " + words.length + " words, expecting 3");
         return;
      }

      Integer x = Integer.parseInt(words[0]);
      Integer y = Integer.parseInt(words[1]);
      Integer z = Integer.parseInt(words[2]);

      if (pReceiver != null)
      {
         pReceiver.onNewData(System.currentTimeMillis(), x, y, z);
      }
   }

   private void clear()
   {
      mIncomingData = new StringBuilder();
   }

   /** Buffer accumulating data, until full sentence is received and data are pushed to IReceiver */
   private StringBuilder mIncomingData = new StringBuilder();

   private static final String BEGIN_MARKER = "[accel";
   private static final String END_MARKER = "]";

   /** Test code */
   public static void main(String[] pArgs)
   {
      IReceiver<Integer> receiver = new LoggerReceiverDecorator<Integer>();
      Parser parser = new Parser();
      parser.pushData(BEGIN_MARKER + " " + 1 + " " + 2 + " " + 3 + END_MARKER);
      parser.parse(receiver);
      parser.clear();
      parser.pushData("fjlkdsjfdsmk " + BEGIN_MARKER + " " + 1 + " " + 2 + " " + 3 + END_MARKER);
      parser.parse(receiver);
      parser.clear();
      parser.pushData("fjlkdsjfdsmk " + BEGIN_MARKER + " " + 1 + " " + 2 + " " + 3 + END_MARKER + "hfks hfdlkshfdskl fhks");
      parser.parse(receiver);
      parser.clear();
      parser.pushData("fjl]kdsjfdsmk " + BEGIN_MARKER + " " + 1 + " " + 2 + " " + 3 + END_MARKER + "fjl]kdsjfdsmk "
            + BEGIN_MARKER + " " + 1 + " " + 2 + " " + 3 + END_MARKER);
      parser.parse(receiver);
   }
}
