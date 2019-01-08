PImage img1, img2;

void setup() {
  size(1280, 480, P2D);
  img1 = createImage(640, 480, RGB);
  img2 = createImage(640, 480, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes1 != null) updateNewImg(img1, videoBytes1);
  if (videoBytes2 != null) updateNewImg(img2, videoBytes2);
  image(img1, 0, 0);
  image(img2, width/2, 0);
}

void updateNewImg(PImage img, byte[] bytes) {
  img.loadPixels();
  for (int i=0; i<img.pixels.length && i < bytes.length; i++) {
    img.pixels[i] = color(bytes[i]);
  }
  img.updatePixels();
}
