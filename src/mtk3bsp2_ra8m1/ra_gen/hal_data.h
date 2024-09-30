/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_i3c.h"
#include "r_adc.h"
#include "r_adc_api.h"
FSP_HEADER
/** I3C on I3C Instance. */
extern const i3c_instance_t g_i3c0;

/** Access the I3C instance using these structures when calling API functions directly (::p_api is not used). */
extern i3c_instance_ctrl_t g_i3c0_ctrl;
extern const i3c_cfg_t g_i3c0_cfg;

#ifndef g_i3c0_callback
void g_i3c0_callback(i3c_callback_args_t const *const p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_channel_cfg_t g_adc0_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  3
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
