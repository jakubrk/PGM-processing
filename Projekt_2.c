#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS

struct obraz
{
	char typ[3];
	char nazwa[10];
	int wysokosc;
	int szerokosc;
	int glebia_szarosci;
	int **piksele;

};

void cls(HANDLE hConsole);
int wczytajObraz(struct obraz *baza_obrazow, int nr_obrazu);
int czytajKomentarz(FILE **plik);
void wyswietl(struct obraz *aktywny);
void zapiszObraz(struct obraz *aktywna);
void odbijObraz(struct obraz *aktywny, int os);
void obrocObraz(struct obraz *aktywny, int wariant);
void histogram(struct obraz *aktywny);
void progowanie(struct obraz *aktywny, HANDLE **hStdout);
void negatyw(struct obraz *aktywny);
void szum_sol_pieprz(struct obraz *aktywny);
void filtrujMediana(struct obraz *aktywny);
void sortujTablice(int **okno);
void filtrujGauss(struct obraz *aktywny, int rodzaj);


int main(void)
{
	setlocale(LC_ALL, "polish_poland");
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	int opcja = 0;
	int modyfikacja = 0;
	int powrot = 1;
	int ilosc_obrazow = 0;
	int nr_obrazu = 0;

	struct obraz *baza_obrazow;
	struct obraz *do_usuniecia;

	baza_obrazow = malloc (ilosc_obrazow * sizeof(struct obraz));

	struct obraz *aktywny=NULL;
	do
	{
		printf("MENU\n\n[1]Wczytaj obraz\n[2]Lista wczytanych obrazow\n[3]Aktywuj obraz\n[4]Modyfikuj obraz\n[5]Wyswietl\n[6]Usun obraz(stos)\n[7]Zapisz obraz\n[8]Zakoncz program\n");

		opcja = wyswietlBezpiecznie(hStdout, "MENU\n\n[1]Wczytaj obraz\n[2]Lista wczytanych obrazow\n[3]Aktywuj obraz\n[4]Modyfikuj obraz\n[5]Wyswietl\n[6]Usun obraz(stos)\n[7]Zapisz obraz\n[8]Zakoncz program\n");

			switch (opcja)
		{
		case 1: //wczytaj
			cls(hStdout);
			ilosc_obrazow++;
			baza_obrazow = realloc(baza_obrazow, ilosc_obrazow * sizeof(struct obraz));
			ilosc_obrazow=wczytajObraz(&baza_obrazow[ilosc_obrazow-1], ilosc_obrazow);

			printf("\nNacisnij 0, by wrocic do menu glownego...");
			
			scanf("%d", &powrot);
			
			if (powrot == 0)
				cls(hStdout);
			break;
		case 2:
			cls(hStdout);
			printf("Wczytane obrazy\n\n");
			for (int i = 0; i < ilosc_obrazow; i++)
				printf("%d. %s\n", i + 1, baza_obrazow[i].nazwa);

			printf("\nNacisnij 0, by wrocic do menu glownego... \n");
			scanf("%d", &powrot);

			while(opcja!=0){
				cls(hStdout);
				printf("\nNacisnij 0, by wrocic do menu glownego... \n");
				opcja = wyswietlBezpiecznie(hStdout, "\nNacisnij 0, by wrocic do menu glownego... \n");
			} 

			if (powrot == 0)
				cls(hStdout);
			break;

		case 3: //wybierz z wczytanych
			cls(hStdout);
			printf("Wczytane obrazy\n\n");
			for (int i = 0; i < ilosc_obrazow; i++)
			printf("%d. %s\n", i+1,  baza_obrazow[i].nazwa);

			printf("\nWybierz numer, by aktywowac obraz oraz wrocic do menu glownego... \n");
			do
			{
				scanf("%d", &nr_obrazu);
				aktywny = &baza_obrazow[nr_obrazu-1];
				printf("W bazie nie ma wczytanego obrazu o podanym numerze");
			} while (nr_obrazu <= 0 || nr_obrazu > ilosc_obrazow);
			cls(hStdout);
			break;

		case 4: //modyfikuj
			cls(hStdout);
			if (aktywny == NULL)
			{
				printf("Nie aktywowano zadnego obrazu.Aktywuj najpierw obraz\n\n");
				printf("\nNacisnij 0, by wrocic do menu glownego...");
				scanf("%d", &powrot);
				if (powrot == 0)
				cls(hStdout);
				break;
			}

			else
				printf("[1]Odbicie wzgledem osi\n[2]Obrot o k90*\n[3]Histogram\n[4]Progowanie\n[5]Negatyw\n[6]Szum_pieprz_sol\n[7]Filr Gaussa\n[8]Filtr medioanowy\n[0]Powrot do menu glownego\n");
			
				modyfikacja = wyswietlBezpiecznie(hStdout, "[1]Odbicie wzgledem osi\n[2]Obrot o k90*\n[3]Histogram\n[4]Progowanie\n[5]Negatyw\n[6]Szum_pieprz_sol\n[7]Filr Gaussa\n[8]Filtr medianowy\n[0]Powrot do menu glownego\n");
			
				switch (modyfikacja)
			{
			case 1:
				cls(hStdout);

				printf("[1]Wzgledem osi poziomej\n[2]Wzgledem osi pionowej\n");
				do {
					modyfikacja = wyswietlBezpiecznie(hStdout, "[1]Wzgledem osi poziomej\n[2]Wzgledem osi pionowej\n");
				} while (modyfikacja < 1 || modyfikacja>2);

				odbijObraz(aktywny, modyfikacja);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "\nNacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 2:
				cls(hStdout);
				printf("[1]Obrot o 90*\n[2]Obrot o 180*\n[3]Obrot o 270*\n");
				do {
					modyfikacja = wyswietlBezpiecznie(hStdout, "[1]Obrot o 90*\n[2]Obrot o 180*\n[3]Obrot o 270*\n");
					
				} while (modyfikacja < 1 || modyfikacja > 3);
					obrocObraz(aktywny, modyfikacja);

					cls(hStdout);
					printf("Nacisnij 0, by wrocic do menu glownego...");
					do {
						powrot = wyswietlBezpiecznie(hStdout, "\nNacisnij 0, by wrocic do menu glownego...");
					} while (powrot != 0);
					if (powrot == 0)
						cls(hStdout);
					break;

			case 3:
				cls(hStdout);
				histogram(aktywny);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "\nNacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 4:
				cls(hStdout);
				progowanie(aktywny, &hStdout);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 5:
				cls(hStdout);
				negatyw(aktywny);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 6:
				cls(hStdout);
				szum_sol_pieprz(aktywny);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 7:
				cls(hStdout);

				printf("[1]Rozmycie\n[2]Wyostrzenie\n");
				do {
					modyfikacja = wyswietlBezpiecznie(hStdout, "[1]Rozmycie\n[2]Wyostrzenie\n");
				} while (modyfikacja < 1 || modyfikacja > 2);
				filtrujGauss(aktywny, modyfikacja);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;
				
			case 8:
				cls(hStdout);
				filtrujMediana(aktywny);

				cls(hStdout);
				printf("Nacisnij 0, by wrocic do menu glownego...");
				do {
					powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
				} while (powrot != 0);
				if (powrot == 0)
					cls(hStdout);
				break;

			case 0:
				cls(hStdout);
				break;

			default:
				cls(hStdout);
				break;
			}
			break;

		case 5: //wyswietl
			cls(hStdout);
			wyswietl(aktywny);

			printf("Nacisnij 0, by wrocic do menu glownego...");
			do {
				powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
			} while (powrot != 0);
			if (powrot == 0)
				cls(hStdout);
			break;

		case 6: //usun
			cls(hStdout);

			ilosc_obrazow=usunObraz(&baza_obrazow, ilosc_obrazow);

			printf("Nacisnij 0, by wrocic do menu glownego...");
			do {
				powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
			} while (powrot != 0);
			if (powrot == 0)
				cls(hStdout);
			break;

		case 7: //zapisz
			cls(hStdout);
			zapiszObraz(aktywny);
			
			printf("Nacisnij 0, by wrocic do menu glownego...");
			do {
				powrot = wyswietlBezpiecznie(hStdout, "Nacisnij 0, by wrocic do menu glownego...");
			} while (powrot != 0);
			if (powrot == 0)
				cls(hStdout);
			break;

		case 8:
			for (int i = 0; i < ilosc_obrazow; i++)
			{
				for (int j = 0; j < baza_obrazow[i].szerokosc; j++)
				{
					free(baza_obrazow[i].piksele[j]);
				}
			baza_obrazow[i].piksele=NULL; 
			}
			free(baza_obrazow);
			exit(0);
			
		default:
			cls(hStdout);
			break;
		}

	} while (1);
}

void cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer. 

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer 
		(TCHAR) ' ',     // Character to write to the buffer
		dwConSize,       // Number of cells to write 
		coordScreen,     // Coordinates of first cell 
		&cCharsWritten))// Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Set the buffer's attributes accordingly.

	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer 
		csbi.wAttributes, // Character attributes to use
		dwConSize,        // Number of cells to set attribute 
		coordScreen,      // Coordinates of first cell 
		&cCharsWritten)) // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(hConsole, coordScreen);
}

int czytajKomentarz(FILE **plik)
{
	int bufor;
	char temp;
	if(fscanf(*plik, "%d", &bufor))
	{
		return bufor;
	}
	else
	{
		temp = fgetc(*plik);
		if (temp == '#')
		{
			do
				temp = fgetc(*plik);
			while (temp != '\n');
		}
		return czytajKomentarz(&(*plik));

	}
}
int wczytajObraz(struct obraz *baza_obrazow, int ilosc_obrazow)
{
	 
	FILE *plik;

	printf("Podaj nazwe pliku: ");
	scanf("%s", baza_obrazow->nazwa);
	plik = fopen(baza_obrazow->nazwa, "r");

	if (plik) {

		fgets((*baza_obrazow).typ, 3, plik);
		if ((*baza_obrazow).typ[0] == 'P' &&  baza_obrazow->typ[1] == '2')
		{
			fprintf(stderr, "To plik PGM w standardzie ASCII\n");
			printf("%c%c\n", baza_obrazow->typ[0], baza_obrazow->typ[1]);
		}
	
		baza_obrazow->szerokosc = czytajKomentarz(&plik);
		baza_obrazow->wysokosc = czytajKomentarz(&plik);
		baza_obrazow->glebia_szarosci = czytajKomentarz(&plik);
		
		printf("%d\n", baza_obrazow->szerokosc);
		printf("%d\n", baza_obrazow-> wysokosc);
		printf("%d\n", baza_obrazow->glebia_szarosci);
		
		baza_obrazow->piksele=malloc(baza_obrazow->szerokosc * sizeof(int*));
		
		for (int i = 0; i < baza_obrazow->szerokosc; i++)
		{
			baza_obrazow->piksele[i] =malloc(baza_obrazow->wysokosc * sizeof(int));
		}

		for (int i = 0; i < baza_obrazow->wysokosc; i++)
			for (int j = 0; j < baza_obrazow->szerokosc; j++)
			{
				fscanf(plik, "%d ", &baza_obrazow->piksele[j][i]);
			}

		fclose(plik);
	}
	
	else
	{
		printf("Blad wczytania");
		return ilosc_obrazow - 1;
	}
	return ilosc_obrazow;
}
int usunObraz(struct obraz **baza_obrazow, int ilosc_obrazow)
{

	for (int i = 0; i < (*baza_obrazow + (ilosc_obrazow - 1))->szerokosc; i++)
	{
		free((*baza_obrazow + (ilosc_obrazow - 1))->piksele[i]);
	}
	baza_obrazow[ilosc_obrazow-1] = NULL;
	 *baza_obrazow = realloc(*baza_obrazow, (ilosc_obrazow-1)*sizeof(struct obraz));
		return (ilosc_obrazow - 1);
}

