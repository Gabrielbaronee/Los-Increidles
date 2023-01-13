#include <stdio.h>
#include "kronos.h"
#include "increidle.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


#define MAX_FILAS 20
#define MAX_COLUMNAS 20
#define MAX_COORDENADAS 50

const char MOVIMIENTO_ARRIBA = 'W';
const char MOVIMIENTO_DERECHA = 'D';
const char MOVIMIENTO_ABAJO = 'S';
const char MOVIMIENTO_IZQUIERDA = 'A';
const char ACTIVACION_PODER = 'C';

const char PERSONAJE_MR_INCREIBLE = 'I';
const char PERSONAJE_VIOLETA = 'V';
const char PERSONAJE_DASH = 'D';
const char PERSONAJE_ELASTIC_GIRL = 'E';

const char OBSTACULO_ROBOT = 'R';
const char OBSTACULO_LASER = 'L';
const char OBSTACULO_PINZA = 'P';
const char OBSTACULO_SALIDA = 'S';
const char OBSTACULO_SUPERTRAJE = 'T';

const int VALOR_GANADOR_JUEGO = 1;
const int VALOR_CONTINUE_JUEGO = 0;
const int VALOR_PERDIO_JUEGO = -1;

const int PRIMER_CUADRANTE = 1;
const int SEGUNDO_CUADRANTE = 2;
const int TERCER_CUADRANTE = 3;
const int CUARTO_CUADRANTE = 4;

const int MOVIMIENTOS_ELASTIC = 25;
const int MOVIMIENTOS_VIOLETA = 30;
const int MOVIMIENTOS_DASH = 20;
const int MOVIMIENTOS_INCREIBLE = 15;
const int MOVIMIENTOS_PODER = 5, MOVIMIENTOS_PODER_DASH = 1;

const int NO_USADO = -1;
const int FILA_0 = 0, COLUMNA_0 = 0, FILA_10 = 9, COLUMNA_10 = 9, FILA_20 = 19, COLUMNA_20 = 19;
const int CANT_PERSONAJES = 4, CANT_SUPERTRAJE = 4, CANT_ROBOTS = 4, CANT_PINZAS = 16;
const int SIN_MOVIMIENTOS = 0;

// Pre: - 
// Pos: Inicializa todas las variables de los robots del juego.
void inicializar_robots(robot_t* robot_v1, robot_t* robot_v2, robot_t* robot_v3, robot_t* robot_v4, bool contrasenia_completa, juego_t* juego){

	juego->robots[0] = *robot_v1;
	juego->robots[1] = *robot_v2;
	juego->robots[2] = *robot_v3;
	juego->robots[3] = *robot_v4;
	juego->tope_robots = 0;
	juego->tope_pinzas = 0;
	
	if(contrasenia_completa == true){
		juego->longitud_laser = 3;
		for(int i = 0; i < CANT_ROBOTS; i++){
			juego->robots[i].tope_lasers = 6;
		}
	}
	else if(contrasenia_completa == false){
		juego->longitud_laser = 5;
		for(int i = 0; i < CANT_ROBOTS; i++){
			juego->robots[i].tope_lasers = 10;
		}
	}
}

// Pre: -
// Pos:	Inicializa todas las variables de los personajes del juego.
void inicializar_personajes(personaje_t* personaje_increible, personaje_t* personaje_violeta, 
	personaje_t* personaje_dash, personaje_t* personaje_elastic_girl, juego_t* juego){

	juego->personajes[0] = *personaje_elastic_girl;
	juego->personajes[1] = *personaje_violeta;
	juego->personajes[2] = *personaje_dash;
	juego->personajes[3] = *personaje_increible;
	juego->tope_personajes = 0;
	juego->id_personaje_actual = 1;

	for(int i = 0; i < CANT_PERSONAJES; i++){
		juego->personajes[i].cuadrante_inicial = i + 1;
		juego->personajes[i].tiene_supertraje = false;
		juego->personajes[i].poder_activado = false;
		if(i == 2){
			juego->personajes[i].movimientos_con_poder = MOVIMIENTOS_PODER_DASH;
		}
		else{
			juego->personajes[i].movimientos_con_poder = MOVIMIENTOS_PODER;
		}
	}
	juego->personajes[0].movimientos = MOVIMIENTOS_ELASTIC;
	juego->personajes[1].movimientos = MOVIMIENTOS_VIOLETA;
	juego->personajes[2].movimientos = MOVIMIENTOS_DASH;
	juego->personajes[3].movimientos = MOVIMIENTOS_INCREIBLE;

}

