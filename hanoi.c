/*
 * hanoi.c
 * 
 * Copyright 2014 Pablo Hazan <pablo@pablo-Satellite-L305>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include "Pila.h"

typedef struct
{
	TPila *torre1, *torre2, *torre3;
} Hanoi;

void crearJuego (Hanoi *juego)
{
	juego->torre1= malloc(sizeof(TPila));
	P_Crear(juego->torre1, sizeof(int));
	juego->torre2= malloc(sizeof(TPila));
	P_Crear(juego->torre2, sizeof(int));
	juego->torre3= malloc(sizeof(TPila));
	P_Crear(juego->torre3, sizeof(int));
}

void ponerFichas (int n, TPila *torre)
{
	P_Poner(torre, &n);
	if (n > 1)
		ponerFichas(n-1, torre);
}

void moverFicha(TPila *desde, TPila *hasta)
{
	int ficha;
	P_Sacar(desde, &ficha);
	P_Poner(hasta, &ficha);
}

TPila * primerMovimiento (Hanoi *juego, int n, int *movimientos)
{
	(*movimientos)++;
	printf("\nmovimiento %d", *movimientos);
	if (( n % 2 ) == 0 )
	{
		moverFicha(juego->torre1, juego->torre2);
		return juego->torre2;
	}
	else
	{
		moverFicha(juego->torre1, juego->torre3);
		return juego->torre3;
	}
}

void mostrarTorre (TPila *torre)
{
	TPila aux;
	int ficha;
	P_Crear(&aux, sizeof(int));
	while (!P_Vacia(*torre))
	{
		P_Sacar(torre, &ficha);
		printf("-%d-\n",ficha);
		P_Poner(&aux,&ficha);
	}
	while (!P_Vacia(aux))
	{
		P_Sacar(&aux, &ficha);
		P_Poner(torre,&ficha);
	}
}

void mostrar (Hanoi *juego)
{
	
	printf("\nTorre 1:\n");
	mostrarTorre(juego->torre1);
	printf("Torre 2:\n");
	mostrarTorre(juego->torre2);
	printf("Torre 3:\n");
	mostrarTorre(juego->torre3);
	printf ("---------------------------");
}

void movimientoPosible (Hanoi *juego, TPila **recienUsada, int *movimientos)
{
	TPila *torre1,*torre2, *aux;
	int ficha1, ficha2, ficha;
	
	if (juego->torre1 == *recienUsada)
	{
		torre1 = juego->torre2;
		torre2 = juego->torre3;
	}
	else
	{
		torre1 = juego->torre1;
		if (juego->torre2 == *recienUsada)
			torre2 = juego->torre3;
		else
			torre2 = juego->torre2;
	}
	
	if (!P_Vacia(*torre1) && (!P_Vacia(*torre2)))
	{
		P_Sacar(torre1, &ficha1);
		P_Sacar(torre2, &ficha2);
		if (ficha1 > ficha2)
		{
			P_Poner(torre1, &ficha1);
			P_Poner(torre1, &ficha2);
			ficha= ficha2;
		}
		else
		{
			P_Poner(torre2, &ficha2);
			P_Poner(torre2, &ficha1);
			ficha=ficha1;
			
			aux = torre1;
			torre1 = torre2;
			torre2= aux;
		}
	}
	else
	{
		if ( P_Vacia(*torre1) )
		{
			P_Sacar(torre2, &ficha);
			P_Poner(torre1, &ficha);
		}
		else
		{
			P_Sacar(torre1, &ficha);
			P_Poner(torre2, &ficha);
			
			aux = torre1;
			torre1 = torre2;
			torre2= aux;
		}
	}
	
	(*movimientos)++;
	printf ("\nmovimiento %d", *movimientos);
	mostrar(juego);
	
	if ((ficha % 2) == 0)
	{
		moverFicha(*recienUsada, torre1);
		*recienUsada = torre1;
	}
	else
	{
		moverFicha(*recienUsada, torre2);
		*recienUsada = torre2;
	}
	(*movimientos)++;
	printf("\nmovimiento %d", *movimientos);
	mostrar(juego);
	
}

void jugar(Hanoi *juego, TPila **movimientoAnterior, int *movimientos)
{
	int ultimaFicha;
	if ( (!P_Vacia(*(juego->torre1))) || (!P_Vacia(*(juego->torre2))) )
	{
		movimientoPosible (juego, movimientoAnterior, movimientos);
		jugar(juego, movimientoAnterior, movimientos);
	}
}

void terminar(Hanoi *juego)
{
	P_Vaciar(juego->torre1);
	free(juego->torre1);
	P_Vaciar(juego->torre2);
	free(juego->torre2);
	P_Vaciar(juego->torre3);
	free(juego->torre3);
}

int main(int argc, char **argv)
{
	int n, movimientos;
	Hanoi juego;
	TPila *movimientoAnterior;
	
	printf ("Con cuantos discos quiere que el juego sea resuelto: ");
	scanf("%d" , &n);
	movimientos = 0;
	
	crearJuego (&juego);
	ponerFichas (n, juego.torre1);
	mostrar(&juego);
	movimientoAnterior = primerMovimiento (&juego, n, &movimientos);
	mostrar(&juego);
	jugar(&juego, &movimientoAnterior, &movimientos);
	printf("\nTotal de movimientos: %d\n",movimientos);
	terminar(&juego);
	/**/
	return 0;
}
