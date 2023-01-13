#include <stdio.h>
#include <stdbool.h>
#include "increidle.h"


const char PRIMERA_LETRA_CONT = 'K';
const char SEGUNDA_LETRA_CONT = 'R';
const char TERCERA_LETRA_CONT = 'O';
const char CUARTA_LETRA_CONT = 'N';
const char QUINTA_LETRA_CONT = 'S';
const char CONT_VACIA = 'X';
const int VALOR_MAXIMO = 100;
const int VALOR_MINIMO = 1;
const int NUMERO_RAYO = 95;

// Pre: -
// Pos: Despues de que el usuario responda la pregunta, guarda en la variable primera_verificacion si la 
// respondio de manera correcta o incorrecta.
void primera_pregunta(bool* primera_verificacion){
	char primera_respuesta;

	printf("Antes de comenzar el juego, haremos unas preguntas para determinar la dificultad del mismo.\nLa tematica de las preguntas seran sobre peliculas de Disney. Si contesta todas las preguntas correctamente, la dificultad del juego sera reducida.\n\n");

	do{
		printf("¿Cuál es el código de emergencia utilizado en Monsters Inc. cuando uno de los personajes tiene una media humana pegada a su cuerpo?\n");
		printf("(A) 3312\n(B) 3317\n(C) 2312\n(D) 2031\n");
		scanf(" %c", &primera_respuesta);

	}while((primera_respuesta != 'A') && (primera_respuesta != 'B') && (primera_respuesta != 'C') && (primera_respuesta != 'D'));

	if(primera_respuesta == 'A'){
		*(primera_verificacion) = true;
	}
	else{
		*(primera_verificacion) = false;
	}
}
// Pre: -
// Pos: Despues de que el usuario responda la pregunta, guarda en la variable segunda_verificacion si la 
// respondio de manera correcta o incorrecta. 
void segunda_pregunta(bool* segunda_verificacion){
	char segunda_respuesta;
	

	do{
		printf("¿Cuál era el nombre del pez mascota de Lilo?\n");
		printf("(A) Stich\n(B) Pez\n(C) Pato\n(D) Aurelio\n");
		scanf(" %c", &segunda_respuesta);

	}while((segunda_respuesta != 'A') && (segunda_respuesta != 'B') && (segunda_respuesta != 'C') && (segunda_respuesta != 'D'));

	if(segunda_respuesta == 'C'){
		*(segunda_verificacion) = true;
	}
	else{
		*(segunda_verificacion) = false;
	}
}
// Pre: -
// Pos: Funcion que le pide al usuario que ingrese un numero, luego de ser ingresado, el numero se guarda en
// un vector.
void ingresar_num(int Numeros[MAX_NUMEROS], int tope_numeros){

	int numero_ingresado;
	do{
		printf("Ingrese un numero: ");
		scanf(" %i", &numero_ingresado);

	}while((numero_ingresado) > VALOR_MAXIMO || (numero_ingresado < VALOR_MINIMO));
	
	Numeros[tope_numeros] = numero_ingresado;
}
// Pre: -
// Pos: Funcion que recorre el vector "Numeros" y  verifica si los numeros ingresados por el usuario son numeros
// primos o no.
void verificar_numero_primo(int Numeros[MAX_NUMEROS], int tope_numeros, bool* numero_no_primo){
	
	for(int i = 0; i < tope_numeros ;i++){
		int numero = Numeros[i]; 
		int contador = 0;
		for(int j = 1; j <= numero; j++){
			if(numero % j == 0){
				contador++;
			}
		}
		if(contador > 2 || numero == 1){
			(*numero_no_primo) = true;
		}
	}
}
// Pre: -
// Pos: Funcion que contiene varias subfunciones que le permite al usuario responder la tercera pregunta.
void tercera_pregunta(bool* es_primo){
	printf("Tadashi necesita que lo ayudes con unos cálculos. Ingrese 5 números, todos deben ser primos\n");
	
	int tope_numeros = 0;
	int Numeros[MAX_NUMEROS];

	ingresar_num(Numeros, tope_numeros);
	tope_numeros++;
	
	ingresar_num(Numeros, tope_numeros);
	tope_numeros++;
	
	ingresar_num(Numeros, tope_numeros);
	tope_numeros++;
	
	ingresar_num(Numeros, tope_numeros);
	tope_numeros++;

	ingresar_num(Numeros, tope_numeros);
	tope_numeros++;

	bool numero_no_primo = false;
	verificar_numero_primo(Numeros, tope_numeros, &numero_no_primo);

	if(numero_no_primo == true){
		*es_primo = false;
	}
	else if(numero_no_primo == false){
		*es_primo = true;
	}
}
// Pre: -
// Pos: Despues de que el usuario responda la pregunta, guarda en la variable cuarta_verificacion si la 
// respondio de manera correcta o incorrecta.
void cuarta_pregunta(bool* cuarta_verificacion){
	int cuarta_respuesta;
	
	do{
		printf("¿Qué número lleva el Rayo McQueen?\n");
		scanf(" %i", &cuarta_respuesta);

	}while(cuarta_respuesta > VALOR_MAXIMO || cuarta_respuesta < VALOR_MINIMO);

	if(cuarta_respuesta == NUMERO_RAYO){
		*(cuarta_verificacion) = true;
	}
	else if(cuarta_respuesta != NUMERO_RAYO){
		*(cuarta_verificacion) = false;
	}
}
// Pre: -
// Pos: Despues de que el usuario responda la pregunta, guarda en la variable quinta_verificacion si la 
// respondio de manera correcta o incorrecta.
void quinta_pregunta(bool* quinta_verificacion){

	char quinta_respuesta;

	do{
		printf("¿Cómo se llama el tigre de la princesa Jazmín?\n");
		printf("(A) Zafiro\n(B) Abú\n(C) Rajah\n(D) Jafar\n");
		scanf(" %c", &quinta_respuesta);

	}while((quinta_respuesta != 'A') && (quinta_respuesta != 'B') && (quinta_respuesta != 'C') && (quinta_respuesta != 'D'));

	if(quinta_respuesta == 'C'){
		*(quinta_verificacion) = true;
	}
	else{
		*(quinta_verificacion) = false;
	}
}
// Pre: -
// Pos: Analiza todas las respuestas respondidas por el usuario y revela la contraseña.
void verificacion_preguntas(bool primera_verificacion, bool segunda_verificacion, bool es_primo, bool cuarta_verificacion, bool quinta_verificacion, char vector_contrasena[MAX_CONTRASENIA]){
	int tope_letras = 6;
	
	
	if(primera_verificacion == true){
		vector_contrasena[0] = PRIMERA_LETRA_CONT;
	}
	else if(primera_verificacion == false){
		vector_contrasena[0] = CONT_VACIA;
	}
	if(segunda_verificacion == true){
		vector_contrasena[1] = SEGUNDA_LETRA_CONT;
	}
	else if(segunda_verificacion == false){
		vector_contrasena[1] = CONT_VACIA;
	}
	if(es_primo == true){
		vector_contrasena[2] = TERCERA_LETRA_CONT;
		vector_contrasena[4] = TERCERA_LETRA_CONT;
	}
	else if(es_primo == false){
		vector_contrasena[2] = CONT_VACIA;
		vector_contrasena[4] = CONT_VACIA;
	}
	if(cuarta_verificacion == true){
		vector_contrasena[3] = CUARTA_LETRA_CONT;
	}
	else if(cuarta_verificacion == false){
		vector_contrasena[3] = CONT_VACIA;
	}	
	if(quinta_verificacion == true){
		vector_contrasena[5] = QUINTA_LETRA_CONT;
	}
	else if(quinta_verificacion == false){
		vector_contrasena[5] = CONT_VACIA;
	}
	printf("Con las letras que pudiste revelar la contraseña es:\n");
	printf("-");
	for(int i = 0; i < tope_letras; i++){
		printf("%c", vector_contrasena[i]);
	}
	printf("-.\n");
}

void adivinar_contrasenia(char contrasenia_adivinada[MAX_CONTRASENIA]){

	bool primera_verificacion = false;
	primera_pregunta(&primera_verificacion);
	
	bool segunda_verificacion = false;
	segunda_pregunta(&segunda_verificacion);

	bool es_primo = false;
	tercera_pregunta(&es_primo);
	
	bool cuarta_verificacion = false;
	cuarta_pregunta(&cuarta_verificacion);

	bool quinta_verificacion = false;
	quinta_pregunta(&quinta_verificacion);

	verificacion_preguntas(primera_verificacion, segunda_verificacion, es_primo, cuarta_verificacion, quinta_verificacion, contrasenia_adivinada);
	
}