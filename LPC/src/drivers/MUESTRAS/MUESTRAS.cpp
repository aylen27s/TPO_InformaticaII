/*
 * MUESTRAS.cpp
 *
 *  Created on: 24 nov. 2025
 *      Author: aylen
 */

#include "MyIncludes.h"


MUESTRAS::MUESTRAS(){
    mUpperLimit         = LIMITE_SUPERIOR;
    mLowerLimit         = LIMITE_INFERIOR;
    mCounterDanger      = 0;
    IsLimitExceeded     = false;
    IsBufferFull        = false;
    IsProcessed         = false;
    mIndexBuf           = 0;
    mSampleProcessed    = 0;
    mSamplePrev         = 0;
}

MUESTRAS::~MUESTRAS(){

}

int32_t        MUESTRAS::PushBack(int32_t sample){

    if( !IsBufferFull ){

        mSamplingBuff[mIndexBuf] = sample;
        mIndexBuf++;
//        IsBufferFull=false;

        if(mIndexBuf == SIZE_BUFF){
            IsBufferFull = true;
//            mIndexBuf = 0;
        }
        return 1;
    } else {
        return -1;
    }
}

void MUESTRAS::CleanBuff(){
    int i;
    IsBufferFull        = false;
    IsProcessed         = false;
    mIndexBuf           = 0;
    mSampleProcessed    = 0;

    for (i = 0; i < SIZE_BUFF; i++)
        mSamplingBuff[i] = '0';

}

int32_t     MUESTRAS::GetSampleProcessed(void){

    if( IsBufferFull ){
        mSamplePrev = mSampleProcessed;     //Guardo la muestra que se acaba de procesar en un miembro de la clase para poder usarla despues
        CleanBuff();
        return mSamplePrev ;
    }
    else
        return -1;
}

uint32_t    MUESTRAS::Size(void){
    return mIndexBuf;
}

int32_t  MUESTRAS::Process(void){
    int i;

    if( IsBufferFull ){
        for(i=0 ; i < SIZE_BUFF ; i++)
            mSampleProcessed += mSamplingBuff[i];

        mSampleProcessed /= SIZE_BUFF;

        IsProcessed = true;


        if( (mSampleProcessed >= mUpperLimit && mSamplePrev >= mUpperLimit ) ||
            (mSampleProcessed <= mLowerLimit && mSamplePrev >= mLowerLimit ) ){

            mCounterDanger++;
            if(mCounterDanger == TOPE_ALERTA)
                IsLimitExceeded = true;

        } else {
            mCounterDanger = 0;
            IsLimitExceeded = false;
        }

        return 1;

    } else { return -1; }


}
