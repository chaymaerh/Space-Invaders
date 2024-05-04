#include <stdbool.h>
typedef struct { int X;int Y; int num_lutin;}entité;
typedef struct cell { entité e ; struct cell * suivant;}cellule;
typedef cellule * Liste;



void ajout_tete(entité ent, cellule **pL, int x, int y);
void supprime_tete(cellule **pL, int x, int y);
bool bouger_monstre_x(Liste listeMonstres);
void bouger_monstre_y(Liste listeMonstres);
void intersection(Liste *missiles, Liste *monstres);
void Tir_missile(char Tir, cellule **l_missile, int missile, entité *canon);
void gestion_Missile(Liste *missiles, Liste *bouclier);

