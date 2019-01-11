import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);
ArrayList<String> hostList;
int numHosts = 2;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
int datagramSize = 1000000;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  OscProperties op = new OscProperties();
  op.setListeningPort(receivePort);
  op.setDatagramSize(datagramSize);
  oscP5 = new OscP5(this, op);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
  hostList = new ArrayList<String>();
}

// Receive message example
void oscEvent(OscMessage msg) {
  if (msg.checkAddrPattern("/contour") && msg.checkTypetag("sib")) {    
    String hostname = msg.get(0).stringValue();
    int index = msg.get(1).intValue();
    byte[] readBytes = msg.get(2).blobValue();
   
   ArrayList<PVector> points = new ArrayList<PVector>();
   for (int i = 0; i < readBytes.length; i += 8) { //+=16) { 
     byte[] bytesX = { readBytes[i], readBytes[i+1], readBytes[i+2], readBytes[i+3] };
     byte[] bytesY = { readBytes[i+4], readBytes[i+5], readBytes[i+6], readBytes[i+7] };
     //byte[] bytesZ = { readBytes[i+8], readBytes[i+9], readBytes[i+10], readBytes[i+11] };
     //byte[] bytesW = { readBytes[i+12], readBytes[i+13], readBytes[i+14], readBytes[i+15] };

     float x = asFloat(bytesX);
     float y = asFloat(bytesY);
     //float z = asFloat(bytesZ);
     //float w = asFloat(bytesW);
     if (!Float.isNaN(x) && !Float.isNaN(y)) { // && !Float.isNaN(z)) {
       PVector p = new PVector(x, y);
       points.add(p);
       println(p.x + " " + p.y);
     }
   }
    
    Stroke newStroke = new Stroke(index, points);

    boolean doReplace = false;
    int replaceIndex = 0;
    
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (s.index == index) {
        replaceIndex = i;
        doReplace = true;
        break;
      }
    }
        
    if (doReplace) {
      strokesBuffer.set(replaceIndex, newStroke);
    } else {
      strokesBuffer.add(newStroke);
    }
  
    int time = millis();
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (time > s.timestamp + s.lifespan) {
        strokesBuffer.remove(i);
      }
    }
    
    println(hostname + " " + " " + index);
    
    if (hostList.size() >= numHosts) {
      if (hostname.equals(hostList.get(0))) {
        //dot1 = new PVector(x * width, y * height);
      } else {
        //dot2 = new PVector(x * width, y * height);
      }
    } else {
      hostList.add(hostname);
    }
  }
}
