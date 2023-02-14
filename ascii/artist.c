#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "conversion.h"

#define DEFAUT_ASCII "CR7.jpg" //Image CR7.jpg traitée par défaut si rien n'est saisit

int main(int argc, char *argv[])
{
  char * filename;
  int colonnes, lignes;

  if (argc == 4)
  {
    filename = argv[1];
    colonnes = atoi(argv[2]);
    lignes = atoi(argv[3]);

    jpg2ascii(filename, colonnes, lignes);
    reverse(colonnes, lignes);
    pixel_art(filename);
  }
  else{

    printf("\nEntrez le nom de l'image (au format .jpg), le nombre de colonnes et le nombre de lignes de votre image\n");
    printf("par exemple : \n--> ./pixel CR7.jpg 60 60 \n--> Ou bien ./pixel marseille.jpg 70 40 \n--> Ou encore ./pixel lion.jpg 40 25\n\n");

    printf("\n\n>>>>>>>>>>>>>>>>>>>> ASCII_ART PAR DEFAUT AVEC L'IMAGE %s <<<<<<<<<<<<<<<<<<<<<\n\n", DEFAUT_ASCII);

    jpg2ascii(DEFAUT_ASCII, 50, 50);
    reverse(50, 50);
    pixel_art(DEFAUT_ASCII);
  }
  return 0;
}
