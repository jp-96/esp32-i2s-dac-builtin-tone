#include <math.h>
#include "i2sdacbuiltintone.h"

int16_t toneSamples[I2SDAC_TONE_BUF_LEN];

void i2sdacbuiltin_init()
{
    // Install and start i2s driver
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  /* the DAC module will only take the 8bits from MSB */
        .sample_rate = I2SDAC_SAMPLE_RATE,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count  = I2SDAC_DMA_BUF_COUNT, //2,
        .dma_buf_len  = I2SDAC_DMA_BUF_LEN, //1024,
        .use_apll = true,
    };
    i2s_driver_install(I2SDAC_I2S_NUMBER, &i2s_config, 0, NULL);
    // Set i2s clock for the audio file 16k 16bit, STEREO
    i2s_set_clk(I2SDAC_I2S_NUMBER, I2SDAC_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);

    // //for internal DAC, this will enable both of the internal channels
    // i2s_set_pin(I2SDAC_I2S_NUMBER, NULL);

    // I2S DAC mode for i2s_set_dac_mode.
    // built-in DAC mode
    // I2S_DAC_CHANNEL_DISABLE  - Disable I2S built-in DAC signals
    // I2S_DAC_CHANNEL_RIGHT_EN - Enable I2S built-in DAC right channel, maps to DAC channel 1 on GPIO25
    // I2S_DAC_CHANNEL_LEFT_EN  - Enable I2S built-in DAC left channel, maps to DAC channel 2 on GPIO26
    // I2S_DAC_CHANNEL_BOTH_EN  - Enable both of the I2S built-in DAC channels.
    // I2S_DAC_CHANNEL_MAX      - I2S built-in DAC mode max index
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
}

void i2sdacbuiltin_deinit()
{
    // Uninstall i2s driver
    i2s_driver_uninstall(I2SDAC_I2S_NUMBER);
}


size_t i2sdacbuiltin_write_sample(int16_t sample)
{
    sample = sample^0x8000U; // 符号なし変換
    uint16_t samples[2];
    samples[0]=sample;
    samples[1]=sample;
    size_t bytes_written;
    i2s_write(I2SDAC_I2S_NUMBER, (const char *)samples, 4, &bytes_written, portMAX_DELAY);
    return bytes_written;
}

void i2sdacbuiltin_start()
{
    // dumy
    for(int i=0; i<I2SDAC_DMA_BUF_LEN; i++)
    {
        i2sdacbuiltin_write_sample(I2SDAC_SAMPLE_MIN);
    }
    // warm up
    for(int sample=I2SDAC_SAMPLE_MIN; sample<=0; sample+=I2SDAC_UP_DOWN_STEP)
    {
        i2sdacbuiltin_write_sample(sample);
    }
}

void i2sdacbuiltin_stop()
{
    // cool down
    for(int sample=0; sample>=I2SDAC_SAMPLE_MIN; sample-=I2SDAC_UP_DOWN_STEP)
    {
        i2sdacbuiltin_write_sample(sample);
    }
    // purge
    for(int i=0; i<(I2SDAC_DMA_BUF_TOTAL); i++)
    {
        i2sdacbuiltin_write_sample(I2SDAC_SAMPLE_MIN);
    }
}

void i2sdacbuiltin_tone(int16_t tone, int32_t msec)
{
    if (tone<I2SDAC_TONE_MIN)
    {
        tone = I2SDAC_TONE_MIN;
    }
    int cycle = I2SDAC_SAMPLE_RATE / tone;
    float deg = (3.14159265*2) / cycle;
    for(int i=0; i<cycle; i++)
    {
        toneSamples[i] =  (int16_t)(8192*sin(deg*i));
    }
    int repeat = tone*msec/1000;
    for(int j=0; j<repeat; j++)
    {
        for(int i=0; i<cycle; i++)
        {
            i2sdacbuiltin_write_sample(toneSamples[i]);
        }
    }
}