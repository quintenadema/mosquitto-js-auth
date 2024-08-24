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
int mosquitto_auth_plugin_version(void) {
    return MOSQ_AUTH_PLUGIN_VERSION; // Return the plugin version
}

// Plugin initialization function
int mosquitto_auth_plugin_init(void **user_data, struct mosquitto_opt *options, int option_count) {
    mosquitto_log_printf(MOSQ_LOG_INFO, "Mosquitto plugin initialized.");
    
    mosquitto_plugin_id_t *plugin_id = (mosquitto_plugin_id_t *)(*user_data);
    
    // Register the auth callback
    mosquitto_callback_register(plugin_id, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL, NULL);
    
    return MOSQ_ERR_SUCCESS; // Initialization successful
}

// Plugin cleanup function
int mosquitto_auth_plugin_cleanup(void *user_data, struct mosquitto_opt *options, int option_count) {
    mosquitto_log_printf(MOSQ_LOG_INFO, "Mosquitto plugin cleanup.");
    
    mosquitto_plugin_id_t *plugin_id = (mosquitto_plugin_id_t *)user_data;
    
    // Unregister the auth callback
    mosquitto_callback_unregister(plugin_id, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL);
    
    return MOSQ_ERR_SUCCESS; // Cleanup successful
}


int mosquitto_auth_security_init(void *user_data, struct mosquitto_opt *options, int option_count, bool reload) {
	return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_security_cleanup(void *user_data, struct mosquitto_opt *options, int option_count, bool reload) {
	return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_acl_check(void *user_data, int access, const struct mosquitto *client, const struct mosquitto_acl_msg *message) {
	return MOSQ_ERR_SUCCESS;
}