#ifndef CAPTURE_H
#define CAPTURE_H

#include <pcap.h>
#include <pthread.h>

extern struct pcap_pkthdr packet_buffer[];
extern u_char *packet_data[];
extern int packet_index;
extern pthread_mutex_t capture_mutex;

// Callback function for packet capture
void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet);

// Function to start packet capture
void *start_packet_capture();



#endif // CAPTURE_H
