class Stroke {

  ArrayList<PVector> points;
  int index;
  int timestamp;
  int lifespan = 1000;
  
  Stroke() {
    index = 0;
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx) {
    index = idx;
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx, ArrayList<PVector> pts) {
    index = idx;
    points = pts;
    timestamp = millis();
  }
  
  void draw() {
    noFill();
    stroke(255, 127);
    strokeWeight(4);
    beginShape();
    for (PVector p : points) {
      vertex(p.x, p.y, p.z);
    }
    endShape();
  }
}
