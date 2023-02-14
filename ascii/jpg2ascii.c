#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include  <math.h>
#include <turbojpeg.h>
#include <sys/stat.h>

#include "conversion.h"

#define AOUTFILENAME "ascii_art.txt" //Nom du fichier texte où est enregistré l'ascii-art
#define JCOMPONENTS 3

/**
*fonction calculate_hue
*@param rgb un pixelRGB
*renvoie un double correspondant à la couleur moyenne de rgb
*/
double calculate_hue(pixelRGB rgb) {
  double valMax = 0.0;
  double valMin = 0.0;

  double dR;
  double dG;
  double dB;

  dR = (double) rgb.R / 255.0;
  dG = (double) rgb.G / 255.0;
  dB = (double) rgb.B / 255.0;

  //trouver le maximum
  if (dR >= dG && dR >= dB)
    valMax = dR;
  else if (dG >= dB && dG >= dR)
    valMax = dG;
  else if (dB >= dG && dB >= dR)
    valMax = dB;

  //trouver le minimum
  if (dR <= dG && dR <= dB)
    valMin = dR;
  else if (dG <= dB && dG <= dR)
    valMin = dG;
  else if (dB <= dG && dB <= dR)
    valMin = dB;

  double hue;
  if (valMax == valMin) {
    hue = 0.0;
  } else {
    if (valMax == dR) {
      hue = (dG - dB) / (valMax - valMin);
    } else if (valMax == dG) {
      hue = 2.0 + (dB - dR) / (valMax - valMin);
    } else if (valMax == dB) {
      hue = 4.0 + (dR - dG) / (valMax - valMin);
    }
    hue = hue / 6;

  }
  return hue;
}

/**
*fonction calculate_luminance
*@param rgb un pixelRGB
*renvoie un double correspondant à la luminance de rgb
*/
double calculate_luminance(pixelRGB rgb) {
  double valMax = 0.0;
  double valMin = 0.0;

  double dR;
  double dG;
  double dB;

  dR = (double) rgb.R / 255.0;
  dG = (double) rgb.G / 255.0;
  dB = (double) rgb.B / 255.0;

  if (dR >= dG && dR >= dB)
    valMax = dR;
  else if (dG >= dB && dG >= dR)
    valMax = dG;
  else if (dB >= dG && dB >= dR)
    valMax = dB;

  if (dR <= dG && dR <= dB)
    valMin = dR;
  else if (dG <= dB && dG <= dR)
    valMin = dG;
  else if (dB <= dG && dB <= dR)
    valMin = dB;

  double luminance = (valMin + valMax) / 2.0;

  return luminance;
}

/**
*fonction calculate_saturation
*@param rgb un pixelRGB, luminance un double
*renvoie un double correspondant à la saturation de rgb et la luminance de rgb
*/
double calculate_saturation(pixelRGB rgb, double luminance) {
  double valMax = 0.0;
  double valMin = 0.0;

  double dR;
  double dG;
  double dB;

  dR = (double) rgb.R / 255.0;
  dG = (double) rgb.G / 255.0;
  dB = (double) rgb.B / 255.0;

  if (dR >= dG && dR >= dB)
    valMax = dR;
  else if (dG >= dB && dG >= dR)
    valMax = dG;
  else if (dB >= dG && dB >= dR)
    valMax = dB;

  if (dR <= dG && dR <= dB)
    valMin = dR;
  else if (dG <= dB && dG <= dR)
    valMin = dG;
  else if (dB <= dG && dB <= dR)
    valMin = dB;

  double saturation;

  if (valMax == valMin) {
    saturation = 0.0;
  } else {
    if (luminance < 0.5) {
      saturation = (valMax - valMin) / (valMax + valMin);
    } else {
      saturation = (valMax - valMin) / (2.0 - valMax - valMin);
    }
  }

  return saturation;
}



//Tableau des caractères prédéfini donné dans le projet
unsigned char caractere[6][7] =
                      {
                        {'.', '-', '/', 'r', 'L', 'o', '*'},
                        {'~', '_', '|', 'c', 'C', 'a', '&'},
                        {'`', '+', '(', 'v', 'J', 'h', '%'},
                        {',', '<', ')', 'u', 'U', 'k', '$'},
                        {'^', 'i', 'l', 'n', 'Y', 'b', '#'},
                        {':', '?', ']', 'x', 'X', 'd', '@'}
                      };