// Pre: -
// Pos: Inicializa las variables de todos los supertrajes del juego.
void inicializar_supertrajes(supertraje_t* supertraje_v1, supertraje_t* supertraje_v2, supertraje_t* supertraje_v3, 
	supertraje_t* supertraje_v4, juego_t* juego){

	juego->supertrajes[0] = *supertraje_v1;
	juego->supertrajes[1] = *supertraje_v2;
	juego->supertrajes[2] = *supertraje_v3;
	juego->supertrajes[3] = *supertraje_v4;
	juego->tope_supertraje = 0;
	
	for(int i = 0; i < CANT_SUPERTRAJE; i++){
		juego->supertrajes[i].cuadrante = i + 1;
		juego->supertrajes[i].recolectado = false;
		juego->supertrajes[i].usado = false;
	}
}

// Pre: marca = 0, se chequea que las posiciones de los robots no coincidan.
//      marca = 1, se chequea que las pinzas creadas no coincidan con los robots o laseres.
// 		marca = 2, se chequea que los supertrajes no coincidan con robots, laseres y pinzas.
// 		marca = 3, se chequea que los personajes no coincidan con robots, laseres, pinzas y supertrajes.
// Pos: Retorna true si la posicion aleatoria generada coincide con una posicion generada anteriormente,
//		false si no coincide.
bool chequeo_posiciones(int marca, int numero_aleatorio_fil, int numero_aleatorio_col, juego_t*  juego){

	bool posicion_confirmada = false;

	if(marca >= 0){
		for(int i = 0; i < juego->tope_robots; i++){
			if(numero_aleatorio_fil == juego->robots[i].posicion.fila && numero_aleatorio_col == 
				juego->robots[i].posicion.columna){
				posicion_confirmada = true;
			}
		}
		for(int i = 0; i < juego->tope_robots; i++){
			for(int j = 0; j < juego->robots[i].tope_lasers; j++){
				if(numero_aleatorio_fil == juego->robots[i].lasers[j].fila && 
					numero_aleatorio_col == juego->robots[i].lasers[j].columna){
					posicion_confirmada = true;
				}
			}
		}
	}
	if(marca >= 1){
		for(int i = 0; i < juego->tope_pinzas; i++){
			if(numero_aleatorio_fil == juego->pinzas[i].fila && numero_aleatorio_col == 
			juego->pinzas[i].columna){
				posicion_confirmada = true;
			}
		}
	}
	if(marca >= 2){
		for(int i = 0; i < juego->tope_supertraje; i++){
			if(numero_aleatorio_fil == juego->supertrajes[i].posicion.fila && numero_aleatorio_col == 
				juego->supertrajes[i].posicion.columna){
				posicion_confirmada = true;
			}
		}
	}
	if(marca >= 3){
		for(int i = 0; i < juego->tope_personajes; i++){
			if(numero_aleatorio_fil == juego->personajes[i].posicion.fila && numero_aleatorio_col == 
				juego->personajes[i].posicion.columna){
				posicion_confirmada = true;
			}
		}
	}
	return posicion_confirmada;
}	

// Pre: marca_pinzas determina en que cuadrante las pinzas apareceran.
// Pos: Calcula un numero aleatorio para la fila y la columna dentro del cuadrante donde cada personaje 
// u objeto pertenece.
void asignacion_objetos_personajes(int* numero_aleatorio_fil, int* numero_aleatorio_col, int cuadrante, 
	int marca_pinzas){
	
	int rango = 19 - 10;
	if(cuadrante == PRIMER_CUADRANTE || (marca_pinzas >= 0 && marca_pinzas < 4)){
		*numero_aleatorio_fil = rand()% FILA_10 + FILA_0;
		*numero_aleatorio_col = rand()% COLUMNA_10 + COLUMNA_0;
	}	
	else if(cuadrante == SEGUNDO_CUADRANTE || (marca_pinzas >= 4 && marca_pinzas < 8)){
		*numero_aleatorio_fil = rand()% rango + (FILA_10+1);
		*numero_aleatorio_col = rand()% rango + (COLUMNA_10+1);
	}
	else if(cuadrante == TERCER_CUADRANTE || (marca_pinzas >= 8 && marca_pinzas < 12)){
		*numero_aleatorio_fil = rand()% rango + (FILA_10+1);
		*numero_aleatorio_col = rand()% COLUMNA_10 + COLUMNA_0;
	}
	else if(cuadrante == CUARTO_CUADRANTE || (marca_pinzas >= 12 && marca_pinzas < 16)){
		do{
			*numero_aleatorio_fil = rand()% FILA_10 + FILA_0;
			*numero_aleatorio_col = rand()% rango + (COLUMNA_10+1);
		}while(*numero_aleatorio_fil == FILA_0 && *numero_aleatorio_col == COLUMNA_20);
	}
}

