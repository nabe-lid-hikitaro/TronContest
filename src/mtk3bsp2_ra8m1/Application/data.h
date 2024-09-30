/*
 * data.h
 *
 *  Created on: 2024/08/18
 *      Author: 180018
 */

#ifndef DATA_H_
#define DATA_H_

/*===== 構造体宣言 =================================================================================*/
typedef struct  Cooking_Recipes                                     /* 調理レシピ                   */
{
    unsigned long       ulSVTime;                                   /* 温度圧力さらし時間 0～0xFFFFFFFF(sec)  */
    unsigned long       ulSVTemp;                                   /* 温度SV値(℃)                 */
    unsigned long       ulSVPress;                                  /* 圧力SV値(Pa)                 */
    unsigned long       ulPressureReleaseSVTime;                    /* 大気圧さらし時間             */
    unsigned long       ulSoakLoopCount;                            /* 調理ステップ（湿潤繰り返し回数）*/
} COOKING_RECIPES;


/*===== マクロ定義 =================================================================================*/
//共用
#define SYCLETIME 500

typedef enum                            /* Value status状態                                        */
{
    CLEAR,
    SET
} VALUE_STATUS;


typedef enum                            /* ON/OFF状態                                             */
{
    OFF,
    ON
} SWITCH_VALUE;

typedef enum                            /* ON/OFF状態                                             */
{
    CLOSE,
    OPEN
} DIO_VALUE;

//温度制御タスク用
#define TEMP_THRESHOLD 60 //温度到達閾値(℃)
#define TEMP_TIME 10 //温度安定判断カウント

//圧力制御タスク用
#define COUNT 10
#define PRESS_THRESHOLD  -80 //単位kPa
#define PRESS_TIME 60 //圧力安定判断カウント

//調理タスク用
//#define COOK_TIME 120                                  	/* 温度圧力さらし時間 0～0xFFFFFFFF(sec)  */
//#define COOK_TEMP 60										/* 温度SV値(℃)                 */
//#define PRESSURE_RELEASE_TIME 60                    		/* 大気圧さらし時間             */
//#define SOAKCOUNT 5										/* 湿潤繰り返し回数             */


/*===== モード定義 =================================================================================*/
//圧力制御タスク用
typedef enum                            /* 圧力制御状態                                            */
{
    DECOMPRESSION_POWER_OFF,            // 0：電源OFF
    DECOMPRESSION_CONTROL,              // 1：減圧制御状態
    PRESSURE_REACHE,                    // 2：圧力到達
    PRESSURE_RELEASE,                   // 3：大気開放状態
	PRESSURE_CC     					// 4：調理完了
} CONTROL_PRESS_STAT;

//温度制御タスク用
typedef enum                            /* 温度制御状態                                            */
{
    TEMPERATURE_POWER_OFF,              // 0：電源OFF
    TEMPERATURE_CONTROL,                // 1：温度制御状態
    TEMPERATURE_REACHE,                 // 2：温度到達状態
	TEMPERATURE_CC						// 3：調理完了
} CONTROL_TEMP_STAT;

/*===== 外部変数宣言 ===============================================================================*/
extern float g_fTemp1;                                          /* 温度センサー 入力値(℃)          */
extern float g_fPressure1;                                      /* 圧力センサー 入力値(Pa)          */
extern int   g_iPressState;                                     /* 圧力制御状態                     */
extern int   g_iTempState;                                      /* 温度制御状態                     */
extern const COOKING_RECIPES   g_CookingRecipes;				/* 調理レシピ                   	*/
extern unsigned long g_ulPVTime;                                /* 温度圧力さらし経過時間           */
extern unsigned long g_ulPressureReleasePVTime;                 /* 大気圧さらし経過時間             */
extern unsigned long g_ulSoakLoopPVCount;						/* 調理ステップ						*/
extern int    g_iLED1State;                                     /* LED1状態                         */
extern int    g_iLED2State;                                     /* LED2状態                         */
extern int    g_iLED3State;                                     /* LED3状態                         */



/*===== 外部関数宣言 ===============================================================================*/

#endif /* DATA_H_ */
