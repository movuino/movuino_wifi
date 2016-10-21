import processing.serial.*;
import oscP5.*;
import netP5.*;

//-----------
//-----------
OscP5 oscP5;
/* a NetAddress contains the ip address and port number of a remote location in the network. */
NetAddress myBroadcastLocation;

//Movuino data
float ax=0;
float ay=0;
float az=0;
//float gx=0;
//float gy=0;
//float gz=0;

void setup () {
  // set the window size:
  size(800, 600);
  background(255);
  
  // create a new instance of oscP5. 12000 is the port number you are listening for incoming osc messages.
  oscP5 = new OscP5(this, 7000);
  
  // create a new NetAddress. a NetAddress is used when sending osc messages with the oscP5.send method.
  // the address of the osc broadcast server
  myBroadcastLocation = new NetAddress("127.0.0.1", 3011);
}


void draw() {
  
}