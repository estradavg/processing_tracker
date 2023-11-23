/**
 * Constrain. 
 * 
 * Move the mouse across the screen to move the circle. 
 * The program constrains the circle to its box. 
 */
import processing.net.*;
Server s;

String x = "0", y = "0";
int numberx, numbery; 
float mx, my;
float easing = 0.25;
int radius = 24;
int edge = 100;
int inner = edge + radius;

public static String removeLastChar(String s) {
    return (s == null || s.length() == 0)
      ? "0" 
      : (s.substring(0, s.length() - 1));
}

void setup() {
  size(1080, 720);
  noStroke(); 
  ellipseMode(RADIUS);
  rectMode(CORNERS);
  s = new Server(this, 12000);
}

void draw() { 
  background(51);
  
  Client c = s.available();
  if (c != null) {
    x = c.readStringUntil(byte('\n'));
    y = c.readStringUntil(byte('\n'));
    x = removeLastChar(x);
    y = removeLastChar(y);
    numberx = 1080-Integer.valueOf(x);
    numbery = 720-Integer.valueOf(y);
    println("(" + numberx + " " + numbery + ")");
  }

  if (abs(numberx - mx) > 0.1) {
    mx = mx + (numberx - mx) * easing;
  }
  if (abs(numbery - my) > 0.1) {
    my = my + (numbery- my) * easing;
  }
  
  mx = constrain(mx, inner, width - inner);
  my = constrain(my, inner, height - inner);
  fill(76);
  rect(edge, edge, width-edge, height-edge);
  fill(255);  
  ellipse(mx, my, radius, radius);
}
