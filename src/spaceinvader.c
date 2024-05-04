#include "spaceinvader.h"
#include "Graphique/libgraph.h"
#include "variables.h"
#include <SDL/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

cellule *allouer() { return ((cellule *)malloc(sizeof(cellule))); }
void affiche_monstres(Liste ls) {
  cellule *cel = ls;
  while (NULL != cel) {
    afficherLutin((cel->e).num_lutin, (cel->e).X, (cel->e).Y);
    cel = cel->suivant;
  }
}
// fonction qui permet l'ajout en tete d'une entité dans une liste.
void ajout_tete(entité ent, cellule **pL, int x, int y) {
  cellule *p = allouer();
  p->e = ent;
  p->e.X = x;
  p->e.Y = y;
  p->suivant = *pL;
  *pL = p;
}

// Fonction qui permet de supprimer un monstre
void supprime_monstre(Liste *list, int x, int y) {
  if (*list == NULL) {
    return;
  }
  cellule *cel_courant =
      *list; // Définir un pointeur vers la première cellule de la liste
  cellule *cel_prev = NULL; // Définir un pointeur vers la cellule précedente

  while (cel_courant != NULL) {
    if (cel_courant->e.X == x &&
        cel_courant->e.Y ==
            y) // verifier si la cellule contient l'entité à supprimer
    {
      if (cel_prev == NULL) {
        *list = cel_courant->suivant;
      } else {
        cel_prev->suivant = cel_courant->suivant;
      }
      // liberer la memoire de la cellule à supprimer
      free(cel_courant);
      return;
    }
    cel_prev = cel_courant;
    cel_courant = cel_courant->suivant;
  }
}

// deplacement horizontal de monstres
bool bouger_monstre_x(Liste listeMonstres) {
  static int versLaGauche = 1; // variable pour suivre la direction des monstres
  cellule *cel_courant = listeMonstres;
  bool descendre = false;
  while (cel_courant != NULL) {
    entité *monstre = &(cel_courant->e);
    monstre->X += decalage_x_mons * versLaGauche;
    if (monstre->X == 0 || monstre->X > largeur_fen - decalage_x_mons) {
      descendre = true;
    }
    cel_courant = cel_courant->suivant;
  }
  if (descendre) {
    versLaGauche = -versLaGauche;
  }
  return descendre;
}

// deplacement vertical de monstres
void bouger_monstre_y(Liste listeMonstres) {
  cellule *cel_courant = listeMonstres;
  while (cel_courant != NULL) {
    entité *monstre = &(cel_courant->e);
    monstre->Y += decalage_y_mons;

    cel_courant = cel_courant->suivant;
  }
}

// Test des collisions avec les coordonnées x
int collision_X(entité *missile, entité *monstre) {
  int x_missile = missile->X;
  int x_monstre = monstre->X;
  int bord_missile = missile->X + missile_largeur;
  int bord_monstre = monstre->X + monstre_largeur;
  if (x_missile <= bord_monstre && x_monstre <= bord_missile) {
    return 1;
  }
  return 0;
}

// Test des collisions avec les coordonnées y
int collision_Y(entité *missile, entité *monstre) {
  int y_missile = missile->Y;
  int y_monstre = monstre->Y;
  int bas_missile = missile->Y + missile_hauteur;
  int bas_monstre = monstre->Y + monstre_hauteur;
  if (y_missile <= bas_monstre && y_monstre <= bas_missile) {
    return 1;
  }
  return 0;
}

// Fonction pour supprimer le missile et le monstre en cas de collisions
void intersection(Liste *missiles, Liste *monstres) {
  if (*missiles == NULL || *monstres == NULL) {
    return;
  }
  cellule **cel1 = missiles;
  while (*cel1 != NULL) {
    cellule **cel2 = monstres;
    while (*cel2 != NULL && *cel1 != NULL) {
      if (collision_Y(&((*cel1)->e), &((*cel2)->e)) &&
          collision_X(
              &((*cel1)->e),
              &((*cel2)->e))) // teste s'il ya collision d'après le return des
                              // fonctions collision_X et collision_Y
      {
        supprime_monstre(missiles, (*cel1)->e.X, (*cel1)->e.Y);
        supprime_monstre(monstres, (*cel2)->e.X, (*cel2)->e.Y);
        return;
      }
      cel2 = &((*cel2)->suivant);
    }
    cel1 = &((*cel1)->suivant);
  }
}

