#include <panel.h> // panel.h usa a ncurses.h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#if defined(_GNU_LIBRARY_) && !defined(_SEM_SEMUN_UNDEFINED)
#else
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif



#define ESCAPE 27
#define ENTER 10
#define UNO 49
#define DOS 50
#define PATHSEMAFORO "/home/mario/compartido/semaforo"
#define PATHMEMORIA "/home/mario/compartido/memoria"

int dim_y, dim_x; // Variables para tamaño de pantalla
int valor;
int tiempoJuego=0;
struct campo ini;

void inicializar_pantalla(); 
int win_cerrar(char *msg);
void centrar_texto_x(int pos_y, char *msg_textC);
void derecha_texto(int pos_y,char *msg_textD); 
void dibuja_barra(int pos_y);
void barras();
void mensajeInicio();
void seleccionarBando();
void mensajeJugador();
void mensajeSeleccion();
void defensor();
void invasor();
void juego();
void barrasCampo();
void crearMemoria();
void modificarMemoria(int contador);
struct campo obtenerMemoria();
void eliminarMemoria();
//struct campo *puntCompartida=NULL;

struct campo iniCampoBatalla();



int main()
{

	int key_int; 

	inicializar_pantalla(); 
	barras();
	
	mensajeInicio();

	do {
		key_int = getch();
		valor = 1;
		if (key_int == ESCAPE) {
			refresh();
			win_cerrar("¿Quiere salir del juego?");
		}else if (key_int == ENTER)  {
			refresh();
			clear();
			barras();
			seleccionarBando();
			break;
		}
	} while (valor != 0);
	
	
}




void barras(){
	bkgd(COLOR_PAIR(1)); 
	attron(COLOR_PAIR(2)); 
	dibuja_barra(0);
	dibuja_barra(dim_y - 1);
	centrar_texto_x(0,"SPACE INVADERS");
	move (dim_y - 1 ,2); printw ("(ESC) - Salir"); 
	derecha_texto(dim_y - 1, "SISTEMAS OPERATIVOS 1");
	attroff(COLOR_PAIR(2));

}


void mensajeInicio(){
	centrar_texto_x(6,"BIENVENIDO :D");
	//move (7,2); printw ("%i",dim_x); 
	//move (8,2); printw ("%i",dim_y); 
	
	attron(A_BOLD);
	centrar_texto_x(11,"Presione <ENTER> para comenzar");
	attroff(A_BOLD);
	centrar_texto_x(19,"200614833  - MARIO ROBERTO MORALES SITAVI");

}


//SELECCIONAR BANDO

void mensajeJugador(){
	
	centrar_texto_x(3,"Presione el número del jugador que desea utlizar.");
	
	//JUGADOR 1
	int i;
	char c;
	c=95;//caracter ascci horizontal
	for(i=10;i<=50;i++)
	{
		//linea horizontal superior
		
		move(5,i);  //Aqui estamos moviendo el cursor para a linea - columna i.
		printw("%c",c);  //Imprimimos un texto en la posición establecida.
		//linea horizontal inferior
		move(10,i);  //Aqui estamos moviendo el cursor para a linea - columna i.
		printw("%c",c);  //Imprimimos un  texto en la posición establecida.
	}
	
	c=124 ; //caracter ascci vertical
	for(i=6;i<=10;i++)
	{
		//linea vertical izquierda
		move(i,9);
		printw("%c",c);
		//linea vertical derecha
		move(i,51);
		printw("%c",c);
	 }
		
	move(6,13);
	printw("Jugador 1");
	
	move(8,24);
	printw("Defensor");
	
	
	//JUGADOR 2
	
	c=95;//caracter ascci horizontal
	for(i=10;i<=50;i++)
	{
		//linea horizontal superior
		
		move(12,i);  //Aqui estamos moviendo el cursor para a linea - columna i.
		printw("%c",c);  //Imprimimos un texto en la posición establecida.
		//linea horizontal inferior
		move(17,i);  //Aqui estamos moviendo el cursor para a linea - columna i.
		printw("%c",c);  //Imprimimos un  texto en la posición establecida.
	}
	
	c=124 ; //caracter ascci vertical
	for(i=13;i<=17;i++)
	{
		//linea vertical izquierda
		move(i,9);
		printw("%c",c);
		//linea vertical derecha
		move(i,51);
		printw("%c",c);
	 }
		
	move(13,13);
	printw("Jugador 2");
	
	move(15,24);
	printw("Invasor");
	
}


