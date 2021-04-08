/*
 * clientProcessing.h
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */
#include "../../Networking/Wifi/Wifi.h"
#include "../../Networking/Messaging/MessagingService.h"
#include "../../Employee/Employee.h"
#include "../../Device/Device.h"
#include "../../MyTime/SntpTime.h"
#include <vector>
#include <algorithm>

extern "C" {
	#include "cJSON.h"
}

#define DATA_PROCESSING "DATA PROCESSING"

#ifndef COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_
#define COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_

class DataProcessing {
private:
	Wifi WifiHandler;
	MessagingService messagingService;
	std::vector<Employee> *employeeListPtr;
	std::vector<Device> *deviceListPtr;
	SntpTime globalTime;

	message recvdMessage;
	void commandReceived();
public:
	DataProcessing();
	DataProcessing(std::vector<Employee> *employeeListPtr, std::vector<Device> *deviceListPtr);
	void init();
	void doMessageProcessing();
	MessagingService getMessagingService();

	void sendDevices();
	void sendEmployees();
	void addDevice();
	void addEmployee();

	bool findDevice(Device& deviceToCheck);
	bool findEmployee(Employee& employeeToCheck);
	bool updateDeviceInList(Device deviceToUpdate);
	bool updateEmployeeInList(Employee EmployeeToUpdate);
	void printLists();
};

#endif /* COMPONENTS_CLASSES_APPLICATION_CLIENTPROCESSING_CLIENTPROCESSING_H_ */