void zapiszObraz(struct obraz *aktywny)
{

	if (aktywny == NULL)
		printf("Nie aktywowano zadnego obrazu. Aktywuj najpierw obraz\n");
	else
	{
		FILE *plik;
		char nowa_nazwa[100];
		printf("Podaj nowa nazwe pliku\n");
		scanf("%s", &nowa_nazwa);
		plik = fopen(nowa_nazwa, "w");
		fprintf(plik, "%s\n%d %d\n%d\n", aktywny->typ, aktywny->szerokosc, aktywny->wysokosc, aktywny->glebia_szarosci);
		for (int i = 0; i < aktywny->wysokosc; i++)
		{
			for (int j = 0; j < aktywny->szerokosc; j++)
			{
				fprintf(plik, "%d ", aktywny->piksele[j][i]);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
	}
}

void wyswietl(struct obraz *aktywny)
{
	if (aktywny == NULL)
		printf("Nie aktywowano zadnego obrazu. Aktywuj najpierw obraz\n");
	else
	for (int i = 0; i < aktywny->szerokosc; i++)
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				printf("%d\t", aktywny->piksele[i][j]);
			}
			printf("\n");
		}
}

int wyswietlBezpiecznie(HANDLE *hStdout, char komunikat[300])
{
	int liczba;
	while (scanf("%d", &liczba) != 1) //dopóki nie uda siê wczytaæ
		{
			int c;
			while ((c = getchar()) != '\n' && c != EOF); //pêtla wyci¹gaj¹ca znaki z bufora

			cls(hStdout);
			printf("%s",komunikat);
		}
	return liczba;
}