void seleccionarBando(){
	
	int key_int; 
	
//	mensajeJugador();
	
	//SEMAFORO
	
	key_t pass;
	int Id_Semaforo;
	struct sembuf Operacion;
	union semun arg;
	
	pass = ftok (PATHSEMAFORO, 35);
	if (pass == (key_t)-1)
	{
		printf("%s\n", "No puedo conseguir clave de semáforo");
		exit(0);
	}
	Id_Semaforo = semget (pass, 1, 0600 | IPC_CREAT);
	if (Id_Semaforo == -1)
	{
		printf("%s\n", "No se creo el semaforo - Error");
		exit (0);
	}
	Operacion.sem_num = 0;
	Operacion.sem_op =  -1;
	Operacion.sem_flg = 0;
	int valsem2 = semctl(Id_Semaforo, 0, GETVAL);
	
	//RESET SEMAFORO
	/*
	if(valsem2 > 0){
		while(valsem2 > 0){
			semop (Id_Semaforo, &Operacion, 1);
			valsem2 = semctl(Id_Semaforo, 0, GETVAL);
			mvprintw (2, 1 , "%i", valsem2);
		}
	}
	*/
	
	
	mvprintw (2, 1 , "%i", valsem2);
	Operacion.sem_op = 1;
	mensajeJugador();
	
	do {
		
		key_int = getch();
		valor = 1;
		int bb;
		if (key_int == ESCAPE) {
			refresh();
			win_cerrar("¿Quiere salir del juego?");
			
		}else if(key_int == UNO){
			clear();
			refresh();
			barras();
			mvprintw (6, 0 , "Seleccionaste: 1");
			centrar_texto_x(9,"Defensor");
			centrar_texto_x(19,"Espere mientras se conecta su rival ... ");
			refresh();
			semop (Id_Semaforo, &Operacion, 1);
			int valsem = semctl(Id_Semaforo, 0, GETVAL);
			mvprintw (3, 1 , "Cont %i  -  Semaforo %i", bb, valsem);
			while(valsem != 2){
				valsem = semctl(Id_Semaforo, 0, GETVAL);
			}
			//printf("%s\n", "EMPIEZA");
			Operacion.sem_op =  -1;
			semop (Id_Semaforo, &Operacion, 1);
			
			//usleep(3000000);
			
			juego(1);
			valor =0;
			
		}else if (key_int == DOS)  {
			clear();
			refresh();
			barras();
			mvprintw (6, 0 , "Seleccionaste: 2");
			centrar_texto_x(9,"Invasor");
			centrar_texto_x(19,"Espere mientras se conecta su rival ... ");
			refresh();
			
			semop (Id_Semaforo, &Operacion, 1);
			int valsem = semctl(Id_Semaforo, 0, GETVAL);
			mensajeSeleccion("2","Invasor");
			mvprintw (3, 1 , "Cont %i  -  Semaforo %i", bb, valsem);
			while(valsem != 2){
				valsem = semctl(Id_Semaforo, 0, GETVAL);
			}
			//printf("%s\n", "EMPIEZA");
			Operacion.sem_op =  -1;
			semop (Id_Semaforo, &Operacion, 1);
			
			//usleep(3000000);
			
			juego(2);
			valor =0;
		}
		
		
		
	} while (valor != 0);
	
}

void mensajeSeleccion(char *msg, char *tipo){
	refresh();
	clear();
	barras();
	mvprintw (6, 0 , "Seleccionaste: %s",msg);
	centrar_texto_x(9,tipo);
	centrar_texto_x(19,"Espere mientras se conecta su rival ... ");
	
}

struct cuadro
{
	int pos_x;
	int pos_y;
	//0-Vacio 1-Defensor  2-Invasor 3-Bala
	int tipo;
	int posicion;
	char contenido;
	//\-.-/ -5
	//(/-1-\) -7
	
};

struct campo
{
	int defensorQE;
	int invasorQE;
	int turno;
	int numero;
	int tiempo;
	int vida_Invasor;
	int vida_Defensor;
	int puntos;
	struct cuadro tablero[57][25];
};





struct campo iniCampoBatalla(){
	
	struct campo n;
	
	n.defensorQE=0;
	n.invasorQE=0;
	n.turno=0;
	n.numero=300;
	n.tiempo=0;
	n.vida_Invasor=5;
	n.vida_Defensor=5;
	n.puntos = 0;
	
