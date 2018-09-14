import oscP5.*;
import netP5.*;

String ipNumber = "127.0.0.1";
int sendPort = 7110;
int receivePort = 9998;
OscP5 oscP5;
NetAddress myRemoteLocation;

String hostName1 = "RPi_180219175326360";
String hostName2 = "RPi_180219180801264";

void oscSetup() {
  oscP5 = new OscP5(this, receivePort);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
}
