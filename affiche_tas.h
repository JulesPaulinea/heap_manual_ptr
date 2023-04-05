/* Definition du tas */

#define TAILTAS 127	/* l'indice max et la taille de la plus grande donnee possible */
#define TAILMIN 2	/* le plus petit bloc utilisable */
#define FIN -1		/* marqueur de fin de liste */

char tas[TAILTAS+1];
int libre;		/* indice du premier bloc libre dans le tas, ou
			   FIN si aucun bloc libre */

void afficher_tas(void);
    /* affiche le contenu du tas */
    
void tas_init(void);
    /* initialise le contenu du tas quand celui-ci ne contient pas de donnees */
    