	struct cuadro c;
	
	c.pos_x=0;
	c.pos_y=0;
	/*
	0-Vacio 
	1-Defensor 
	2-Invasor 
	3-Invasor Fuerte  
	4 - Invasor Comun  
	5 - Bala Defensa  
	6- Bala Alien 
	*/
	c.tipo=0;
	c.posicion=0;
	c.contenido=' ';
	
	int x,y;
	
	
	for (x = 0; x< 57; x++){
 		for (y = 0; y< 25; y++){
			n.tablero[x][y] = c;			
		}
	}
	
	//Invasor
	for (x = 25; x<34; x++){
		n.tablero[x][1].tipo=1;
		n.tablero[x][1].contenido='0';
	}
	
	//Defensor
	for (x = 25; x<34; x++){
		n.tablero[x][22].tipo=1;
		n.tablero[x][22].contenido='^';
	}
	
	
	
	y=5;
	for (x = 11; x<45; x++){
		
		
		if(x==11||x==18||x==33||x==40){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='\\';
		}
		if(x==12||x==14||x==19||x==21||x==34||x==36||x==41||x==43){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='-';
		}
		
		
		if(x==13||x==20||x==35||x==42){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='.';
		}
		
		if(x==15||x==22||x==37||x==44){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='/';
		}
		
		if(x==24){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='(';
		}
		if(x==25){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='/';
		}
		if(x==26){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==27){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='1';
		}
		if(x==28){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==29){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='\\';
		}
		if(x==30){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido=')';
		}
		
		
	}
	
	
	
	
	y=8;
	for (x = 11; x<45; x++){
		
		
		if(x==11||x==26||x==33||x==40){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='\\';
		}
		if(x==12||x==14||x==27||x==29||x==34||x==36||x==41||x==43){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='-';
		}
		
		
		if(x==13||x==28||x==35||x==42){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='.';
		}
		
		if(x==15||x==30||x==37||x==44){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='/';
		}
		
		if(x==18){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='(';
		}
		if(x==19){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='/';
		}
		if(x==20){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==21){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='2';
		}
		if(x==22){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==23){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='\\';
		}
		if(x==24){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido=')';
		}
		
		
	}
	
	y=11;
	for (x = 11; x<45; x++){
		
		
		if(x==11||x==18||x==26||x==40){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='\\';
		}
		if(x==12||x==14||x==19||x==21||x==27||x==29||x==41||x==43){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='-';
		}
		
		
		if(x==13||x==20||x==28||x==42){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='.';
		}
		
		if(x==15||x==22||x==30||x==44){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='/';
		}
		
		if(x==32){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='(';
		}
		if(x==33){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='/';
		}
		if(x==34){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==35){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='3';
		}
		if(x==36){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==37){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='\\';
		}
		if(x==38){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido=')';
		}
		
		
	}
	
	y=14;
	for (x = 11; x<45; x++){
		
		
		if(x==26||x==18||x==33||x==40){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='\\';
		}
		if(x==27||x==29||x==19||x==21||x==34||x==36||x==41||x==43){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='-';
		}
		
		
		if(x==28||x==20||x==35||x==42){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='.';
		}
		
		if(x==30||x==22||x==37||x==44){
			n.tablero[x][y].tipo=4;
			n.tablero[x][y].contenido='/';
		}
		
		if(x==11){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='(';
		}
		if(x==12){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='/';
		}
		if(x==13){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==14){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='4';
		}
		if(x==15){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='-';
		}
		if(x==16){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido='\\';
		}
		if(x==17){
			n.tablero[x][y].tipo=3;
			n.tablero[x][y].contenido=')';
		}
		
		
	}
	
	
	
	return n;
}


