#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f10x_cfg.h"
#include "global.h"
#include "i2c.h"

/* BMP280 address definition */
#define BMP280_ADDRESS               (0x76)
  
#define BMP280_ID                    (0x58)
#define BMP280_RESET_REG_VALUE       (0xB6)  
#define BMP280_SPEED                 (100000)

/*calibration parameters */  
#define BMP280_DIG_T1_LSB_REG                0x88  
#define BMP280_DIG_T1_MSB_REG                0x89  
#define BMP280_DIG_T2_LSB_REG                0x8A  
#define BMP280_DIG_T2_MSB_REG                0x8B  
#define BMP280_DIG_T3_LSB_REG                0x8C  
#define BMP280_DIG_T3_MSB_REG                0x8D  
#define BMP280_DIG_P1_LSB_REG                0x8E  
#define BMP280_DIG_P1_MSB_REG                0x8F  
#define BMP280_DIG_P2_LSB_REG                0x90  
#define BMP280_DIG_P2_MSB_REG                0x91  
#define BMP280_DIG_P3_LSB_REG                0x92  
#define BMP280_DIG_P3_MSB_REG                0x93  
#define BMP280_DIG_P4_LSB_REG                0x94  
#define BMP280_DIG_P4_MSB_REG                0x95  
#define BMP280_DIG_P5_LSB_REG                0x96  
#define BMP280_DIG_P5_MSB_REG                0x97  
#define BMP280_DIG_P6_LSB_REG                0x98  
#define BMP280_DIG_P6_MSB_REG                0x99  
#define BMP280_DIG_P7_LSB_REG                0x9A  
#define BMP280_DIG_P7_MSB_REG                0x9B  
#define BMP280_DIG_P8_LSB_REG                0x9C  
#define BMP280_DIG_P8_MSB_REG                0x9D  
#define BMP280_DIG_P9_LSB_REG                0x9E  
#define BMP280_DIG_P9_MSB_REG                0x9F  
  
#define BMP280_CHIPID_REG                    0xD0  /*Chip ID Register */  
#define BMP280_RESET_REG                     0xE0  /*Softreset Register */  
#define BMP280_STATUS_REG                    0xF3  /*Status Register */  
#define BMP280_CTRLMEAS_REG                  0xF4  /*Ctrl Measure Register */  
#define BMP280_CONFIG_REG                    0xF5  /*Configuration Register */  
#define BMP280_PRESSURE_MSB_REG              0xF7  /*Pressure MSB Register */  
#define BMP280_PRESSURE_LSB_REG              0xF8  /*Pressure LSB Register */  
#define BMP280_PRESSURE_XLSB_REG             0xF9  /*Pressure XLSB Register */  
#define BMP280_TEMPERATURE_MSB_REG           0xFA  /*Temperature MSB Reg */  
#define BMP280_TEMPERATURE_LSB_REG           0xFB  /*Temperature LSB Reg */  
#define BMP280_TEMPERATURE_XLSB_REG          0xFC  /*Temperature XLSB Reg */  


/* bmp280 structure */
typedef struct 
{  
    Handle i2c;
    uint16 t1;
    int16 t2;
    int16 t3;
    uint16 p1;
    int16 p2;
    int16 p3;
    int16 p4;
    int16 p5;
    int16 p6;
    int16 p7;
    int16 p8;
    int16 p9;
    int32 t_fine;
    uint8 t_sb;  
    uint8 mode;  
    uint8 t_oversampling;  
    uint8 p_oversampling;  
    uint8 filter_coefficient;  
}BMP280_T;

typedef enum {  
    BMP280_T_SKIP = 0x0, 
    BMP280_T_x1,
    BMP280_T_x2,
    BMP280_T_x4,
    BMP280_T_x8,
    BMP280_T_x16
} BMP280_T_OVERSAMPLING;  
  
typedef enum {  
    BMP280_SLEEP = 0x0,  
    BMP280_FORCED = 0x01,  
    BMP280_NORMAL = 0x03,  
} BMP280_WORK_MODE;  
  
