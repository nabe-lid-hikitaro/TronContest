/*
 * control_temp.c
 *
 *  Created on: 2024/08/18
 *      Author: 180018
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "data.h"

/*===== 変数宣言 ===================================================================================*/
static int s_iTempReacheCount; //温度到達カウント回数（TEMP_TIMEにて定義）

/*===== 関数定義 ===================================================================================*/
static  void    __task_control_temp_init( void );

/*==============================================================================*/
/*  関数名 : task_control_temp()                                                     */
/*  機能  : 温度調整タスク処理                                                     */
/*==============================================================================*/
void task_control_temp(INT stacd, void *exinf)
{
/*--------------------------------------------------------------------------------------------------*/
    __task_control_temp_init();                                         /* 計測タスク初期化処理           */
    tk_slp_tsk(TMO_FEVR);                                               /* タスクスリープ。               */

    /*--- 温度調整タスク定周期処理 --------------------------------------------------------------------------*/
    while( 1 )                                                          /* Aoタスクノーマルモード常駐       */
    {
//        tm_printf((UB*)"\nタスクtask_control_temp起床\n");

        int iSSR; //SSR
        int iReacheTemp; //温度到達

    /* テスト用----------------------------------------- */
//        int temp, init;
//
//        if (init != 1)
//        {
//            temp = 0;
//            init = 1;
//        }
//
//
//        if(temp < 80)
//        {
//            temp++;
//        }
//        else
//        {
//            temp = 50;
//        }
//
//        g_fTemp1 = (float)temp;
//
//        tm_printf((UB*)"温度 %d℃\n", temp);
//        tm_printf((UB*)"安定 %d回\n", s_iTempReacheCount);
//        /* テスト用----------------------------------------- */

        /* --- 温度判定処理 --- */
        if(g_fTemp1 >= TEMP_THRESHOLD)
        {
            if(s_iTempReacheCount >= TEMP_TIME)
            {
                iReacheTemp = SET;                  // 温度到達＋安定判断
                if( g_iTempState == TEMPERATURE_CONTROL )
                {
                    g_iTempState = TEMPERATURE_REACHE;  // 2：温度到達状態へ
                }
            }
            else
            {
                s_iTempReacheCount++;            // 温度到達＋安定待ち
            }
        }
        else
        {
            iReacheTemp = CLEAR;                 //温度未到達
            s_iTempReacheCount = CLEAR;
        }

        if(iReacheTemp == SET)
        {
            iSSR = OFF;                           //温度到達時 SSR OFF
        }
        else
        {
            iSSR = ON;                            //温度調整時 SSR ON
        }

        
        if(iSSR == ON)
        {
            out_h(PORT_PODR(6), in_h(PORT_PODR(6))|(1<<2));//SSR ON
        }
        else
        {
            out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<2));//SSR OFF
        }

        tk_slp_tsk(TMO_FEVR);                                               /* タスクスリープ。               */
    }
}
/* end of task_control_temp() */


/*------------------------------------------------------------------------------*/
/*  関数名 : __task_control_temp_init()                                               */
/*  機能  : 温度調整タスク初期化処理                                             */
/*------------------------------------------------------------------------------*/
static  void    __task_control_temp_init( void )
{
    g_iTempState = TEMPERATURE_CONTROL;         // 1：温度制御状態へ
    s_iTempReacheCount = CLEAR;
}
/* end of __task_control_temp_init() */
