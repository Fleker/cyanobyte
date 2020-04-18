/*
* Copyright (C) 2020 Google Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Auto-generated file for BMP180 v0.1.0.
* Generated from peripherals/BMP180.yaml using Cyanobyte Codegen v0.1.0
* Class for BMP180
* Bosch Digital Temperature / Pressure Sensor

*/




#include "BMP180.h"
#define DEVICE_ADDRESS 119

#define REGISTER_CONTROL 244
#define REGISTER_PRESSURECALAC1 170
#define REGISTER_PRESSURECALAC2 172
#define REGISTER_PRESSURECALVB1 182
#define REGISTER_PRESSURECALVB2 184
#define REGISTER_RESULT 246
#define REGISTER_TEMPCAL3 174
#define REGISTER_TEMPCAL4 176
#define REGISTER_TEMPCAL5 178
#define REGISTER_TEMPCAL6 180
#define REGISTER_TEMPCALMC 188
#define REGISTER_TEMPCALMD 190

// Provide an I2C connect function, return status
int bmp180_init(int (*connect)(uint8_t)) {
    // Initialize bus
    if (connect(DEVICE_ADDRESS) != 0) {
        return -1;
    }
}


int bmp180_writeControl(
    uint8_t* data,
    int (*write)(uint8_t, uint8_t, uint8_t*, uint8_t)
) {
    if (write(DEVICE_ADDRESS, REGISTER_CONTROL, data, 1) != 0) {
        return -1;
    }
    return 0;
}int bmp180_readPressureCalAC1(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_PRESSURECALAC1, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readPressureCalAC2(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_PRESSURECALAC2, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readPressureCalVB1(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_PRESSURECALVB1, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readPressureCalVB2(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_PRESSURECALVB2, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readResult(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_RESULT, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCal3(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCAL3, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCal4(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCAL4, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCal5(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCAL5, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCal6(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCAL6, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCalMC(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCALMC, val, 2) != 0) {
        return -2;
    }
    return 0;
}

int bmp180_readTempCalMD(
    uint16_t* val,
    int (*read)(uint8_t, uint8_t, uint16_t*, uint8_t)
) {
    if (val == NULL) {
        return -1; // Need to provide a valid value pointer
    }
    if (read(DEVICE_ADDRESS, REGISTER_TEMPCALMD, val, 2) != 0) {
        return -2;
    }
    return 0;
}



void bmp180_pressure_asmbars(
    float* val,
    int (*read)(uint8_t, uint8_t, int*, uint8_t),
    int (*write)(uint8_t, uint8_t, int*, uint8_t)
) {
    short ac1; // Variable declaration
    short ac2; // Variable declaration
    short ac3; // Variable declaration
    uint16_t ac4; // Variable declaration
    float b1; // Variable declaration
    float c3; // Variable declaration
    float c4; // Variable declaration
    float p0; // Variable declaration
    float p1; // Variable declaration
    float p2; // Variable declaration
    float pressure; // Variable declaration
    float rawComp; // Variable declaration
    float temperature; // Variable declaration
    short vb1; // Variable declaration
    short vb2; // Variable declaration
    float x; // Variable declaration
    float x1; // Variable declaration
    float x2; // Variable declaration
    float y; // Variable declaration
    float y0; // Variable declaration
    float y1; // Variable declaration
    float y2; // Variable declaration
    float z; // Variable declaration


    bmp180_writeControl(&52, write);
    bmp180_readResult(&pressure, read);
    bmp180_temperature_ascelsius(&temperature, read, write);
    rawComp = (temperature-25);
    bmp180_readPressureCalAC1(&ac1, read);
    bmp180_readPressureCalAC2(&ac2, read);
    x1 = (160*pow(2, -13)*ac2);
    bmp180_readPressureCalVB2(&vb2, read);
    x2 = (pow(160, 2)*pow(2, -25)*vb2);
    x = ((x2*pow(rawComp, 2))+(x1*rawComp)+ac1);
    bmp180_readTempCal3(&ac3, read);
    c3 = (160*pow(2, -15)*ac3);
    bmp180_readTempCal4(&ac4, read);
    c4 = (pow(10, -3)*pow(2, -15)*ac4);
    bmp180_readPressureCalVB1(&vb1, read);
    b1 = (pow(160, 2)*pow(2, -30)*vb1);
    y0 = (c4*pow(2, 15));
    y1 = (c4*c3);
    y2 = (c4*b1);
    y = ((y2*pow(rawComp, 2))+(y1*rawComp)+y0);
    z = ((pressure-x)/y);
    p0 = ((3791-8)/1600);
    p1 = (1-(7357*pow(2, -30)));
    p2 = (3038*100*pow(2, -36));
    pressure = ((p2*pow(z, 2))+(p1*z)+p0);


    *val = pressure;
}

void bmp180_temperature_ascelsius(
    float* val,
    int (*read)(uint8_t, uint8_t, int*, uint8_t),
    int (*write)(uint8_t, uint8_t, int*, uint8_t)
) {
    uint16_t ac5; // Variable declaration
    uint16_t ac6; // Variable declaration
    float c5; // Variable declaration
    float c6; // Variable declaration
    float mc; // Variable declaration
    float md; // Variable declaration
    short rawComp; // Variable declaration
    short rawMc; // Variable declaration
    short rawMd; // Variable declaration
    float temperature; // Variable declaration


    bmp180_writeControl(&46, write);
    bmp180_readResult(&temperature, read);
    bmp180_readTempCal5(&ac5, read);
    bmp180_readTempCal6(&ac6, read);
    c5 = ((pow(2, -15)/160)*ac5);
    c6 = ac6
    rawComp = (c5*(temperature-ac6));
    bmp180_readTempCalMC(&rawMc, read);
    mc = ((pow(2, 11)/pow(160, 2))*rawMc);
    bmp180_readTempCalMD(&rawMd, read);
    md = (rawMd/160);
    temperature = (rawComp+(mc/(rawComp+md)));


    *val = temperature;
}

