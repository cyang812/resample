#include "speex_resampler.h"
#include <stdio.h>

#define COUNT_OF(x)   sizeof(x)/sizeof(*x)

#define IN_FILE  "E:\\project\\resample\\codecs\\test_file\\24k_1.wav"
#define OUT_FILE "E:\\project\\resample\\codecs\\test_file\\48k.wav"

FILE *in_file, *out_file;

short in_buf[2304];
short out_buf[2304*4];

int main()
{
    in_file = fopen(IN_FILE, "rb");
    out_file = fopen(OUT_FILE, "wb");

    if(!in_file || !out_file)
    {
        printf("open file err!\n");
    }

    int err = 0;
    SpeexResamplerState *st = NULL;

    st = speex_resampler_init(2, 24000, 48000, 0, &err);
    printf("init ret: %s\n", speex_resampler_strerror(err));

    int in_rate, out_rate;
    speex_resampler_get_rate(st, &in_rate, &out_rate);
    printf("in_rate = %d, out_rate = %d\n", in_rate, out_rate);

    while(1)
    {
        int out_len = sizeof(out_buf);
        int in_len = fread(in_buf, sizeof(short), COUNT_OF(in_buf), in_file);
        if(in_len == 0)
        {
            break;
        }

        printf("in_len = %d\n", in_len);
        err = speex_resampler_process_int(st, 1, in_buf, &in_len, out_buf, &out_len);
//        err = speex_resampler_process_float(st, 1, in_buf, &in_len, out_buf, &out_len);
//        err = speex_resampler_process_interleaved_int(st, in_buf, &in_len, out_buf, &out_len);
        printf("proc ret: %s\n", speex_resampler_strerror(err));

        if(out_len)
        {
            fwrite(out_buf, sizeof(short), out_len, out_file);
        }

    }

    speex_resampler_destroy(st);
    fclose(in_file);
    fclose(out_file);

    return 0;
}
