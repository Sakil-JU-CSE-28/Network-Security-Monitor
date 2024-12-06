#ifndef ALERTS_H
#define ALERTS_H

#include <time.h>

// Structure to store alerts
typedef struct {
    char alert_message[256];
    time_t timestamp;
} Alert;

// Function prototypes
void add_alert(const char *message);
void view_alerts();

#endif // ALERTS_H
