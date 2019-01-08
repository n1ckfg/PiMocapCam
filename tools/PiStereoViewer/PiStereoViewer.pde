
PImage img1, img2;

void setup() {
  size(640, 960, P2D);
  img1 = createImage(640, 480, RGB);
  img2 = createImage(640, 480, RGB);
  
  setupStereo();
  setupShaders();
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes1 != null) img1 = fromJpeg(videoBytes1);
  if (videoBytes2 != null) img2 = fromJpeg(videoBytes2);
  drawStereo(img1, img2);

  surface.setTitle("" + frameRate);
}
