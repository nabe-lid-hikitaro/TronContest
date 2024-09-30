/*
 * control_press.c
 *
 *  Created on: 2024/08/18
 *      Author: 180018
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "data.h"

/*===== 構造体宣言 =================================================================================*/
/*----- ドライバ制御情報 -----------------------------------------------------------------------------*/
typedef struct {
    int iPump; //ポンプ
    int iSV1; //SV1
    int iSV2; //SV2
    int iSV3; //SV3
}PRESSURE_CONTROL_DATA;

/*===== 変数宣言 ===================================================================================*/
static int s_iPress_num;                                        //圧力到達カウント回数（PRESS_TIMEにて定義）
static PRESSURE_CONTROL_DATA s_PressCtrl;                       //圧力制御ドライバコントロール情報

/*===== 関数定義 ===================================================================================*/
static  void    _task_control_press_init( void );
static  void    _KeepDecompressionState( void );
static  void    _TransDecompressionStateCookingPot( void );
static  void    _TransDecompressionStateVacuumBox( void );
static  void    _TransPressureReleaseState( void );

/*==============================================================================*/
/*  関数名 : task_control_press()                                               */
/*  機能  : 圧力調整タスク処理                                                  */
/*==============================================================================*/
void task_control_press(INT stacd, void *exinf)
{
/*--------------------------------------------------------------------------------------------------*/
    int iGoalPress;                                                         //圧力到達
    unsigned int uiCountVacu; //UINT型の定義

    _task_control_press_init();                                             // 圧力調整タスク初期化処理

    tk_slp_tsk(TMO_FEVR);                                                   // タスクスリープ

    /*--- 圧力調整タスク定周期処理 ------------------------------------------------------------------*/
    while( 1 )
    {
        /* --- 圧力判定処理 -------------------------------------------------------------------------*/
        if( g_fPressure1 <= PRESS_THRESHOLD )
        {
            if(s_iPress_num >= PRESS_TIME)
            {
                iGoalPress = SET;                                           // 圧力到達＋安定判断
                if(g_iPressState == DECOMPRESSION_CONTROL)
                {
                    g_iPressState = PRESSURE_REACHE;                            // 圧力制御状態を2：圧力到達 状態へ
                }
            }
            else
            {
                s_iPress_num++;                                             // 圧力到達＋安定待ち
            }
        }
        else
        {
            iGoalPress = CLEAR;                                             // 圧力未到達
            s_iPress_num = CLEAR;
        }

        /* --- 圧力制御処理 -------------------------------------------------------------------------*/
        switch( g_iPressState )
        {
            case DECOMPRESSION_POWER_OFF:                                   // 0：電源OFF 時
                break;
            case DECOMPRESSION_CONTROL:                                     // 1：減圧制御状態
            case PRESSURE_REACHE:                                           // 2：圧力到達
                if(iGoalPress == SET)                                       // 圧力が到達し安定した状態
                {
                    _KeepDecompressionState();                              // 減圧状態をKEEPする処理
                }
                else                                                        // 圧力が未到達または到達待ちの状態
                {
                    if(uiCountVacu >= COUNT)
                    {
                        _TransDecompressionStateCookingPot();  				// 鍋から真空ボックスへ圧力を引く処理
                        uiCountVacu = CLEAR;
                    }
                    else
                    {
                        _TransDecompressionStateVacuumBox();   				//ポンプで真空ボックスの圧力を引く処理
                        uiCountVacu++;
                    }
                }
                break;
            case PRESSURE_RELEASE:                                          // 3：大気開放状態
                _TransPressureReleaseState();                               // 大気開放して圧力を戻す処理
                break;
            default:
               break;
        }

        /* --- ドライバ処理  -------------------------------------------------------------------------*/
        if( s_PressCtrl.iPump == ON )
        {
            out_h(PORT_PODR(6), in_h(PORT_PODR(6))|(1<<1));//pump ON
        }
        else
        {
            out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<1));//pump OFF
        }
        if( s_PressCtrl.iSV1 == ON )
        {
           out_h(PORT_PODR(9), in_h(PORT_PODR(9))|(1<<6)); //SV1 ON
        }
        else
        {
            out_h(PORT_PODR(9), in_h(PORT_PODR(9))&~(1<<6));//SV1 OFF
        }
        if( s_PressCtrl.iSV2 == ON )
        {
            out_h(PORT_PODR(9), in_h(PORT_PODR(9))|(1<<7));//SV2 ON
        }
        else
        {
            out_h(PORT_PODR(9), in_h(PORT_PODR(9))&~(1<<7));//SV2 OFF
        }
        if( s_PressCtrl.iSV3 == ON )
        {
            out_h(PORT_PODR(9), in_h(PORT_PODR(9))|(1<<5));//SV3 ON
        }
        else
        {
            out_h(PORT_PODR(9), in_h(PORT_PODR(9))&~(1<<5));//SV3 OFF
        }
        /* --- タスクスリープ  -----------------------------------------------------------------------*/
        tk_slp_tsk( TMO_FEVR );
    }
}
/* end of task_control_press() */


