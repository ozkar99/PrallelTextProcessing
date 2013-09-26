#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>

#define N 4

//maxima palabra = 45 caracteres, segun diccionario.

long int LINEAS=0,PALABRAS=0,CARACTERES=0;


char *PATH_A = "Archivos\\A.txt"; //entrada, leer y escribir reporte final (thread 1).
char *PATH_B = "Archivos\\B.txt"; //texto de linea - cantidad de palabras (thread 2).
char *PATH_D = "Archivos\\D.txt"; // caracter - cantidad de ocurrencias (thread 3).
char *PATH_E = "Archivos\\E.txt"; //archivo D ordenado ascendentemente.


int chars[177]; //caracteres ascii para ocurrencias.
long int sortedChars[177][2]; //para archivo D, [ocurrencias][caracter];


/* Lexicographically compare two arrays of size NUM_COLS. */
int CompareArrays(const void* arr1, const void* arr2) {
     /* Convert back to the proper type. */
     const int* one = (const int*) arr1;
     const int* two = (const int*) arr2;

     /* Do an element-by-element comparison.  If a mismatch is found, report how
      * the arrays compare against one another.
      */
     for (size_t i = 0; i < 2; i++) {
         if (one[i] < two[i]) return -1;
         if (one[i] > two[i]) return +1;
     }

     /* If we get here, the arrays are equal to one another. */
     return 0;
}

void reverse_C_file( ) {

	char *tmpfilename = "Archivos\\F.txt";
	char tmpfileC[200];

			
	FILE *fp = fopen(tmpfilename, "a");
	
	for(int i=45; i>0; i--) {

		printf("%d\n", i);
		sprintf_s(tmpfileC, "Archivos\\C\\%dC.txt", i);
		FILE *fpc = fopen(tmpfileC, "r");
			if ( fpc != NULL) {
			//reverse this shizzle:
				char line[1024];
			while( fgets(line, 1024, fpc) ) {
		
				fprintf(fp, "%s", line);
			}
			
			fclose(fpc);
		}
	   
	}
	fclose(fp);

}

void blank_file(char *s) {	
	remove(s);
	FILE *fp = fopen(s, "w");	
	fclose(fp);
}

void sort_D_file() {
	
	FILE *fout2;

	
	for(int i=0; i<177; i++) {
		sortedChars[i][0] = chars[i];
		sortedChars[i][1] = i;
	}

	//qsort(sortedChars, 177, sizeof(int), compare);
	/* Use qsort to sort the arrays */
	qsort((long int*)&sortedChars, 177, sizeof(int[2]), CompareArrays);

	fout2 = fopen(PATH_E, "w");

	for(int i=177; i>=0; i--) {
			
			if (sortedChars[i][0] != 0) {
				fprintf(fout2, "%c - %d\n", sortedChars[i][1], sortedChars[i][0]);
			}
	
	}

	fclose(fout2);
}

void print_report() {
	printf("Numero de Lineas: %d\n", LINEAS);
	printf("Numero de Palabras: %d\n", PALABRAS);
	printf("Numero de Caracteres: %d\n", CARACTERES);
	system("pause");
}

void send_to_C_file(int num, char *word, int numline) {

	FILE *fptemp;

	char tempchar[200];	

	sprintf_s(tempchar, "Archivos\\C\\%dC.txt", num);

	fptemp = fopen(tempchar, "a");
	
	if (fptemp != NULL) {
		fprintf(fptemp, "%d - %s - %d\n", numline, word, num);
	}

	fclose(fptemp);

}

int getStrLen(char s[45]) {
	int len=0;

	for(int i=0; i<45; i++) {
		if ( s[i] == '\0') {
			len = i;
			break;
		}
	}
	
	return len;
}

