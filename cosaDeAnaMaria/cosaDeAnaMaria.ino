const int IZQUIERDA = 0;
const int FRENTE = 1;
const int DERECHA = 2;


const int sensorInfrarrojo = 12;  // Cambiar al pin que corresponda

const int motorIzqN = 11; const int motorIzqP = 10;
const int motorDerN =  8; const int motorDerP =  9;

const int COLOR_NEGRO = 0;

const int Interruptor = A0;

//Estructura de sensor
struct sensor {
  int echo;
  int trig;
};

sensor sensorIzq = { 2, 3 };  // { Echo, Trigger }
sensor sensorFrn = { 4, 5 };
sensor sensorDer = { 6, 7 };

void setup() {
  // Configurar los pines de los sensores como entradas
  Serial.begin(9600);  // Inicializar la comunicacion serial para ver los mensajes en el monitor

  pinMode(motorIzqP, OUTPUT);
  pinMode(motorIzqN, OUTPUT);
  pinMode(motorDerP, OUTPUT);
  pinMode(motorDerN, OUTPUT);

  //Activar con PinMode los pines de los sensores
  //TRIGGERS OUTPUT
  //ECHOES INPUT
  pinMode(sensorIzq.trig, OUTPUT);
  pinMode(sensorIzq.echo, INPUT);

  pinMode(sensorDer.trig, OUTPUT);
  pinMode(sensorDer.echo, INPUT);

  pinMode(sensorFrn.trig, OUTPUT);
  pinMode(sensorFrn.echo, INPUT);

  pinMode(Interruptor, OUTPUT);
}

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
  delay(1000);
  detenerMotores();
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
int ultrasonicos() {

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


/*
=======ESTRATEGIAS
*/

//Estrategia de AnaMaria que ella entendera xdddddd
//Estrategia como pensando
void estrategia1() {
  int locacionEnemigo = ultrasonicos();

  switch (ultrasonicos()) {  //ACA PUEDE SUCEDER UN ERROR
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
    if (ultrasonicos() == FRENTE) { enemigoDetectado = true; }
  }
  moverAdelante();
}
//      MAIN LOOP
void loop() {
  estrategia2();
  /*
  delay(1000);
  Serial.print(contador);
  Serial.print(" Detectando con ultrasonicos -> ");
  Serial.println(ultrasonicos());
  Serial.println("");
  contador += 1;
  */
  //Serial.println(digitalRead(Interruptor));
}