void juego(int jugador){

	int key_int; 
	int x = 0, y = 1, max_x= 0, next_x = 0, direction = 1, max_y =0;
	//int comp;
	int tiempolocal=0;
	
	
	
	//CREAR MEMORIA
		key_t clavecompartida;
		int mem = 0;

		struct campo *mcompartida = NULL;

		clavecompartida = ftok (PATHMEMORIA,33);

		mem = shmget (clavecompartida,sizeof(struct campo *),0777 | IPC_CREAT);
		mcompartida = (struct campo*) shmat (mem,NULL,0);
		
		mvprintw (6, 0 , "Creando el segmento %d de memoria compartida\n\n",mem);
		obtenerMemoria(); 
	
	
	if (jugador == 1)  { //Defensor
		
		/*
		
		//CREAR MEMORIA
		key_t clavecompartida;
		int mem = 0;

		struct campo *mcompartida = NULL;

		clavecompartida = ftok (PATHMEMORIA,33);

		mem = shmget (clavecompartida,sizeof(struct campo *),0777 | IPC_CREAT);
		mcompartida = (struct campo*) shmat (mem,NULL,0);
		
		mvprintw (6, 0 , "Creando el segmento %d de memoria compartida\n\n",mem);
		
		*/
		
		while(1) {	 
			
			cbreak();
  			nodelay(stdscr,1);
   			keypad(stdscr,1);
   			srand(time(NULL));
			key_int = getch();
			struct campo *puntCompartida = (struct campo*) shmat (mem,NULL,0);
			
			puntCompartida->defensorQE = 1;
			
			while(puntCompartida->invasorQE ==1){
				if(puntCompartida->turno == 2){
					puntCompartida->defensorQE=0;
					while(puntCompartida->turno==2){
					}
					puntCompartida->defensorQE=1;
				}
			
			}
			
			//REGION CRITICA
			puntCompartida = (struct campo*) shmat (mem,NULL,0);
			
			getmaxyx(stdscr, max_y, max_x);
			clear();
			barrasCampo();
						
			mvprintw(y, x, "%d", puntCompartida->numero);
			
			refresh();
			usleep(300000);
			next_x = x + direction;
			if (next_x >= max_x || next_x < 0) {
				direction*= -1;
			} else {
				x+= direction;
			}
			
			if(key_int == UNO){
				break;
			}
			
			puntCompartida->numero=puntCompartida->numero+20;
			
			//TERMINA REGION CRITICA
			
			puntCompartida->turno = 2;
			puntCompartida->defensorQE = 0;
			
			//Lo que no es critico... que creo que no hay nada...
				
		}
		
		//ELIMINAR MEMORIA
		shmctl (mem, IPC_RMID, (struct shmid_ds *)NULL);
		unlink (PATHMEMORIA);
		
		
		
	}else if (jugador == 2)  { //Invasor
		
		int conteo=0;
		
		while(1) {
			
			
			
			cbreak();
  			nodelay(stdscr,1);
   			keypad(stdscr,1);
   			srand(time(NULL));
			key_int = getch();
			
			struct campo *puntCompartida = (struct campo*) shmat (mem,NULL,0);
			
			//IMPLEMENTANOD DECKER
			
			puntCompartida->invasorQE = 1;
			while(puntCompartida->defensorQE ==1){
				if(puntCompartida->turno == 1){
					puntCompartida->invasorQE=0;
					while(puntCompartida->turno==1){
					}
					puntCompartida->invasorQE=1;
				}
			
			}
			
			
			
			//REGION CRITICA
			puntCompartida = (struct campo*) shmat (mem,NULL,0);
			
			getmaxyx(stdscr, max_y, max_x);
			clear();
			barrasCampo();
						
			mvprintw(19, x, "%d", puntCompartida->numero);
			
			refresh();
			usleep(300000);
			next_x = x + direction;
			if (next_x >= max_x || next_x < 0) {
				direction*= -1;
			} else {
				x+= direction;
			}
			
			if(key_int == UNO){
				break;
			}
			
			
			
			conteo+=1;
			modificarMemoria(conteo);
			
			//TERMINA REGION CRITICA
			
			puntCompartida->turno = 1;
			puntCompartida->invasorQE = 0;
			
			//Lo que no es critico... que creo que no hay nada...
			
		}		
	}
	
}