void odbijObraz(struct obraz *aktywny, int os)
{
	int **temp = NULL;
	temp = malloc(aktywny->szerokosc * sizeof(int*));

	for (int i = 0; i < aktywny->szerokosc; i++)
	{
		temp[i] = malloc(aktywny->wysokosc * sizeof(int));
	}

	if (os == 1)
	
		for (int i = 0; i < aktywny->szerokosc; i++)
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				temp[i][aktywny->wysokosc - 1 - j] = aktywny->piksele[i][j];
			}
		}
		else
		{
			for (int i = 0; i < aktywny->szerokosc; i++)
			{
				for (int j = 0; j < aktywny->wysokosc; j++)
				{
					temp[aktywny->szerokosc-1-i][j] = aktywny->piksele[i][j];
				}
			}
		}
		for (int i = 0; i < aktywny->szerokosc; i++)
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				aktywny->piksele[i][j] = temp[i][j];
			}
		}
}

void obrocObraz(struct obraz *aktywny, int wariant)
{
	for (int i = 0; i < wariant; i++) {
		int **temp = NULL;
		int nowa_szerokosc = aktywny->wysokosc;
		int nowa_wysokosc = aktywny->szerokosc;
		temp = malloc(nowa_szerokosc * sizeof(int*));
		for (int i = 0; i < nowa_szerokosc; i++)
		{
			temp[i] = malloc(nowa_wysokosc * sizeof(int));
		}
		for (int i = 0; i < nowa_szerokosc; i++)
		{
			for (int j = 0; j < nowa_wysokosc; j++)
			{
				temp[i][j] = aktywny->piksele[j][nowa_szerokosc - 1 - i];
			}
		}

		for (int i = 0; i < aktywny->szerokosc; i++)
		{
			free(aktywny->piksele[i]);
		}
		free(aktywny->piksele);

		aktywny->piksele = malloc(nowa_szerokosc * sizeof(int*));
		for (int i = 0; i < nowa_szerokosc; i++)
		{
			aktywny->piksele[i] = malloc(nowa_wysokosc * sizeof(int));
		}

		for (int i = 0; i < nowa_szerokosc; i++)
		{
			for (int j = 0; j < nowa_wysokosc; j++)
			{
				aktywny->piksele[i][j] = temp[i][j];
			}
		}

		for (int i = 0; i < nowa_szerokosc; i++)
		{
			free(temp[i]);
		}
		free(temp);

		aktywny->szerokosc = nowa_szerokosc;
		aktywny->wysokosc = nowa_wysokosc;
	}
	return 0;
}
void histogram(struct obraz *aktywny)
{
	int *licznik=NULL;
	licznik = calloc(aktywny->glebia_szarosci, sizeof(int));
	for (int i = 0; i < aktywny->szerokosc; i++)
		for (int j = 0; j < aktywny->wysokosc; j++)
		{
			licznik[aktywny->piksele[i][j]]++;
		}
	FILE *plik;
	plik = fopen("histogram.csv", "w");
	if (plik)
	{
		for (int i = 0; i < aktywny->glebia_szarosci; i++)
			fprintf(plik, "%d ; %d\n", i, licznik[i]);
		fclose(plik);
		printf("Histogram zostal sporzadzony poprawnie\n\n");
	}
	else printf("Nie udalo sie uzyskac dostepu do pliku!\n\n");
}
void progowanie(struct obraz *aktywny, HANDLE **hStdout)
{
	int prog = 0;
	printf("Podaj prog(<255): ");
	do {

		prog = wyswietlBezpiecznie(*hStdout, "Podaj prog(<255): ");

	} while (prog < 0 || prog>255);
	for(int i=0; i<aktywny->szerokosc; i++)
		for (int j = 0; j < aktywny->wysokosc; j++)
		{
			if (aktywny->piksele[i][j] <= prog)
			{
				aktywny->piksele[i][j] = 0;
			}
			else aktywny->piksele[i][j] = 255;
		}
}
void negatyw(struct obraz *aktywny)
{
	for (int i = 0; i < aktywny->szerokosc; i++)
		for (int j = 0; j < aktywny->wysokosc; j++)
		{
			aktywny->piksele[i][j] = 255 - aktywny->piksele[i][j];
		}
}
void szum_sol_pieprz(struct obraz *aktywny)
{
	int ilosc = 0;
	srand(time(NULL));
	for (int i = 0; i < aktywny->szerokosc; i++) 
	{
		for (int j = 0; j < aktywny->wysokosc; j++)
		{

			if (rand()%100 == 1)
			{
				if (rand() % 2) aktywny->piksele[i][j] = 0;
				else
				{
					aktywny->piksele[i][j] = aktywny->piksele[i][j]=255;
				}
			}

		}
	}
}

