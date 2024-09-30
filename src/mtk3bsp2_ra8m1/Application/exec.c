/*
 * exec.c
 *
 *  Created on: 2024/08/18
 *      Author: 180018
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "data.h"

/*===== 構造体宣言 =================================================================================*/
/*===== 変数宣言 ===================================================================================*/
/*===== 関数定義 ===================================================================================*/
static  void    _task_exec_init( void );

/*==============================================================================*/
/*  関数名 : task_exec()                                                        */
/*  機能  : 運転タスク処理                                                      */
/*==============================================================================*/
void task_exec(INT stacd, void *exinf)
{
	int iTransPressState;
	int iTransTempState;
	
/*--------------------------------------------------------------------------------------------------*/
    _task_exec_init();                                          // 計測タスク初期化処理
    tk_slp_tsk(TMO_FEVR);                                       // タスクスリープ

    /*--- 運転タスク定周期処理 ----------------------------------------------------------------------*/
    while( 1 )
    {
        if( ( g_iPressState == PRESSURE_REACHE ) &&				// 圧力到達状態 かつ
        	( g_iTempState == TEMPERATURE_REACHE ) )			// 温度到達状態 ?
        {
            if ( g_ulPVTime > g_CookingRecipes.ulSVTime )		// 圧力＋温度さらし調理時間 到達？
            {
	            g_ulPVTime = CLEAR; 							// 圧力＋温度さらし調理時間クリア
                g_iPressState = PRESSURE_RELEASE;				// 状態：3：大気開放状態へ
            	g_ulPressureReleasePVTime = CLEAR;				// 大気開放さらし調理時間クリア
            }
        	else												// 圧力＋温度さらし調理中
        	{
	            g_ulPVTime += SYCLETIME; 						// さらし時間経過カウント
            }
        }
    	else if ( g_iPressState == PRESSURE_RELEASE )			// 大気開放状態?
    	{
			if( g_ulPressureReleasePVTime > g_CookingRecipes.ulPressureReleaseSVTime )	//大気開放さらし時間 到達?
            {
            	g_ulPressureReleasePVTime = CLEAR;				// 大気開放さらし調理時間クリア
            	if( g_ulSoakLoopPVCount >= g_CookingRecipes.ulSoakLoopCount )	// 調理ステップ残なし
            	{
		    		while(1) 									// 調理完了
		    		{
	                    g_iPressState = PRESSURE_CC;          // 状態：4：調理完了状態へ
	                    g_iTempState = TEMPERATURE_CC;           // 状態：3：調理完了状態へ
		    		}
            	}
            	else											// 調理ステップ残あり
            	{
            		g_ulSoakLoopPVCount++;						// 調理ステップを進める
            		g_iPressState = DECOMPRESSION_CONTROL;		// 状態：1：減圧制御状態へ
            		g_iTempState = TEMPERATURE_CONTROL;			// 状態：1：温度制御状態へ
            	}
            }
    		else												// 大気開放さらし中
    		{
            	g_ulPressureReleasePVTime += SYCLETIME; 		// 大気圧さらし時間経過カウント
            }
    	}
    	
//    	g_iPressState = iTransPressState;
    	
        tk_slp_tsk(TMO_FEVR);                                   // タスクスリープ
    }
}
/* end of task_exec() */


/*------------------------------------------------------------------------------*/
/*  関数名 : _task_exec_init()                                                  */
/*  機能  : 運転タスク初期化処理                                                */
/*------------------------------------------------------------------------------*/
static  void    _task_exec_init( void )
{
    g_ulPVTime                    = 0;                            // 温度圧力さらし経過時間
    g_ulPressureReleasePVTime     = 0;                            // 大気圧さらし経過時間

}
/* end of _task_exec_init() */
