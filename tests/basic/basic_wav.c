#include "../../ez_wav.h"
#include <stdio.h>
#include <math.h>

/// Simple mono sin wave out

#define RUNTIME     5
#define FS          44100
#define NS          (RUNTIME * FS)
#define SIG_F       500

int main(int argc, char **argv){
    int16_t sampled_sin[NS];

    FILE *fp = fopen("tes.txt", "w");
    double radf = 2.0 * M_PI * (double)SIG_F;
    double sampf = (double)FS;
    for(uint32_t n = 0; n < NS; ++n){
        sampled_sin[n] = (int16_t) (((double)(INT16_MAX / 2))*sin(radf * ((double)n / sampf)));
        fprintf(fp, "%d\n", sampled_sin[n]);
    }
    fclose(fp);

    struct WAVE_Header myfile;

    output_wav(&myfile, "coolname.wav", &sampled_sin, 16, NS, FS, 1);

    printf("Chunk ID: 0x%08X\n", myfile.ChunkID);

    

    return 0;
}