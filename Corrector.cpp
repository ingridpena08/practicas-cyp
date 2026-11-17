/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"



char aMinuscula(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		c = c + 32; // 32 es la distancia entre cualquier letra mayúscula y su minúscula en ASCII
	}
	return c;
}

int esSeparador(unsigned char c)
{
	int separador = false;
	if (c == ' ' || c == '\t' /*tabulador*/ || c == '\n' /*salto de linea*/ || c == '\r' /*retorno de carro*/ ||
		c == ',' || c == '.' || c == ';' || c == '(' || c == ')')
	{
		separador = true;
	}
	return separador;
}

void OrderDic(char szPalabras[][TAMTOKEN], int iEstadisticas[], int iNumElementos)
{
	//burbuja, alfabeticamente y por sus estadisticas
	char tempPalabra[TAMTOKEN];
	int tempEst, change;
	for (int i = 0; i < iNumElementos - 1; i++)
	{
		change = 0;
		for (int j = 0; j < iNumElementos - 1 - i; j++)
		{
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				tempEst = iEstadisticas[j];
				iEstadisticas[j] = iEstadisticas[j + 1];
				iEstadisticas[j + 1] = tempEst;

				strcpy_s(tempPalabra, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, tempPalabra);
				change = 1;
			}
		}
		if (!change) {
			break;
		}
	}
}

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* fp = NULL;
	iNumElementos = 0;

	if (fopen_s(&fp, szNombre, "r") != 0 || fp == NULL)
	{
		return; /*termina la ejecucion  de la funcion Diccionario si falla la apertura del archivo*/
	}

	char Palabrastemp[TAMTOKEN];
	int posicion = 0;
	int encontrado;
	int c; /*guarda el caracter leido, tiene que ser int para almacenar EOF*/

	do
	{
		c = fgetc(fp); // Lee el carácter al inicio de cada vuelta

		// Si es un separador O si ya llegamos al final del archivo (EOF), cerramos y guardamos la palabra
		if (c == EOF || esSeparador((unsigned char)c))
		{
			if (posicion > 0)
			{
				Palabrastemp[posicion] = '\0';
				posicion = 0;
				encontrado = 0;

				// Buscar si la palabra ya existe en el diccionario una por una
				for (int i = 0; i < iNumElementos; i++)
				{
					if (strcmp(szPalabras[i], Palabrastemp) == 0)
					{
						iEstadisticas[i]++;
						encontrado = 1;
						break;
					}
				}

				// Si no existe, agregarla
				if ((encontrado != 1) && (iNumElementos < NUMPALABRAS))
				{
					strcpy_s(szPalabras[iNumElementos], TAMTOKEN, Palabrastemp);
					iEstadisticas[iNumElementos] = 1;
					iNumElementos++;
				}
			}
		}
		else
		{
			if (posicion < (TAMTOKEN - 1))
			{
				Palabrastemp[posicion] = aMinuscula((unsigned char)c);
				posicion++;
			}
		}
	} while (c != EOF);

	fclose(fp);
	OrderDic(szPalabras, iEstadisticas, iNumElementos);
}







void OrderWord(char szListaFinal[][TAMTOKEN], int iNumLista, int iPeso[])
{
	// Ordenar szListaFinal por peso de MAYOR a MENOR, y en caso de ser la misma ya vienen ordenadas por alfabetico
	// burbuja
	char tempPalabra[TAMTOKEN];
	int tempPeso, i, j;
	for (i = 0; i < iNumLista - 1; i++)
	{
		for (j = 0; j < iNumLista - 1 - i; j++)
		{
			if (iPeso[j] < iPeso[j + 1])
			{
				tempPeso = iPeso[j];
				iPeso[j] = iPeso[j + 1];
				iPeso[j + 1] = tempPeso;

				strcpy_s(tempPalabra, TAMTOKEN, szListaFinal[j]);
				strcpy_s(szListaFinal[j], TAMTOKEN, szListaFinal[j + 1]);
				strcpy_s(szListaFinal[j + 1], TAMTOKEN, tempPalabra);
			}
		}
	}
}
/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista)							//Numero de elementos en la szListaFinal
{
	int listed;  // evita palabras duplicadas
	int izq; // lim izq
	int der; //lim derecho
	int posDic; // posicion donde se encontro
	int med; //mitad del diccionario
	int cmp;

	iNumLista = 0;

	for (int i = 0; i < iNumSugeridas; i++) //recorremos las palabras creadas, para ver si esa palabra se repirtio
	{
		listed = 0; // 0 - no repetida, 1 - listada
		for (int k = 0; k < iNumLista; k++)
		{
			if (strcmp(szListaFinal[k], szPalabrasSugeridas[i]) == 0)
			{
				listed = 1;
				break;
			}
		}
		if (listed) {
			continue; // pasa a la siguiente palabra si esta repetida
		}

		// Búsqueda binaria
		izq = 0;
		der = iNumElementos - 1;
		posDic = -1;

		while (izq <= der)
		{
			med = izq + (der - izq) / 2;
			cmp = strcmp(szPalabras[med], szPalabrasSugeridas[i]);
			if (cmp == 0)
			{
				posDic = med; // ya se encontro
				break;
			}
			else if (cmp < 0)
			{
				izq = med + 1; // Descarta la mitad izquierda y busca en la mitad derecha
			}
			else
			{
				der = med - 1; // Descarta la mitad derecha y busca en la mitad izquierda
			}
		}

		// Si existe en el diccionario, agregar a szListaFinal
		if (posDic != -1)
		{
			strcpy_s(szListaFinal[iNumLista], TAMTOKEN, szPalabras[posDic]);
			iPeso[iNumLista] = iEstadisticas[posDic];
			iNumLista++;
		}
	}
	OrderWord(szListaFinal, iNumLista, iPeso);
}








