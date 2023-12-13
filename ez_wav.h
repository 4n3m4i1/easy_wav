#ifndef EZ_WAV_h
#define EZ_WAV_h

#include <inttypes.h>
#include <stdio.h>

enum AUDIO_FMTS {
    FMT_PCM = 1
};

//#define STD_CHUNKID_        0x52494646 
//#define STD_FMT_            0x57415645 
//#define STD_SUBCHUNK1ID_    0x666D7420 
//#define STD_SUBCHUNK2ID_    0x64617461 


#define STD_CHUNKID_        0x46464952
#define STD_FMT_            0x45564157
#define STD_SUBCHUNK1ID_    0x20746D66
#define STD_SUBCHUNK2ID_    0x61746164


// Data is little endian unless noted
struct WAVE_Header {
    // RIFF Chunk Descriptor
    uint32_t    ChunkID;        // 'RIFF' Big Endian
    uint32_t    ChunkSize;      // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    uint32_t    Format;         // 'WAVE' Big Endian
    
    // Format Sub-Chunk
    uint32_t    Subchunk1ID;    // 'fmt ' Big Endian
    uint32_t    Subchunk1Size;  // 16 for PCM
    uint16_t    AudioFormat;    // 1 == PCM
    uint16_t    NumChannels;    // Well... Yeah
    uint32_t    SampleRate;     // ...
    uint32_t    ByteRate;       // SampleRate * NumChannels * BitsPerSample / 8
    uint16_t    BlockAlign;     // NumChannels * BitsPerSample/8
    uint16_t    BitsPerSample;

    // Data Sub-Chunk
    uint32_t    Subchunk2ID;    // 'Data' Big Endian
    uint32_t    Subchunk2Size;  // NumSamples * NumChannels * BitsPerSample/8
    void        *data;
};


struct WAVE_Data {
    uint16_t    num_channels;
    uint16_t    dlen;
    uint16_t    dsize;
    void        **data_arr;
};

// Assumption made that L R L R interleaving is done for 2 channels
int output_wav(struct WAVE_Header *wav, char *fname, void *data, uint16_t dsize, uint32_t dlen, uint32_t fs, uint16_t channels){
    if((dsize != 8) && (dsize != 16)) return -1;
    if(!data) return -2;

    wav->ChunkID        = STD_CHUNKID_;
    wav->Format         = STD_FMT_;
    wav->Subchunk1ID    = STD_SUBCHUNK1ID_;
    wav->Subchunk2ID    = STD_SUBCHUNK2ID_;

    wav->Subchunk1Size  = 16;

    wav->AudioFormat    = FMT_PCM;

    wav->NumChannels    = channels;
    wav->SampleRate     = fs;
    wav->ByteRate       = fs * channels * (dsize >> 3);
    wav->BlockAlign     = channels * (dsize >> 3);
    wav->BitsPerSample  = dsize;

    wav->Subchunk2Size  = channels * dlen * (dsize >> 3);
    wav->data           = data;
    
    wav->ChunkSize      = 4 + (8 + wav->Subchunk1Size) + (8 + wav->Subchunk2Size);

    FILE *fp;
    fp = fopen(fname, "wb");

    printf("Size of Wave Headr: %u\n", sizeof(struct WAVE_Header));

    fwrite(wav, sizeof(struct WAVE_Header), 1, fp);

    fwrite(data, dlen, (dsize >> 3), fp);
//
    //for(uint32_t n = 0; n < (dlen * channels); ++n){
    //    fwrite(data, dlen, (dsize >> 3), fp);
    //    //for(uint16_t m = 0; m < channels); ++m){
    //    //    fwrite
    //    //}
    //}
//
    fclose(fp);

    return 0;
}


#endif