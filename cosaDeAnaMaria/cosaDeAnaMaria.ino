//Constantes 
const int IZQUIERDA = 0;
const int FRENTE    = 1;
const int DERECHA   = 2;

const int COLOR_NEGRO = 0;
//

//Pines de los SENSORES INFRARROJOS
const int sensorInfrarrojo = 12;  // Cambiar al pin que corresponda
//

//Pines de los MOTORES
const int motorIzqN = 11; const int motorIzqP = 10;
const int motorDerN =  8; const int motorDerP =  9;
//

//Estructura de SensorUltrasonico
struct SensorUltrasonico {
  int echo;
  int trig;
};

SensorUltrasonico sensorIzq = { 2, 3 };  // { Echo, Trigger }
SensorUltrasonico sensorFrn = { 4, 5 };
SensorUltrasonico sensorDer = { 6, 7 };


//Configurar todos los pines necesarios para la ejecución
void setup() {
  Serial.begin(9600);  // Inicializar la comunicacion serial para ver los mensajes en el monitor

  // Configurar los pines de los sensore☻☻s como entradas
  pinMode(motorIzqP, OUTPUT);
  pinMode(motorIzqN, OUTPUT);
  pinMode(motorDerP, OUTPUT);
  pinMode(motorDerN, OUTPUT);

  //Activar con PinMode los pines de los SensoresUltrasonicos
  //TRIGGERS necesita OUTPUT
  //ECHOES   necesita INPUT
  pinMode(sensorIzq.trig, OUTPUT);
  pinMode(sensorIzq.echo, INPUT);

  pinMode(sensorDer.trig, OUTPUT);
  pinMode(sensorDer.echo, INPUT);

  pinMode(sensorFrn.trig, OUTPUT);
  pinMode(sensorFrn.echo, INPUT);
}

#pragma region Movimientos posibles
//Calcula la distancia para un transmisor
float readDistance(int trigPin, int echoPin) {
  // Generar un pulso de 10 microsegundos en el pin TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer el tiempo de duracion del pulso de eco en el pin ECHO
  float duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centimetros usando la velocidad del sonido
  // en el aire (aproximadamente 343 m/s a 20gradosC)
  float distance = duration * 0.0343 / 2; //58.2;

  return distance;
}

//Detiene todos los motores, REVISADO
void detenerMotores() {
  Serial.println("DETENIENDO motores");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, LOW);
}

//Gira el zumo hacia la derecha, REVISADO
void girarDerecha() {
  Serial.println("Girando a IZQUIERDA");
  digitalWrite(motorIzqP, HIGH);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, LOW);
}

//Gira el zumo hacia la izquierda, REVISADO
void girarIzquierda() {
  Serial.println("Girando a DERECHA");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, HIGH);
  digitalWrite(motorDerN, LOW);
}

//Mueve el zumo hacia adelante, REVISADO
void moverAdelante() {
  Serial.println("Moviendo ADELANTE");
  digitalWrite(motorIzqP, HIGH);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, HIGH);
  digitalWrite(motorDerN, LOW);
}

//Mueve el zumo hacia atras, REVISADO
void moverAtras(){
  Serial.println("Moviendo ATRAS");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, HIGH);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, HIGH);
}

//Elegir el ultrasonico que marque la menor distancia, y retornar la direccion de menor distancia
int ultrasonicoMenor() {

  //Lee la distacia en cada sensor
  //  funcion-> readDistance(trigger, echo);
  float distIzq = readDistance(sensorIzq.trig, sensorIzq.echo);
  float distFrn = readDistance(sensorFrn.trig, sensorFrn.echo);
  float distDer = readDistance(sensorDer.trig, sensorDer.echo);

  Serial.println("Mostrando distancias Izq,Frn,Der: ");
  Serial.print(distIzq);
  Serial.print(", ");
  Serial.print(distFrn);
  Serial.print(", ");
  Serial.println(distDer);

  //Esto reordena la lista de distancias de menor a mayor

  if (distIzq < distFrn && distIzq < distDer) {
    return IZQUIERDA;
  } else if (distDer < distFrn && distDer < distIzq) {
    return DERECHA;
  } else {
    return FRENTE;
  }
}
#pragma endregion

/*
=======ESTRATEGIAS
*/

//Estrategia de AnaMaria que ella entendera xdddddd
//Estrategia como pensando
void estrategia1() {
  int locacionEnemigo = ultrasonicoMenor();

  switch (ultrasonicoMenor()) {  //ACA PUEDE SUCEDER UN ERROR
    case IZQUIERDA:
      girarIzquierda();
      break;

    case FRENTE:
      moverAtras();
      delay(200);
      moverAdelante();
      break;

    case DERECHA:
      girarDerecha();
      break;

    default:
      Serial.println("Algo salio mal en el switch de la estrategia1 xd");
      break;
  }
}


int contador = 0;
bool enemigoDetectado = false;
//EL zumo gira a la izquierda hasta que detecte al enemigo delante suyo, en ese momento se mueve hacia adelante.
void estrategia2() {
  
  while (!enemigoDetectado) {
    girarIzquierda();
    if (ultrasonicoMenor() == FRENTE) { enemigoDetectado = true; }
  }
  moverAdelante();
}

//      MAIN LOOP
void loop() {



}