/*

void juego(int jugador){

	int key_int; 
	int x = 0, y = 1, max_x= 0, next_x = 0, direction = 1, max_y =0;
	
	int tiempolocal=0;
	
	
	//CREAR MEMORIA
		key_t clavecompartida;
		int mem = 0;

		struct campo *mcompartida = NULL;

		clavecompartida = ftok (PATHMEMORIA,33);

		mem = shmget (clavecompartida,sizeof(struct campo *),0777 | IPC_CREAT);
		mcompartida = (struct campo*) shmat (mem,NULL,0);
		
		mvprintw (6, 0 , "Creando el segmento %d de memoria compartida\n\n",mem);
		obtenerMemoria(); 
	
	
	if (jugador == 1)  { //Defensor
		
		
		
		
		while(1) {	 
			
			
			cbreak();
  			nodelay(stdscr,1);
   			keypad(stdscr,1);
   			srand(time(NULL));
			key_int = getch();
			
			
			// AQUI HACE USO DE LA REGION CRITICA
			
			//IMPLEMENTANOD DECKER
			
			puntCompartida->defensorQE = 1;
			
			while(puntCompartida->invasorQE ==1){
				if(puntCompartida->turno == 2){
					puntCompartida->defensorQE=0;
					while(puntCompartida->turno==2){
					}
					puntCompartida->defensorQE=1;
				}
			
			
			
			
			//REGION CRITICA
			
			getmaxyx(stdscr, max_y, max_x);
			clear();
			barrasCampo();
			
			struct campo abc = obtenerMemoria();
			//EL JUEGO
			
			
			//Leer el arreglo y mostrar el contenido de cada posicion
			
			
			struct cuadro temp;
			
			
			for (x = 0; x< 57; x++){
				for (y = 0; y< 25; y++){
					temp = puntCompartida->tablero[x][y];
					mvprintw(y, x, "%c", temp.contenido);
				}
				
			}
			
			//mvprintw(0, 0, "%s", puntCompartida->tablero[2][25].contenido);
			
			mvprintw(y, x, "%d", abc.numero);
			
			next_x = x + direction;
			if (next_x >= 57 || next_x < 0) {
				direction*= -1;
			} else {
				x+= direction;
			}
			
			
			
			//CONTROL DEL CRONOMETRO - SOLO DEFENSOR
			if(tiempolocal!=1){
				tiempolocal=tiempolocal+1;
			}else{
				tiempolocal=0;
				tiempoJuego=tiempoJuego+1;
				puntCompartida->tiempo=tiempoJuego;
			}
			//FIN CRONOMETRO
			
			//Imprimir tiempo
			attron(A_BOLD);		
			mvprintw(11, 63 , "TIEMPO");
			mvprintw(14, 69 , "%d", puntCompartida->tiempo);
			attroff(A_BOLD);
			
			
			//Imprimir vida 
			attron(A_BOLD);		
			mvprintw(2, 63 , "DEFENSOR");
			mvprintw(4, 63 , "Vida: %d", puntCompartida->vida_Defensor);
			attroff(A_BOLD);
			
			attron(A_BOLD);		
			mvprintw(19, 63 , "INVASOR");
			mvprintw(21, 63 , "Vida: %d", puntCompartida->vida_Invasor);
			mvprintw(22, 63 , "Puntos: %d", puntCompartida->puntos);
			attroff(A_BOLD);
			
			
			refresh();
			usleep(250000);
						
			
			//Es cuando se tenga que mover el defensor o disparar
			if(key_int == UNO){
				
				mvprintw(0, 0, "%d", key_int);
				usleep(1000000);
				       
				//break;
			}
			
			//TERMINA REGION CRITICA
			
			
			puntCompartida->turno = 2;
			puntCompartida->defensorQE = 0;
			
			//Lo que no es critico... que creo que no hay nada...
			
			
		}
		
		//ELIMINAR MEMORIA
		shmctl (mem, IPC_RMID, (struct shmid_ds *)NULL);
		unlink (PATHMEMORIA);
		
		
		
	}else if (jugador == 2)  { //Invasor
		obtenerMemoria(); 
		//modificarMemoria();
		int cont=0;
		while(1) {
			
			cbreak();
  			nodelay(stdscr,1);
   			keypad(stdscr,1);
   			srand(time(NULL));
			key_int = getch();
			
			
			// AQUI HACE USO DE LA REGION CRITICA
			
			//IMPLEMENTANOD DECKER
			
			puntCompartida->invasorQE = 1;
			while(puntCompartida->defensorQE ==1){
				if(puntCompartida->turno == 1){
					puntCompartida->invasorQE=0;
					while(puntCompartida->turno==1){
					}
					puntCompartida->invasorQE=1;
				}
			
			}
			
			
			//REGION CRITICA
			cont= cont+1;
			modificarMemoria(cont);
			
			getmaxyx(stdscr, max_y, max_x);
			clear();
			barrasCampo();
			
			//EL JUEGO
			
			
			//Leer el arreglo y mostrar el contenido de cada posicion
			
			
			struct cuadro temp;
			
			for (x = 0; x< 57; x++){
				for (y = 0; y< 25; y++){
					temp = puntCompartida->tablero[x][y];
					mvprintw(y, x, "%c", temp.contenido);
				}
				
			}
			
			//mvprintw(0, 0, "%s", puntCompartida->tablero[2][25].contenido);
			
			mvprintw(y, x, "%d", puntCompartida->numero);
			
			next_x = x + direction;
			if (next_x >= 57 || next_x < 0) {
				direction*= -1;
			} else {
				x+= direction;
			}
			
			
			
			//CONTROL DEL CRONOMETRO - SOLO DEFENSOR
			if(tiempolocal!=1){
				tiempolocal=tiempolocal+1;
			}else{
				tiempolocal=0;
				tiempoJuego=tiempoJuego+1;
				puntCompartida->tiempo=tiempoJuego;
			}
			//FIN CRONOMETRO
			
			
			
			//Imprimir tiempo
			attron(A_BOLD);		
			mvprintw(11, 63 , "TIEMPO");
			mvprintw(14, 69 , "%d", puntCompartida->tiempo);
			attroff(A_BOLD);
			
			puntCompartida->vida_Defensor = puntCompartida->vida_Defensor +1;
			//Imprimir vida 
			attron(A_BOLD);		
			mvprintw(2, 63 , "DEFENSOR");
			mvprintw(4, 63 , "Vida: %d", puntCompartida->vida_Defensor);
			attroff(A_BOLD);
			
			puntCompartida->vida_Defensor = puntCompartida->vida_Defensor +1;
			
			attron(A_BOLD);		
			mvprintw(19, 63 , "INVASOR");
			mvprintw(21, 63 , "Vida: %d", puntCompartida->vida_Defensor);
			mvprintw(22, 63 , "Puntos: %d", puntCompartida->puntos);
			attroff(A_BOLD);
			
			mcompartida = (struct campo*) shmat (mem,NULL,0);
			mcompartida = puntCompartida;
			
			refresh();
			usleep(250000);
			
			
			
			//Es cuando se tenga que mover el defensor o disparar
			if(key_int == UNO){
				
				mvprintw(0, 0, "%d", key_int);
				usleep(1000000);
				//break;
			}
			
			
			//TERMINA REGION CRITICA
			
			puntCompartida->turno = 1;
			puntCompartida->invasorQE = 0;
			
			//Lo que no es critico... que creo que no hay nada...
			
		}		
	}
	
}

*/

