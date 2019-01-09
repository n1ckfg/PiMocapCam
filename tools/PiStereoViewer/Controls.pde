void keyPressed() {
  switch(key) {
    case 's':
      doStereoBM = !doStereoBM;
      break;
    case 'i':
      maskInput = !maskInput;
      break;
    case 'o':
      maskOutput = !maskOutput;
      break;
  }
}
