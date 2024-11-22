#include <stdio.h>
#include <pcap.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "capture.h"

#define MAX_PACKETS 5000

// Shared buffer to hold captured packets
struct pcap_pkthdr packet_buffer[MAX_PACKETS];
u_char *packet_data[MAX_PACKETS];
int packet_index = 0;
pthread_mutex_t capture_mutex = PTHREAD_MUTEX_INITIALIZER;

void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    pthread_mutex_lock(&capture_mutex);  // Lock the mutex to ensure safe access to shared data
    if (packet_index < MAX_PACKETS) {
        packet_data[packet_index] = malloc(header->len);  // Allocate memory for packet
        if (packet_data[packet_index] == NULL) {
            fprintf(stderr, "Memory allocation failed for packet %d\n", packet_index);
            return;
        }
        memcpy(packet_data[packet_index], packet, header->len);  // Copy packet data
        packet_index++;  // Increment packet index
    } else {
        printf("Packet buffer full, skipping packet capture\n");
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

    printf("Capturing packets for 10 seconds...\n");

    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 10) { // Capture for 10 seconds
        pcap_dispatch(handle, 1, packet_handler, NULL);
    }

    printf("10 seconds elapsed. Stopping capture.\n");
    pcap_close(handle);
    return NULL;
}
