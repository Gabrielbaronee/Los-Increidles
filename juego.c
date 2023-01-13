#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kronos.h"
#include "increidle.h"

const int GANADOR_JUEGO = 1;
const int CONTINUE_JUEGO = 0;
const int PERDIO_JUEGO = -1;

const char MOV_DERECHA = 'D';
const char MOV_ABAJO = 'S';
const char MOV_IZQUIERDA = 'A';
const char MOV_ARRIBA = 'W';
const char ACT_PODER = 'C';

const char LETRA_NO_ADIVINADA = 'X';

// Pre: -
// Pos: Analiza las respuestas del usuario y devuelve a la funcion main si el usuario adivino o no la contraseña.
bool es_contrasenia_correcta(char contrasenia_adivinada[MAX_CONTRASENIA]){
	int tope_letras = 6;
	bool es_correcta = true;
	for(int i = 0; i < tope_letras; i++){
		if(contrasenia_adivinada[i] == LETRA_NO_ADIVINADA){
			es_correcta = false;
		}
	}
	return es_correcta;
}


// Pre: -
// Pos: Guia al usuario sobre como jugar al juego.
void interfaz_usuario(juego_t* juego){

	printf("\n-------------------------------------------º---------------------------------------------------\n");
	if(juego->id_personaje_actual == 1){
		printf("Personaje actual: Elastic Girl (E). Busca a Violeta (V)\n");
	}	
	else if(juego->id_personaje_actual == 2){
		printf("Personaje actual: Violeta (V). Busca a Dash (D)\n");
	}	
	else if(juego->id_personaje_actual == 3){
		printf("Personaje actual: Dash (D). Busca a Mr.Increible (I)\n");
	}
	else if(juego->id_personaje_actual == 4){
		printf("Corre hacia la salida! (S)\n");
	}		
	
	printf("Evita tocar los laseres! (L)\n");
	printf("Esquiva las pinzas! (P)\n");	
	printf("Movimientos restantes: %i\n", juego->personajes[juego->id_personaje_actual-1].movimientos);
	if(juego->personajes[juego->id_personaje_actual-1].poder_activado == true){
		if(juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder > 0){
			printf("Movimientos con poder: %i", juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder);
		}
	}

	printf("\nSeleccione su movimiento: (W) Arriba, (D) Derecha, (S) Abajo, (A) Izquierda, (C) Activar Poder\n");
}


int main(){

	char contrasenia_adivinada[MAX_CONTRASENIA];
	adivinar_contrasenia(contrasenia_adivinada);
	bool es_correcta;
	es_correcta = es_contrasenia_correcta(contrasenia_adivinada);
	system("clear");
	srand((unsigned)time(NULL));
	
	juego_t juego;
	inicializar_juego(&juego, es_correcta);
	char movimiento;
	int est_juego;
	do{
		imprimir_terreno(juego);
		interfaz_usuario(&juego);
		do{
			scanf(" %c", &movimiento);
		}while(movimiento != MOV_ARRIBA && movimiento != MOV_DERECHA && movimiento != MOV_ABAJO && 
			movimiento != MOV_IZQUIERDA && movimiento != ACT_PODER);	
		realizar_jugada(&juego, movimiento);
		est_juego = estado_juego(juego);
		system("clear");	
	}while(est_juego == CONTINUE_JUEGO);
	
	
	if(est_juego == GANADOR_JUEGO){
		printf(" \
 ######      ###    ##    ##    ###     ######  ######## ######## \n \
##    ##    ## ##   ###   ##   ## ##   ##    ##    ##    ##       \n \
##         ##   ##  ####  ##  ##   ##  ##          ##    ##       \n \
##   #### ##     ## ## ## ## ##     ##  ######     ##    ######   \n \
##    ##  ######### ##  #### #########       ##    ##    ##       \n \
##    ##  ##     ## ##   ### ##     ## ##    ##    ##    ##       \n \
 ######   ##     ## ##    ## ##     ##  ######     ##    ######## \n");
	}
	else if(est_juego == PERDIO_JUEGO){
		printf(" \
########  ######## ########  ########  ####  ######  ######## ######## \n \
##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n \
##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n \
########  ######   ########  ##     ##  ##   ######     ##    ######   \n \
##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n \
##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n \
##        ######## ##     ## ########  ####  ######     ##    ######## \n");
	}

	return 0;
}