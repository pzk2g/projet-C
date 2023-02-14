#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include  <math.h>
#include <turbojpeg.h>
#include <sys/stat.h>

#include "conversion.h"

#define INFILENAME "CR7.jpg"
#define JCOMPONENTS 3

/*
 Fonctions pour passer d'un buffer HSL à un buffer RGB
 */
#define R(buf) *(buf)
#define G(buf) *((buf)+1)
#define B(buf) *((buf)+2)

#define H(buf) *(buf)
#define S(buf) *((buf)+1)
#define L(buf) *((buf)+2)


/* Fonction générique de conversion hsl to rgb */
double hue2rgb(double v1,double v2,double vH)
{
    if (vH<0) vH+=1;
    if (vH>1) vH-=1;
    if ((6*vH)<1) return (v1+(v2-v1)*6*vH);
    if ((2*vH)<1) return (v2);
    if ((3*vH)<2) return (v1+(v2-v1)*((2./3)-vH)*6);
    return(v1);
}

/* Fonction qui convertie le triplet hsl pointé par in en son triplé rvb correspondant
 pointé par out.
 */
void hsl2rgb(double * in, unsigned char * out)
{
    double x,y;
    if (S(in)==0) {               // On est en monochrome
        R(out)=L(in)*255;
        G(out)=L(in)*255;
        B(out)=L(in)*255;
        return;
    }
    if (L(in)<0.5)
        y = L(in)*(1+S(in));
    else y=(L(in)+S(in))-(S(in)*L(in));
    x=2*L(in)-y;

    R(out)=255*hue2rgb(x,y,H(in)+1./3);
    G(out)=255*hue2rgb(x,y,H(in));
    B(out)=255*hue2rgb(x,y,H(in)-1./3);
}

/* fonction qui prend un buffer image HSL et retourne un buffer image RVB où
 chaque pixel est encodé par trois unsigned char */
unsigned char * convhsl2rgb(double * hsl, int width, int height)
{
    int i,j;
    unsigned char *rvb=(unsigned char *)malloc(height*width*3*sizeof(char));

    for (i=0;i<height;i++)
        for (j=0;j<width;j++)
            hsl2rgb(hsl+(i*width*3)+j*3,rvb+(i*width*3)+j*3);
    return rvb;
}

int pixel_art(char * filename) {

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PIXEL-ART ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    long unsigned int jsize;
    FILE * infile;
    unsigned char * jbuffer, * srcbuffer;
    int jwidth, jheight, tjsamp, tjstatus;
    tjhandle handle;

    /// Open infile and get size
    if (filename == NULL) {
        infile = fopen(INFILENAME, "rb");
    } else {
        infile = fopen(filename, "rb");
    }
    if (!infile) printf("Error opening infile.\n");
    // if ((fseek (infile, 0, SEEK_END) < 0)  ||  (jsize = ftell(infile) < 0)  ||  (fseek (infile, 0, SEEK_SET) < 0))
    // printf ("Error with infile size.\n") ;
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
    if(handle == NULL)
    {
        printf("Erreur de décompression du buffer.\n");
        exit(EXIT_FAILURE);
    }
    tjDecompressHeader2(handle, jbuffer, jsize, & jwidth, & jheight, & tjsamp);
    printf("format is %dx%d, tjsamp is %d.\n", jwidth, jheight, tjsamp);
    srcbuffer = malloc(jwidth * jheight * JCOMPONENTS);
    tjstatus = tjDecompress2(handle, jbuffer, jsize, srcbuffer, jwidth, 0, jheight, TJPF_RGB, TJFLAG_FASTDCT);
    tjDestroy(handle);
    printf("tjstatus is %d.\n", tjstatus);

    //Taille de la sortie à l'écran
    int C_W = 80;
    int C_H =25;
//    if (argc == 4)
//    {
//        C_W = atoi(argv[2]);
//        C_H = atoi(argv[3]);
//    }
//    else{
//        printf("Entrer le nombre de caractères et le nombre de lignes de votre image\n");
//    }
    int sub_width = jwidth / C_W;
    int sub_height = jheight / C_H;

    int im_size = sub_width * sub_height;
    pixelRGB rgb;
    pixelHSL hsl;
    double h, s, l;

    double **pixelhsl = malloc(C_W*JCOMPONENTS*sizeof(*pixelhsl));
    for( int i = 0; i < C_W; i++) {
        pixelhsl[i] = malloc(JCOMPONENTS*C_H*sizeof(double));
    }

    printf("%d  %d  %d   %d \n", jwidth, jheight, sub_width, sub_height);

    //#####################   PIXEL ART   #####################//

    printf("\n\n");

    unsigned char ***moyenneRGB = NULL;
    moyenneRGB = (unsigned char***)malloc(JCOMPONENTS*C_W*sizeof(**moyenneRGB));

    if(moyenneRGB == NULL)
    {
        printf("Echec de l'allocation 1");
        exit(EXIT_FAILURE);
    }
    for( int i = 0; i < C_W; i++) {
        moyenneRGB[i] = (unsigned char**)malloc(JCOMPONENTS*C_H*sizeof(unsigned char*));
    }
    for( int i = 0; i < C_W; i++) {
        if(moyenneRGB[i] == NULL) {
            printf("Echec de l'allocation 2");
            exit(EXIT_FAILURE);
        }
    }

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

                    //Conversion RGBtoHSL

                    hsl.H = calculate_hue(rgb);
                    hsl.L = calculate_luminance(rgb);
                    hsl.S = calculate_saturation(rgb, hsl.L);

                    // faire la somme des valeurs HSL d'une partie en vue de calculer la moyenne
                    grandHue += hsl.H;
                    grandSaturation += hsl.S;
                    grandLuminance += hsl.L;
                }
            }
            // calcul de la moyenne et stockage
            h = grandHue / im_size;
            s = grandSaturation / im_size;
            l = grandLuminance / im_size;

            //#####################   PIXEL ART   #####################//

            double *hsl1 = NULL;
            hsl1 = malloc(JCOMPONENTS*sizeof(double));
            if(hsl1 == NULL)
            {
                printf("Echec de l'allocation 3");
                exit(EXIT_FAILURE);
            }
            hsl1[0] = h;
            hsl1[1] = s;
            hsl1[2] = l;

            moyenneRGB[ i/sub_width ][ j/sub_height ] = convhsl2rgb(hsl1,sub_width,sub_height);
            free(hsl1);
        }
    }

    printf("\n\n");

    //Ensuite on affiche, il faut bien saisir C_W = 80 et C_H = 25 quand on lance l'exécution pour que ça s'affiche correctement sur un terminal de 80x25

    for(int i = 0; i < C_W; i++) {
        for(int j = 0; j < C_H; j++) {
            printf("\033[48;5;%dm \033[m",(moyenneRGB[i][j][0]+moyenneRGB[i][j][1]+moyenneRGB[i][j][2])/3);
        }
        printf("\n");
    }
    return 0;
}
