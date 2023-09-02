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
  int maximaDistancia;
};

SensorUltrasonico sensorIzq = { 2, 3, 500 };  // { Echo, Trigger, MaximaDistancia }
SensorUltrasonico sensorFrn = { 4, 5, 500 };
SensorUltrasonico sensorDer = { 6, 7, 500 };


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

  pinMode(sensorInfrarrojo, INPUT);
}

//Calcula la distancia para un transmisor
float readDistance(SensorUltrasonico sensor) {
  // Generar un pulso de 10 microsegundos en el pin TRIG
  digitalWrite(sensor.trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trig, LOW);

  // Leer el tiempo de duracion del pulso de eco en el pin ECHO
  float duration = pulseIn(sensor.echo, HIGH);

  // Calcular la distancia en centimetros usando la velocidad del sonido
  // en el aire (aproximadamente 343 m/s a 20gradosC)
  float distance = duration * 0.0343 / 2; //58.2;

  return distance;
}

bool detectarInfrarrojo(int pinSensor){
  if( digitalRead(pinSensor) ==  1 ){
    Serial.println("Algo Detected!");
    return TRUE;
  }
  else {
    return FALSE;
    Serial.println("No hay NADA");
  }
}

#pragma region Movimientos posibles
//Detiene todos los motores
void detenerMotores() {
  Serial.println("DETENIENDO motores");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, LOW);
}

//Gira el zumo hacia la derecha
void girarDerecha() {
  Serial.println("Girando a DERECHA");
  digitalWrite(motorIzqP, HIGH);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, LOW);
}
void girarDerecha(int tiempo){
  girarDerecha();
  delay(tiempo);
  detenerMotores();
}

//Gira el zumo hacia la izquierda
void girarIzquierda() {
  Serial.println("Girando a IZQUIERDA");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, HIGH);
  digitalWrite(motorDerN, LOW);
}
void girarIzquierda(int tiempo){
  girarIzquierda();
  delay(tiempo);
  detenerMotores();
}

//Mueve el zumo hacia adelante
void moverAdelante() {
  Serial.println("Moviendo ADELANTE");
  digitalWrite(motorIzqP, HIGH);
  digitalWrite(motorIzqN, LOW);
  digitalWrite(motorDerP, HIGH);
  digitalWrite(motorDerN, LOW);
}
void moverAdelante(int tiempo){
  moverAdelante();
  delay(tiempo);
  detenerMotores();
}

//Mueve el zumo hacia atras
void moverAtras(){
  Serial.println("Moviendo ATRAS");
  digitalWrite(motorIzqP, LOW);
  digitalWrite(motorIzqN, HIGH);
  digitalWrite(motorDerP, LOW);
  digitalWrite(motorDerN, HIGH);
}
void moverAtras(int tiempo){
  moverAtras();
  delay(tiempo);
  detenerMotores();
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

int detectarUltrasonico(){
  /*
  
  HAY UNA MEJOR FORMA DE HACER ESTA FUNCIÓN
  Cada sensor deberia de tener una "pared invisible máxima" desde la cual detecte si hay o no alguien ahí
  
  */
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
void estrategiaAntigua() {
  
  while (!enemigoDetectado) {
    girarIzquierda();
    if (ultrasonicoMenor() == FRENTE) { enemigoDetectado = true; }
  }
  moverAdelante();
}


void estrategiaBASE(){
    while( ultrasonicoMenor != FRENTE ) {
    girarDerecha();
  }
  moverAdelante();
}


bool unavez = true;
//      MAIN LOOP
void loop() {

  if (unavez){
    delay(5000);
    unavez = false;
  }
  estrategiaBASE();

}
//
