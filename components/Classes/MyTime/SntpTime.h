/*
 * Time.h
 *
 *  Created on: 3/04/2021
 *      Author: lance
 */
extern "C" {
	#include <time.h>
	#include <sys/time.h>
	#include "esp_sntp.h"
	#include "esp_log.h"
	#include "esp_system.h"
}
#include <string>

#ifndef COMPONENTS_CLASSES_MYTIME_SNTPTIME_H_
#define COMPONENTS_CLASSES_MYTIME_SNTPTIME_H_

#define TIME_TAG "TIME"
#define TIMEZONE_STR_NY "EST5EDT,M3.2.0/2,M11.1.0"
#define TIMEZONE_STR_NZ "NZST-12NZDT,M10.1.0/2,M3.3.0/3"

/** \brief Class represents the data-time using 24 hour format
 * 		The class also provides sync with a ntp server
 *
 */
class SntpTime {
private:
	static void time_sync_notification_cb(struct timeval *tv);
	static struct tm timeInfo;
	static time_t timeNow;
public:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	SntpTime();
	void config();
	void SyncTime();
	void printTime();
	void updateToCurrentTime(); // This is the important one x
	void setTimeZoneNZ(const char * timezoneInfo);
	std::string timeString();

};

#endif /* COMPONENTS_CLASSES_MYTIME_SNTPTIME_H_ */