DWORD WINAPI Thread1(LPVOID arg) {
	//cuenta lo de el reporte final
	printf("Lanzando Thread #1\n");
	FILE *fin; 
	fin = fopen(PATH_A, "r"); //read	

	if (fin == NULL) {
		printf("No Encuentro El Archivo\n");		
	} else {

	while (1)
     {   
		 char ch = fgetc(fin);
		 ++CARACTERES;        
         if (ch == '\n') ++LINEAS; 
		 if (ch == ' ') ++PALABRAS;
		 if (ch == EOF ) break;
     }
	 fclose(fin);
	}
	printf("Termina Thread #1\n");
	return 0;
}

DWORD WINAPI Thread2(LPVOID arg) {
	printf("Lanzando Thread #2\n");
	FILE *fin,*fout; 
	fin = fopen(PATH_A, "r"); 
	fout = fopen(PATH_B, "w");	
	char line[1024];	
	long int words=0;

	if (fin == NULL) {
		printf("No Encuentro El Archivo\n");		
	} else {

		while( fgets(line, 1024, fin) ) {				 		 		 

         int words=0;															

				int len = strlen(line);
				for(int i=0; i <= len; i++) {
					if (line[i] == ' ') { 
						words += 1;					
					}
					
					if (line[i] == '\n') { line[i] = '\0';} //quita el \n

				}
												
						fprintf(fout,"%s - %d\n", line, words+1);																	
		}
		fclose(fin);
		fclose(fout);
	}
	printf("Termina Thread #2\n");
	return 0;
}

DWORD WINAPI Thread3(LPVOID arg) {
	printf("Lanzando Thread #3\n");

	FILE *fin; 
	fin = fopen(PATH_A, "r"); //read	

	if (fin == NULL) {
		printf("No Encuentro El Archivo\n");		
	} else {

	while (1)
     {   
		 char ch = fgetc(fin);		 
         
		 for(int i=0; i<=177; i++) {
			 if (ch == i) {
				 chars[i]++;
			 }
		 }

		 if (ch == EOF) { break; } //salir
     }
	 fclose(fin);
	}

	FILE *fout;
	fout = fopen(PATH_D, "w");

	for(int i=0; i<=177; i++) {
		
		if (chars[i] != 0) {
			fprintf(fout, "%c - %d\n", i, chars[i]); //caracter - numero de ocurrencias si existe mas que uno.
		}

	}

	fclose(fout);

	printf("Sorteando Archivo D...via Thread #3\n");
	sort_D_file();
	printf("Termina Thread #3");
	return 0;
}

DWORD WINAPI Thread4(LPVOID arg) {
	printf("Lanzando Thread #4\n");
	FILE *fin; 
	fin = fopen(PATH_A, "r"); //read	
	int linenumber=0;

	if (fin == NULL) {
		printf("No Encuentro El Archivo\n");		
	} else {
		
		char line[1024];
				
	while ( fgets(line, 1024, fin) )
		{   
			linenumber += 1; //numero de linea
			
			char *words = strtok(line, "\ ");
				
			while ( words != NULL ) {	
				int numStr = getStrLen(words);
				send_to_C_file(numStr, words, linenumber);				
				words = strtok(NULL, "\ ");
			} //fin words
	
	
		}//lineas


     }
	 fclose(fin);
	
	 printf("Comenzando Llenado de F.txt Via Thread #4\n");
	 reverse_C_file();
	
    printf("Termina Thread #4\n");
	return 0;
}


int main(int argc, char *argv[]) {
	
	
	HANDLE h[N];
		
	h[0] = CreateThread(NULL, 0, Thread1, NULL, 0, NULL); //lee A y genera el reporte.
	h[1] = CreateThread(NULL, 0, Thread2, NULL, 0, NULL); //llena B				
	h[2] = CreateThread(NULL, 0, Thread3, NULL, 0, NULL); //llena D y E
	h[3] = CreateThread(NULL, 0, Thread4, NULL, 0, NULL); //llena archivos C y F
	

	WaitForMultipleObjects(N, h, 1, INFINITE);	
	blank_file(PATH_A);
	
	print_report();
	return 0;
}


