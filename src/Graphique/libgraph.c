/**** Bibliotheque graphique ****/

/** Fichiers d'inclusion **/

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "libgraph.h"

/** Types **/

typedef struct
{
    int r, v, b;
} couleur;

/** Constantes **/

#define BITS_PAR_PIXEL 32
#define TAILLE_POLICE 20

static const couleur couleurs[] = { { 255, 255, 255 }, { 0, 0, 0 },   { 255, 0, 0 },
                                    { 0, 255, 0 },     { 0, 0, 255 }, { 255, 105, 180 },
                                    { 150, 150, 150 }, { -1, -1, -1 } };

static const char policeDefaut[]="/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

/** Variables globales **/

static SDL_Surface *surface;
TTF_Font* police;

/** Fonctions **/

void choisirPolice(const char *chemin, int taille){
  police=TTF_OpenFont(chemin, taille);
}

void initialiserTexte() {
  TTF_Init();
  choisirPolice(policeDefaut, TAILLE_POLICE);
}

/* Initialisation de la surface dessinable */
unsigned char creerSurface (int largeur, int hauteur, char *titre)
{
    SDL_Init (SDL_INIT_VIDEO);
    SDL_WM_SetCaption (titre, titre);
    surface = SDL_SetVideoMode (largeur, hauteur, BITS_PAR_PIXEL, SDL_DOUBLEBUF);
    initialiserTexte();
    
    return (surface != NULL && police != NULL);
}

/* Fermeture de la surface dessinable */

void fermerSurface (void)
{
    if (surface != NULL) SDL_FreeSurface (surface);
    if (police != NULL) TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit ();
}

/* Creation d'une couleur */

static int creerCouleur (int ncouleur)
{
    couleur c = couleurs[ncouleur];
    return SDL_MapRGB (surface->format, c.r, c.v, c.b);
}

/* Dessin d'un rectangle plein */

void rectanglePlein (int x, int y, int l, int h, int c)
{
    SDL_Rect rectangle = { x, y, l, h };
    SDL_FillRect (surface, &rectangle, creerCouleur (c));
    // SDL_Flip(surface);
}

/* Manipulation de lutins */

static SDL_Surface *lutins[MAX_LUTINS];
static int          lutins_nb = 0;

int lutinTexte(char* texte, int couleurTexte) {
  couleur c=couleurs[couleurTexte];
  SDL_Color couleur={c.r, c.v, c.b};
  SDL_Surface* lutin=TTF_RenderText_Solid(police, texte, couleur);
  if (lutin != NULL)
    {
      lutins[lutins_nb++] = lutin;
      return lutins_nb - 1;
    }
  return -1;
}

static void configurerLutin (SDL_Surface *lutin, int ncouleur)
{
    couleur c    = couleurs[ncouleur];
    int     fond = SDL_MapRGB (lutin->format, c.r, c.v, c.b);
    SDL_SetColorKey (lutin, SDL_SRCCOLORKEY | SDL_RLEACCEL, fond);
}

int chargerLutin (char *fichier, int couleur)
{
    if (lutins_nb >= MAX_LUTINS) return -2;
    SDL_Surface *lutin = SDL_LoadBMP (fichier);
    if (lutin != NULL)
    {
        lutins[lutins_nb++] = lutin;
        if (couleur >= 0) configurerLutin (lutin, couleur);
        return lutins_nb - 1;
    }
    return -1;
}

void afficherLutin (int lutin, int x, int y)
{
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface (lutins[lutin], NULL, surface, &position);
}

int creerLutin (int x, int y, int largeur, int hauteur, int couleur)
{
    if (lutins_nb >= MAX_LUTINS) return -2;
    int rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    if (couleur < 0) amask = 0x00000000;
    SDL_Surface *lutin =
    SDL_CreateRGBSurface (0, largeur, hauteur, BITS_PAR_PIXEL, rmask, gmask, bmask, amask);
    SDL_Rect fenetre;
    fenetre.x = x;
    fenetre.y = y;
    fenetre.h = hauteur;
    fenetre.w = largeur;
    SDL_BlitSurface (surface, &fenetre, lutin, NULL);
    lutins[lutins_nb++] = lutin;
    if (couleur >= 0) configurerLutin (lutin, couleur);
    return lutins_nb - 1;
}

void tailleLutin (int lutin, int *largeur, int *hauteur)
{
    *largeur = lutins[lutin]->w;
    *hauteur = lutins[lutin]->h;
}

int sauverLutin (int lutin, char *nom) { return SDL_SaveBMP (lutins[lutin], nom); }

/* Manipulation de copie de surface en BMP */

int sauverSurface (char *fichier) { return SDL_SaveBMP (surface, fichier); }

unsigned char chargerSurface (char *fichier)
{
    SDL_Surface *image = SDL_LoadBMP (fichier);
    if (image != NULL)
    {
        SDL_BlitSurface (image, NULL, surface, NULL);
        SDL_Flip (surface);
    }
    return (image != NULL);
}

void majSurface (void) { SDL_Flip (surface); }

/* Trouver la couleur d'un pixel */

int couleurPixel (int x, int y)
{
    int     bpp = surface->format->BytesPerPixel;
    Uint32 *p   = (Uint32 *)(surface->pixels + y * surface->pitch + x * bpp);
    Uint8   r, v, b;
    SDL_GetRGB (*p, surface->format, &r, &v, &b);
    int i = 0;
    while (1)
    {
        if (couleurs[i].r < 0) break;
        if (r == couleurs[i].r && v == couleurs[i].v && b == couleurs[i].b) break;
        i++;
    }
    if (couleurs[i].r < 0)
        return -1;
    else
        return i;
}

/* Fonction de traitement des événements */

void lireEvenement (evenement *evt, char *touche, void **detail)
{
    static SDL_keysym _detail;
    SDL_Event         event;
    while (SDL_PollEvent (&event))
    {
        if (event.type == SDL_QUIT) *evt = quitter;
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            *evt      = (event.type == SDL_KEYDOWN) ? toucheBas : toucheHaut;
            char *nom = SDL_GetKeyName (event.key.keysym.sym);
            if (strlen (nom) == 1 && nom[0] >= 32 && nom[0] < 128)
                *touche = nom[0];
            else
                *touche = 0;
            if (detail != NULL)
            {
                _detail = event.key.keysym;
                *detail = &_detail;
            }
	    break;
        }
    }
}

void attendreEvenement (void)
{
    SDL_Event event;
    while (SDL_WaitEvent (&event)) switch (event.type)
        {
        case SDL_QUIT:
            exit (0);
        case SDL_KEYDOWN:
        case SDL_MOUSEBUTTONDOWN:
            return;
        }
}
