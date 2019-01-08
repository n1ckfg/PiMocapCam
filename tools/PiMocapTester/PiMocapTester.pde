ArrayList<AutoDot> dots;
int numDots = 5;
float birthOdds = 0.02;
boolean death = true;

void setup() {
  size(640, 480, P3D);
  oscSetup();

  dots = new ArrayList<AutoDot>();
  for (int i=0; i<numDots; i++) {
    dots.add(new AutoDot(i));
  }
}

void draw() {
  blendMode(NORMAL);
  background(0);
  
  blendMode(ADD);
  for (int i=0; i<dots.size(); i++) {
    AutoDot dot = dots.get(i);
    dot.index = i;
    dot.run();
    if (death && !dot.alive) dots.remove(i);
  }
  
  if (death && random(1.0) < birthOdds) dots.add(new AutoDot(dots.size()));

  surface.setTitle("" + frameRate);
}
