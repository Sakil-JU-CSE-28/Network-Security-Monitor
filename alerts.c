#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "alerts.h"

#define MAX_ALERTS 100

// Array to store alerts
Alert alerts[MAX_ALERTS];
int alert_index = 0;

// Add a new alert
void add_alert(const char *message) {
    if (alert_index < MAX_ALERTS) {
        strncpy(alerts[alert_index].alert_message, message, sizeof(alerts[alert_index].alert_message) - 1);
        alerts[alert_index].timestamp = time(NULL);
        alert_index++;
    } else {
        printf("Alert buffer full. Cannot add new alert.\n");
    }
}

// View all alerts
void view_alerts() {
    if (alert_index == 0) {
        printf("No alerts to display.\n");
        return;
    }

    printf("=== Alerts ===\n");
    for (int i = 0; i < alert_index; i++) {
        char time_buffer[26];
        struct tm *tm_info = localtime(&alerts[i].timestamp);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("[%s] %s\n", time_buffer, alerts[i].alert_message);
    }
}