//MEMORIA COMPARTIDA


void modificarMemoria(int contador){

	key_t clavecompartida;
	int mem = 0;
	
	struct campo *mcompartida = NULL;

	clavecompartida = ftok (PATHMEMORIA,33);
	
	mem =  shmget (clavecompartida,sizeof(struct campo *),0777);
	mcompartida = (struct campo*) shmat (mem,NULL,0);
	
	clear();
	mvprintw (8, 0 , "Trabjando el segmento %d de memoria compartida",mem);
		
	mvprintw (9, 0 , "%d",mcompartida->numero);
	mcompartida->numero = contador;
	refresh();

}

struct campo obtenerMemoria(){
	
	//Inicializar el struct
	
	ini = iniCampoBatalla();
	
	key_t clavecompartida;
	int mem = 0;
	struct campo *mcompartida = NULL;

	clavecompartida = ftok (PATHMEMORIA,33);
	
	mem =  shmget (clavecompartida,sizeof(struct campo *),0777);
	mcompartida = (struct campo*)  shmat (mem,NULL,0);
	
	//printf ("Trabajando con el segmento: %d\n",mem);	
	//printf ("El valor compartido es: %d\n", mcompartida->numero);
	mvprintw (15,0 , "El valor compartido es: %d\n",mcompartida->numero);
	struct campo n = *mcompartida;
	
	//Inicializar struct principal.
	mcompartida = &ini;
	
	//puntCompartida = mcompartida;
	return n;
}


//PINTAR CAMPO DE BATALLA


void barrasCampo(){
	
	int a;
	
	bkgd(COLOR_PAIR(1)); 
	attron(COLOR_PAIR(2)); 
	
	for (a=0; a<dim_y; a++)
	{
		move (a,58); printw(" ");
	}	
	
	for (a=58; a<dim_x; a++)
	{
		move (7,a); printw(" ");
	}
	
	for (a=58; a<dim_x; a++)
	{
		move (17,a); printw(" ");
	}
	
	//dibuja_barra(dim_y - 1);
	
	attroff(COLOR_PAIR(2));
}


