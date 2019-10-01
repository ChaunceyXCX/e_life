#include "ets_sys.h"
#include "osapi.h"
#include "httpclient.h"

static ETSTimer test_timer; //test http

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_rf_pre_init(void)
{
}
static void ICACHE_FLASH_ATTR test_timer_cb(char *url)
{
    http_get(url, "", http_callback_example);
    os_timer_arm(&test_timer, 15000, 0);
}

void ICACHE_FLASH_ATTR http_test()
{
    os_timer_disarm(&test_timer);
    os_timer_setfn(&test_timer, test_timer_cb, NULL);
    os_timer_arm(&test_timer, 10000, 0);
}