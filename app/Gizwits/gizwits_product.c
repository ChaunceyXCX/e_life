/**
************************************************************
* @file         gizwits_product.c
* @brief        Control protocol processing, and platform-related hardware initialization
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "driver/hal_key.h"
#include "httpclient.h"
#define USER_TIME_MS 100
#define LED_TIMEOUT (1000 / USER_TIME_MS) //定义100ms

/** User area The current device state structure */
dataPoint_t currentDataPoint;

bool LITE_STA = 0;

int8 GATE_KEY = 1;

bool GATE_STA = 0;

/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t ICACHE_FLASH_ATTR gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t * dataPointPtr = (dataPoint_t *)data;
    moduleStatusInfo_t * wifiData = (moduleStatusInfo_t *)data;

    if((NULL == info) || (NULL == data))
    {
        GIZWITS_LOG("!!! gizwitsEventProcess Error \n");
        return -1;
    }

    for(i = 0; i < info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_LiteSwitch :
            currentDataPoint.valueLiteSwitch = dataPointPtr->valueLiteSwitch;
            LITE_STA = currentDataPoint.valueLiteSwitch;
            GIZWITS_LOG("Evt: EVENT_LiteSwitch %d \n", currentDataPoint.valueLiteSwitch);
            if(0x01 == currentDataPoint.valueLiteSwitch)
            {
                GPIO_OUTPUT_SET(GPIO_ID_PIN(4), LITE_STA);
            }
            else
            {
                GPIO_OUTPUT_SET(GPIO_ID_PIN(4), LITE_STA);
            }
            break;

        case EVENT_Gate:
            currentDataPoint.valueGate = dataPointPtr->valueGate;
            GIZWITS_LOG("Evt: EVENT_Gate %d\n", currentDataPoint.valueGate);
            switch(currentDataPoint.valueGate)
            {
            case Gate_VALUE0:
                http_get("http://192.168.100.10:8081/opengate", "", http_callback_example);//user handle
                GATE_STA = 1;
                break;
            case Gate_VALUE1:
                http_get("http://192.168.100.10:8081/stop", "", http_callback_example);
                //user handle
                break;
            case Gate_VALUE2:
                http_get("http://192.168.100.10:8081/closegate", "", http_callback_example);
                GATE_STA = 0;
                //user handle
                break;
            default:
                break;
            }
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
            GIZWITS_LOG("@@@@ connected router\n");
 
            break;
        case WIFI_DISCON_ROUTER:
            GIZWITS_LOG("@@@@ disconnected router\n");
 
            break;
        case WIFI_CON_M2M:
            GIZWITS_LOG("@@@@ connected m2m\n");
			setConnectM2MStatus(0x01);
 
            break;
        case WIFI_DISCON_M2M:
            GIZWITS_LOG("@@@@ disconnected m2m\n");
			setConnectM2MStatus(0x00);
 
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("@@@@ RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
            break;
            
        default:
            break;
        }
    }
    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
    
    return 0; 
}


/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm
* 个人理解为本地事件的状态报告（如：按键触发灯亮，报告当前灯的状态给远程服务器）
* @param none
* @return none
*/
void ICACHE_FLASH_ATTR userHandle(void)
{
    //考虑到局域网控制的情况，需要加入大门状态轮询
    //http_get("http://192.168.100.10:8081/state", "", http_callback_state);
    static uint8_t ledtime = 0; //每100ms上报一次数据点状态
    ledtime++;
    if (LED_TIMEOUT < ledtime)
    {
        ledtime = 0;
        currentDataPoint.valueGateSta = GATE_STA;
        currentDataPoint.valueLiteSwitch = LITE_STA;
    }
	 
    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
}


/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void ICACHE_FLASH_ATTR userInit(void)
{
    gizMemset((uint8_t *)&currentDataPoint, 0, sizeof(dataPoint_t));

 	/** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
   		currentDataPoint.valueLiteSwitch = ;
   		currentDataPoint.valueGate = ;
   		currentDataPoint.valueGateSta = ;
    */
   currentDataPoint.valueLiteSwitch = LITE_STA;
}