// Pre: -
// Pos: Asigna las posiciones de los laseres de cada robot generado de 4 maneras aleatorias.
void asignacion_lasers(int marca_lasers, int numero_aleatorio_fil, int numero_aleatorio_col, juego_t* juego){
	
	int numero_aleatorio = rand()% 2 + 1;
	int numero_aleatorio_2 = rand()% 2 + 1;
	
	if(numero_aleatorio_2 == 1){
		int j = 0;
		for(int i = 0; i < juego->robots[marca_lasers].tope_lasers; i++){
			if(i < 3){
				coordenada_t coordenada_1;
				coordenada_1.fila = numero_aleatorio_fil;
				coordenada_1.columna = numero_aleatorio_col + i + 1;
				juego->robots[marca_lasers].lasers[i] = coordenada_1;
			}
			else if(i >= 3){
				if(numero_aleatorio == 1){
					coordenada_t coordenada_2;
					coordenada_2.fila = numero_aleatorio_fil + j + 1;
					coordenada_2.columna = numero_aleatorio_col;
					juego->robots[marca_lasers].lasers[i] = coordenada_2;
					j++;
				}
				else if(numero_aleatorio == 2){
					coordenada_t coordenada_2;
					coordenada_2.fila = numero_aleatorio_fil - j - 1;
					coordenada_2.columna = numero_aleatorio_col;
					juego->robots[marca_lasers].lasers[i] = coordenada_2;
					j++;
				}	
			}		
		}
	}	
	if(numero_aleatorio_2 == 2){	
		int j = 0;
		for(int i = 0; i < juego->robots[marca_lasers].tope_lasers; i++){
			if(i < 3){
				coordenada_t coordenada_1;
				coordenada_1.fila = numero_aleatorio_fil;
				coordenada_1.columna = numero_aleatorio_col - i - 1;
				juego->robots[marca_lasers].lasers[i] = coordenada_1;
			}
			else if(i >= 3){
				if(numero_aleatorio == 1){
					coordenada_t coordenada_2;
					coordenada_2.fila = numero_aleatorio_fil + j + 1;
					coordenada_2.columna = numero_aleatorio_col;
					juego->robots[marca_lasers].lasers[i] = coordenada_2;
					j++;
				}
				else if(numero_aleatorio == 2){
					coordenada_t coordenada_2;
					coordenada_2.fila = numero_aleatorio_fil - j - 1;
					coordenada_2.columna = numero_aleatorio_col;
					juego->robots[marca_lasers].lasers[i] = coordenada_2;
					j++;
				}	
			}
		}
	}
}

// Pre: -
// Pos:	Contiene varias subfunciones donde se analiza y designa la posicion de cada personaje.
void posicion_personajes(juego_t* juego){
	int i = 0;
	int j = NO_USADO;
	int marca = 3;
	int numero_aleatorio_fil;
	int numero_aleatorio_col;

	while(i < CANT_PERSONAJES){
		bool posicion_coincide = false;
		do{
			asignacion_objetos_personajes(&numero_aleatorio_fil, &numero_aleatorio_col, i+1, j);
			coordenada_t coordenada;
			coordenada.fila = numero_aleatorio_fil;
			coordenada.columna = numero_aleatorio_col;
			posicion_coincide = chequeo_posiciones(marca, numero_aleatorio_fil, numero_aleatorio_col, juego);
			juego->personajes[i].posicion = coordenada;
		
		}while(posicion_coincide == true);
		i++;
		juego->tope_personajes+=1;
	}	
}

// Pre: -
// Pos: Contiene varias subfunciones donde se analiza y designa la posicion de cada robot.
void posicion_robots(juego_t* juego){

	int i = 0;
	int j = NO_USADO;
	int marca = 0;
	int marca_lasers = 0;
	int numero_aleatorio_fil;
	int numero_aleatorio_col;

	while(i < CANT_ROBOTS){
		bool posicion_coincide = false;
		do{
			asignacion_objetos_personajes(&numero_aleatorio_fil, &numero_aleatorio_col, i+1, j);
			coordenada_t coordenada;
			coordenada.fila = numero_aleatorio_fil;
			coordenada.columna = numero_aleatorio_col;
			posicion_coincide = chequeo_posiciones(marca, numero_aleatorio_fil, numero_aleatorio_col, juego);
			juego->robots[i].posicion = coordenada;
			asignacion_lasers(marca_lasers, numero_aleatorio_fil, numero_aleatorio_col, juego);

		}while(posicion_coincide == true);
		i++;
		marca_lasers++;
		juego->tope_robots+=1;
	}
}	

