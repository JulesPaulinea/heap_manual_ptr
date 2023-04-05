#include <stdio.h>
#include "affiche_tas.h"
#include <ctype.h>

void afficher_tas(){
  printf ("libre = %d\n", libre);
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 16; j++) {
      printf("%4d", j + 16*i);
    }
    printf("\n");
    for (j = 0; j < 16; j++) {
      printf("%4d", tas[j + 16*i]);
    }
    printf("\n");
    for (j = 0; j < 16; j++) {
      if (isprint(tas[j + 16*i])) {
	printf("%4c", tas[j + 16*i]);
      } else {
	printf("    ");
      }
    }
    printf("\n\n");
  }
  printf("---------------------------------------------------------------\n\n");
}

 void tas_init(){
   libre = 0;
   tas[0] = TAILTAS;
   tas[1] = FIN;
   // debug
   for (int i = 2; i < TAILTAS+1; i++) {
     tas[i] = 'z';
   }
 }
