#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include  <math.h>
#include <turbojpeg.h>
#include <sys/stat.h>

#include "conversion.h"

#define AOUTFILENAME "ascii_art.txt"
#define ROUTFILENAME "reverse.jpg"
#define JCOMPONENTS 3
#define JQUALITY 90

/**
*Fonction char2pixel
*@param c un char
*Renvoie l'entier correspondant à la luminance (clair ou sombre) du caractere c
*/
int char2pixel(char c)
{

double luminance = 0.0;
  switch (c) {
  	case '.':
  	case '~':
  	case '`':
  	case ',':
  	case '^':
  	case ':':
  		luminance = 0.07;
  		break;
  	case '-':
  	case '_':
  	case '+':
  	case '<':
  	case 'i':
  	case '?':
  		luminance = 0.22;
  		break;
  	case '/':
  	case '|':
  	case '(':
  	case ')':
  	case 'l':
  	case ']':
  		luminance = 0.37;
  		break;
  	case 'r':
  	case 'c':
  	case 'v':
  	case 'u':
  	case 'n':
  	case 'x':
  		luminance = 0.53;
  		break;
    case 'L':
    case 'C':
    case 'J':
    case 'U':
    case 'Y':
    case 'X':
    		luminance = 0.69;
    		break;
  	case 'o':
  	case 'a':
  	case 'h':
  	case 'k':
  	case 'b':
  	case 'd':
  		luminance = 0.83;
  		break;
  	case '*':
  	case '&':
  	case '%':
  	case '$':
  	case '#':
  	case '@':
  		luminance = 0.98;
  		break;
  	default:
  		luminance = 0.5;
  		break;

  }
return ceil(255*luminance);
}

/**
*Fonction reverse
*@param colonnes un entier strictement positif
@param lignes un entier strictement positif
*retourne 1 si l'image "reverse.jpg" (ROUTFILENAME) en noir et blanc est crée avec succes,
* -1 sinon
*
*l'image produite est de taille colonnes x lignes à partir des caractères
*présents dans le fichier texte "ascii_art.txt" (AOUTFILENAME)
*/
int reverse(int colonnes, int lignes)
{

printf("\n*********************************** REVERSE ************************************\n");

FILE *f_in;
int c;
if ((f_in = fopen(AOUTFILENAME,"r")) == NULL)
{
fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n", AOUTFILENAME);
return(EXIT_FAILURE);
}

long unsigned int jsize ;
unsigned char *jbuffer, *srcbuffer ;
int tjsamp, tjstatus ;
tjhandle handle;

int size = colonnes*lignes;
srcbuffer = malloc(size*JCOMPONENTS) ;

int i = 0;
while((c = fgetc(f_in)) != EOF)
{
    if(c == '\n') --i;
		int px = char2pixel(c);
    for(int a = 0; a < JCOMPONENTS; ++a)
    {
      srcbuffer[i*JCOMPONENTS + a] = px;
    }
		++i;
}

handle = tjInitCompress() ;   // check for non NULL
tjstatus = tjCompress2 (handle, srcbuffer, colonnes, 0, lignes, TJPF_RGB , &jbuffer, &jsize, TJSAMP_444, JQUALITY, 0) ;
tjDestroy (handle) ;
printf ("tjstatus is %d.\n", tjstatus) ;
printf("jsize is %ld \n", jsize);

FILE *outfile = fopen (ROUTFILENAME, "wb") ;
fwrite (jbuffer, jsize, 1, outfile) ;
printf("\n>>>>>>>>>>>>>>>> IMAGE REVERSE DANS LE FICHIER %s CREEE AVEC SUCCES ! <<<<<<<<<<<<<<< \n", ROUTFILENAME);
fclose (outfile) ;

return 1;
}
