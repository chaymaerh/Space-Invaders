/**** Bibliotheque graphique (definitions) ****/

/** Constantes **/

#define COULEUR_BLANC 0
#define COULEUR_NOIR 1
#define COULEUR_ROUGE 2
#define COULEUR_VERT 3
#define COULEUR_BLEU 4
#define COULEUR_ROSE 5
#define COULEUR_GRIS 6

#define MAX_LUTINS 16

typedef enum {toucheBas, toucheHaut, quitter} evenement;

/** Prototypes **/

/**
 * @brief cree une fenetre 2D
 *
 * @param largeur en pixels de la fenetre
 * @param hauteur en pixels de la fenetre
 * @param titre   de la fenetre (chaine de caractere)
 */
unsigned char creerSurface (int largeur, int hauteur, char *titre);

/**
 * @brief permet de charger un fichier image au format bmp (bitmap)
 *
 * @param fichier nom du fichier
 */
unsigned char chargerSurface (char *fichier);


/**
 * @brief permet de sauvegarder une surface en image (format bmp)
 *
 * @param fichier   nom du fichier
 * @return          0 si OK, valeur negative sinon
 */
int sauverSurface (char *fichier);

/**
 * @brief met a jour la surface d'affichage
 */
void majSurface (void);


/**
 * @brief libere la surface d'affichage
 *        a faire lors de la fermeture
 *        du programme
 */
void fermerSurface (void);

/**
 * @brief choisit la police de caractères à utiliser pour afficher du texte
 * @param chemin nom du fichier de police (format .ttf, voir /usr/share/fonts/truetype)
 * @param taille taille de la police
 */
void choisirPolice(const char *chemin, int taille);

/**
 * @brief   dessine un rectange de taille (l,h) aux coordonnêes
 *          (x,y) et de couleur c
 *
 * @param x 0 <= x <= l_surface
 * @param y 0 <= y <= h_surface
 * @param l largeur en pixels
 * @param h longueur en pixels
 * @param c indice de couleur voir variable couleurs dans le fichier .c
 */
void rectanglePlein (int x, int y, int l, int h, int c);


/**
 * @brief permet de determiner l'indice du tableau de couleur du
 *        pixel aux coordonnees (x,y)
 *
 * @param x 0 <= x <= l_surface
 * @param y 0 <= y <= h_surface
 * @return indice de couleur voire variable couleurs dans le fichier .c
 */
int couleurPixel (int x, int y);

/**
 * @brief crée un lutin à partir d'un texte
 *
 * @param texte le texte
 * @param couleur indice de couleur du texte
 * @return numero de lutin dans le tableau dynamique de lutin (< MAX_LUTINS)
 */
int lutinTexte(char *texte, int couleur);

/**
 * @brief charge un lutin à partir du fichier
 *
 * @param fichier image bitmap du lutin à charger
 * @param couleur indice de couleurs à charger
 * @return numero de lutin dans le tableau dynamique de lutin (< MAX_LUTINS)
 */
int chargerLutin (char *fichier, int couleur);

/**
 * @brief afficher un lutin aux coordonnées (x,y)
 *
 * @param lutin numero du lutin à afficher (< MAX_LUTINS)
 * @param x     abscisse de départ
 * @param y     ordonnée de départ
 */
void afficherLutin (int lutin, int x, int y);

/**
 * @brief creer un lutin de taille (l,h) aux coordonnées (x,y)
 *
 * @param x         abscisse de départ
 * @param y         ordonnée de départ
 * @param largeur   largeur du lutin
 * @param hauteur   hauteur du lutin
 * @param couleur   indice de couleur à partir du tableau _couleurs_
 * @return          indice du lutin dans le tableau global (< MAX_LUTINS)
 */
int creerLutin (int x, int y, int largeur, int hauteur, int couleur);

/**
 * @brief       sauvegarde un lutin dans un fichier
 *
 * @param lutin numero de lutin à sauvegarder (< MAX_LUTINS)
 * @param nom   fichier pour la sauvegarde
 * @return      0 si OK valeur négative sinon
 */
int sauverLutin (int lutin, char *nom);

/**
 * @brief calcule la taille (largeur,hauteur) d'un lutin
 *
 * @param lutin     index du lutin (< MAX_LUTINS)
 * @param largeur   pointeur sur la largeur
 * @param hauteur   pointeur sur la hauteur
 */
void tailleLutin (int lutin, int *largeur, int *hauteur);

/**
 * @brief lire une touche au clavier
 *
 * @param evt    pointeur sur evenement
 * @param touche pointeur sur la touche pressée
 * @param detail NULL ou keysim
 */
void lireEvenement (evenement *evt, char *touche, void **detail);
// detail = NULL 
//a chaque fois que j'appuie ca me donne la touche et haut 
/**
 * @brief attente d'un evenement bouton, souris, fin de programme
 */
void attendreEvenement (void);
