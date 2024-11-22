#ifndef MITM_H
#define MITM_H

#include <stdbool.h>

// Function to check for duplicate IPs associated with multiple MACs
bool detect_duplicate_ip(const char *ip, const char *mac);

// Function to detect MITM attacks in captured packets
void detect_mitm();

#endif // MITM_H
