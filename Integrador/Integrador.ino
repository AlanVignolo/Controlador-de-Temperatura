//  Bibliotecas del sensor
#include <DHT.h>

//  Variables volatiles para la interrupcion

volatile unsigned int tiempo1 = 0;
volatile unsigned int tiempo = 0;
volatile unsigned int tiempo2 = 0;
volatile unsigned int diftiempo = 0;

//  Inicializo sensor

#define Type DHT11 //Constante
int dhtpin = 7;
DHT HT(dhtpin, Type);

//Asigno pines e inicio variables de utilidad

int led1 = 6; //verde
int led2 = 4; //rojo
int led3 = 5; //amarillo
int pulsador = 3;
float tempC;
int verde, amarillo, rojo, pos1, pos2;
String leds, leds2, red, green, yellow;
int X = 3500;

void setup() {
  Serial.begin(9600); //Puerto serial para la comunicacion serial
  HT.begin();
  //Asignacion de entradas y salidas
  pinMode(verde, OUTPUT); 
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(pulsador, INPUT);
  attachInterrupt(digitalPinToInterrupt(pulsador), interrupcion, CHANGE);
}

void loop() {
  tiempo = millis();

  //Cambio de tiempo entre mediciones en el caso de interrupcion 
  
  if (diftiempo>=2500 && diftiempo <=10000){
      X = diftiempo;
      }
    if (diftiempo>10000){
      X = 10000;
      }

  //Lectura del sensor para un N asignado (3.5 segundos por defecto)
  
  if (tiempo%X == 0){
    
    //  Lectura del sensor
    tempC = HT.readTemperature();
    Serial.print("Temperatura:");
    Serial.println(tempC);
    
    //  Recepcion de instrucciones de Python para la activacion de leds
   
    leds = Serial.readString();
    pos1 = leds.indexOf(',');
    red=leds.substring(0,pos1);
    leds2=leds.substring(pos1+1);
    pos2=leds2.indexOf(',');
    yellow=leds2.substring(0,pos2);
    green=leds2.substring(pos2+1);
    rojo=red.toInt();
    amarillo=yellow.toInt();
    verde=green.toInt();
    
    // Ejecuta las instruciones de los leds
    
    digitalWrite (led1,verde);
    digitalWrite (led2,rojo);
    digitalWrite (led3,amarillo);
  }
}

//Interrupcion en el cambio de flanco del pulsador
void interrupcion() {
   if (digitalRead(pulsador) == LOW){
    tiempo1 = tiempo;
   }
   else {
    tiempo2 = tiempo;
    diftiempo = tiempo2 - tiempo1;
    Serial.print("Tiempo:");
    Serial.println(diftiempo);
   }   
}