void filtrujMediana(struct obraz *aktywny)
{
	int **temp = NULL;
	int **okno = NULL;
	temp = malloc(aktywny->szerokosc * sizeof(int*));

	for (int i = 0; i < aktywny->szerokosc; i++)
	{
		temp[i] = malloc(aktywny->wysokosc * sizeof(int));
	}
	 
	okno = malloc(3 * sizeof(int*));
	
	for (int i = 0; i < 3; i++)
	{
		okno[i] = malloc(3 * sizeof(int));
	}

	for (int i = 0; i < aktywny->szerokosc; i++) // przepisywanie z aktywnej do temp
	{
		for (int j = 0; j < aktywny->wysokosc; j++)
		{
			temp[i][j] = aktywny->piksele[i][j];

		}
	}

	for (int i = 0; i < aktywny->szerokosc - 2; i++)
	{
		for (int j = 0; j < aktywny->wysokosc-2; j++)
		{
			okno[0][0] = aktywny->piksele[i + 0][j + 0];
			okno[0][1] = aktywny->piksele[i + 0][j + 1];
			okno[0][2] = aktywny->piksele[i + 0][j + 2];
			okno[1][0] = aktywny->piksele[i + 1][j + 0];
			okno[1][1] = aktywny->piksele[i + 1][j + 1];
			okno[1][2] = aktywny->piksele[i + 1][j + 2];
			okno[2][0] = aktywny->piksele[i + 2][j + 0];
			okno[2][1] = aktywny->piksele[i + 2][j + 1];
			okno[2][2] = aktywny->piksele[i + 2][j + 2];
			
			sortujTablice(okno);
			
			temp[i + 1][j + 1] = okno[1][1];
		}
	}
	
	for (int i = 0; i < aktywny->szerokosc; i++)
	{
		for (int j = 0; j < aktywny->wysokosc; j++)
		{
			aktywny->piksele[i][j] = temp[i][j];
		}
	}
}

