package com.iba.amaris;

import info.monitorenter.gui.chart.Chart2D;
import info.monitorenter.gui.chart.ITrace2D;
import info.monitorenter.gui.chart.rangepolicies.RangePolicyFixedViewport;
import info.monitorenter.gui.chart.traces.Trace2DLtd;
import info.monitorenter.util.Range;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;
import java.util.*;
import java.util.Map.Entry;

import javax.swing.*;

public class Display implements IDisplay
{

   private static final Color COLOR_X1 = new Color(255, 128, 128);
   private static final Color COLOR_Y1 = new Color(128, 255, 128);
   private static final Color COLOR_Z1 = new Color(128, 128, 255);
   
   private static final Color COLOR_X2 = new Color(255, 30, 30);
   private static final Color COLOR_Y2 = new Color(30, 255, 30);
   private static final Color COLOR_Z2 = new Color(30, 30, 255);
   
   private static final Color COLOR_X3 = new Color(128, 0, 0);
   private static final Color COLOR_Y3 = new Color(0, 128, 0);
   private static final Color COLOR_Z3 = new Color(0, 0, 128);

   public Display()
   {
      createTraces();
      mAccelChart.setSynchronizedXStartChart(mInclinChart);
      mInclinChart.getAxisY().setRangePolicy(new RangePolicyFixedViewport(new Range(-50, +50)));
   }

   @Override
   public void setIntValue(Axis pAxis, Nature pNature, long pMilli, int pValue)
   {
      setIntValue(getName(pAxis, pNature), pMilli, pValue);
   }

   @Override
   public void setIntValue(String pName, long pMilli, int pValue)
   {
      setDoubleValue(pName, pMilli, pValue); // Anyway, Trace works only with double...
   }

   @Override
   public void setDoubleValue(Axis pAxis, Nature pNature, long pMilli, double pValue)
   {
      setDoubleValue(getName(pAxis, pNature), pMilli, pValue);
   }

   @Override
   public void setDoubleValue(String pName, long pMilli, double pValue)
   {

      // Update time plot
      ITrace2D trace = mTraces.get(pName);
      if (trace == null)
      {
         System.out.println("No trace defined for " + pName); // TODO: avoid repeating?
         return;
      }

      trace.addPoint(pMilli, pValue);
      
      // Update digital display
      JLabel numberDisplay = mNumberDisplays.get(pName);
      if (numberDisplay!=null)
      {
         numberDisplay.setText(""+(int)pValue);
      }
      
      // Update boat view
      if (pName.equals(getName(Axis.heel, Nature.filtered1)))
      {
         mHeelViewer1.setTilt(pValue);
      }
      else if (pName.equals(getName(Axis.trim, Nature.filtered1)))
      {
         mTrimViewer1.setTilt(pValue);
      }
      else if (pName.equals(getName(Axis.heel, Nature.filtered2)))
      {
         mHeelViewer2.setTilt(pValue);
      }
      else if (pName.equals(getName(Axis.trim, Nature.filtered2)))
      {
         mTrimViewer2.setTilt(pValue);
      }
   }

   public JComponent getCharts()
   {
      if (mPanel == null)
      {
         mPanel = new JPanel(new BorderLayout());
         Box chartPanel = new Box(BoxLayout.Y_AXIS);
         chartPanel.add(mAccelChart);
         chartPanel.add(mInclinChart);
         mPanel.add(chartPanel, BorderLayout.CENTER);
         
         JPanel rightPanel = new JPanel(new BorderLayout());
         rightPanel.add(createNumbersPanel(), BorderLayout.NORTH);
         rightPanel.add(createBoatAnglePanel(), BorderLayout.CENTER);

         mPanel.add(rightPanel, BorderLayout.EAST);
      }
      return mPanel;
   }

   private JComponent createNumbersPanel()
   {
      Box panel = new Box(BoxLayout.Y_AXIS);
      
      for(Entry<String, JLabel> entry : mNumberDisplays.entrySet())
      {
         panel.add(new JLabel(entry.getKey()));
         panel.add(entry.getValue());
      }
      
      return panel;
   }

