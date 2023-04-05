#include <stdio.h>
#include "affiche_tas.h"
#include <ctype.h>
#include <stdlib.h>

int first_fit(int taille, int pred);
char* tas_malloc(unsigned int taille);
int tas_free(char* ptr);
int(*strategie_fit(int choice))(int, int*);
int zone_vide(char* ptr);
int inside_tas(char* ptr);
int indice_buffer(char* ptr);
int taille_buffer(char* ptr);
char* next_buffer(char* ptr);
char* prev_zone_vide(char* ptr);

int main()
{
    tas_init();
    afficher_tas();

//    char* p1 = tas_malloc(10);
//    char* p2 = tas_malloc(9);
//    char* p3 = tas_malloc(5);
//    strcpy(p1,"tp 1");
//    afficher_tas();
//    strcpy(p2,"tp 2");
//    afficher_tas();
//    strcpy(p3,"tp 3");
//    afficher_tas();
//    tas_free(p2);
//    afficher_tas();
//    char* p4 = tas_malloc(8);
//    strcpy(p4,"systeme");
//    afficher_tas();

    char* p1 = tas_malloc(10);
    char* p2 = tas_malloc(10);
    char* p3 = tas_malloc(10);
    char* p4 = tas_malloc(10);
    char* p5 = tas_malloc(10);

    strcpy(p1,"tp1");
    strcpy(p2,"tp2");
    strcpy(p3,"tp3");
    strcpy(p4,"tp4");
    strcpy(p5,"tp5");
    afficher_tas();

    tas_free(p2);
    tas_free(p4);
    tas_free(p3);
    tas_free(p5);
    tas_free(p1);
    afficher_tas();

    return 0;
}

int zone_vide(char* ptr)
{
    // test if buffer is FREE
    return *(ptr+1)  == -1;
}

int inside_tas(char* ptr)
{
    // test if buffer is within heap
    return ptr - tas < TAILTAS && ptr - tas >= 0;
}

int indice_buffer(char* ptr)
{
    // return index of buffer
    return ptr - tas;
}

int taille_buffer(char* ptr)
{
    // return size of buffer
    return *ptr;
}

char* next_buffer(char* ptr)
{
    // return next buffer
    return ptr + taille_buffer(ptr) + 1;
}

char* prev_zone_vide(char* ptr)
{
    //return previous previous FREE buffer if any exists
    char* pred = ptr-2;
    while(!zone_vide(pred) && inside_tas(pred))
    {
        pred--;
    }
    return pred;
}

int first_fit(int taille, int pred)
{
    char* ptr = pred;

    while (inside_tas(ptr)) //parcours buffer
    {
        if(zone_vide(ptr)) //test si zone vide
        {
            if(taille_buffer(ptr) >= taille) // taille zone libre
            {
                return indice_buffer(ptr);
            }
        }
        ptr = next_buffer(ptr); //on avance jusqu'a prochaine zone
    }
    return -1;
}

int best_fit(int taille, int* pred)
{
    char* ptr = tas + libre; //pour parcourir
    char* best = tas + libre; //adresse meilleure zone possible(plus petite)
    int valide = 0;
    while (inside_tas(ptr)) //parcours buffer
    {
        if(zone_vide(ptr)) //test si zone vide
        {
            if(taille_buffer(ptr) >= taille && taille_buffer(ptr)  <= taille_buffer(best) ) // zone de plus petite taille
            {
                best =  ptr ;
                valide = 1;
            }
        }
        ptr = next_buffer(ptr); //on avance jusqu'a prochaine zone
    }
    if(valide) return indice_buffer(best);
    else return -1;
}

int worst_fit(int taille, int* pred)
{
    char* ptr = tas + libre; //pour parcourir
    char* worst = tas + libre; //adresse plus large zone possible(plus petite)
    int valide = 0;
    while (inside_tas(ptr)) //parcours buffer
    {
        if(zone_vide(ptr)) //test si zone vide
        {
            if(taille_buffer(ptr) >= taille && taille_buffer(ptr)  >= taille_buffer(worst) )
            {
                worst =  ptr ;
                valide = 1;
            }
        }
        ptr = next_buffer(ptr); //on avance jusqu'a prochaine zone
    }
    if(valide) return indice_buffer(worst);
    else return -1;
}

int(*strategie_fit(int choice))(int, int*)
{
    switch(choice)
    {
        case 0: return &best_fit; break;
        case 1: return &worst_fit; break;
        default : return &first_fit; break;
    }
}

char* tas_malloc(unsigned int taille)
{
    int start = strategie_fit(2)(taille + 1, &tas);
    if (start != -1) //si malloc possible
    {
        if(libre != start)
        {
            if(taille_buffer(tas + libre) - taille >=2)
            {
                tas[start + taille + 1] = taille_buffer(tas + start) - (taille + 1); //new taille libre
                tas[start + taille + 2] = -1; // -1
            }
            else
            {
                if(taille_buffer(tas + start) - taille == 1) taille++; // pour eviter case seule
            }
        }
        else
        {
            if(taille_buffer(tas + libre) - taille >=2)
            {
                tas[libre + taille + 1] = taille_buffer(tas + libre) - (taille + 1); //new taille libre
                tas[libre + taille + 2] = -1; // -1
                libre += taille + 1; //update premiere zone libre
            }
            else
            {
                if(taille_buffer(tas + libre) - taille == 1) taille++; // pour eviter case seule
                libre = first_fit(2, start + taille);;
            }
        }
        char *ptr = &tas[start];
        *ptr = taille; // 1er element: taille de la zone
        while(ptr<&tas[start] + taille)
        {
            ptr ++;
            *ptr = ' '; // initialise a ' '
        }
    }
    return tas+start+1;
}



int tas_free(char* ptr)
{
    // merge zone suivante
    if (zone_vide(next_buffer(ptr - 1)))
    {
        *(next_buffer(ptr - 1) + 1) = ' ';
        *(ptr - 1) += taille_buffer(next_buffer(ptr - 1) ) + 1 ; //une seule zone vide
    }
    // merge zone precedente
    char* pred = prev_zone_vide(ptr); // recherche zone vide precedente
    if(zone_vide(pred) && next_buffer(pred) + 1 == ptr) //zone vide ?
    {
        *(pred) += taille_buffer(ptr -1) + 1;
    }
    else // si pas zone vide
    {
        *ptr = -1;
        if (tas + libre > ptr) libre = indice_buffer(ptr)- 1; //update libre
    }
}