void defensor(){
	
	//Movimiento del defensor, desde el juego
}




void inicializar_pantalla()
{

	if ( initscr () != NULL )
	{
		if (has_colors())
		{
			getmaxyx(stdscr, dim_y, dim_x);
			start_color();
			init_pair(1,COLOR_WHITE,COLOR_BLUE);
			init_pair(2,COLOR_RED,COLOR_WHITE);
			init_pair(3,COLOR_WHITE,COLOR_RED);
			curs_set(0);
			noecho();
			keypad(stdscr, TRUE);
		}
	}

}


int win_cerrar(char *msg)
{

	int t_win_x, t_win_y; // Variables tamaño ventana
	int p_win_x, p_win_y; // Variables posicion ventana
	int win_a, win_b, win_c; // Variables para operaciones dentro de la ventana
	int tam_scr_y, tam_scr_x; // Variables tamaño de la pantalla
	int pos_S_x, pos_N_x, pos_SN_y;
	int Si_No, Tecla;

	WINDOW *v2; // Declaramos el puntero donde se guardara la ventana
	PANEL *panel_v2;

	// Calculamos tamaños y posiciones de la ventana y de los comentarios

	getmaxyx (stdscr, tam_scr_y, tam_scr_x);
	t_win_x = strlen(msg + 4);
	t_win_y = 8;
	p_win_x = ( tam_scr_x / 2 ) - ( t_win_x / 2 );
	p_win_y = ( tam_scr_y / 2 ) - ( t_win_y / 2 );
	pos_SN_y = t_win_y - 3;
	pos_S_x = (t_win_x / 4) - 2;
	pos_N_x = t_win_x - pos_S_x;

	// Dibujamos la ventana

	v2 = newwin ( t_win_y, t_win_x + 8, p_win_y, p_win_x ); 
	panel_v2 = new_panel(v2);

	wbkgd(v2, COLOR_PAIR(2));
	box(v2,0,0);	
	mvwprintw (v2,2,2,"%s",msg);
	mvwprintw (v2, pos_SN_y, pos_S_x,"<SI>"); 
	wcolor_set(v2,3,0);
	mvwprintw (v2, pos_SN_y, pos_N_x,"<NO>"); 
	Si_No = 0;
	wrefresh(v2);
	do 
	{

		Tecla = getch();
		if ( Tecla == KEY_LEFT )
		{
			wcolor_set(v2,2,0);
			mvwprintw (v2, pos_SN_y, pos_N_x,"<NO>");
			wcolor_set(v2,3,0);
			mvwprintw (v2, pos_SN_y, pos_S_x,"<SI>");
			Si_No = 1;
			wrefresh(v2);
		}
		if ( Tecla == KEY_RIGHT )
		{
			wcolor_set(v2,3,0);
			mvwprintw (v2, pos_SN_y, pos_N_x,"<NO>"); 
			wcolor_set(v2,2,0);
			mvwprintw (v2, pos_SN_y, pos_S_x,"<SI>"); 
			Si_No = 0;
			wrefresh(v2);
		}

	} while ( Tecla != ENTER );

	switch (Si_No)
	{

	case 0:

	hide_panel(panel_v2);
	delwin(v2);
	valor = 1;
	return(valor);

	case 1:

	endwin();
	valor = 0;
	return(valor);
	break;
	}

}

void centrar_texto_x(int pos_y, char *msg_textC) // Funcion que nos sirve para centrar texto en X Horizontal
{
	int tam_text, pos_text; // Variables donde almacenaremos el tamaño y posicion en X del texto

	tam_text = strlen(msg_textC); // Calculamos el tamaño del comentario
	pos_text = ( dim_x / 2 ) - ( tam_text / 2 ); // Calculamos la posicion del texto
	mvprintw ( pos_y, pos_text, "%s", msg_textC ); // Imprimimos el mensage en pantalla

}

void derecha_texto(int pos_y, char *msg_textD)
{

	int tam_text, pos_text;
	tam_text = strlen(msg_textD);
	pos_text = (dim_x -1) - tam_text;
	mvprintw (pos_y, pos_text, "%s",msg_textD);
}

void dibuja_barra(int pos_y) 
{
	int a;
	
	for (a=0; a<dim_x; a++)
	{
		move (pos_y,a); printw(" ");
	}
}