void eliminate(char* szPalabra, int l, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas, char auxWord[TAMTOKEN])
{
	int k, i, j;
	for (i = 0; i < l; i++) { //caracter a eliminar
		k = 0; //rellenar la palabra auxiliar

		for (j = 0; j < l; j++) { //recorrer la palabra original
			if (j != i) { // copia todas las letras menos la que estamos eliminando
				auxWord[k] = szPalabra[j];
				k++;
			}
		}
		auxWord[k] = '\0';
		if (k > 0) {
			strcpy_s(szPalabrasSugeridas[(iNumSugeridas)++], TAMTOKEN, auxWord);
		}
	}
}

void changePlace(char* szPalabra, int l, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas, char auxWord[TAMTOKEN])
{
	char temp;
	for (int i = 0; i < (l - 1); i++) { //recorrer la palabra original
		strcpy_s(auxWord, TAMTOKEN, szPalabra);
		// cambiar de lugares
		temp = auxWord[i];
		auxWord[i] = auxWord[i + 1];
		auxWord[i + 1] = temp;
		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, auxWord);
		iNumSugeridas++;
	}
}

void sustitute(char szPalabra[], int l, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas, char letters[], char auxWord[TAMTOKEN])
{
	for (int i = 0; i < l; i++) { //recorrer la palabra original
		for (int j = 0; j < (int)strlen(letters); j++) { //recorrer el alfabeto
			strcpy_s(auxWord, TAMTOKEN, szPalabra);
			auxWord[i] = letters[j];
			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, auxWord);
			iNumSugeridas++;
		}
	}
}

void add(char szPalabra[], int l, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas, char letters[], char auxWord[TAMTOKEN])
{
	int i, j, k, posicion;
	int lenLetters = (int)strlen(letters);
	for (i = 0; i <= l; i++) { // i va desde antes de la primera letra (0) hasta despues de la ultima letra (l)
		for (j = 0; j < lenLetters; j++) {
			k = 0;

			// Copiar caracteres antes de la posición i
			for (posicion = 0; posicion < i; posicion++) {
				auxWord[k] = szPalabra[posicion];
				k++;
			}

			// Insertar la letra del alfabeto en la posición i
			auxWord[k] = letters[j];
			k++;

			// Copiar el resto de los caracteres de la palabra original
			for (posicion = i; posicion < l; posicion++) {
				auxWord[k] = szPalabra[posicion];
				k++;
			}

			auxWord[k] = '\0';
			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, auxWord);
			iNumSugeridas++;
		}
	}
}

void OrderList(char szPalabrasSugeridas[][TAMTOKEN], int iNumSugeridas)
{
	//burbuja
	int i, j;
	char temp[TAMTOKEN];
	int changed;

	for (i = 0; i < iNumSugeridas - 1; i++)
	{
		changed = 0;
		for (j = 0; j < iNumSugeridas - 1 - i; j++)
		{
			if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[j + 1]) > 0)
			{
				// Intercambiar palabras
				strcpy_s(temp, TAMTOKEN, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, szPalabrasSugeridas[j + 1]);
				strcpy_s(szPalabrasSugeridas[j + 1], TAMTOKEN, temp);
				changed = 1;
			}
		}
		if (changed == 0) {
			break;
		}
	}
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
{
	int l = (int)strlen(szPalabraLeida);
	char letters[] = "abcdefghijklmn\xF1opqrstuvwxyz\xE1\xE9\xED\xF3\xFA";
	char auxWord[TAMTOKEN];
	iNumSugeridas = 0;
	strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, szPalabraLeida);

	eliminate(szPalabraLeida, l, szPalabrasSugeridas, iNumSugeridas, auxWord);
	changePlace(szPalabraLeida, l, szPalabrasSugeridas, iNumSugeridas, auxWord);
	sustitute(szPalabraLeida, l, szPalabrasSugeridas, iNumSugeridas, letters, auxWord);
	add(szPalabraLeida, l, szPalabrasSugeridas, iNumSugeridas, letters, auxWord);
	OrderList(szPalabrasSugeridas, iNumSugeridas);
}