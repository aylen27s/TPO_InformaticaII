#ifndef MCONFIGDATA_H
#define MCONFIGDATA_H

#include <QtCore>

//Valores por defecto
#define PS_MAX_DEF      150
#define PS_MIN_DEF      110
#define PD_MAX_DEF      70
#define PD_MIN_DEF      98
#define TIME_TO_REFRESH     1000            //Tiempo para el muestro en tiempo real

class MConfigData
{
public:
    MConfigData(float,float,float,float,int32_t);
    MConfigData();
    friend bool operator==(MConfigData a, MConfigData b);

// private:
    float psMax;
    float psMin;
    float pdMax;
    float pdMin;
    int32_t tSample;
};

#endif // MCONFIGDATA_H
