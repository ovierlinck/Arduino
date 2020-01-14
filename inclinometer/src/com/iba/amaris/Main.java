package com.iba.amaris;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class Main
{

   public static void main(String[] pArgs)
   {
      IParser<Integer> parser = new Parser();
      final Computer computer = new Computer();
      Collector<Integer> collector = new Collector<>();

      collector.setParser(parser);
      collector.setReceiver(computer);// new LoggerReceiverDecorator<>(computer);
      String portName = "COM9";
      if (pArgs.length>0)
      {
         portName = pArgs[0];
      }
      collector.setPortName(portName);

      Display display = new Display();
      JComponent chart = display.getCharts();

      computer.setDisplay(display);

      JPanel panel = new JPanel(new BorderLayout());
      panel.add(chart, BorderLayout.CENTER);

      final JTextField nbSampleTF = new JTextField(10);
      nbSampleTF.setText("" + computer.getRAnbSample());
      nbSampleTF.addActionListener(new ActionListener()
      {

         @Override
         public void actionPerformed(ActionEvent pE)
         {
            String text = nbSampleTF.getText();
            int value = Integer.parseInt(text);
            computer.setRAnbSample(value);
         }
      });

      final JTextField alphaTF = new JTextField(10);
      alphaTF.setText("" + computer.getFOAlpha());
      alphaTF.addActionListener(new ActionListener()
      {

         @Override
         public void actionPerformed(ActionEvent pE)
         {
            String text = alphaTF.getText();
            double value = Double.parseDouble(text);
            computer.setFOAlpha(value);
         }
      });

      JPanel tunePanel = new JPanel();
      tunePanel.add(new JLabel("RA nb samples"));
      tunePanel.add(nbSampleTF);
      tunePanel.add(new JLabel("1stOrder alpha"));
      tunePanel.add(alphaTF);

      panel.add(tunePanel, BorderLayout.SOUTH);

      JFrame frame = new JFrame("Inclinometer");
      frame.setContentPane(panel);
      frame.setSize(800, 800);
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.setVisible(true);

      collector.start();
   }

}
