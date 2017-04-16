/*
envoie un paquet UDP toutes les 300ms à REMOTE_IP sur son port REMOTE_PORT

Note: y aussi la fonction d'écoute, commentée dans la boucle ;-)

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* REMOTE_IP = "192.168.12.1";
const int REMOTE_PORT = 4210;
const char* ssid = "Skandnet";
const char* password = "tarteauxfraises";


WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char  replyPacekt[] = "Hi there from esp";
int ledPin = D0;

void setup()
{
  Serial.begin(115200);
  
  // setup led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // connect to wifi
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  // start udp
  Udp.begin(localUdpPort);

  // print success
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(REMOTE_IP, REMOTE_PORT);
    Udp.write(replyPacekt);
    Udp.endPacket();



  /*
  // receive incoming UDP packets
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    char incomingPacket[255];
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
  */


  if (WiFi.status() != WL_CONNECTED)
    digitalWrite(ledPin, LOW);
   else
    digitalWrite(ledPin, HIGH);

    delay(300);

}