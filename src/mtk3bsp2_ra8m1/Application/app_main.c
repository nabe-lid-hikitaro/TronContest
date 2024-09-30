#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/*===== タスク宣言 ===================================================================================*/
/* 周期ハンドラの生成情報と関連データ */
LOCAL void cychdr_a(void *exinf);               // 実行関数
LOCAL ID    cycid_a;                            // ID 番号
LOCAL T_CCYC ccyc_a = {
    .cycatr = TA_HLNG,                          // 周期ハンドラ属性
    .cychdr = cychdr_a,                         // 周期起動ハンドラアドレス
    .cyctim = 100,                              // 周期起動時間間隔
};

/* タスクsuperの生成情報と関連データ */
void task_super(INT stacd, void *exinf);        // task execution function
ID    tskid_super;                              // Task ID number
T_CTSK ctsk_super = {                           // Task creation information
    .itskpri    = 2,
    .stksz      = 1024,
    .task       = task_super,
    .tskatr     = TA_HLNG | TA_RNG3,
};


/*------------------------------------------------------------------------------*/
/*  関数名 : cychdr_a()                                                         */
/*  機能  : メインタスク起動用周期ハンドラ                                      */
/*------------------------------------------------------------------------------*/
LOCAL void cychdr_a(void *exinf)
{
//    tm_printf((UB*)"\n周期ハンドラ cychdr_a 100ms起床\n");
    tk_wup_tsk( tskid_super );                  // 周期管理タスクの起床要求発行
}
/* end of cychdr_a() */


/*------------------------------------------------------------------------------*/
/*  関数名 : usermain()                                                         */
/*  機能  : ユーザー用メイン処理                                                */
/*------------------------------------------------------------------------------*/
EXPORT INT usermain(void)
{
    tm_putstring((UB*)"Start User-main program.\n");

    /* Turn off the LED on the board. */
    out_h(PORT_PODR(6), in_h(PORT_PODR(6))&~(1<<0));
    out_h(PORT_PODR(4), in_h(PORT_PODR(4))&~(1<<14));
    out_h(PORT_PODR(1), in_h(PORT_PODR(1))&~(1<<7));

    /* Create & Start Tasks */
    tskid_super = tk_cre_tsk(&ctsk_super);
    tk_sta_tsk(tskid_super, 0);

    cycid_a = tk_cre_cyc(&ccyc_a);              // 周期ハンドラの生成
    tk_sta_cyc(cycid_a);

    tk_slp_tsk(TMO_FEVR);

    return 0;
}
/* end of usermain() */
