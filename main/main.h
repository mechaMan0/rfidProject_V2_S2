/*
 * main.h
 *
 *  Created on: 17/03/2021
 *      Author: lance
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#define RFID_UUID_CHAR_SIZE 10

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/gpio.h"

#include "mqtt_client.h"
#include "protocol_examples_common.h"

#include "../components/Classes/Networking/Messaging/MessagingService.h"

//extern "C" {
//#include "../components/Processing/networkProcessing/networkProcessor.h"
//#include "../components/Processing/inputProcessing/inputProcessor.h"
//}


#endif /* MAIN_MAIN_H_ */
