#ifndef I2SDACBUILTINTONE_H
#define I2SDACBUILTINTONE_H

#include "driver/i2s.h"

#define I2SDAC_I2S_NUMBER       (0)
#define I2SDAC_SAMPLE_RATE      (16000)

#define I2SDAC_DMA_BUF_COUNT    (2)
#define I2SDAC_DMA_BUF_LEN      (1024)
#define I2SDAC_DMA_BUF_TOTAL    (I2SDAC_DMA_BUF_COUNT * I2SDAC_DMA_BUF_LEN)

#define I2SDAC_SAMPLE_MIN       (-32768)

#define I2SDAC_TONE_MIN         (200)
#define I2SDAC_TONE_BUF_LEN     (I2SDAC_SAMPLE_RATE/I2SDAC_TONE_MIN)

#define I2SDAC_UP_DOWN_STEP     (256)

void i2sdacbuiltin_init();
void i2sdacbuiltin_deinit();
size_t i2sdacbuiltin_write_sample(int16_t sample);
void i2sdacbuiltin_start();
void i2sdacbuiltin_stop();
void i2sdacbuiltin_tone(int16_t tone, int32_t msec);

#endif // #ifndef I2SDACBUILTINTONE_H