typedef enum {  
    BMP280_P_SKIP = 0x0,
    BMP280_P_x1,
    BMP280_P_x2, 
    BMP280_P_x4,
    BMP280_P_x8, 
    BMP280_P_x16
} BMP280_P_OVERSAMPLING;  
  
typedef enum {  
    BMP280_FILTER_OFF = 0x0,     /*filter off*/  
    BMP280_FILTER_x2,            /*0.223*ODR*/  
    BMP280_FILTER_x4,            /*0.092*ODR*/  
    BMP280_FILTER_x8,            /*0.042*ODR*/  
    BMP280_FILTER_x16            /*0.021*ODR*/  
} BMP280_FILTER_COEFFICIENT;  
  
typedef enum {  
    BMP280_T_SB0 = 0x0,     /*0.5ms*/  
    BMP280_T_SB1,           /*62.5ms*/  
    BMP280_T_SB2,           /*125ms*/  
    BMP280_T_SB3,           /*250ms*/  
    BMP280_T_SB4,           /*500ms*/  
    BMP280_T_SB5,           /*1000ms*/  
    BMP280_T_SB6,           /*2000ms*/  
    BMP280_T_SB7,           /*4000ms*/  
} BMP280_T_SB;  
  

/* interface */
static uint8 bmp280ReadReg(__in Handle i2c, __in uint8 reg);
static void bmp280WriteReg(__in Handle i2c, __in uint8 reg, __in uint8 value);
static BOOL bmp280Init(__in BMP280_T *bmp280);
static void bmp280Reset(__in BMP280_T *bmp280);
static void bmp280SetWorkMode(__in BMP280_T *bmp280, BMP280_WORK_MODE mode);
static double bmp280CompensateTemperatureDouble(__in BMP280_T *bmp280, 
                                                 __in int32 temper);
static double bmp280CompensatePressureDouble(__in BMP280_T *bmp280, 
                                               __in int32 value);
#if 0
static int32 bmp280CompensateTemperatureInt32(__in BMP280_T *bmp280, 
                                               __in int32 temper);
static uint32 bmp280CompensatePressureInt64(__in BMP280_T *bmp280, 
                                              __in int32 value);
#endif
static double bmp280GetTemperature(__in BMP280_T *bmp280);
static double bmp280GetPressure(__in BMP280_T *bmp280);
static void bmp280GetTemperatureAndPressure(__in BMP280_T *bmp280, 
                                     __out double *temperature, 
                                     __out double *pressure);


/**
 * @brief process bmp280 data
 */
