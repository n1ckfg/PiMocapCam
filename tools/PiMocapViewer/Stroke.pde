class Stroke {
  
  ArrayList<Point> points;
  int point_life = 1000;
  int time = 0;
  PVector lastPos = new PVector(0,0,0);
  
  Stroke() {
    points = new ArrayList<Point>();
  }
  
  void update() {
    time = millis();
  }
  
  void draw() {
    noFill();
    strokeWeight(10);
    stroke(255, random(40, 63));
    if (points.size() > 1) {
      beginShape();
      for (int i=0; i<points.size(); i++) {
        Point p = points.get(i);
        if (time < p.timestamp + point_life) {
          vertex(p.x, p.y, p.z);
        } else {
          points.remove(i);
        }
      }
      endShape();
    }
    
    Point p = points.get(points.size()-1);
    PVector pos = new PVector(p.x, p.y, p.z); 
    noStroke();
    if (dist(pos.x, pos.y, pos.z, lastPos.x, lastPos.y, lastPos.z) > 0.1) {
      fill(0, 240, 100);
    } else {
      fill(140);
    }
    pushMatrix();
    translate(p.x, p.y, p.z);
    sphere(dotSize/1.6);
    popMatrix();
    
    lastPos = pos;
  }
  
  void run() {
    update();
    draw();
  }
  
  void addPoint(float x, float y, float z, int i) {
    points.add(new Point(x, y, z, millis()));
  }

  void addPoint(Point point) {
    points.add(point);
  }

}
