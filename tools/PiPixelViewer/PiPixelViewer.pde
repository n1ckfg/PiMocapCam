import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
Locator loc = new Locator(4);
ArrayList<Stroke> strokes;
Stroke newStroke;
boolean clicked = false;
boolean isDrawing = false;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  strokes = new ArrayList<Stroke>();
  newStroke = new Stroke();
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
  
  loc.run();
  
  pushMatrix();
  translate(loc.cursor.x, loc.cursor.y, loc.cursor.z);
  fill(255);
  sphere(10);
  popMatrix();
  
  if (clicked && !isDrawing) {
    isDrawing = true;
    newStroke.points.add(loc.cursor);
    newStroke.draw();
  } else if (clicked && isDrawing) {
    newStroke.points.add(loc.cursor);  
    newStroke.draw();
  } else if (!clicked && isDrawing) {
    isDrawing = false;
    strokes.add(newStroke);
    newStroke = new Stroke();
  }
    
  for (Stroke s : strokes) {
    s.draw();
  }

  
  surface.setTitle("" + frameRate);
}
