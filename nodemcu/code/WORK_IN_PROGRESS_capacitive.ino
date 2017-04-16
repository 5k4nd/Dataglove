
/*

ATTENTION, LE CAPTEUR CAPACITIF NE DOIT PAS ÊTRE TOUCHÉ LORS DE L'INITIALISATION (CALCUL DE LA MASSE).
  NOTE : le calibrage de ce capteur capacitif se fait dans la loop avec un hack tout moche et pas dans la setup.
  pourquoi ? parce que dans la setup CapacitiveSensor(4, 2) me renvoie des valeurs aberrantes. chais pas
  pourquoi, ptet un fonctionnement différent pour l'arduino lors de la loop setup, en termes d'alim, de mémoire, ... !??

>> il fait un FRONT MONTANT de 1 seconde



*/





// pour le capteur capacitif
#define capa_delta       20      // la valeur plancher au-dessus de laquelle le capteur est considéré comme "touché". ATTENTION c'est un multiplieur !
#define CALIBRAGE_LENGTH  100    // nombre de valeurs sommées pour établir la valeur référence du capteur capacitif
#include <CapacitiveSensor.h>
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 3);  // 1M resistor between pins 4 & 2, pin 2 is sensor pin
long capacitive_ground;
int oldTouchedState = LOW;
int oldTouchedTime;


void setup() {
  Serial.begin(115200);
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
}


int i;
void loop() {

  if (i<CALIBRAGE_LENGTH) //  à nettoyer, c'est moche.
    capacitive_calibrate();
  capacitive_mesure();

  Serial.println("coucou");
  delay(1000);
}




void capacitive_calibrate()
{
  // CALIBRAGE: on stocke les CALIBRAGE_LENGTH premières valeurs capacitives pour calculer la masse de référence du capteur capacitif
  long ground_array[CALIBRAGE_LENGTH];
  while (i < CALIBRAGE_LENGTH)
  {
    long capt_capacitif =  cs_4_2.capacitiveSensor(30);
    
    // on stocke
    ground_array[i] = capt_capacitif;
    i++;
    
    // last loop
    if (i == CALIBRAGE_LENGTH)
    {
      // on fait la somme
      for (int j = 0; j < CALIBRAGE_LENGTH; j++)
      {
        capacitive_ground = capacitive_ground + ground_array[j];
      }
      // puis on fait la moyenne
      capacitive_ground = capacitive_ground /CALIBRAGE_LENGTH;

      Serial.print("{'capacitive_ground':");
      Serial.print(capacitive_ground);
      Serial.println("}");   
    }

    delay(10);
  }
}

void capacitive_mesure()
{
    long capt_capacitif =  cs_4_2.capacitiveSensor(30);
  if (capt_capacitif > capacitive_ground*capa_delta)  // on considère le capacitif touché
  {
    // WARNING, we only want to print on the output change, not state
      if (oldTouchedState == LOW) {
        Serial.println("{'capa': True}");  // presence
        oldTouchedState = HIGH;
        oldTouchedTime = millis();
      }
  }
  else
  {
    if ((oldTouchedState == HIGH) && ( (millis() - oldTouchedTime) >= 1000)) {  // 1 sec rising edge
      Serial.println("{'capa': False}");  // presence
      oldTouchedState = LOW;
    }
  }

  Serial.print("{'value': "); Serial.print(capt_capacitif); Serial.println("}");


  
  /*
  else
  {
    Serial.println("{'alive': True}");
  }
  */
}
