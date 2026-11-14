#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct {
	char calle1[50];
	char calle2[50];
	int trafico;
} Semaforo;

void agregarSemaforo(Semaforo **arr, int *n, int *capacidad) {
	if (*n >= *capacidad) {
		*capacidad = (*capacidad == 0) ? 5 : (*capacidad * 2);
		Semaforo *temp = realloc(*arr, (*capacidad) * sizeof(Semaforo));
		if (temp == NULL) {
			printf("Error de asignacion de memoria.\n");
			return;
		}
		*arr = temp;
	}
	
	printf("Ingrese nombre de la Calle 1: ");
	scanf(" %[^\n]s", (*arr)[*n].calle1);
	printf("Ingrese nombre de la Calle 2: ");
	scanf(" %[^\n]s", (*arr)[*n].calle2);
	
	do {
		printf("Ingrese intensidad de trafico (1-100): ");
		scanf("%d", &(*arr)[*n].trafico);
	} while ((*arr)[*n].trafico < 1 || (*arr)[*n].trafico > 100);
	
	(*n)++;
	printf("Semaforo agregado exitosamente.\n");
}

void buscarPorCalle(Semaforo *arr, int n) {
	char busqueda[50];
	printf("Ingrese el nombre de la calle a buscar: ");
	scanf(" %[^\n]s", busqueda);
	
	printf("\nIntersecciones encontradas para '%s':\n", busqueda);
	int encontrados = 0;
	for (int i = 0; i < n; i++) {
		if (strstr(arr[i].calle1, busqueda) != NULL || strstr(arr[i].calle2, busqueda) != NULL) {
			printf("- Interseccion: %s y %s | Trafico: %d\n", arr[i].calle1, arr[i].calle2, arr[i].trafico);
			encontrados++;
		}
	}
	if (encontrados == 0) {
		printf("No se encontraron intersecciones para esa calle.\n");
	}
}

void operarCadenas(Semaforo *arr, int n) {
	if (n == 0) {
		printf("No hay semaforos cargados.\n");
		return;
	}
	int idx;
	printf("Ingrese el indice del semaforo (0 a %d): ", n - 1);
	scanf("%d", &idx);
	
	if (idx < 0 || idx >= n) {
		printf("Indice invalido.\n");
		return;
	}
	
	char combinada[100];
	strcpy(combinada, arr[idx].calle1);
	strcat(combinada, " / ");
	strcat(combinada, arr[idx].calle2);
	
	printf("Nombre combinado de la interseccion (copia y concatenacion): %s\n", combinada);
	
	char letra;
	printf("Ingrese un caracter a buscar en el nombre combinado: ");
	scanf(" %c", &letra);
	char *resultado = strchr(combinada, letra);
	if (resultado != NULL) {
		printf("Caracter '%c' encontrado en la posicion: %ld\n", letra, resultado - combinada);
	} else {
		printf("Caracter no encontrado.\n");
	}
}

void mostrarTodos(Semaforo *arr, int n) {
	if (n == 0) {
		printf("No hay semaforos registrados.\n");
		return;
	}
	printf("\n--- LISTA DE SEMAFOROS ---\n");
	for (int i = 0; i < n; i++) {
		printf("[%d] %s & %s - Trafico: %d\n", i, arr[i].calle1, arr[i].calle2, arr[i].trafico);
	}
}

int main() {
	Semaforo *arr = NULL;
	int n = 0;
	int capacidad = 0;
	int opcion;
	
	do {
		printf("\n--- MENU DE SEMAFOROS ---\n");
		printf("1. Agregar semaforo\n");
		printf("2. Mostrar todos los semaforos\n");
		printf("3. Buscar intersecciones por calle\n");
		printf("4. Operaciones de cadenas en interseccion\n");
		printf("5. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		
		switch (opcion) {
		case 1:
			agregarSemaforo(&arr, &n, &capacidad);
			break;
		case 2:
			mostrarTodos(arr, n);
			break;
		case 3:
			buscarPorCalle(arr, n);
			break;
		case 4:
			operarCadenas(arr, n);
			break;
		case 5:
			printf("Saliendo del programa...\n");
			break;
		default:
			printf("Opcion invalida.\n");
		}
	} while (opcion != 5);
	
	free(arr);
	return 0;
}