// Pre: -
// Pos: Analiza y designa la posicion de cada pinza.
void posicion_pinzas(juego_t* juego){

	int i = NO_USADO;
	int marca_pinzas = 0;
	int marca = 1;
	
	while(marca_pinzas < CANT_PINZAS){
		bool posicion_coincide = false;
		int numero_aleatorio_fil;
		int numero_aleatorio_col;
		do{
			asignacion_objetos_personajes(&numero_aleatorio_fil, &numero_aleatorio_col, i, marca_pinzas);
			coordenada_t coordenada;
			coordenada.fila = numero_aleatorio_fil;
			coordenada.columna = numero_aleatorio_col;
			posicion_coincide = chequeo_posiciones(marca, numero_aleatorio_fil, numero_aleatorio_col, juego);
			juego->pinzas[marca_pinzas] = coordenada;
		
		}while(posicion_coincide == true);
		marca_pinzas++;
		juego->tope_pinzas+=1;
	}
}

// Pre: -
// Pos: Contiene varias subfunciones donde se analiza y designa la posicion de cada supertraje.
void posicion_supertrajes(juego_t* juego){

	int i = 0;
	int j = NO_USADO;
	int marca = 2;
	
	while(i < CANT_SUPERTRAJE){
		bool posicion_coincide = false;
		int numero_aleatorio_fil;
		int numero_aleatorio_col;
		do{
			asignacion_objetos_personajes(&numero_aleatorio_fil, &numero_aleatorio_col, i+1, j);
			coordenada_t coordenada;
			coordenada.fila = numero_aleatorio_fil;
			coordenada.columna = numero_aleatorio_col;
			posicion_coincide = chequeo_posiciones(marca, numero_aleatorio_fil, numero_aleatorio_col, juego);
			juego->supertrajes[i].posicion = coordenada;
		
		}while(posicion_coincide == true);
		i++;
		juego->tope_supertraje+=1;
	}
}

// Pre: -
// Pos: Verifica si el personaje recogio el supertraje que le corresponde.
void chequeo_supertraje(juego_t* juego){

	int i = juego->id_personaje_actual-1;

	if(juego->personajes[i].posicion.fila == juego->supertrajes[i].posicion.fila && 
		juego->personajes[i].posicion.columna == juego->supertrajes[i].posicion.columna && 
		juego->personajes[i].cuadrante_inicial == juego->supertrajes[i].cuadrante){
		
		juego->personajes[i].tiene_supertraje = true;
		juego->supertrajes[i].recolectado = true;
		printf("Supertraje recogido!");
	}
}

// Pre: -
// Pos: Verifica si el personaje piso una pinza. Si lo hizo, el jugador sera teletransportado a un area
// cercana del robot que le corresponde al personaje.
void situacion_pisar_pinza(juego_t* juego){
	for(int i = 0; i < juego->tope_pinzas; i++){
		if(juego->personajes[juego->id_personaje_actual-1].posicion.fila == juego->pinzas[i].fila && 
			juego->personajes[juego->id_personaje_actual-1].posicion.columna == juego->pinzas[i].columna){
			if((juego->id_personaje_actual >= 1 && juego->id_personaje_actual <= 3) || 
				(juego->id_personaje_actual == 4 && juego->personajes[3].poder_activado == false)){
				
				int fila_aleatoria;
				int columna_aleatoria;
				bool piso_pinza;
			
				do{
					int fila_robot = juego->robots[juego->personajes[juego->id_personaje_actual-1].cuadrante_inicial-1].posicion.fila;
					int columna_robot = juego->robots[juego->personajes[juego->id_personaje_actual-1].cuadrante_inicial-1].posicion.columna;
					
					do{
						fila_aleatoria = rand()% FILA_20 + FILA_0;
						columna_aleatoria = rand()% COLUMNA_20 + COLUMNA_0;
					}while(fila_aleatoria > (fila_robot+1) || fila_aleatoria < (fila_robot-1) || 
						columna_aleatoria > (columna_robot+1) || columna_aleatoria < (columna_robot-1) || 
						(fila_aleatoria == fila_robot && columna_aleatoria == columna_robot));
					
					juego->personajes[juego->id_personaje_actual-1].posicion.fila = fila_aleatoria;
					juego->personajes[juego->id_personaje_actual-1].posicion.columna = columna_aleatoria;

					piso_pinza = false;
					for(int i = 0; i < juego->tope_pinzas; i++){
						if(fila_aleatoria == juego->pinzas[i].fila && columna_aleatoria == juego->pinzas[i].columna){
							piso_pinza = true;
						}
					}

				}while(juego->personajes[juego->id_personaje_actual-1].posicion.fila < FILA_0 || juego->personajes[juego->id_personaje_actual-1].posicion.fila > FILA_20 || 
					juego->personajes[juego->id_personaje_actual-1].posicion.columna < COLUMNA_0 || juego->personajes[juego->id_personaje_actual-1].posicion.columna > COLUMNA_20 || 
					piso_pinza == true);
			}
			else if(juego->id_personaje_actual == 4 && juego->personajes[3].poder_activado == true){
				juego->personajes[juego->id_personaje_actual-1].posicion.fila = juego->personajes[juego->id_personaje_actual-1].posicion.fila;
				juego->personajes[juego->id_personaje_actual-1].posicion.columna = juego->personajes[juego->id_personaje_actual-1].posicion.columna;
			}
		}
	}
}

