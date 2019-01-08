import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
ArrayList<Stroke> strokes;

void setup() {
  size(1280, 480, P3D);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);

  oscSetup();
  strokes = new ArrayList<Stroke>();
}

void draw() {
  background(0);
  lights();

  noStroke();
  fill(0,0,50);
  rect(0,0,width/2,height);
  fill(50,0,0);
  rect(width/2,0,width/2,height);
  
  stroke(127);  
  line(width/2, 0, width/2, height);

  ArrayList<Point> pointsToAdd = new ArrayList<Point>();

  for (int i=0; i<maxDots; i++) {
    boolean doDot1 = false;
    boolean doDot2 = false;
    if (dot1[i] != null) doDot1 = true;
    if (dot2[i] != null) doDot2 = true;
    
    stroke(63);
    strokeWeight(1);
    if (doDot1) {
      line(0, dot1[i].y, dot1[i].x, dot1[i].y);
      line(dot1[i].x, 0, dot1[i].x, dot1[i].y);
    }
    if (doDot2) {
      line(width, dot2[i].y, dot2[i].x + (width/2), dot2[i].y);
      line(dot2[i].x + (width/2), height, dot2[i].x + (width/2), dot2[i].y);
    }
    
    stroke(255);
    strokeWeight(2);
    if (doDot1 && doDot2) {
      line(dot2[i].x, dot2[i].y, dot1[i].x, dot1[i].y);
      line(dot1[i].x, dot1[i].y, dot2[i].x, dot2[i].y);
    }
    
    strokeWeight(8);
    if (doDot1) {
      stroke(0,127,255);
      point(dot1[i].x, dot1[i].y);
    }
    if (doDot2) {
      stroke(255,63,0);
      point(dot2[i].x + (width/2), dot2[i].y);
    }
    
    stroke(255);
    strokeWeight(2);
    fill(127);
    ellipseMode(CENTER);
    if (doDot1) ellipse(dot1[i].x, dot1[i].y, dotSize, dotSize);
    if (doDot2) ellipse(dot2[i].x + (width/2), dot2[i].y, dotSize, dotSize);
    
    if (doDot1 && doDot2) {
      float x = (dot1[i].x + dot2[i].x)/2;
      float y = (dot1[i].y + dot2[i].y)/2;
      float z = dist(dot1[i].x, dot1[i].y, dot2[i].x, dot2[i].y);
      pointsToAdd.add(new Point(x, y, z, millis()));
    }
  }
  
  if (pointsToAdd.size() > strokes.size()) {
    for (int i=strokes.size(); i<pointsToAdd.size(); i++) {
      strokes.add(new Stroke());
    }
  }
  
  for (int i=0; i<pointsToAdd.size(); i++) {
    strokes.get(i).addPoint(pointsToAdd.get(i));
  }
  
  for (int i=0; i<strokes.size(); i++) {
    strokes.get(i).run();
  }
  
  surface.setTitle("" + frameRate);
}
