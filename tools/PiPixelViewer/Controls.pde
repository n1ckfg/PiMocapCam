void keyPressed() {
  if (keyCode == 11 || keyCode == 16) clicked = true;
}

void keyReleased() {
  if (keyCode == 11 || keyCode == 16) clicked = false;
  
  if (key == ' ') {
    strokes = new ArrayList<Stroke>();
    newStroke = new Stroke();
  }
}