// Pre: -
// Pos: Funcion del movimiento generico del jugador mientras no tenga un poder activado.
void movimiento_jugador_sin_poder(juego_t* juego, char movimiento){
	
	if(movimiento == MOVIMIENTO_ARRIBA){
		juego->personajes[juego->id_personaje_actual-1].posicion.fila -= 1;
	}
	else if(movimiento == MOVIMIENTO_DERECHA){
		juego->personajes[juego->id_personaje_actual-1].posicion.columna += 1;
	}
	else if(movimiento == MOVIMIENTO_ABAJO){
		juego->personajes[juego->id_personaje_actual-1].posicion.fila += 1;
	}
	else if(movimiento == MOVIMIENTO_IZQUIERDA){
		juego->personajes[juego->id_personaje_actual-1].posicion.columna -= 1;
	}
}	

// Pre: -
// Pos: Funcion que se ejecuta cuando Elastic Girl tiene el poder activado.
void movimiento_elastic_girl(juego_t* juego, char movimiento){

	if(juego->personajes[juego->id_personaje_actual-1].poder_activado == true){
		if(movimiento == MOVIMIENTO_ARRIBA){
			juego->personajes[juego->id_personaje_actual-1].posicion.fila -= 3;
		}
		else if(movimiento == MOVIMIENTO_DERECHA){
			juego->personajes[juego->id_personaje_actual-1].posicion.columna += 3;
		}	
		else if(movimiento == MOVIMIENTO_ABAJO){
			juego->personajes[juego->id_personaje_actual-1].posicion.fila += 3;
		}
		else if(movimiento == MOVIMIENTO_IZQUIERDA){
			juego->personajes[juego->id_personaje_actual-1].posicion.columna -= 3;
		}
	}
	juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder-=1;
}

// Pre: -
// Pos: Funcion que se ejecuta cuando Dash tiene el poder activado.
void movimiento_dash(juego_t* juego){
	int numero_aleatorio_fil;
	int numero_aleatorio_col;
	do{
		int rango = 19 - 10;
		numero_aleatorio_fil = rand()% FILA_10 + FILA_0;
		numero_aleatorio_col = rand()% rango + (COLUMNA_10+1);
	}while(numero_aleatorio_fil == FILA_0 && numero_aleatorio_col == COLUMNA_20);
		
	juego->personajes[2].posicion.fila = numero_aleatorio_fil;
	juego->personajes[2].posicion.columna = numero_aleatorio_col;
	juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder-=1;
}

// Pre: -
// Pos: Funcion que controla y decide los movimientos de los personajes.
void movimiento_personaje(juego_t* juego, char movimiento){
	
	if(juego->id_personaje_actual == 1 || juego->id_personaje_actual == 3){
			if(juego->personajes[juego->id_personaje_actual-1].poder_activado == true){
				if(juego->id_personaje_actual == 1 && movimiento != ACTIVACION_PODER){
					movimiento_elastic_girl(juego, movimiento);
				}
				else if(juego->id_personaje_actual == 3){
					movimiento_dash(juego);
				}
			}
			else{
				movimiento_jugador_sin_poder(juego, movimiento);
			}	
	}		
	else if(juego->id_personaje_actual == 2 || juego->id_personaje_actual == 4){
		movimiento_jugador_sin_poder(juego, movimiento);
		if(juego->personajes[juego->id_personaje_actual-1].poder_activado == true && movimiento != ACTIVACION_PODER && 
			juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder > 0){
			juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder-=1;
		}
	}
	
	if(movimiento != ACTIVACION_PODER){
		juego->personajes[juego->id_personaje_actual-1].movimientos-=1;
	}	
}

