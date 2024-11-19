#include <stdio.h>
#include <pcap.h>
#include <pthread.h>
#include "capture.h"

#define MAX_PACKETS 10000

// Shared buffer to hold captured packets
struct pcap_pkthdr packet_buffer[MAX_PACKETS];
u_char *packet_data[MAX_PACKETS];
int packet_index = 0;
pthread_mutex_t capture_mutex = PTHREAD_MUTEX_INITIALIZER;

void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    pthread_mutex_lock(&capture_mutex);  // Lock the mutex to ensure safe access to shared data
    if (packet_index < MAX_PACKETS) {
        packet_buffer[packet_index] = *header;
        packet_data[packet_index] = (u_char *)packet;
        packet_index++;
    }
    pthread_mutex_unlock(&capture_mutex);  // Unlock the mutex after updating the buffer
}

void *start_packet_capture() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        return NULL;
    }

    printf("Capturing packets...\n");
    pcap_loop(handle, 0, packet_handler, NULL);
    pcap_close(handle);
    return NULL;
}
