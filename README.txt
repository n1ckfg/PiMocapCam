PiMocapCam blob tracks bright objects on a Raspberry Pi and sends OSC coordinates. The host machine can receive OSC tracking input from multiple Raspberry Pis running PiMocapCam.
Based on the work of @jvcleave and @orgicus. 

PiMocapCam can use Bonjour (aka Zeroconf) to send OSC without configuring IP addresses.

Setup instructions:
https://learn.adafruit.com/bonjour-zeroconf-networking-for-windows-and-linux/overview

More discussion:
https://www.raspberrypi.org/forums/viewtopic.php?f=66&t=18207

TOOLS
Use PiMocapTester to send local test OSC messages that imitate the output of PiMocapCam.

Use PiMocapViewer to see input from RPis running PiMocapCam.