/**
*fonction get_associated_character
*@param l un double
*@param h un double
*retourne le caractere correspondant dans le tableau caractères
*en fonction de la luminance l et de la hue h du pixel de l'image
*/
unsigned char get_associated_character(double l, double h)
{
  unsigned char c;
  if ((h >= 0 && h <= 0.17) && (l >= 0 && l <= 0.15)) {
    c = caractere[0][0];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.15 && l <= 0.30)) {
    c = caractere[0][1];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.30 && l <= 0.45)) {
    c = caractere[0][2];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.45 && l <= 0.60)) {
    c = caractere[0][3];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.60 && l <= 0.75)) {
    c = caractere[0][4];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.75 && l <= 0.90)) {
    c = caractere[0][5];
  }
  if ((h >= 0 && h <= 0.17) && (l > 0.90 && l <= 1)) {
    c = caractere[0][6];
  }

  //Tableau deuxieme ligne

  if ((h > 0.17 && h <= 0.34) && (l >= 0 && l <= 0.15)) {
    c  = caractere[1][0];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.15 && l <= 0.30)) {
     c = caractere[1][1];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.30 && l <= 0.45)) {
     c = caractere[1][2];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.45 && l <= 0.60)) {
     c = caractere[1][3];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.60 && l <= 0.75)) {
     c = caractere[1][4];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.75 && l <= 0.90)) {
     c = caractere[1][5];
  }
  if ((h > 0.17 && h <= 0.34) && (l > 0.90 && l <= 1)) {
    c = caractere[1][6];
  }

  //Tableau troisieme ligne

  if ((h > 0.34 && h <= 0.51) && (l >= 0 && l <= 0.15)) {
    c = caractere[2][0];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.15 && l <= 0.30)) {
    c = caractere[2][1];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.30 && l <= 0.45)) {
    c = caractere[2][2];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.45 && l <= 0.60)) {
    c = caractere[2][3];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.60 && l <= 0.75)) {
    c = caractere[2][4];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.75 && l <= 0.90)) {
    c = caractere[2][5];
  }
  if ((h > 0.34 && h <= 0.51) && (l > 0.90 && l <= 1)) {
    c = caractere[2][6];
  }

  //Tableau quatrieme ligne

  if ((h > 0.51 && h <= 0.68) && (l >= 0 && l <= 0.15)) {
    c = caractere[3][0];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.15 && l <= 0.30)) {
    c = caractere[3][1];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.30 && l <= 0.45)) {
    c = caractere[3][2];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.45 && l <= 0.60)) {
    c = caractere[3][3];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.60 && l <= 0.75)) {
    c = caractere[3][4];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.75 && l <= 0.90)) {
    c = caractere[3][5];
  }
  if ((h > 0.51 && h <= 0.68) && (l > 0.90 && l <= 1)) {
    c = caractere[3][6];
  }

  //Tableau cinquieme ligne

  if ((h > 0.68 && h <= 0.85) && (l >= 0 && l <= 0.15)) {
    c = caractere[4][0];
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.15 && l <= 0.30)) {
    c = caractere[4][1];
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.30 && l <= 0.45)) {
    c = caractere[4][2];;
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.45 && l <= 0.60)) {
    c = caractere[4][3];
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.60 && l <= 0.75)) {
    c = caractere[4][4];
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.75 && l <= 0.90)) {
    c = caractere[4][5];
  }
  if ((h > 0.68 && h <= 0.85) && (l > 0.90 && l <= 1)) {
    c = caractere[4][6];
  }

  //Tableau sixieme ligne

  if ((h > 0.85 && h <= 1) && (l >= 0 && l <= 0.15)) {
    c = caractere[5][0];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.15 && l <= 0.30)) {
    c = caractere[5][1];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.30 && l <= 0.45)) {
    c = caractere[5][2];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.45 && l <= 0.60)) {
    c = caractere[5][3];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.60 && l <= 0.75)) {
    c = caractere[5][4];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.75 && l <= 0.90)) {
    c = caractere[5][5];
  }
  if ((h > 0.85 && h <= 1) && (l > 0.90 && l <= 1)) {
    c = caractere[5][6];
  }
  return c;
}

