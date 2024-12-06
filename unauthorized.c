#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "capture.h"
#include "unauthorized.h"

// Define authorized IPs
#define MAX_AUTHORIZED_IPS 5
const char *authorized_ips[MAX_AUTHORIZED_IPS] = {
    "192.168.1.1",
    "192.168.1.2",
    "192.168.1.3",
    "192.168.1.4",
    "192.168.1.5"
};

void detect_unauthorized_access() {
    printf("Detecting unauthorized access...\n");

    pthread_mutex_lock(&capture_mutex);  // Lock the mutex to access shared data

    for (int i = 0; i < packet_index; i++) {
        struct ip *ip_header = (struct ip *)(packet_data[i] + 14);  // Adjust for Ethernet header offset
        char src_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);

        int is_authorized = 0;
        for (int j = 0; j < MAX_AUTHORIZED_IPS; j++) {
            if (strcmp(src_ip, authorized_ips[j]) == 0) {
                is_authorized = 1;
                break;
            }
        }

        if (!is_authorized) {
            printf("Unauthorized access detected from IP: %s\n", src_ip);
            char alert_message[256];
            snprintf(alert_message, sizeof(alert_message), "Unauthorized access from IP: %s", src_ip);
            add_alert(alert_message);  // Log the alert
        }
    }

    pthread_mutex_unlock(&capture_mutex);  // Unlock the mutex after processing
}
