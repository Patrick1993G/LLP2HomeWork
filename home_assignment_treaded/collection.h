#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED
#include <time.h>

typedef struct SENSORS
{
    unsigned int MOISTURE;
    unsigned int PH;
    unsigned int SUNLIGHT;
} sensor;

typedef struct SENSORDATA
{
    unsigned int MOISTURE;
    float PH;
    unsigned int SUNLIGHT;
} sensorData;


int generate(int lower, int upper)
{
    srand((unsigned)time(NULL));
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

float generateFloat(float lower, float upper)
{
    float num = ((float)rand() / RAND_MAX) * (float)(upper);
    return num;
}

sensorData generateData()
{
    sensorData data;
    data.MOISTURE = generate(1, 10);
    data.PH = generateFloat(1.0f, 10.0f);
    data.SUNLIGHT = generate(0, 2000);
    return data;
}
sensor *initialiseCollection()
{
    sensor *recSensData = (sensor*)malloc(sizeof(sensor));

    if(recSensData != NULL){
        recSensData->MOISTURE = 0;
        recSensData->PH = 0;
        recSensData->SUNLIGHT = 0;
    }
    return recSensData;
}

#endif