// fonction permettant de lancer un missile
void Tir_missile(char Tir, cellule **l_missile, int missile, entité *canon) {
  cellule *ptr = *l_missile;
  entité e_missile = {0, 0, missile};
  if (Tir == 't') {
    if (ptr != NULL) {
      if (ptr->e.Y < Y_MAX_Missile) {
        ajout_tete(e_missile, l_missile, canon->X + milieu_canon, canon->Y);
      }
    }
  }

  while (ptr != NULL) {
    ptr->e.Y -= MISSILE_deplacement;
    afficherLutin(missile, ptr->e.X, ptr->e.Y);
    ptr = ptr->suivant;
  }
}

// fonction pour supprimer le missile s'il touche le bouclier
void gestion_Missile(Liste *missiles, Liste *bouclier) {
  cellule **cel1 = missiles;
  while (*cel1 != NULL) {
    cellule **cel2 = bouclier;
    while (*cel2 != NULL && *cel1 != NULL) {
      if (collision_Y(&((*cel1)->e), &((*cel2)->e)) &&
          collision_X(
              &((*cel1)->e),
              &((*cel2)->e))) // teste s'il ya collision d'après le return des
                              // fonctions collision_X et collision_Y
      {
        supprime_monstre(missiles, (*cel1)->e.X, (*cel1)->e.Y);
        // supprime_monstre(monstres, (*cel2)->e.X, (*cel2)->e.Y);
        return;
      }
      cel2 = &((*cel2)->suivant);
    }
    cel1 = &((*cel1)->suivant);
  }
}

int main() {
  creerSurface(largeur_fen, longeur_fen, "Space Invader");
  rectanglePlein(0, 0, largeur_fen, longeur_fen, 1);
  // chargement des images utilisées dans le jeu
  int mons_blanc = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_monstre1_2.bmp",
      COULEUR_NOIR);
  int mons_rose = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_monstre3_1.bmp",
      COULEUR_NOIR);
  int mons_bleu = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_monstre2_1.bmp",
      COULEUR_NOIR);
  int canon = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_canon.bmp",
      COULEUR_NOIR);
  int bouclier = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_bouclier.bmp",
      COULEUR_NOIR);
  int missile = chargerLutin(
      "/home/c/r/crhanim/Projet_Tpi_2/tpi/Lutins/invader_missile.bmp",
      COULEUR_NOIR);
  // les différents listes
  Liste l_monstres = NULL;
  Liste l_bouclier = NULL;
  Liste l_canon = NULL;
  Liste l_missile = NULL;
  entité e1 = {0, 0, mons_blanc};
  entité e2 = {0, 0, mons_rose};
  entité e3 = {0, 0, mons_bleu};
  entité e_canon = {canon_x, canon_y, canon};
  entité e_bouclier = {0, 0, bouclier};
  entité e_missile = {0, 0, missile};
  // ajout des monstres monstres rose
  for (int i = 0; i < Nbr_monstres - 1; i++) {
    ajout_tete(e2, &l_monstres, i * dis_mons_rose, 38);
    ajout_tete(e3, &l_monstres, i * dis_mons_rose, 95);
  }
  // ajout des monstres monstres blanc
  for (int i = 0; i < Nbr_monstres; i++) {
    ajout_tete(e1, &l_monstres, i * dis_mons_blanc, 10);
    ajout_tete(e1, &l_monstres, i * dis_mons_blanc, 66);
  }
  // ajout des boucliers
  for (int i = 1; i < Max_bouclier; i++) {
    ajout_tete(e_bouclier, &l_bouclier, i * dis_bouclier, bouclier_y);
  }
  // ajout du canon, missile
  ajout_tete(e_canon, &l_canon, canon_x, canon_y);
  ajout_tete(e_missile, &l_missile, canon_x + milieu_canon, canon_y);

  while (1) {
    evenement event;
    char touche = 'y';
    lireEvenement(&event, &touche, NULL);
    if (event == toucheBas && touche == 'j') {
      e_canon.X = e_canon.X + deplacement_canon;
    } else if (event == toucheBas && touche == 'f') {
      e_canon.X = e_canon.X - deplacement_canon;
    } else if (event == quitter) {
      break;
    }
    Tir_missile(touche, &l_missile, missile, &e_canon);
    // utilisation de la fonction bouger_monstre_x pour tester si les monstres
    // arrivent au bord de la fenetre il y'aura appel a la fonction
    // bouger_monstre_y
    bool desc = bouger_monstre_x(l_monstres);
    if (desc) {
      bouger_monstre_y(l_monstres);
    }
    rectanglePlein(0, 0, largeur_fen, longeur_fen, COULEUR_NOIR);
    afficherLutin(canon, e_canon.X, e_canon.Y);
    gestion_Missile(&l_missile, &l_bouclier);
    intersection(&l_missile, &l_monstres);
    affiche_monstres(l_monstres);
    affiche_monstres(l_missile);
    affiche_monstres(l_bouclier);

    majSurface();
    SDL_Delay(100);
  }
  return 0;
}
