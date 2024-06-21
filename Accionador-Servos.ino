#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>
//#include <ArdOSC.h>

int CALMADO=9; 
int IZQUIERDA=8;
int DERECHA=7;
int EMPUJAR=5; 
//int JALAR=6; 
//int LEVANTAR=11; 



// you can find this written on the board of some Arduino Ethernets or shields
byte mac[] = { 0xC4, 0x65, 0x16, 0xAB, 0x2C, 0x6A }; 

// NOTE: Alternatively, you can assign a fixed IP to configure your Ethernet shield.
      // byte ip[] = { 192, 168, 137, 209};


int serverPort = 7400; // Emotiv BCI out port

//Create UDP message object
EthernetUDP Udp;

void setup(){
  pinMode(CALMADO,OUTPUT);
  //pinMode(EMPUJAR,OUTPUT);
  //pinMode(JALAR,OUTPUT);
  //pinMode(LEVANTAR,OUTPUT);
  //pinMode(HUNDIR,OUTPUT);
  pinMode(IZQUIERDA,OUTPUT);
  pinMode(DERECHA,OUTPUT);
  
  Serial.begin(9600); //9600 for a "normal" Arduino board (Uno for example). 115200 for a Teensy ++2 
  Serial.println("Emotiv BCI OSC test");

  // start the Ethernet connection:
  // NOTE: Alternatively, you can assign a fixed IP to configure your Ethernet shield.
  //       Ethernet.begin(mac, ip);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  
  // print your local IP address:
  Serial.print("Arduino IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();

  Udp.begin(serverPort);
}

void loop(){
  //process received data from Emotiv BCI
  OSCMsgReceive();


} 

void OSCMsgReceive() {
  
  int size = Udp.parsePacket();  
  if(size > 0) {    
    OSCBundle bundleIN;
    while(size--)
      bundleIN.fill(Udp.read());
      
    if(!bundleIN.hasError()){
        bundleIN.route("/fac/eyeAct", processFEEyeAct);  // Facial_Expressions
        bundleIN.route("/com", processMC); // Mental_Commands
    }
  }
}

void processFEEyeAct(OSCMessage &msg, int addrOffset) {
  if(msg.match("/lookL", addrOffset)) {
    Serial.println("FE - Look Left");
  } else if(msg.match("/lookR", addrOffset)) {
    Serial.println("FE - Look Right");
  } else if(msg.match("/blink", addrOffset)) {
    Serial.println("FE - Blink");
  } else if(msg.match("/winkL", addrOffset)) {
    Serial.println("FE - Wink Left");
  } else if(msg.match("/winkR", addrOffset)) {
    Serial.println("FE - Wink Right");
  }
}




void processMC(OSCMessage &msg, int addrOffset) {  
  if(msg.match("/neutral", addrOffset)) {
    Serial.print("MC - Neutral: ");
    digitalWrite(CALMADO,HIGH);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,LOW);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/push", addrOffset)) {
    Serial.print("MC - Push: ");
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,LOW);
    digitalWrite(EMPUJAR,HIGH);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
  
  } else if(msg.match("/pull", addrOffset)) {
    Serial.print("MC - Pull: ");
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,LOW);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/left", addrOffset)) {
    Serial.print("MC - Left: ");
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,HIGH);
    digitalWrite(DERECHA,LOW);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/right", addrOffset)) {
    Serial.print("MC - Right: ");
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,HIGH);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/lift", addrOffset)) {
    Serial.print("MC - lift: ");
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,LOW);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/drop", addrOffset)) {
    digitalWrite(CALMADO,LOW);
    digitalWrite(IZQUIERDA,LOW);
    digitalWrite(DERECHA,LOW);
    //digitalWrite(EMPUJAR,LOW);
    //digitalWrite(JALAR,LOW);
    //digitalWrite(LEVANTAR,LOW);
    //digitalWrite(HUNDIR,LOW);
    
  } else if(msg.match("/rotateLeft", addrOffset)) {
    Serial.print("MC - rotateLeft: ");
  } else if(msg.match("/rotateRight", addrOffset)) {
    Serial.print("MC - rotateRight: ");
  } else if(msg.match("/rotateClockwise", addrOffset)) {
    Serial.print("MC - rotateClockwise: ");
  } else if(msg.match("/rotateCounterClockwise", addrOffset)) {
    Serial.print("MC - rotateCounterClockwise: ");
  } else if(msg.match("/rotateForwards", addrOffset)) {
    Serial.print("MC - rotateForwards: ");
  } else if(msg.match("/rotateReverse", addrOffset)) {
    Serial.print("MC - rotateReverse: ");
  } else if(msg.match("/disappear", addrOffset)) {
    Serial.print("MC - disappear: ");
  }

  if(msg.isFloat(0)) {
    float value = msg.getFloat(0);
    Serial.println(value);
  }
}
