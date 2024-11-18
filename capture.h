#ifndef CAPTURE_H
#define CAPTURE_H

#include <pcap.h>
#include <pthread.h>

// Function declarations
void *start_packet_capture(void *arg);
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void stop_packet_capture();

// Global variable for capturing thread status
extern pthread_t capture_thread;
extern int capture_running;

#endif // CAPTURE_H