// Pre: -
// Pos: Chequea si el juego cumple con las condiciones para continuar con el siguiente personaje.
void chequeo_siguiente_personaje(juego_t* juego){
	
	if(juego->id_personaje_actual >= 1 && juego->id_personaje_actual <= 3){
		if(juego->personajes[juego->id_personaje_actual-1].posicion.fila == juego->personajes[juego->id_personaje_actual].posicion.fila && 
			juego->personajes[juego->id_personaje_actual-1].posicion.columna == juego->personajes[juego->id_personaje_actual].posicion.columna){
			juego->id_personaje_actual+=1;
		}
	}		
}

// Pre: -
// Pos: Imprime el juego por pantalla.
void imprimir_terreno(juego_t juego){

	char terreno_juego[MAX_FILAS][MAX_COLUMNAS];

	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			terreno_juego[i][j] = '-';
		}
	}
	printf("\n");

	for(int i = 0; i < juego.tope_supertraje; i++){
		if(juego.supertrajes[i].recolectado == false){
			terreno_juego[juego.supertrajes[i].posicion.fila][juego.supertrajes[i].posicion.columna] = OBSTACULO_SUPERTRAJE;
		}
		else if(juego.supertrajes[i].recolectado == true){
			terreno_juego[juego.supertrajes[i].posicion.fila][juego.supertrajes[i].posicion.columna] = '-';
		}
	}
	for(int i = 0; i < juego.tope_pinzas; i++){
		terreno_juego[juego.pinzas[i].fila][juego.pinzas[i].columna] = OBSTACULO_PINZA;
	}
	for(int i = 0; i < juego.tope_personajes; i++){
		if(i == 0){
			if(juego.id_personaje_actual == 1){
				terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = PERSONAJE_ELASTIC_GIRL;
			}
			else{
				terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = '-';
			}
		}
		else if(i == 1){
			terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = PERSONAJE_VIOLETA;
			if(juego.id_personaje_actual > 2){
				terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = '-';
			}
		}
		else if(i == 2){
			terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = PERSONAJE_DASH;
			if(juego.id_personaje_actual > 3){
				terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = '-';
			}
		}
		else if(i == 3){
			terreno_juego[juego.personajes[i].posicion.fila][juego.personajes[i].posicion.columna] = PERSONAJE_MR_INCREIBLE;
		}
	}
	for(int i = 0; i < juego.tope_robots; i++){
		for(int j = 0; j < juego.robots[i].tope_lasers; j++){
			if((juego.robots[i].lasers[j].fila >= FILA_0 && juego.robots[i].lasers[j].fila <= FILA_20) && 
				(juego.robots[i].lasers[j].columna <= COLUMNA_20 && juego.robots[i].lasers[j].columna >= COLUMNA_0)){
				terreno_juego[juego.robots[i].lasers[j].fila][juego.robots[i].lasers[j].columna] = OBSTACULO_LASER;
			}
		}
	}
	for(int i = 0; i < juego.tope_robots; i++){
		terreno_juego[juego.robots[i].posicion.fila][juego.robots[i].posicion.columna] = OBSTACULO_ROBOT;
	}
	terreno_juego[FILA_0][COLUMNA_20] = OBSTACULO_SALIDA;

	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			printf(" %c ", terreno_juego[i][j]);
		}
		printf("\n");
	}
}

// Pre: -
// Pos: Funcion que aplica la rotacion de 45 grados para cada laser de los robots del mapa segun la posicion 
// en la que los laseres estaban previamente.
void aplicar_rotacion(juego_t* juego, int robot, int rotacion){
	
	if(rotacion == 0){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila + i + 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna + i + 1;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna + i + 1;
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = 
			juego->robots[robot].posicion.fila - i - 1;
		}
	}
	else if(rotacion == 1){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila + i + 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = juego->robots[robot].posicion.fila;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna + i + 1;
		}
	}
	else if(rotacion == 2){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila + i + 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna - i - 1;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = 
			juego->robots[robot].posicion.fila + i + 1;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna + i + 1;
		}
	}
	else if(rotacion == 3){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna - i - 1;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = 
			juego->robots[robot].posicion.fila + i + 1;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna;
		}
	}
	else if(rotacion == 4){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila - i - 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna - i - 1;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = 
			juego->robots[robot].posicion.fila + i + 1;
			
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna - i - 1;
		}
	}
	else if(rotacion == 5){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila - i - 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila = 
			juego->robots[robot].posicion.fila;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna - i - 1;
		}
	}
	else if(rotacion == 6){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila - i - 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna + i + 1;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila  = 
			juego->robots[robot].posicion.fila - i - 1;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna - i - 1;
		}
	}
	else if(rotacion == 7){
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[i].fila = juego->robots[robot].posicion.fila - i - 1;
			juego->robots[robot].lasers[i].columna = juego->robots[robot].posicion.columna;
		}
		for(int i = 0; i < juego->longitud_laser; i++){
			juego->robots[robot].lasers[juego->longitud_laser + i].fila  = 
			juego->robots[robot].posicion.fila;
			juego->robots[robot].lasers[juego->longitud_laser + i].columna = 
			juego->robots[robot].posicion.columna + i + 1;
		}
	}
}

