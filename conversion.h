#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  unsigned char R;
  unsigned char G;
  unsigned char B;
}
pixelRGB;

typedef struct {
  double H;
  double S;
  double L;
}
pixelHSL;

// --------------------------------------------------------
// FUNCTION PROTOTYPES
// --------------------------------------------------------

double calculate_hue(pixelRGB rgb);
double calculate_saturation(pixelRGB rgb, double luminance);
double calculate_luminance(pixelRGB rgb);
unsigned char get_associated_character(double luminance, double hue);
int char2pixel(char c);

double hue2rgb(double v1,double v2,double vH);
void hsl2rgb(double * in, unsigned char * out);
unsigned char * convhsl2rgb(double * hsl, int width, int height);

int jpg2ascii(char * filenamein, int colonnes, int lignes);
int reverse(int colonnes, int lignes);
int pixel_art(char * filename);
