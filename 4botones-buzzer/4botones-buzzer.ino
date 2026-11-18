const byte pinBotones = A0;
const byte pinBuzzer = 8;
int botonAnterior = -1;

int detectarBoton(int valor) {
  if (valor < 100) return -1;
  if (valor < 150) return 1;
  if (valor < 250) return 2;
  if (valor < 350) return 3;
  if (valor < 850) return 4;
  return 5;
}

void setup() {
  Serial.begin(9600);
  pinMode(pinBuzzer, OUTPUT);
  Serial.println("Listo. Presiona un boton.");
}

void loop() {
  int valor = analogRead(pinBotones);
  int botonActual = detectarBoton(valor);

  if (botonActual != botonAnterior) {
    if (botonActual != -1) {
      Serial.print("Funciona el boton ");
      Serial.println(botonActual);

      tone(pinBuzzer, 440, 150);  // Sonido de 440 Hz durante 150 ms
    }

    botonAnterior = botonActual;
    delay(40);
  }
}