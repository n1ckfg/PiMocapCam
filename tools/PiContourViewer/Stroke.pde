class Stroke {

  ArrayList<PVector> points;
  int index;
  
  Stroke() {
    index = 0;
    points = new ArrayList<PVector>();
  }
  
  Stroke(int idx) {
    index = idx;
    points = new ArrayList<PVector>();
  }
  
  Stroke(int idx, ArrayList<PVector> pts) {
    index = idx;
    points = pts;
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
