import processing.net.*;

Server s;

void setup() {
  s = new Server(this, 12000);
}

void draw() {
  
  //println("new loop");
  
  // check if a client sent something
  Client c = s.available();
  if (c != null) {
    String x = c.readStringUntil(byte('\n'));
    String y = c.readStringUntil(byte('\n'));
    println(x);
    println(y);
  }
  delay(1000);
}