// Pre: -
// Pos: Busca en que posicion se encuentran los laseres de cada robot para luego aplicar una rotacion
// de 45 grados.
void rotacion_lasers(juego_t* juego){

	int rotacion = 0;

	for(int i = 0; i < juego->tope_robots; i++){

		bool posicion_01 = false;
		bool posicion_02 = false;
		bool posicion_03 = false;
		bool posicion_04 = false;
		bool posicion_05 = false;
		bool posicion_06 = false;
		bool posicion_08 = false;
		bool posicion_09 = false;
	
		for(int j = 0; j < juego->robots[i].tope_lasers; j++){
			
			if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna + 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila){
				posicion_01 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila - 1){
				posicion_02 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna + 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila + 1){
				posicion_03 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna + 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila - 1){
				posicion_04 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila + 1){
				posicion_05 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna - 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila){
				posicion_06 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna - 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila - 1){
				posicion_08 = true;
			}
			else if(juego->robots[i].lasers[j].columna == juego->robots[i].posicion.columna - 1 && 
			juego->robots[i].lasers[j].fila == juego->robots[i].posicion.fila + 1){
				posicion_09 = true;
			}
		}

		if(posicion_01 == true && posicion_02 == true){
			rotacion = 0;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_03 == true && posicion_04 == true){
			rotacion = 1;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_01 == true && posicion_05 == true){
			rotacion = 2;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_03 == true &&  posicion_09 == true){
			rotacion = 3;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_05 == true && posicion_06 == true){
			rotacion = 4;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_08 == true && posicion_09 == true){
			rotacion = 5;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_06 == true && posicion_02 == true){
			rotacion = 6;
			aplicar_rotacion(juego, i, rotacion);
		}
		else if(posicion_08 == true && posicion_04 == true){
			rotacion = 7;
			aplicar_rotacion(juego, i, rotacion);
		}
	}
}

// Pre: -
// Pos: Evalua si el personaje piso un robot o un laser para determinar si seguir el juego o no.
void chequeo_perdio(int fila, int columna, juego_t* juego){
	bool perdio = false;

	for(int i = 0; i < juego->tope_robots; i++){
		for(int j = 0; j < juego->robots[i].tope_lasers; j++){
			if(fila == juego->robots[i].lasers[j].fila && columna == juego->robots[i].lasers[j].columna){
				perdio = true;
			}
		}
	}
	if(juego->id_personaje_actual == 2){
		if(juego->personajes[1].poder_activado == true && juego->personajes[1].movimientos_con_poder >= 0){
			perdio = false;
		}
	}
	for(int i = 0; i < juego->tope_robots; i++){
		if(fila == juego->robots[i].posicion.fila && columna == juego->robots[i].posicion.columna){
			perdio = true;
		}
	}	
	
	if(perdio == true){
		juego->personajes[juego->id_personaje_actual-1].movimientos = SIN_MOVIMIENTOS;
	}
}

// Pre: -
// Pos: Chequea si el usuario hizo un movimiento para moverse fuera del mapa. En caso de que el usuario
// haya salido del mapa, se vuelve a restablecer la posicion anterior (no se pierden movimientos).
bool personaje_fuera_mapa(juego_t* juego, int movimiento){
	bool se_fue_mapa = false;

	if(juego->personajes[juego->id_personaje_actual-1].posicion.fila < FILA_0 || 
		juego->personajes[juego->id_personaje_actual-1].posicion.fila > FILA_20 || 
		juego->personajes[juego->id_personaje_actual-1].posicion.columna < COLUMNA_0 || 
		juego->personajes[juego->id_personaje_actual-1].posicion.columna > COLUMNA_20){
		se_fue_mapa = true;
		if(movimiento == MOVIMIENTO_ARRIBA){
			if(juego->personajes[0].poder_activado == true){
				juego->personajes[juego->id_personaje_actual-1].posicion.fila += 3;
			}
			else{
				juego->personajes[juego->id_personaje_actual-1].posicion.fila += 1;
			}
		}
		else if(movimiento == MOVIMIENTO_DERECHA){
			if(juego->personajes[0].poder_activado == true){
				juego->personajes[juego->id_personaje_actual-1].posicion.columna -= 3;
			}
			else{
				juego->personajes[juego->id_personaje_actual-1].posicion.columna -= 1;
			}	
		}
		else if(movimiento == MOVIMIENTO_ABAJO){
			if(juego->personajes[0].poder_activado == true){
				juego->personajes[juego->id_personaje_actual-1].posicion.fila -= 3;
			}
			else{
				juego->personajes[juego->id_personaje_actual-1].posicion.fila -= 1;
			}	
		}
		else if(movimiento == MOVIMIENTO_IZQUIERDA){
			if(juego->personajes[0].poder_activado == true){
				juego->personajes[juego->id_personaje_actual-1].posicion.columna += 3;
			}
			else{
				juego->personajes[juego->id_personaje_actual-1].posicion.columna += 1;
			}
		}
	}
	return se_fue_mapa;
}