   private JComponent createBoatAnglePanel()
   {
      JPanel panel = new JPanel(new GridLayout(2,2));
      
      mHeelViewer1 = new BoatAngleViewer(true);
      mTrimViewer1 = new BoatAngleViewer(false);
      mHeelViewer2 = new BoatAngleViewer(true);
      mTrimViewer2 = new BoatAngleViewer(false);
      panel.add(mHeelViewer1);
      panel.add(mTrimViewer1);
      panel.add(mHeelViewer2);
      panel.add(mTrimViewer2);
      
      return panel;
   }

   private void createTraces()
   {
      addTrace(mAccelChart, IDisplay.Axis.accelX, IDisplay.Nature.raw, COLOR_X1);
      addTrace(mAccelChart, IDisplay.Axis.accelX, IDisplay.Nature.filtered1, COLOR_X2);
      addTrace(mAccelChart, IDisplay.Axis.accelX, IDisplay.Nature.filtered2, COLOR_X3);
      
      addTrace(mInclinChart, IDisplay.Axis.heel, IDisplay.Nature.raw, COLOR_X1);
      addTrace(mInclinChart, IDisplay.Axis.heel, IDisplay.Nature.filtered1, COLOR_X2);
      addTrace(mInclinChart, IDisplay.Axis.heel, IDisplay.Nature.filtered2, COLOR_X3);

      addTrace(mAccelChart, IDisplay.Axis.accelY, IDisplay.Nature.raw, COLOR_Y1);
      addTrace(mAccelChart, IDisplay.Axis.accelY, IDisplay.Nature.filtered1, COLOR_Y2);
      addTrace(mAccelChart, IDisplay.Axis.accelY, IDisplay.Nature.filtered2, COLOR_Y3);

      addTrace(mInclinChart, IDisplay.Axis.psi, IDisplay.Nature.raw, COLOR_Y1);
      addTrace(mInclinChart, IDisplay.Axis.psi, IDisplay.Nature.filtered1, COLOR_Y2);
      addTrace(mInclinChart, IDisplay.Axis.psi, IDisplay.Nature.filtered2, COLOR_Y3);
      
      addTrace(mAccelChart, IDisplay.Axis.accelZ, IDisplay.Nature.raw, COLOR_Z1);
      addTrace(mAccelChart, IDisplay.Axis.accelZ, IDisplay.Nature.filtered1, COLOR_Z2);
      addTrace(mAccelChart, IDisplay.Axis.accelZ, IDisplay.Nature.filtered2, COLOR_Z3);

      addTrace(mInclinChart, IDisplay.Axis.trim, IDisplay.Nature.raw, COLOR_Z1);
      addTrace(mInclinChart, IDisplay.Axis.trim, IDisplay.Nature.filtered1, COLOR_Z2);
      addTrace(mInclinChart, IDisplay.Axis.trim, IDisplay.Nature.filtered2,COLOR_Z3);
   }

   private void addTrace(Chart2D pChart, IDisplay.Axis pAxis, IDisplay.Nature pNature, Color pColor)
   {
      addTrace(pChart, getName(pAxis, pNature), pColor);
   }

   private void addTrace(Chart2D pChart, String pName, Color pColor)
   {
      final ITrace2D trace = new Trace2DLtd(NB_SAMPLES);
      trace.setName(pName);
      trace.setColor(pColor);
      mTraces.put(pName, trace);
      pChart.addTrace(trace);

      JLabel label = new JLabel("000000000000000000000" );
      label.setForeground(pColor);
      mNumberDisplays.put(pName, label);
   }

   private String getName(IDisplay.Axis pAxis, IDisplay.Nature pNature)
   {
      return pAxis.name() + "." + pNature.name();
   }

   private final static int NB_SAMPLES = 300;

   private Map<String, ITrace2D> mTraces = new HashMap<>();
   private Map<String, JLabel> mNumberDisplays= new LinkedHashMap<>();

   private Chart2D mAccelChart = new Chart2D();
   private Chart2D mInclinChart = new Chart2D();

   private JComponent mPanel;
   private BoatAngleViewer mHeelViewer1;
   private BoatAngleViewer mTrimViewer1;
   private BoatAngleViewer mHeelViewer2;
   private BoatAngleViewer mTrimViewer2;

}
