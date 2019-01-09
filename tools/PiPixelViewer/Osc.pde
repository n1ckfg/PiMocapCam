import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);
ArrayList<String> hostList;
int numHosts = 2;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  oscP5 = new OscP5(this, receivePort);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
  hostList = new ArrayList<String>();
}

// Receive message example
void oscEvent(OscMessage msg) {
  if (msg.checkAddrPattern("/blob") && msg.checkTypetag("siff")) {
    
    String hostname = msg.get(0).stringValue();
    int index = msg.get(1).intValue();
    float x = msg.get(2).floatValue();
    float y =  msg.get(3).floatValue();
    
    //println(hostname + " " + " " + x + " " + y);
    
    if (hostList.size() >= numHosts) {
      if (hostname.equals(hostList.get(0))) {
        dot1 = new PVector(x * width, y * height);
      } else {
        dot2 = new PVector(x * width, y * height);
      }
    } else {
      hostList.add(hostname);
    }
  }
}
