#pragma once
#include "esp_http_server.h"
#include "web_controller.hpp"
#include "esp_log.h"
#include "webserver.h"

class WebControllerRegistrar {
    private:
        const char* _logger_tag = "WebControllerRegistrar";
        httpd_handle_t _server = NULL;
    public:
        WebControllerRegistrar(httpd_handle_t server) : _server(server) {}

        void registerEndpoints(const httpd_uri_t* endpoints, const uint32_t endpoint_count) {
            for(int i = 0; i < endpoint_count; i++)
                this->registerEndpoint(&endpoints[i]);
        }

        void registerEndpoint(const httpd_uri_t* endpoint) {
            ESP_LOGI(this->_logger_tag, "Registering uri handler %s method %i", endpoint->uri, endpoint->method);
            register_endpoint(endpoint);
        }

        void registerController(WebController* controller) {
            uint32_t endpoint_count = 0;
            const httpd_uri_t* endpoints = controller->getEndpoints(&endpoint_count);
            
            ESP_LOGD(this->_logger_tag, "Registering %i endpoints...", endpoint_count);
            for(int endpoint_index = 0; endpoint_index < endpoint_count; endpoint_index++) {
                const httpd_uri_t endpoint = endpoints[endpoint_index];
                this->registerEndpoint(&endpoint);
            }
        }
        
        void registerControllers(WebController* controllers[], uint32_t controller_count) {
            ESP_LOGD(this->_logger_tag, "Registering %i controllers", controller_count);

            for(int controller_index = 0; controller_index < controller_count; controller_index++) {
                ESP_LOGD(this->_logger_tag, "Registering controller %i...", controller_index);
                this->registerController(controllers[controller_index]);
            }
        }
};