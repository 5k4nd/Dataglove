#include <ESP8266WiFi.h>
int ledPin = D0;

/* ####### ESP NETWORK CONFIG ####### */
#include <WiFiUdp.h>

//const char* REMOTE_IP = "192.168.1.35";
//const char* REMOTE_IP = "NULL";  // déterminé automatiquement. le python envoie un paquet au démarrage
int REMOTE_IP = -1;
const int REMOTE_PORT = 4210;
//const char* ssid = "Skandnet";
const char* ssid = "L'Antre des Anges";
//const char* ssid = "Connectify-glove";  // "Livebox-Nougatine" "Skandnet" "Connectify-glove"
//const char* password = "tarteauxfraises" ;
const char* password = "pilipilou" ;  // "BABA1294" "tarteauxfraises" "pilipilou"

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on

/* ####### ADXL config ####### */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup()
{
  Serial.begin(115200);

  init_adxl();
  
  // setup led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // connect to wifi
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  Serial.printf("Now connect at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  // start udp connection with computer
  udp_computer_connect();


}


void loop()
{
  /* acceleration listener, in m/s^2 */
  sensors_event_t event; 
  accel.getEvent(&event);

  String packet_x = "'X':"+String(event.acceleration.x)+',';
  String packet_y = "'Y':"+String(event.acceleration.y)+',';
  String packet_z = "'Z':"+String(event.acceleration.z)+',';

  /* send via udp with python dict structure */
  Udp.beginPacket(REMOTE_IP, REMOTE_PORT);
  Udp.write( ('{'+packet_x+packet_y+packet_z+'}').c_str() );
  Udp.endPacket();


  if (WiFi.status() != WL_CONNECTED)
    digitalWrite(ledPin, LOW);
   else
    digitalWrite(ledPin, HIGH);

  delay(100);
}





char incomingPacket[255];
void udp_computer_connect()
{
  // on ouvre le port UDP
  Udp.begin(localUdpPort);
  
  // on attend un paquet du PC
  Serial.println("waiting for computer call...");
  while(REMOTE_IP==-1)
  {
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      
      // si le paquet reçu est le bon "READY_TO_START", on est tout bon !
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      if (String(incomingPacket)=="READY_TO_START")
        REMOTE_IP = Udp.remoteIP();
        Serial.printf("Starting streaming to %s, port %d\n", Udp.remoteIP().toString().c_str(), String(REMOTE_PORT).c_str());
        //Serial.println(REMOTE_IP.toString());
    }
    delay(500);
  }
}




/* ##################################################### */
/*                     ADXL SENSOR                       */
/* ##################################################### */

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

void init_adxl()
{

  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}