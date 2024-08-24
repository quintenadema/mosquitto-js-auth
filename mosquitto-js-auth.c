#include <stdio.h>
#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>

// A basic auth callback that logs a message
static int auth_callback(int event, void *event_data, void *user_data) {
    mosquitto_log_printf(MOSQ_LOG_INFO, "Basic auth callback triggered.");
    return MOSQ_ERR_SUCCESS; // Always allow the connection
}

// Plugin version function
int mosquitto_auth_plugin_version(int supported_version_count, const int *supported_versions) {
    return MOSQ_AUTH_PLUGIN_VERSION; // Return the plugin version
}

// Plugin initialization function
int mosquitto_auth_plugin_init(mosquitto_plugin_id_t *plugin_id, void **user_data, struct mosquitto_auth_opt *options, int option_count) {
    mosquitto_log_printf(MOSQ_LOG_INFO, "Mosquitto plugin initialized.");
    mosquitto_callback_register(plugin_id, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL, NULL); // Register the auth callback
    return MOSQ_ERR_SUCCESS; // Initialization successful
}

// Plugin cleanup function
int mosquitto_auth_plugin_cleanup(void *user_data, struct mosquitto_auth_opt *options, int option_count) {
    mosquitto_log_printf(MOSQ_LOG_INFO, "Mosquitto plugin cleanup.");
    return MOSQ_ERR_SUCCESS; // Cleanup successful
}
