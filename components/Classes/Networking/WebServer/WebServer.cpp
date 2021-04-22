/*
 * WebServer.cpp
 *
 *  Created on: 18/04/2021
 *      Author: lance
 */

#include "../WebServer/WebServer.h"

httpd_uri_t WebServer::uri_list[CONFIG_MAX_NUM_RESPONSES];
httpResponse *WebServer::responseList[CONFIG_MAX_NUM_RESPONSES];
int WebServer::pageCount = 0;
#define TAG_HR "TAG HTTP RESPONSE"

/** \brief MyWebServer is an implementation of the web server class!
 * 	this can be used as a demonstration of how to implement the web server class.
 *
 */
WebServer::WebServer() : WebServerSkeleton() {
	// Set URI handlers
   ESP_LOGI(TAG_WS, "Registering URI handlers");

   // Register URIs Here - This needs to be done when a page is added but theese are for the dafault handlers
   //httpd_register_uri_handler(WebServer::server, &WebServer::root); // root handler. Make this a class thing so we can add classes of
   //httpd_register_uri_handler(WebServer::server, &WebServer::notRoot); // root handler. Make this a class thing so we can add classes of

   WebServer::pageCount = 0;

}

/* Add An HTTP GET handler */
esp_err_t WebServer::handler(httpd_req_t *req)
{
	ESP_LOGI(TAG_WS,"URI: %s", req->uri);
    //response("text/html","Cunt", -1, req);
    WebServer::find_uri_and_respond(req);
    return ESP_OK;
}

void WebServer::addResponse(httpResponse *httpResponse) {


	WebServer::uri_list[WebServer::pageCount].handler = WebServer::handler;
	WebServer::uri_list[WebServer::pageCount].uri = httpResponse->uriString.c_str();
	WebServer::uri_list[WebServer::pageCount].method = httpResponse->method;
	WebServer::uri_list[WebServer::pageCount].user_ctx = NULL;

	// Add the http response to the list <3
	WebServer::responseList[WebServer::pageCount] = httpResponse;

	ESP_LOGI(TAG_WS, "Register New Page. URI: %s", httpResponse->uriString.c_str());
	ESP_ERROR_CHECK(httpd_register_uri_handler(WebServer::server, &WebServer::uri_list[this->pageCount])); // root handler. Make this a class thing so we can add classes of

	WebServer::pageCount++;
}

void WebServer::response(const char *responseType, const char *data, ssize_t bufferLen, httpd_req_t *req) {
	httpd_resp_set_type(req, responseType);
	httpd_resp_send(req, data, bufferLen);
}

void WebServer::find_uri_and_respond(httpd_req_t *req) {
	for (int i = 0; i < WebServer::pageCount; i++) {
		ESP_LOGI(TAG_WS, "Comparing Strings. STR 1: %s STR 2: %s",WebServer::responseList[i]->uriString.c_str(),std::string(req->uri).c_str());
		// If the uri strings are legit and were not pointing to a NULL!
		if (!(WebServer::responseList[i]->uriString.compare(std::string(req->uri)))) {
			responseList[i]->respond(req);
			ESP_LOGI(TAG_WS, "EXIT LOOP - if statement success");
			break;
		}
	}

}

httpResponse::httpResponse(std::string data, int dataSize, std::string uriString, std::string responseType, httpd_method_t method) {
	this->dataSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(this->dataSemaphore);
	this->data.assign(data);
	this->dataSize = dataSize;
	this->deleteDataOnResponse = false;
	this->responseType = responseType;
	this->method = method;
	this->uriString.assign(uriString);

	ESP_LOGI(TAG_WS, "constructor data %s", this->data.c_str());
	ESP_LOGI(TAG_WS, "uri string %s", this->uriString.c_str());


}

void httpResponse::respond(httpd_req_t *req) {
	ESP_LOGI(TAG_HR, "RESPOND FUNCTION");
	if (req->user_ctx != NULL) {
		ESP_LOGI(TAG_HR, "CONTEXT DATA: %s", (char *) req->user_ctx);
	}
	if  (this->dataSemaphore != NULL) {
		ESP_LOGI(TAG_HR, "RESPOND CORRECT");

		if (this->method == HTTP_GET) {
			this->respond_get(req);
		} else if (this->method == HTTP_POST) {
			this->respond_post(req);
		}


		if (this->deleteDataOnResponse) {
			this->deleteData();
		}

		xSemaphoreGive(this->dataSemaphore);
	} else {
		httpd_resp_set_type(req, "text/html");
		httpd_resp_send(req, "Semaphore Blocked Data Aquisition", HTTPD_RESP_USE_STRLEN);
	}
}

void httpResponse::respond_get(httpd_req_t *req) {
	httpd_resp_set_type(req, this->responseType.c_str());
	httpd_resp_send(req, this->data.c_str(), this->dataSize);
}

void httpResponse::respond_post(httpd_req_t *req) {

	char buffer[100];
	int ret, remaining = req->content_len;

	//
	while (remaining > 0) {
		/* Read the data for the request */
		if ((ret = httpd_req_recv(req, buffer, MIN(remaining, sizeof(buffer)))) <= 0) {
			if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
				/* Retry receiving if timeout occurred */
				ret = httpd_req_recv(req, buffer, MIN(remaining, sizeof(buffer)));
			}
			// If bytes have sucessfully been received
			if (ret > 0) {
				this->updateData(buffer);
			}
		}
	}

	// Send the received data
	httpd_resp_set_type(req, this->responseType.c_str());
	httpd_resp_send(req, this->data.c_str(), this->dataSize);
}

void httpResponse::updateData(std::string data) {
	xSemaphoreTake(this->dataSemaphore, 1);
	this->data.assign(data);
	xSemaphoreGive(this->dataSemaphore);
}

void httpResponse::updateData(char *data) {
	std::string tempData = std::string(data);
	xSemaphoreTake(this->dataSemaphore, 1);
	this->data.assign(tempData);
	xSemaphoreGive(this->dataSemaphore);
}

void httpResponse::deleteData() {
	xSemaphoreTake(this->dataSemaphore, 1);
	this->data.clear();
	xSemaphoreGive(this->dataSemaphore);
}

std::string httpResponse::getData() {
	return this->data;
}

void httpResponse::addToData(std::string data) {
	xSemaphoreTake(this->dataSemaphore, 0);
	this->data.append(data);
	xSemaphoreGive(this->dataSemaphore);
}

// TODO. Send notification to event loop
//void httpResponse::sendNotification() {
//
//}

//httpd_uri_t WebServer::root = {
//    .uri       = "/",
//    .method    = HTTP_GET,
//    .handler   = WebServer::root_get_handler
//};
//
//httpd_uri_t WebServer::notRoot = {
//    .uri       = "/not-root",
//    .method    = HTTP_GET,
//    .handler   = WebServer::root_get_handler
//};