static void vBMP280Process(void *pvParameters)
{
    UNUSED(pvParameters);
    const TickType_t xDelay = 1600 / portTICK_PERIOD_MS;
    const TickType_t xNotifyWait = 100 / portTICK_PERIOD_MS;
    Sensor_Info sensorInfo = {BMP280 , 0};
    Handle i2c = I2c_Request(I2c1);
    BMP280_T bmp280;
    bmp280.i2c = i2c;
    xSemaphoreTake(xI2cMutex, portMAX_DELAY);
    bmp280Init(&bmp280);
    xSemaphoreGive(xI2cMutex);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    double temperature = 0, pressure = 0;
    uint32 prevValue = 0;
    for(;;)
    {
        //sample once
        xSemaphoreTake(xI2cMutex, portMAX_DELAY);
        bmp280SetWorkMode(&bmp280, BMP280_FORCED);
        xSemaphoreGive(xI2cMutex);
        vTaskDelay(xDelay);
        xSemaphoreTake(xI2cMutex, portMAX_DELAY);
        bmp280GetTemperatureAndPressure(&bmp280, &temperature, &pressure);
        xSemaphoreGive(xI2cMutex);
        sensorInfo.value = (uint32)pressure;
        if(sensorInfo.value != prevValue)
        {
            prevValue = sensorInfo.value;
            xQueueSend(xSensorValues, (const void *)&sensorInfo, 
                           xNotifyWait);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief setup gp2y1050 process
 */
void vBMP280Setup(void)
{
    xTaskCreate(vBMP280Process, "BMP280Process", PM2_5_STACK_SIZE, 
                NULL, PM2_5_PRIORITY, NULL);
}
  

/**
 * @brief read bmp280 register value
 * @param i2c handle
 * @param register address
 * @return register value
 */
static uint8 bmp280ReadReg(__in Handle i2c, __in uint8 reg)
{
    uint8 data = 0;
    I2c_Write(i2c, (const char *)&reg, 1);
    I2c_Read(i2c, (char *)&data, 1);
    return data;
}

/**
 * @brief write value to bmp280 register
 * @param i2c handle
 * @param register address
 * @param register value
 */
static void bmp280WriteReg(__in Handle i2c, __in uint8 reg, __in uint8 value)
{
    uint8 data[2] = {0, 0};
    data[0] = reg;
    data[1] = value;
    I2c_Write(i2c, (const char *)data, 2);
}
 
  
/**
 * @brief init bmp280
 * @param bmp280 structure
 * @return init status
 */
static BOOL bmp280Init(__in BMP280_T *bmp280)  
{  
    Handle i2c = bmp280->i2c;
    I2c_SetSpeed(i2c, BMP280_SPEED);
    I2c_SetSlaveAddress(i2c, BMP280_ADDRESS);
    I2c_Open(i2c);
    
    uint8 id;
    id = bmp280ReadReg(i2c, BMP280_CHIPID_REG);
    if(id != BMP280_ID)
        return FALSE;
    
  
    bmp280->mode = BMP280_SLEEP;  
    bmp280->t_sb = BMP280_T_SB1;  
    bmp280->p_oversampling = BMP280_P_x4;  
    bmp280->t_oversampling = BMP280_T_x1;  
    bmp280->filter_coefficient = BMP280_FILTER_x4;
  
    /* read the temperature calibration parameters */
    bmp280->t1 = bmp280ReadReg(i2c, BMP280_DIG_T1_MSB_REG);
    bmp280->t1 <<= 8;
    bmp280->t1 |= bmp280ReadReg(i2c, BMP280_DIG_T1_LSB_REG);
    
    bmp280->t2 = bmp280ReadReg(i2c, BMP280_DIG_T2_MSB_REG);
    bmp280->t2 <<= 8;
    bmp280->t2 |= bmp280ReadReg(i2c, BMP280_DIG_T2_LSB_REG);
    
    bmp280->t3 = bmp280ReadReg(i2c, BMP280_DIG_T3_MSB_REG);
    bmp280->t3 <<= 8;
    bmp280->t3 |= bmp280ReadReg(i2c, BMP280_DIG_T3_LSB_REG); 
  
    /* read the pressure calibration parameters */
    bmp280->p1 = bmp280ReadReg(i2c, BMP280_DIG_P1_MSB_REG);
    bmp280->p1 <<= 8;
    bmp280->p1 |= bmp280ReadReg(i2c, BMP280_DIG_P1_LSB_REG);
    
    bmp280->p2 = bmp280ReadReg(i2c, BMP280_DIG_P2_MSB_REG);
    bmp280->p2 <<= 8;
    bmp280->p2 |= bmp280ReadReg(i2c, BMP280_DIG_P2_LSB_REG);
    
    bmp280->p3 = bmp280ReadReg(i2c, BMP280_DIG_P3_MSB_REG);
    bmp280->p3 <<= 8;
    bmp280->p3 |= bmp280ReadReg(i2c, BMP280_DIG_P3_LSB_REG);
    
    bmp280->p4 = bmp280ReadReg(i2c, BMP280_DIG_P4_MSB_REG);
    bmp280->p4 <<= 8;
    bmp280->p4 |= bmp280ReadReg(i2c, BMP280_DIG_P1_LSB_REG);
    
    bmp280->p5 = bmp280ReadReg(i2c, BMP280_DIG_P5_MSB_REG);
    bmp280->p5 <<= 8;
    bmp280->p5 |= bmp280ReadReg(i2c, BMP280_DIG_P5_LSB_REG);
    
    bmp280->p6 = bmp280ReadReg(i2c, BMP280_DIG_P6_MSB_REG);
    bmp280->p6 <<= 8;
    bmp280->p6 |= bmp280ReadReg(i2c, BMP280_DIG_P6_LSB_REG);
    
    bmp280->p7 = bmp280ReadReg(i2c, BMP280_DIG_P7_MSB_REG);
    bmp280->p7 <<= 8;
    bmp280->p7 |= bmp280ReadReg(i2c, BMP280_DIG_P7_LSB_REG);
    
    bmp280->p8 = bmp280ReadReg(i2c, BMP280_DIG_P8_MSB_REG);
    bmp280->p8 <<= 8;
    bmp280->p8 |= bmp280ReadReg(i2c, BMP280_DIG_P8_LSB_REG);
    
    bmp280->p9 = bmp280ReadReg(i2c, BMP280_DIG_P9_MSB_REG);
    bmp280->p9 <<= 8;
    bmp280->p9 |= bmp280ReadReg(i2c, BMP280_DIG_P9_LSB_REG);

    //reset bmp280
    bmp280Reset(bmp280);  
  
    //set work mode
    uint8 ctrlmeas, config;   
    ctrlmeas = (bmp280->t_oversampling << 5) | (bmp280->p_oversampling << 2) | 
                bmp280->mode;  
    config = (bmp280->t_sb << 5) | (bmp280->filter_coefficient << 2);  
  
    bmp280WriteReg(i2c, BMP280_CTRLMEAS_REG, ctrlmeas);  
    bmp280WriteReg(i2c, BMP280_CONFIG_REG, config);
  
    return TRUE; 
}  

/**
 * @brief reset bmp280
 * @param bmp280 structure
 */
static void bmp280Reset(__in BMP280_T *bmp280)
{  
    bmp280WriteReg(bmp280->i2c, BMP280_RESET_REG, BMP280_RESET_REG_VALUE);  
}  


/**
 * @brief set bmp280 work mode
 * @param bmp280 structure
 * @param bmp280 work mode
 */
static void bmp280SetWorkMode(__in BMP280_T *bmp280, BMP280_WORK_MODE mode)
{  
    uint8 ctrlmeas;  
  
    bmp280->mode = mode;  
    ctrlmeas = (bmp280->t_oversampling << 5) | (bmp280->p_oversampling << 2) | 
                bmp280->mode;  
  
    bmp280WriteReg(bmp280->i2c, BMP280_CTRLMEAS_REG, ctrlmeas);  
}  
  
/**
 * @brief compensate bmp280 temperature value
 * @param bmp280 structure
 * @param temperature value
 */
static double bmp280CompensateTemperatureDouble(__in BMP280_T *bmp280, 
                                                 __in int32 temper)
{  
    double var1, var2;  
  
    var1 = (((double)temper) / 16384.0 - ((double)bmp280->t1) / 1024.0)  
            * ((double)bmp280->t2);  
    var2 = ((((double)temper) / 131072.0 - ((double)bmp280->t1) / 8192.0)  
            * (((double)temper) / 131072.0 - ((double)bmp280->t1) / 8192.0))  
            * ((double)bmp280->t3);  
    bmp280->t_fine = (int32)(var1 + var2);
  
    return (var1 + var2) / 5120.0;  
}  
  
/**
 * @brief compensate bmp280 pressure value
 * @param bmp280 structure
 * @param pressure value
 */
static double bmp280CompensatePressureDouble(__in BMP280_T *bmp280, 
                                               __in int32 value)
{  
    double var1, var2, pressure;  

    var1 = ((double)bmp280->t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)bmp280->p6) / 32768.0;
    var2 = var2 + var1 * ((double)bmp280->p5) * 2.0;
    var2 = (var2 / 4.0) + (((double)bmp280->p4) * 65536.0);
    var1 = (((double)bmp280->p3) * var1 * var1 / 524288.0
            + ((double)bmp280->p2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)bmp280->p1);

    if (var1 == 0.0) 
        return 0;

    pressure = 1048576.0 - (double)value;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double) bmp280->p9) * pressure * pressure / 2147483648.0;
    var2 = pressure * ((double) bmp280->p8) / 32768.0;
    pressure = pressure + (var1 + var2 + ((double) bmp280->p7)) / 16.0;

    return pressure;
}  

