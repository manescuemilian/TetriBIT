#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>

//Manescu Emilian-Claudiu, 313CB

//functie care automatizeaza afisarea hartii
void afisare_harta(uint64_t numar) {
	//numarul cu 1 pe ultima pozitie si 0 in rest
    	uint64_t numar_maxim = 1ul << 63;
    	int i;
	for(i = 0; i < 64; i++) {	
		//verifica daca bitul este setat
		printf("%s", numar & numar_maxim ? "#" : ".");
		//shift la stanga pentru a verifica urmatorul bit
	    	numar = numar << 1;
	    	//newline dupa fiecare 8 elemente
	    	if ((i+1) % 8 == 0) {
	    		printf("\n");
	    	}
	}
	printf("\n");
}

//calculeaza numarul de zerouri la finalul executiei
int calcul_zero (uint64_t numar) {
	int nr_zero = 0, i;
	uint64_t numar_maxim = 1ul << 63;
	for(i = 0; i < 64; i++) {
	    if ( (numar & numar_maxim) == 0) nr_zero++;
	    numar = numar << 1;
	}
	return nr_zero; //returneaza numarul de zerouri din numar
}

//afla cate linii complete are numarul la un moment dat
int cate_linii_complete (uint64_t numar) {
	int i, j;
	int este_setat;
	int linii_complete = 0;
	for (i = 0; i < 8; i++) {
		este_setat = 0;
		for (j = 0; j < 8; j++) {
			if ((numar & (1ul << (j + i * 8))) != 0) este_setat++;
		}
		
		if (este_setat == 8) {
			linii_complete++;
		}
	}
	return linii_complete;
}

//elimina liniile complete
int elimin (uint64_t *numar, int linii_eliminate) {
	int este_setat = 0;
	int unde_completa; //memoreaza pozitia unde incepe linia completa
	int cate_linii; //daca trebuie sterse mai multe linii deodata
	int i, j, k;
	//memoreaza liniile pana la pozitia de unde incepe linia completa
	uint64_t copie_linii;
	//copiaza harta pentru ca se vor face modificari la numar
	uint64_t copie_harta = *numar;
	uint64_t masca = 0ul;
	for (i = 0; i < 8; i++) {
		este_setat = 0;
		for (j = 0; j < 8; j++) {
			//daca linia i a hartii este completa
			if ((*numar & (1ul << (j + i * 8))) != 0) este_setat++;
		}
		
		if (este_setat == 8) {
			unde_completa = i * 8;
			linii_eliminate++;
			
			if (unde_completa != 0) {
				int t;
				for (k = 0; k < unde_completa; k++) {
					//masca cu 1 pana la linia completa
					masca |= (1ul << k);
					copie_linii = *numar & masca;
					copie_harta |= (1ul << k);
					//harta cu 1 pana la linia completa
				}
				for (t = 63; t >= unde_completa; t--) {
					copie_linii |= (1ul << t);
				}
				//shiftare la dreapta cu o linie
				copie_harta >>= 8;
				*numar = copie_linii & copie_harta;
			//else: daca prima linie este completa
			} else if (unde_completa == 0) { 
				*numar = *numar >> 8;
			}
			cate_linii = cate_linii_complete (*numar);
			if (cate_linii == 0) {
				afisare_harta (*numar);
			}
		}
		este_setat = 0;
	}
	return linii_eliminate;
}

int main() {
	uint64_t nr_harta;
	double scor;
	int  nr_zero, nr_mutari, i, j, u;
	int linii_eliminate;
	//pe linia i a matricei se afla mutarile corespunzatoare piesei i
	int64_t mutari[50][50];
	//valoarea decimala a piesei este de forma mutari[i][-1]
	scanf("%lu", &nr_harta);
	scanf("%d", &nr_mutari);
	
	for(i = 0; i < nr_mutari; i++) {//citeste mutarile pentru fiecare
		for(int j = -1; j < 8; j++) {
			scanf("%ld", &mutari[i][j]);
		}
	}
		
	afisare_harta(nr_harta);
	uint64_t copie_numar = nr_harta;
	uint64_t copie;
	//numarul care are 1 pe prima coloana si 0 in rest
	uint64_t st = 9259542123273814144ul;
	//numarul care are 1 pe ultima coloana si 0 in rest
	uint64_t dr = 72340172838076673ul;
	
	//parcurgem piesele si le integram
	for (i = 0; i < nr_mutari; i++) {
		int64_t piesa = mutari[i][-1];
		int64_t piesa_originala = piesa;
		int64_t copie_piesa = piesa;
		uint64_t debug = ~(1ul << 63);
	
		//parcurgem mutarile
		for (j = 0; j < 8; j++) {
			//urcam piesa pe ultima linie	
			piesa = copie_piesa << (56 - 8 * j);
			int intra = nr_harta & piesa;
			int mut1, mut2;
			//daca mutarea este spre dreapta
			if (mutari[i][j] > 0) {
				int nr_mutare1 = 0;
				mut1 = nr_mutare1 < mutari[i][j];
				while ((piesa & dr) == 0 && mut1 && intra == 0) {
					piesa >>= 1;
					copie_piesa >>= 1;
					if ((nr_harta & piesa) != 0) {
						piesa <<= 1;
						copie_piesa <<= 1;
						goto continuare;
					}
					nr_mutare1++;
					mut1 = nr_mutare1 < mutari[i][j];
					//fara conditie, ultimul bit e setat
					piesa = piesa & debug;
				}  
			//daca mutarea este spre stanga	
			} else if (mutari[i][j] < 0) {
				int nr_mutare2 = 0;
				mut2 = nr_mutare2 < abs(mutari[i][j]);
				while ((piesa & st) == 0 && mut2 && intra == 0) {
					piesa <<= 1;
					copie_piesa <<= 1;
					if ((nr_harta & piesa) != 0) {
						piesa >>= 1;
						copie_piesa >>= 1;
						goto continuare;	
					}
					nr_mutare2++;
					mut2 = nr_mutare2 < abs(mutari[i][j]);
				}
			} else if (mutari[i][j] == 0) {
			}
			continuare:
			if ((nr_harta & piesa) == 0) {
				nr_harta |= (uint64_t)piesa;
				//salveaza harta corecta dupa fiecare mutare
				copie = nr_harta;
			} else {
				nr_harta = copie;
				//daca piesa nu intra complet in harta
				if (j == 0) {
					afisare_harta (nr_harta);
					goto final;
				} else if (j == 1 && piesa_originala > 255) {
					afisare_harta(nr_harta);
					goto final;
				}
				break;
			}
			//instructiunile de "reparare" a mutarii precedente
			afisare_harta(nr_harta);
			if (j == 7) {
				break;
			}
			nr_harta = nr_harta & copie_numar;
		}
		for (u = 0; u < 8; u++) {
			linii_eliminate = elimin(&nr_harta, linii_eliminate);
		}
		copie_numar = nr_harta;
	}
	final:
	nr_zero = calcul_zero (nr_harta);
	scor = sqrt (nr_zero) + pow (1.25, linii_eliminate);
	printf("GAME OVER!\n");
	printf("Score:%.2lf", scor);
	return 0;
}