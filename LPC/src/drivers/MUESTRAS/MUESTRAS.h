/*
 * MUESTRAS.h
 *
 *  Created on: 24 nov. 2025
 *      Author: aylen
 */

#ifndef DRIVERS_MUESTRAS_MUESTRAS_H_
#define DRIVERS_MUESTRAS_MUESTRAS_H_

#define SIZE_BUFF           3
#define LIMITE_SUPERIOR     60		//Lo bajé para hacer la demostracion
#define LIMITE_INFERIOR     50
#define TOPE_ALERTA         3

class MUESTRAS {
private:
	uint32_t     mSamplingBuff[SIZE_BUFF];
    uint32_t     mSampleProcessed;
    uint32_t     mSamplePrev;
    uint32_t    mIndexBuf;

    uint32_t    mUpperLimit;
    uint32_t    mLowerLimit;
    uint32_t    mCounterDanger;

    bool IsLimitExceeded;
    bool IsBufferFull;
    bool IsProcessed;

public:
    MUESTRAS();
    ~MUESTRAS();


    int32_t PushBack(int32_t sample);
    void CleanBuff(void);
    int32_t GetSampleProcessed(void);
    uint32_t Size(void);
    int32_t Process(void);


    bool StatusBuff(void)    { return IsBufferFull; }
    bool StatusLimit(void)   { return IsLimitExceeded; }
    bool StatusProcess(void){ return IsProcessed; }
};

#endif /* DRIVERS_MUESTRAS_MUESTRAS_H_ */
