#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>

static int auth_callback(int event, void *event_data, void *user_data) {
    struct mosquitto_evt_basic_auth *auth_data = event_data;
    struct stat buffer;
    int exist;

    const char *username = auth_data->username;
    const char *password = auth_data->password;
    const char *script_path = "auth.js";

    // Check if validate.js exists
    exist = stat(script_path, &buffer);
    if(exist != 0) {
        mosquitto_log_printf(MOSQ_LOG_ERR, "Auth script %s not found.", script_path);
        return MOSQ_ERR_AUTH; // Deny connection if script doesn't exist
    }

    if(username && password) {
        char command[256];
        snprintf(command, sizeof(command), "node %s %s %s", script_path, username, password);
        
        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            mosquitto_log_printf(MOSQ_LOG_ERR, "Failed to run command.");
            return MOSQ_ERR_AUTH;
        }

        char result[5];
        if (fgets(result, sizeof(result), fp) != NULL) {
            pclose(fp);
            if (strncmp(result, "true", 4) == 0) {
                return MOSQ_ERR_SUCCESS; // Authentication successful
            } else {
                return MOSQ_ERR_AUTH; // Authentication failed
            }
        } else {
            pclose(fp);
            mosquitto_log_printf(MOSQ_LOG_ERR, "Failed to retrieve script output.");
            return MOSQ_ERR_AUTH;
        }
    }

    return MOSQ_ERR_AUTH; // Deny connection by default
}

int mosquitto_plugin_version(int supported_version_count, const int *supported_versions) {
    return MOSQ_PLUGIN_VERSION;
}

int mosquitto_plugin_init(mosquitto_plugin_id_t *plugin_id, void **user_data, struct mosquitto_opt *options, int option_count) {
    mosquitto_callback_register(plugin_id, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL, NULL);
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_cleanup(void *user_data, struct mosquitto_opt *options, int option_count) {
    return MOSQ_ERR_SUCCESS;
}
