#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "time.h"
 
 
 
/*importacion de datos*/
void load_data(float *velocidad, float *radio, char *archivo, int lineas)
{
 
    char buffer[100];
     
 
    FILE *entrada = fopen(archivo, "r");
 
    // skip header
    fgets(buffer, 100, entrada);
 
    for (int i = 0; i < lineas; ++i)
    {
        fscanf(entrada, "%f %f\n", &radio[i], &velocidad[i]);
    }
 
    fclose(entrada);
}
 
 
 
 
void imprimir_datos(int k, float Mbi, float Mdi, float Mhi)
{
    printf("%d\t%f\t%f\t%f\n", k, Mbi, Mdi, Mhi);
}
 
 
 
 
void mcmc(int iteraciones, float *Mb, float *Md, float *Mh, float *like, float *velocidad, float *radio, int lineas)
{
	/*declaracion de constantes*/
    float v_ecuacion;
    float bb;
    float bd;
    float ad;
    float ah;
    float numero_aleatorio;
    float a;
 	/*diatncias*/
    float Mb_prime;
    float Md_prime;
    float Mh_prime;
    float like_prime;
 
    a = 10;
 
     
    bb = 0.2497;
    bd = 5.16;
    ad = 0.3105;
    ah = 64.3;
 
    Mb[0] = 1000;
    Md[0] = 1000;
    Mh[0] = 1000;
 
 
     
    like[0] = 0;
    v_ecuacion = 0;
 
    for (int i = 0; i < lineas; ++i)
    {
         
        v_ecuacion = pow(Mb[0],0.5)*radio[i]/(pow(radio[i]*radio[i]+bb*bb,0.75));
        v_ecuacion = v_ecuacion + pow(Md[0],0.5)*radio[i]/(pow(radio[i]*radio[i] + (bd+ad)*(bd+ad),0.75));
        v_ecuacion = v_ecuacion + pow(Mh[0],0.5)/(pow(radio[i]*radio[i]+ah*ah,0.25));
        like[0] = like[0] + pow(fabs(velocidad[i]-v_ecuacion),2);
    }
    //like[0] = exp(-0.1*like[0]);
 
    printf("\n\nIteracion \tMb \t Md \t Mh\n");
    imprimir_datos(0, Mb[0], Md[0], Mh[0]);
 
 /*Metodo MOnte Carlo*/
 
    for (int k = 1; k < iteraciones; ++k)
    {
        // Calcular los numeros aleatorios como se sugiere en
        // https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
        numero_aleatorio = (float)rand()/(float)(RAND_MAX/a);
        numero_aleatorio = 2*numero_aleatorio - a;
        Mb_prime = Mb[k-1] + numero_aleatorio;
 
        numero_aleatorio = (float)rand()/(float)(RAND_MAX/a);
        numero_aleatorio = 2*numero_aleatorio - a;
        Md_prime = Md[k-1] + numero_aleatorio;
 
        numero_aleatorio = (float)rand()/(float)(RAND_MAX/a);
        numero_aleatorio = 2*numero_aleatorio - a;
        Mh_prime = Mh[k-1] + numero_aleatorio;
 
 
         
        like_prime = 0;
        v_ecuacion = 0;
        for (int i = 0; i < lineas; ++i)
        {
             
            v_ecuacion = pow(Mb_prime,0.5)*radio[i]/(pow(radio[i]*radio[i]+bb*bb,0.75));
            v_ecuacion = v_ecuacion + pow(Md_prime,0.5)*radio[i]/(pow(radio[i]*radio[i] + (bd+ad)*(bd+ad),0.75));
            v_ecuacion = v_ecuacion + pow(Mh_prime, 0.5)/(pow(radio[i]*radio[i]+ah*ah,0.25));
            like_prime = like_prime + pow(fabs(velocidad[i]-v_ecuacion),2);
        }
 
        //like[k] = exp(-0.1*like[k]);
 
 
        if (like_prime/like[k-1] < 1.0)
        {
            // Si likelihood mejora, aceptar nuevos valores
            Mb[k] = Mb_prime;
            Md[k] = Md_prime;
            Mh[k] = Mh_prime;
            like[k] = like_prime;
        }else{
            numero_aleatorio = (float)rand()/(float)(RAND_MAX/1.0);
            if (numero_aleatorio > like_prime/like[k-1])
            {
                Mb[k] = Mb_prime;
                Md[k] = Md_prime;
                Mh[k] = Mh_prime;
                like[k] = like_prime;
            }else{
                Mb[k] = Mb[k-1];
                Md[k] = Md[k-1];
                Mh[k] = Mh[k-1];
                like[k] = like[k-1];
            }
        }
 
         
 
        imprimir_datos(k, Mb[k], Md[k], Mh[k]);
    }
 
}
 
 
 
 
 
int main()
{
    int lineas;
    int iteraciones;
    float *velocidad;
    float *radio;
    float *Mb;
    float *Md;
    float *Mh;
    float *like;
 
    lineas = 300;
    iteraciones = 30000;
 
    velocidad = malloc(lineas*sizeof(float));
    radio = malloc(lineas*sizeof(float));
    Mb = malloc(iteraciones*sizeof(float));
    Md = malloc(iteraciones*sizeof(float));
    Mh = malloc(iteraciones*sizeof(float));
    like = malloc(iteraciones*sizeof(float));
 
     
    printf("\nCargando datos ...... ok\n");
    load_data(velocidad, radio, "RadialVelocities.dat", lineas);
    printf("\nDatos Cargados ...... ok\n");
 
    srand(time(NULL));
    printf("\nSolucionando con MCMC ...... ok\n");  
    mcmc(iteraciones, Mb, Md, Mh, like, velocidad, radio, lineas);
 
    return 0;
}
