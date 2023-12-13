#include "../../ez_wav.h"
#include <stdio.h>
#include <math.h>

/// Basic FM

#define RUNTIME     5
#define FS          44100
#define NS          (RUNTIME * FS)
#define SIG_F       500
#define MOD_F       800
#define DEL_F       0.01024

int main(int argc, char **argv){
    int16_t sampled_sin[NS];

    //FILE *fp = fopen("tes.txt", "w");
    double radf = 2.0 * M_PI * (double)SIG_F;
    double sampf = (double)FS;
    for(uint32_t n = 0; n < NS; ++n){
        double modval = DEL_F * sin(2.0 * M_PI * (double)MOD_F * ((double)n/sampf));
        double sigval = sin(radf * ((double)n / sampf) * modval);
        sampled_sin[n] = (int16_t) (((double)(INT16_MAX / 2)) * sigval);
        //fprintf(fp, "%d\n", sampled_sin[n]);
    }
    //fclose(fp);

    struct WAVE_Header myfile;

    output_wav(&myfile, "coolname.wav", &sampled_sin, 16, NS, FS, 1);

    print_wave_header(&myfile);

    

    return 0;
}