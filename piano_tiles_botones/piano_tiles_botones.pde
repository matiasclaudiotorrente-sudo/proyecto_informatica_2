import processing.serial.*;

Serial miPuerto;

int numCarriles = 4;
float anchoCarril;

ArrayList<Ficha> fichas = new ArrayList<Ficha>();

float velocidadBase = 5.0;
int puntaje = 0;
boolean gameOver = false;
boolean juegoIniciado = false;

String[] nombresPines = {"IZQ", "IZQ MED", "DER MED", "DER"};

void setup() {
  size(400, 600);
  anchoCarril = width / (float)numCarriles;


  printArray(Serial.list());

  
  String nombrePuerto = Serial.list()[1];
  miPuerto = new Serial(this, nombrePuerto, 9600);
  miPuerto.bufferUntil('\n');

  iniciarJuego();
}

void draw() {
  background(240);

  stroke(200);
  strokeWeight(2);
  for (int i = 1; i < numCarriles; i++) {
    line(i * anchoCarril, 0, i * anchoCarril, height);
  }

  for (int i = 0; i < numCarriles; i++) {
    fill(220);
    rect(i * anchoCarril, height - 40, anchoCarril, 40);
    fill(50);
    textSize(12);
    textAlign(CENTER, CENTER);
    text(nombresPines[i], i * anchoCarril + anchoCarril/2, height - 20);
  }

  if (!juegoIniciado) {
    fill(0, 180);
    rect(0, 0, width, height);
    fill(255);
    textSize(22);
    textAlign(CENTER, CENTER);
    text("PIANO TILES\n\nPresiona ESPACIO en el teclado\no cualquier BOTÓN para empezar",
         width/2, height/2);
    return;
  }

  if (gameOver) {
    fill(200, 0, 0, 200);
    rect(0, 0, width, height);
    fill(255);
    textSize(28);
    textAlign(CENTER, CENTER);
    text("¡GAME OVER!", width/2, height/2 - 40);
    textSize(20);
    text("Puntaje final: " + puntaje, width/2, height/2 + 10);
    textSize(14);
    text("Presiona R para reiniciar", width/2, height/2 + 60);
    return;
  }

  for (int i = fichas.size() - 1; i >= 0; i--) {
    Ficha f = fichas.get(i);
    f.actualizar(velocidadBase + (puntaje / 10.0));
    f.dibujar();

    if (f.y > height - 40 && !f.tocada) {
      gameOver = true;
    }

    if (f.y > height) {
      fichas.remove(i);
    }
  }

  if (fichas.isEmpty() || fichas.get(fichas.size() - 1).y > 150) {
    crearNuevaFicha();
  }

  fill(0);
  textSize(24);
  textAlign(LEFT, TOP);
  text("Puntos: " + puntaje, 15, 15);
}

void serialEvent(Serial p) {
  String lectura = p.readStringUntil('\n');

  if (lectura == null) {
    return;
  }

  lectura = trim(lectura);

  try {
    int carrilPresionado = int(lectura);

    if (carrilPresionado >= 0 && carrilPresionado < 4) {
      procesarPulsacion(carrilPresionado);
    }
  } catch (Exception e) {
    println("Dato recibido no válido: " + lectura);
  }
}

void procesarPulsacion(int carril) {
  if (!juegoIniciado) {
    juegoIniciado = true;
    return;
  }

  if (gameOver) {
    return;
  }

  boolean acierto = false;

  for (Ficha f : fichas) {
    if (f.carril == carril && !f.tocada) {
      if (f.y + f.alto > height - 250 && f.y < height - 20) {
        f.tocada = true;
        puntaje++;
        acierto = true;
        break;
      }
    }
  }

  if (!acierto) {
    gameOver = true;
  }
}

void keyPressed() {
  if (key == ' ') {
    if (!juegoIniciado) {
      juegoIniciado = true;
    }
  }

  if ((key == 'r' || key == 'R') && gameOver) {
    iniciarJuego();
  }
}

void iniciarJuego() {
  fichas.clear();
  puntaje = 0;
  gameOver = false;
  juegoIniciado = false;
  crearNuevaFicha();
}

void crearNuevaFicha() {
  int carrilAleatorio = int(random(0, numCarriles));
  fichas.add(new Ficha(carrilAleatorio));
}

class Ficha {
  int carril;
  float y;
  float alto = 140;
  boolean tocada = false;

  Ficha(int c) {
    carril = c;
    y = -alto;
  }

  void actualizar(float velocidad) {
    y += velocidad;
  }

  void dibujar() {
    if (!tocada) {
      fill(30);
      rect(carril * anchoCarril, y, anchoCarril, alto, 5);
    } else {
      fill(180, 180, 180, 100);
      rect(carril * anchoCarril, y, anchoCarril, alto, 5);
    }
  }
}
