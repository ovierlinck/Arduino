package com.iba.amaris;

import java.awt.*;
import java.awt.geom.AffineTransform;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class BoatAngleViewer extends JPanel
{

   public BoatAngleViewer()
   {
      super();
   }
   
   public BoatAngleViewer(boolean pShowHeel)
   {
      super();
      mShowHeel = pShowHeel;
      setTilt(0);
   }

   @Override
   public Dimension getSize()
   {
      return new Dimension(100, 100);
   }

   @Override
   public Dimension getPreferredSize()
   {
      return new Dimension(100, 100);
   }

   @Override
   public Dimension getMinimumSize()
   {
      return new Dimension(100, 100);
   }

   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);
      Graphics2D g2 = (Graphics2D) g;
      
      g2.setColor(mShowHeel ? Color.red : Color.blue);

      g2.drawRect(3, 3, getSize().width - 3, getSize().height - 3);
      g2.transform(transform);
      if (mShowHeel)
      {
         g2.drawArc(-5, -6, 10, 12, 180, 180);  // Hull 
         g2.drawLine(-5, -1, 5, -1);  // hull top;
         g2.drawLine(0, -1, 0, -10);  // mast
      }
      else
      {
         g2.drawPolygon(xLateralHullPoints, yLateralHullPoints, xLateralHullPoints.length);
         g2.drawLine(0, -1, 0, -10);
      }
   }

   /** Heeling in degrees */
   public void setTilt(double pHeel)
   {
      double width = getSize().getWidth();
      double height = getSize().getHeight();
      double scale = width / 25; // drawing is from -10 to +10, so 20 wide, so adapt a size of 25 to the full
                                 // image to have some margin (everything done on X axis, assume display is
                                 // +/- square)
      transform.setToIdentity();
      transform.translate(width / 2, height / 2);
      transform.scale(scale, scale);
      transform.rotate(Math.toRadians(pHeel));
      repaint();
   }

   /** select heel / trim viewer */
   private boolean mShowHeel = true;

   private static final int[] xLateralHullPoints = new int[] { -10, -4, 8, 10 };
   private static final int[] yLateralHullPoints = new int[] { -1, 4, 4, -1 };
   private AffineTransform transform = new AffineTransform();

   public static void main(String[] pArgs)
   {
      JFrame frame = new JFrame("Test Tilt Viewer");
      BoatAngleViewer tiltViewer = new BoatAngleViewer();
      tiltViewer.setTilt(100);
      frame.setContentPane(tiltViewer);

      frame.setSize(800, 800);
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.setVisible(true);

      tiltViewer.setTilt(15);

   }

}
