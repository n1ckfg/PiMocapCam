import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
ArrayList<Stroke> strokes;
boolean clicked = false;
boolean isDrawing = false;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  strokes = new ArrayList<Stroke>();
  oscSetup();
}

void draw() {
  background(0);
  lights();
  
  noStroke();
  fill(255,50,0.127);
  ellipse(dot1.x, dot1.y, 20, 20);
  fill(0,127,255,127);
  ellipse(dot2.x, dot2.y, 20, 20);
    
  for (Stroke s : strokes) {
    s.draw();
  }
  
  surface.setTitle("" + frameRate);
}
