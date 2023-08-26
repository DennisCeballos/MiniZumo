#include <NewPing.h>
//Esta media rara esta libreria porque no existe en mi laptop, lo que implica que lo agrego ariana?
//curioso


const int sensorDerechoPin = 13; // Cambiar al pin que corresponda
const int sensorIzquierdoPin = 12; // Cambiar al pin que corresponda

const int motorIzq = 11;
const int motorIzq2 = 10;
const int motorDer = 9;
const int motorDer2 = 8;

const int echouno = 2; // Conectar al pin ECHO del primer sensor
const int triguno = 3; // Conectar al pin TRIG del primer sensor
const int echodos = 4; // Conectar al pin ECHO del segundo sensor
const int trigdos = 5; // Conectar al pin TRIG del segundo sensor
const int echotres = 6; // Conectar al pin ECHO del tercer sensor
const int trigtres = 7; // Conectar al pin TRIG del tercer sensor

NewPing sensorUno(triguno, echouno, 200); // 200 cm de distancia máxima
NewPing sensorDos(trigdos, echodos, 200); // 200 cm de distancia máxima
NewPing sensorTres(trigtres, echotres, 200); // 200 cm de distancia máxima

void setup() {
  // Configurar los pines de los sensores como entradas
  pinMode(sensorDerechoPin, INPUT);
  pinMode(sensorIzquierdoPin, INPUT);
  Serial.begin(9600); // Inicializar la comunicación serial para ver los mensajes en el monitor

  pinMode(motorIzq, OUTPUT);
  pinMode(motorIzq2, OUTPUT);
  pinMode(motorDer, OUTPUT);
  pinMode(motorDer2, OUTPUT);

  pinMode(triguno, OUTPUT);
  pinMode(echouno, INPUT);
  pinMode(trigdos, OUTPUT);
  pinMode(echodos, INPUT);
  pinMode(trigtres, OUTPUT);
  pinMode(echotres, INPUT);
}

float readDistance(int trigPin, int echoPin) {
  // Generar un pulso de 10 microsegundos en el pin TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer el tiempo de duración del pulso de eco en el pin ECHO
  float duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros usando la velocidad del sonido
  // en el aire (aproximadamente 343 m/s a 20°C)
  float distance = duration * 0.0343 / 2;

  return distance;
}

void ultrasonicos(){
  float dist1 = readDistance( triguno, echouno);
  float dist2 = readDistance(trigdos, echodos);
  float dist3 = readDistance(trigtres, echotres);

  if(dist1<dist2){
    if(dist1<dist3){  // cuando 1 es el menor
      Serial.println ("sensor izquierda");
      digitalWrite(motorIzq, HIGH);
      digitalWrite(motorIzq2, LOW);
      digitalWrite(motorDer, LOW);
      digitalWrite(motorDer2, LOW);
    }
  
    if(dist3<dist1){  // cuando 3 es el menor
      Serial.println ("sensor derecha");
      digitalWrite(motorIzq, LOW);
      digitalWrite(motorIzq2, LOW);
      digitalWrite(motorDer, HIGH);
      digitalWrite(motorDer2, LOW);
    }
  }

  if(dist2<dist1){
    if(dist2<dist3){  // cuando 2 es el menor
      Serial.println ("sensor adelante");
      digitalWrite(motorIzq, HIGH);
      digitalWrite(motorIzq2, LOW);
      digitalWrite(motorDer, HIGH);
      digitalWrite(motorDer2, LOW);

    }
    if(dist3<dist2){  // cuando 3 es el menor
      Serial.println ("sensor derecha");
      digitalWrite(motorIzq, LOW);
      digitalWrite(motorIzq2, LOW);
      digitalWrite(motorDer, HIGH);
      digitalWrite(motorDer2, LOW);
    }
  }

}

void loop() {
  // Leer el estado de los sensores
  int sensorDerechoEstado = digitalRead(sensorDerechoPin);
  int sensorIzquierdoEstado = digitalRead(sensorIzquierdoPin);

  // Verificar las condiciones y enviar los mensajes adecuados
  if (sensorDerechoEstado == LOW && sensorIzquierdoEstado == HIGH) {
    Serial.println("blanco derecho");
    digitalWrite(motorIzq, HIGH);
    digitalWrite(motorIzq2, LOW);
    digitalWrite(motorDer, LOW);
    digitalWrite(motorDer2, HIGH);
    delay(500);
    ultrasonicos();
  } 
  
  else if (sensorIzquierdoEstado == LOW && sensorDerechoEstado == HIGH) {
    Serial.println("blanco izquierda");
    digitalWrite(motorIzq, LOW);
    digitalWrite(motorIzq2, HIGH);
    digitalWrite(motorDer, HIGH);
    digitalWrite(motorDer2, LOW);
    delay(500);
    ultrasonicos();
  } 

  else if (sensorIzquierdoEstado == LOW && sensorDerechoEstado == LOW) {
    Serial.println ("blanco Ambos");
    digitalWrite(motorIzq, LOW);
    digitalWrite(motorIzq2, HIGH);
    digitalWrite(motorDer, LOW);
    digitalWrite(motorDer2, HIGH);
    delay(1000);
    digitalWrite(motorIzq, LOW);
    digitalWrite(motorIzq2, HIGH);
    digitalWrite(motorDer, HIGH);
    digitalWrite(motorDer2, LOW);
    delay(500); //  FUNCION DEL ULTRASONICO
    ultrasonicos();
  } 

  else if (sensorIzquierdoEstado == HIGH && sensorDerechoEstado == HIGH) {
    Serial.println ("NEGRO EN AMBOS");      
  }        
              
  delay(100); // Pequeña pausa para evitar rebotes en los sensores
}

