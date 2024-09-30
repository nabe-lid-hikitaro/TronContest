/* * super.c
 *
 *  Created on: 2024/08/18
 *      Author: 180018
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "data.h"

/*===== 子タスク宣言 ===================================================================================*/
/* タスクmeasureの生成情報と関連データ */
void task_measure(INT stacd, void *exinf);  // task execution function
ID    tskid_measure;            // Task ID number
T_CTSK ctsk_measure = {             // Task creation information
    .itskpri    = 4,
    .stksz      = 1024,
    .task       = task_measure,
    .tskatr     = TA_HLNG | TA_RNG3,
};

/* タスクcontrol_pressの生成情報と関連データ */
void task_control_press(INT stacd, void *exinf);  // task execution function
ID    tskid_control_press;            // Task ID number
T_CTSK ctsk_control_press = {             // Task creation information
    .itskpri    = 6,
    .stksz      = 1024,
    .task       = task_control_press,
    .tskatr     = TA_HLNG | TA_RNG3,
};


/* タスクcontrol_tempの生成情報と関連データ */
void task_control_temp(INT stacd, void *exinf);  // task execution function
ID    tskid_control_temp;            // Task ID number
T_CTSK ctsk_control_temp = {             // Task creation information
    .itskpri    = 8,
    .stksz      = 1024,
    .task       = task_control_temp,
    .tskatr     = TA_HLNG | TA_RNG3,
};

/* タスクcontrol_tempの生成情報と関連データ */
void task_exec(INT stacd, void *exinf);  // task execution function
ID    tskid_exec;            // Task ID number
T_CTSK ctsk_exec = {             // Task creation information
    .itskpri    = 10,
    .stksz      = 1024,
    .task       = task_exec,
    .tskatr     = TA_HLNG | TA_RNG3,
};

/*===== 変数宣言 ===================================================================================*/
static  UW      _su_mode;               /* 周期判定用変数                                           */


/*===== 関数定義 ===================================================================================*/
static  void    __task_super_init( void );

/*==============================================================================*/
/*  関数名 : task_super()                                                       */
/*  機能  : 周期管理タスク処理                                                  */
/*==============================================================================*/
void task_super(INT stacd, void *exinf)
{

    __task_super_init();                                        /* スーパータスク初期化処理         */
    tk_slp_tsk(TMO_FEVR);                                       /* タスクスリープ。しないと最初の周期が間違います   */

    /*--- SUPER定周期処理 --------------------------------------------------------------------------*/
    while( 1 )
    {
        switch( _su_mode )                                      /* 周期判定                         */
        {
            case 0:                                             /* 0msecの場合                     */
//                tm_printf((UB*)"\nタスクsuper起床 000ms目\n");
        	    if( (g_iPressState == DECOMPRESSION_CONTROL) ||
        	    	(g_iPressState == PRESSURE_REACHE) ||
        	    	(g_iPressState == PRESSURE_RELEASE) ||
        	    	(g_iPressState == PRESSURE_CC) )
        		{
                	out_h(PORT_PODR(6), in_h(PORT_PODR(6))|(1<<0));
        		}
        		if( (g_iTempState == TEMPERATURE_CONTROL) ||
        			(g_iTempState == TEMPERATURE_REACHE) ||
        			(g_iTempState == TEMPERATURE_CC) )
        		{
                	out_h(PORT_PODR(4), in_h(PORT_PODR(4))|(1<<14));
        		}
        		if( (g_iPressState == PRESSURE_CC) ||
        			(g_iTempState == TEMPERATURE_CC) )
        		{
                	out_h(PORT_PODR(1), in_h(PORT_PODR(1))|(1<<7));
        		}
                tk_wup_tsk( tskid_measure );                                    /* 周期管理タスクの起床要求発行      */
                break;

            case 1:                                             /* 100msecの場合                   */
//                tm_printf((UB*)"\nタスクsuper起床 100ms目\n");
                /* Inverts the LED on the board. */
        	    if(g_iPressState == DECOMPRESSION_CONTROL)
        		{
            	   	out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<0));
        		}
        		if(g_iTempState == TEMPERATURE_CONTROL)
    	    	{
	                out_h(PORT_PODR(4), in_h(PORT_PODR(4))&~(1<<14));
	        	}
                if( g_iPressState != PRESSURE_CC )
                {
                    tk_wup_tsk( tskid_control_press );                                    /* 周期管理タスクの起床要求発行      */
                }
                 break;

            case 2:                                             /* 200msecの場合                   */
//                tm_printf((UB*)"\nタスクsuper起床 200ms目\n");
                if( g_iTempState != TEMPERATURE_CC )
                {
                    tk_wup_tsk( tskid_control_temp );                                    /* 周期管理タスクの起床要求発行      */
                }else{
     	            out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<2));//SSR OFF
                }
                break;

            case 3:                                             /* 300msecの場合                   */
//                tm_printf((UB*)"\nタスクsuper起床 300ms目\n");
                tk_wup_tsk( tskid_exec );                                    /* 周期管理タスクの起床要求発行      */
                break;

            case 4:                                             /* 400msecの場合                   */
//                tm_printf((UB*)"\nタスクsuper起床 400ms目\n");
                break;

            default:                                            /* その他の場合                       */
                break;
        }

//        set_flg( flgid,  __BIT01 );                             /* イベントフラグセット               */  /*(#16)(01)*/

        if( _su_mode++ >= 5 )                                   /* 回周期処理                      */
        {
            _su_mode = 0;                                       /* 周期リセット                       */
        }
        tk_slp_tsk(TMO_FEVR);                                              /* タスクスリープ                  */
    }                                                           /* →周期ハンドラからの起動待ち       */
}
/* end of task_super() */

/*------------------------------------------------------------------------------*/
/*  関数名 : __task_super_init()                                               */
/*  機能  : SUPERタスク初期化処理                                             */
/*------------------------------------------------------------------------------*/
static  void    __task_super_init( void )
{
    /* Inverts the LED on the board. */
    out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<0));
    out_h(PORT_PODR(4), in_h(PORT_PODR(4))&~(1<<14));
    out_h(PORT_PODR(1), in_h(PORT_PODR(1))&~(1<<7));

	/* Create & Start Tasks */
    tskid_measure = tk_cre_tsk(&ctsk_measure);
    tk_sta_tsk(tskid_measure, 0);

    tskid_control_press = tk_cre_tsk(&ctsk_control_press);
    tk_sta_tsk(tskid_control_press, 0);

    tskid_control_temp = tk_cre_tsk(&ctsk_control_temp);
    tk_sta_tsk(tskid_control_temp, 0);

    tskid_exec = tk_cre_tsk(&ctsk_exec);
    tk_sta_tsk(tskid_exec, 0);

}
/* end of __task_super_init() */


