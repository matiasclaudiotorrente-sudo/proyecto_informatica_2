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

class Led {
public:
    uint8_t pin;

    Led(uint8_t pinLed) {
        pin = pinLed;
    }

    void inicializar() {
        pinMode(pin, OUTPUT);
        apagar();
    }

    void encender() {
        digitalWrite(pin, HIGH);
    }

    void apagar() {
        digitalWrite(pin, LOW);
    }
};

const uint8_t PIN_BOTON_0 = 2;
const uint8_t PIN_BOTON_1 = 3;
const uint8_t PIN_BOTON_2 = 4;
const uint8_t PIN_BOTON_3 = 5;

const uint8_t PIN_LED_GOLPE = 8;
const uint8_t PIN_BUZZER = 9;

LiquidCrystal lcd(12, 11, 6, 7, 10, 13);

SensorPulsador botones[4] = {
    SensorPulsador(PIN_BOTON_0),
    SensorPulsador(PIN_BOTON_1),
    SensorPulsador(PIN_BOTON_2),
    SensorPulsador(PIN_BOTON_3)
};

Led ledGolpe(PIN_LED_GOLPE);
int contadorGolpes = 0;

void setup() {
    Serial.begin(9600);
    ledGolpe.inicializar();

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
    bool algunBotonPresionado = false;

    for (int i = 0; i < 4; i++) {
        if (botones[i].fuePresionado()) {
            Serial.println(i);

            contadorGolpes++;
            actualizarLCD();

            tone(PIN_BUZZER, 440, 150);  // Sonido de 440 Hz durante 150 ms
        }

        if (botones[i].leerMedicion() == LOW) {
            algunBotonPresionado = true;
        }
    }

    if (algunBotonPresionado) {
        ledGolpe.encender();
    } else {
        ledGolpe.apagar();
    }
}

void actualizarLCD() {
    lcd.setCursor(13, 1);
    lcd.print(contadorGolpes);
}