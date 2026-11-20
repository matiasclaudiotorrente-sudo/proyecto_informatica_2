#include <LiquidCrystal.h>

class Sensor {
public:
    float valor;
    String magnitud;

    Sensor(String mag) {
        magnitud = mag;
        valor = 0.0;
    }

    float leerMedicion() {
        return valor;
    }
};

class SensorPulsador : public Sensor {
public:
    uint8_t pinFisico;
    bool estadoAnterior;
    unsigned long ultimoCambio;
    const unsigned long tiempoRebote = 50;

    SensorPulsador(uint8_t pin) : Sensor("Estado") {
        pinFisico = pin;
        estadoAnterior = HIGH;
        ultimoCambio = 0;
    }

    void inicializar() {
        pinMode(pinFisico, INPUT_PULLUP);
    }

    bool fuePresionado() {
        bool lecturaActual = digitalRead(pinFisico);
        bool disparo = false;

        if (lecturaActual != estadoAnterior) {
            if (millis() - ultimoCambio >= tiempoRebote) {
                ultimoCambio = millis();

                if (lecturaActual == LOW) {
                    disparo = true;
                }

                estadoAnterior = lecturaActual;
            }
        }

        valor = lecturaActual;
        return disparo;
    }
};

// Configuración de pines
const uint8_t PIN_BOTON_0 = 2;
const uint8_t PIN_BOTON_1 = 3;
const uint8_t PIN_BOTON_2 = 4;
const uint8_t PIN_BOTON_3 = 5;

const uint8_t PIN_BUZZER = 8; // Buzzer pasivo en el pin 8

LiquidCrystal lcd(12, 11, 6, 7, 10, 13);

SensorPulsador botones[4] = {
    SensorPulsador(PIN_BOTON_0),
    SensorPulsador(PIN_BOTON_1),
    SensorPulsador(PIN_BOTON_2),
    SensorPulsador(PIN_BOTON_3)
};

int contadorGolpes = 0;

// Notas en Hz de "Cumpleaños Feliz" (25 notas)
const int melodiaCumple[25] = {
    262, 262, 294, 262, 349, 330,           // Que los cum-plas fe-liz
    262, 262, 294, 262, 392, 349,           // Que los cum-plas fe-liz
    262, 262, 523, 440, 349, 330, 294,      // Que los cum-plas...
    466, 466, 440, 349, 392, 349            // Que los cum-plas fe-liz
};
int indiceNota = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_BUZZER, OUTPUT);

    for (int i = 0; i < 4; i++) {
        botones[i].inicializar();
    }

    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PIANO TILES UTN");
    lcd.setCursor(0, 1);
    lcd.print("Pulsaciones: 0");
}

void loop() {
    // 1. Lectura de los 4 botones físicos
    for (int i = 0; i < 4; i++) {
        if (botones[i].fuePresionado()) {
            Serial.println(i);
            contadorGolpes++;
            actualizarLCD();
        }
    }

    // 2. Escuchar respuestas de Processing para reproducir sonido en el Buzzer (Pin 8)
    if (Serial.available() > 0) {
        char comando = Serial.read();

        if (comando == 'A') {
            // Acierto: toca la siguiente nota de Cumpleaños Feliz
            tone(PIN_BUZZER, melodiaCumple[indiceNota], 220);
            indiceNota = (indiceNota + 1) % 25;
        } 
        else if (comando == 'G') {
            // Game Over: sonido grave de error y reinicio de melodía
            tone(PIN_BUZZER, 130, 500);
            indiceNota = 0;
        } 
        else if (comando == 'R') {
            // Reinicio manual: vuelve a la primera nota
            indiceNota = 0;
        }
    }
}

void actualizarLCD() {
    lcd.setCursor(13, 1);
    lcd.print(contadorGolpes);
}