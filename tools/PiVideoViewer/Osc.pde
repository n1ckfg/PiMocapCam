import oscP5.*;
import netP5.*;

PImage img1;
PImage img2;
ArrayList<String> hostList;
int numHosts = 2;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  img1 = createImage(640, 480, RGB);
  img2 = createImage(640, 480, RGB);
  oscP5 = new OscP5(this, receivePort);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
  hostList = new ArrayList<String>();
}

// Receive message example
void oscEvent(OscMessage msg) {
  if (msg.checkAddrPattern("/video") && msg.checkTypetag("ss")) {
    
    String hostname = msg.get(0).stringValue();
    String videoString = msg.get(1).stringValue();
    
    println(hostname + " " + videoString + " " + videoString.length());
    
    if (hostList.size() >= numHosts) {
      if (hostname.equals(hostList.get(0))) {
        //img1
      } else {
        //img2
      }
    } else {
      hostList.add(hostname);
    }
  }
}