void sortujTablice(int **okno)
{
	int sortowanie[9];
	int licznik = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			sortowanie[licznik] = okno[i][j];
			licznik++;
		}
	}

	int temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9 - 1; j++)
		{
			if (sortowanie[j] > sortowanie[j + 1])
			{
				temp = sortowanie[j];
				sortowanie[j] = sortowanie[j + 1];
				sortowanie[j + 1] = temp;
			}
		}
	}

	licznik = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			okno[i][j] = sortowanie[licznik];
			licznik++;
		}
	}
}

void filtrujGauss(struct obraz *aktywny, int wariant)
{
	int suma = 0;
	int rozmycie[3][3] = {
						{1,2,1},
						{2,4,2},
						{1,2,1}
						};
	int wyostrzenie[3][3] = {
							{0,-1,0},
							{-1,5,-1},
							{0,-1,0}
							};

	int **temp = NULL;
	int **okno = NULL;
	temp = malloc(aktywny->szerokosc * sizeof(int*));

	for (int i = 0; i < aktywny->szerokosc; i++)
	{
		temp[i] = malloc(aktywny->wysokosc * sizeof(int));
	}

	okno = malloc(3 * sizeof(int*));
	for (int i = 0; i < 3; i++)
	{
		okno[i] = malloc(3 * sizeof(int));
	}

		for (int i = 0; i < aktywny->szerokosc; i++) // przepisywanie z aktywnej do temp
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				temp[i][j] = aktywny->piksele[i][j];
			}
		}

		for (int i = 0; i < aktywny->szerokosc - 2; i++)
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				suma = 0;
				okno[0][0] = aktywny->piksele[i + 0][j + 0];
				okno[0][1] = aktywny->piksele[i + 0][j + 1];
				okno[0][2] = aktywny->piksele[i + 0][j + 2];
				okno[1][0] = aktywny->piksele[i + 1][j + 0];
				okno[1][1] = aktywny->piksele[i + 1][j + 1];
				okno[1][2] = aktywny->piksele[i + 1][j + 2];
				okno[2][0] = aktywny->piksele[i + 2][j + 0];
				okno[2][1] = aktywny->piksele[i + 2][j + 1];
				okno[2][2] = aktywny->piksele[i + 2][j + 2];

				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						if (wariant == 1)
							suma = suma + okno[k][l] * rozmycie[k][l];
						if (wariant ==2)
							suma = suma + okno[k][l] * wyostrzenie[k][l];
					}
				}
				if(wariant==1)
					temp[i + 1][j + 1] = suma/16;
				if(wariant ==2)
					temp[i + 1][j + 1] = suma/1;
			}
		}

		for (int i = 0; i < aktywny->szerokosc; i++)
		{
			for (int j = 0; j < aktywny->wysokosc; j++)
			{
				aktywny->piksele[i][j] = temp[i][j];
			}
		}
}