/*------------------------------------------------------------------------------*/
/*  関数名 : _task_control_press_init()                                         */
/*  機能  : 圧力調整タスク初期化処理                                            */
/*------------------------------------------------------------------------------*/
static  void    _task_control_press_init( void )
{
    g_iPressState = DECOMPRESSION_CONTROL;				// 状態：1：減圧制御状態へ
    s_iPress_num = CLEAR;
}
/* end of _task_control_press_init() */

/*------------------------------------------------------------------------------*/
/*  関数名 : _KeepDecompressionState()                                         */
/*  機能  : 減圧状態をKEEPする処理                                              */
/*------------------------------------------------------------------------------*/
static void    _KeepDecompressionState( void )
{
    s_PressCtrl.iPump = OFF;
    s_PressCtrl.iSV1 = CLOSE;
    s_PressCtrl.iSV2 = CLOSE;
    s_PressCtrl.iSV3 = CLOSE;
}
/* end of _KeepDecompressionState() */


/*------------------------------------------------------------------------------*/
/*  関数名 : _TransDecompressionStateCookingPot()                               */
/*  機能  : 鍋から真空ボックスへ圧力を引く処理                                  */
/*------------------------------------------------------------------------------*/
static void    _TransDecompressionStateCookingPot( void )
{
    s_PressCtrl.iPump = OFF;
    s_PressCtrl.iSV1 = CLOSE;
    s_PressCtrl.iSV2 = OPEN;
    s_PressCtrl.iSV3 = CLOSE;
}
/* end of _TransDecompressionStateCookingPot() */

/*------------------------------------------------------------------------------*/
/*  関数名 : _TransDecompressionStateVacuumBox()                                */
/*  機能  : ポンプで真空ボックスの圧力を引く処理                                 */
/*------------------------------------------------------------------------------*/
static void    _TransDecompressionStateVacuumBox( void )
{
    s_PressCtrl.iPump = ON;
    s_PressCtrl.iSV1 = OPEN;
    s_PressCtrl.iSV2 = CLOSE;
    s_PressCtrl.iSV3 = CLOSE;
}
/* end of _TransDecompressionStateVacuumBox() */

/*------------------------------------------------------------------------------*/
/*  関数名 : _TransPressureReleaseState()                                       */
/*  機能  : 大気開放して圧力を戻す処理                                          */
/*------------------------------------------------------------------------------*/
static void    _TransPressureReleaseState( void )
{
    s_PressCtrl.iPump = OFF;
    s_PressCtrl.iSV1 = CLOSE;
    s_PressCtrl.iSV2 = OPEN;
    s_PressCtrl.iSV3 = OPEN;
}
/* end of _TransPressureReleaseState() */

