#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mosquitto_broker.h"
#include "mosquitto_plugin.h"
#include "mosquitto.h"

static mosquitto_plugin_id_t *mosq_pid = NULL;

static int auth_callback(int event, void *event_data, void *user_data) {
    struct mosquitto_evt_basic_auth *auth_data = event_data;
    struct stat buffer;
    int exist;

    const char *username = mosquitto_client_username(auth_data->client);
    const char *password = auth_data->password;
    const char *script_path = "auth.js";

    // Check if auth.js exists
    exist = stat(script_path, &buffer);
    if (exist != 0) {
        mosquitto_log_printf(MOSQ_LOG_ERR, "Auth script %s not found.", script_path);
        return MOSQ_ERR_AUTH; // Deny connection if script doesn't exist
    }

    if (username && password) {
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
    int i;

    for (i = 0; i < supported_version_count; i++) {
        if (supported_versions[i] == 5) {
            return 5;
        }
    }
    return -1;
}

int mosquitto_plugin_init(mosquitto_plugin_id_t *identifier, void **user_data, struct mosquitto_opt *opts, int opt_count) {
    UNUSED(user_data);
    UNUSED(opts);
    UNUSED(opt_count);

    mosq_pid = identifier;
    return mosquitto_callback_register(mosq_pid, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL, NULL);
}

int mosquitto_plugin_cleanup(void *user_data, struct mosquitto_opt *opts, int opt_count) {
    UNUSED(user_data);
    UNUSED(opts);
    UNUSED(opt_count);

    return mosquitto_callback_unregister(mosq_pid, MOSQ_EVT_BASIC_AUTH, auth_callback, NULL);
}
