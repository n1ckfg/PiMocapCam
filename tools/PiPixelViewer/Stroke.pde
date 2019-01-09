class Stroke {

  ArrayList<PVector> points;

  Stroke() {
    points = new ArrayList<PVector>();
  }
  
  void draw() {
    noFill();
    stroke(255, 127);
    strokeWeight(4);
    beginShape(POINTS);
    for (PVector p : points) {
      vertex(p.x, p.y, p.z);
    }
    endShape();
  }
}