#if 0
/**
 * @brief compensate bmp280 temperature value
 * @param bmp280 structure
 * @param temperature value
 */
static int32 bmp280CompensateTemperatureInt32(__in BMP280_T *bmp280, 
                                               __in int32 temper)
{  
    int32 var1, var2;  
  
    var1 = ((((temper >> 3) - ((int32)bmp280->t1 << 1))) * 
            ((int32_t)bmp280->t2)) >> 11;
    var2 = (((((temper >> 4) - ((int32_t)bmp280->t1)) * 
              ((temper >> 4) - ((int32_t)bmp280->t1))) >> 12) * 
            ((int32_t)bmp280->t3)) >> 14;
    bmp280->t_fine = var1 + var2;
  
    return (bmp280->t_fine * 5 + 128) >> 8;  
}  

/**
 * @brief compensate bmp280 pressure value
 * @param bmp280 structure
 * @param pressure value
 */
static uint32 bmp280CompensatePressureInt64(__in BMP280_T *bmp280, 
                                              __in int32 value)
{  
    int64 var1, var2, pressure;  
  
    var1 = ((int64)bmp280->t_fine) - 128000;
    var2 = var1 * var1 * (int64)bmp280->p6;
    var2 = var2 + ((var1*(int64)bmp280->p5) << 17);
    var2 = var2 + (((int64)bmp280->p4) << 35);
    var1 = ((var1 * var1 * (int64)bmp280->p3) >> 8) + 
           ((var1 * (int64)bmp280->p2) << 12);
    var1 = (((((int64)1) << 47) + var1)) * ((int64)bmp280->p1) >> 33;
    if(var1 == 0) 
        return 0;
  
    pressure = 1048576 - value;
    pressure = (((pressure<<31)-var2)*3125)/var1;
    var1 = (((int64_t)bmp280->p9) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (((int64_t)bmp280->p8) * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)bmp280->p7) << 4);
  
    return (uint32)pressure;
}  
#endif
/**
 * @brief get bmp280 temperature value
 * @param bmp280 structure
 * @return temperature value
 */