// Pre: -
// Pos: Verifica si el supertraje esta listo para usar o si ya fue usado.
void control_supertraje(juego_t* juego, char movimiento){

	if(movimiento == ACTIVACION_PODER && juego->personajes[juego->id_personaje_actual-1].tiene_supertraje == true){
		juego->personajes[juego->id_personaje_actual-1].poder_activado = true;
	}
	if(juego->personajes[juego->id_personaje_actual-1].movimientos_con_poder == SIN_MOVIMIENTOS){
		juego->supertrajes[juego->id_personaje_actual-1].usado = true;
		juego->personajes[juego->id_personaje_actual-1].poder_activado = false;
	}
	if(movimiento == ACTIVACION_PODER && juego->supertrajes[juego->id_personaje_actual-1].recolectado == false){
		printf("No has recolectado el Supertraje! (T)");
	}
}

// Pre: -
// Pos: Contiene a todas las subfunciones donde se inicializan las posiciones de los obstaculos/personajes
// del mapa.
void inicializar_terreno(juego_t* juego){

	posicion_robots(juego);
	posicion_pinzas(juego);
	posicion_supertrajes(juego);
	posicion_personajes(juego);
}

// Pre: -
// Pos: Contiene subfunciones donde se inicializan a todos los atributos de los obstaculos/personajes.
void inicializar_juego(juego_t* juego, bool contrasenia_completa){

	robot_t robot_v1, robot_v2, robot_v3, robot_v4;
	inicializar_robots(&robot_v1, &robot_v2, &robot_v3, &robot_v4, contrasenia_completa, juego);
	supertraje_t supertraje_v1, supertraje_v2, supertraje_v3, supertraje_v4;
	inicializar_supertrajes(&supertraje_v1, &supertraje_v2, &supertraje_v3, &supertraje_v4, juego);
	personaje_t personaje_increible, personaje_violeta, personaje_dash, personaje_elastic_girl;
	inicializar_personajes(&personaje_increible, &personaje_violeta, &personaje_dash, &personaje_elastic_girl, juego);
	inicializar_terreno(juego);
}

// Pre: -
// Pos: Funcion donde se encuentran todas las subfunciones para 
// hacer funcionar al juego.
void realizar_jugada(juego_t* juego, char movimiento){
	
	control_supertraje(juego, movimiento);
	movimiento_personaje(juego, movimiento);
	chequeo_supertraje(juego);
	chequeo_siguiente_personaje(juego);
	
	bool salio_del_mapa = false;
	salio_del_mapa = personaje_fuera_mapa(juego, movimiento);
	if(salio_del_mapa == true){
		printf("\nNo se puede acceder a esa parte del mapa!");
	}

	if(movimiento != ACTIVACION_PODER){
		rotacion_lasers(juego);
	}
	chequeo_perdio(juego->personajes[juego->id_personaje_actual-1].posicion.fila, 
		juego->personajes[juego->id_personaje_actual-1].posicion.columna, juego);

	situacion_pisar_pinza(juego);

	chequeo_perdio(juego->personajes[juego->id_personaje_actual-1].posicion.fila, 
		juego->personajes[juego->id_personaje_actual-1].posicion.columna, juego);

}

// Pre: -
// Pos: Evalua el estado del juego y devuelve un entero al main para seguir jugando o evaluar un resultado.
int estado_juego(juego_t juego){
	int valor_juego = VALOR_CONTINUE_JUEGO;

	if(juego.personajes[juego.id_personaje_actual-1].movimientos == SIN_MOVIMIENTOS){
		valor_juego = VALOR_PERDIO_JUEGO;
	}
	if(juego.id_personaje_actual == 4){
		if(juego.personajes[3].posicion.fila == FILA_0 && juego.personajes[3].posicion.columna == COLUMNA_20){
			valor_juego = VALOR_GANADOR_JUEGO;
		}
	}

	return valor_juego;
}	
