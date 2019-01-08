
void setup() {
  size(1280, 480, P2D);

  oscSetup();
}

void draw() {
  background(0);
  image(img1, 0, 0);
  image(img2, width/2, 0);
}