static double bmp280GetTemperature(__in BMP280_T *bmp280)
{  
    uint32 lsb, msb, xlsb;
    int32 temper;
  
    xlsb = bmp280ReadReg(bmp280->i2c, BMP280_TEMPERATURE_XLSB_REG);
    lsb = bmp280ReadReg(bmp280->i2c, BMP280_TEMPERATURE_LSB_REG);
    msb = bmp280ReadReg(bmp280->i2c, BMP280_TEMPERATURE_MSB_REG);
  
    temper = (msb << 12) | (lsb << 4) | (xlsb >> 4);
    return bmp280CompensateTemperatureDouble(bmp280, temper);
}  

/**
 * @brief get bmp280 pressure value
 * @param bmp280 structure
 * @return pressure value
 */
static double bmp280GetPressure(__in BMP280_T *bmp280)
{  
    uint32 lsb, msb, xlsb;
    int32 pressure;
  
  
    xlsb = bmp280ReadReg(bmp280->i2c, BMP280_PRESSURE_XLSB_REG);
    lsb = bmp280ReadReg(bmp280->i2c, BMP280_PRESSURE_LSB_REG);
    msb = bmp280ReadReg(bmp280->i2c, BMP280_PRESSURE_MSB_REG);
  
    pressure = (msb << 12) | (lsb << 4) | (xlsb >> 4);

    return bmp280CompensatePressureDouble(bmp280, pressure);
}  
  
/**
 * @brief get bmp280 temperature and pressure value
 * @param bmp280 structure
 * @param temperature value
 * @param pressure value
 */
static void bmp280GetTemperatureAndPressure(__in BMP280_T *bmp280, 
                                     __out double *temperature, 
                                     __out double *pressure)
{  
    *temperature = bmp280GetTemperature(bmp280);
    *pressure = bmp280GetPressure(bmp280);
}  
  


