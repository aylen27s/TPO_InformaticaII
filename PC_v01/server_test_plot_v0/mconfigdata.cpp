#include "mconfigdata.h"

MConfigData::MConfigData(float _psmax, float _psmin,float _pdmax,float _pdmin,int32_t _tsample) {
    psMax=_psmax;
    psMin=_psmin;
    pdMax=_pdmax;
    pdMin=_pdmin;
    tSample= _tsample;
}

MConfigData::MConfigData(){
    psMax=PS_MAX_DEF;
    psMin=PS_MIN_DEF;
    pdMax=PD_MAX_DEF;
    pdMin=PD_MIN_DEF;
    tSample= TIME_TO_REFRESH;
}

bool operator==(MConfigData a, MConfigData b){
    return (a.psMax == b.psMax ? true : false);
}