/**
*fonction jpg2ascii
*@param filename un pointeur sur le nom du fichier(image) à convertir en ascii-art
*@param colonnes un entier strictement positif
*@param lignes un entier strictement positif
*retourne 1 si l'ascii-art de colonnes*lignes caractères est crée avec succes -1 sinon
*
*l'ascii-art crée est affiché dans le terminal crée
*et enregistré dans le fichier texte "ascii_art.txt"(AOUTFILENAME)
*/
int jpg2ascii(char * filenamein, int colonnes, int lignes)
{

  printf("\n************************************** ASCII-ART *******************************\n");

  long unsigned int jsize;
  FILE * infile;
  unsigned char * jbuffer, * srcbuffer;
  int jwidth, jheight, tjsamp, tjstatus;
  tjhandle handle;

  /// Open infile and get size

  infile = fopen(filenamein, "rb");
  if (!infile)
  {
    printf("Error opening infile. le fichier %s n'existe pas OU au n'est pas au format .jpg\n", filenamein);
    return -1;
  }


  struct stat st;
  fstat(fileno(infile), & st);
  jsize = st.st_size;
  printf("jsize is %ld.\n", jsize);

  // Get content in jbuffer
  jbuffer = tjAlloc(jsize);
  fread(jbuffer, jsize, 1, infile);
  fclose(infile);

  // Decompress in srcbuffer
  handle = tjInitDecompress(); // check for non NULL
  tjDecompressHeader2(handle, jbuffer, jsize, & jwidth, & jheight, & tjsamp);
  printf("format is %dx%d, tjsamp is %d.\n", jwidth, jheight, tjsamp);
  srcbuffer = malloc(jwidth * jheight * JCOMPONENTS);
  tjstatus = tjDecompress2(handle, jbuffer, jsize, srcbuffer, jwidth, 0, jheight, TJPF_RGB, TJFLAG_FASTDCT);
  tjDestroy(handle);
  printf("tjstatus is %d.\n", tjstatus);

  int sub_width = jwidth / colonnes;
  int sub_height = jheight / lignes;

  int im_size = sub_width * sub_height;
  pixelRGB rgb;
  pixelHSL hsl;
  double h, s, l;
  unsigned char tab_caractere[colonnes][lignes];
  double pixelhsl[colonnes][lignes][JCOMPONENTS];

  printf("%d  %d  %d   %d \n", jwidth, jheight, sub_width, sub_height);

  // Recupération d'une partie de l'image
  for (int i = 0; i < jwidth; i = i + sub_width) {
    for (int j = 0; j < jheight; j = j + sub_height) {

      double grandHue = 0;
      double grandSaturation = 0.0;
      double grandLuminance = 0.0;

      for (int x = i; x < (i + sub_width); ++x) {
        for (int y = j; y < (j + sub_height); ++y) {

          rgb.R = srcbuffer[0 + 3 * x + 3 * jwidth * y];
          rgb.G = srcbuffer[1 + 3 * x + 3 * jwidth * y];
          rgb.B = srcbuffer[2 + 3 * x + 3 * jwidth * y];

          //Converson RGBtoHSL
          hsl.H = calculate_hue(rgb);
          hsl.L = calculate_luminance(rgb);
          hsl.S = calculate_saturation(rgb, hsl.L);

          // faire la somme des valeurs HSL d'une partien envue de calculer la moy
          grandHue += hsl.H;
          grandSaturation += hsl.S;
          grandLuminance += hsl.L;
        }
      }

      // calcul de la moyenne et stockage
      h = grandHue / im_size;
      s = grandSaturation / im_size;
      l = grandLuminance / im_size;
      tab_caractere[i / sub_width][j / sub_height] = get_associated_character(l, h);
    }
  }

  for (int i = 0; i < lignes; ++i) {
    for (int j = 0; j < colonnes; ++j){
      printf("%c ", tab_caractere[j][i]);
    }
    printf("\n");
  }

  //Ecriture dans le fichier ascii_art.txt
  FILE * f_out;
  f_out = fopen(AOUTFILENAME, "wb");
  if (f_out != NULL) {
    for (int i = 0; i < lignes; ++i) {
      for (int j = 0; j < colonnes; ++j) {
        fprintf(f_out, "%c", tab_caractere[j][i]);
      }
      fprintf(f_out, "\n");
    }
  }
  printf("\n>>>>>>>>>>>>>>> IMAGE ASCII_ART CREEE AVEC SUCCES DANS LE FICHIER %s.<<<<<<<<<<\n", AOUTFILENAME);
  fclose(f_out);

  return 1;
}
