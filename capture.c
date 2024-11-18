#include <stdio.h>
#include <pcap.h>
#include <pthread.h>
#include "capture.h"

// Global variables to track capture status and thread
pthread_t capture_thread;
int capture_running = 0;
pcap_t *handle; // pcap handle for packet capture

// Packet handler to process each captured packet
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    printf("Captured a packet of length: %d bytes\n", header->len);
    // Add custom packet processing logic here (e.g., attack detection)
}

// Function to start packet capture in a separate thread
void *start_packet_capture(void *arg) {
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open the network device for live capture (change "any" to a specific interface if needed)
    handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        capture_running = 0;
        return NULL;
    }

    printf("Starting packet capture...\n");
    capture_running = 1;

    // Start capturing packets and call packet_handler for each packet
    pcap_loop(handle, 0, packet_handler, NULL);

    return NULL;
}

// Function to stop packet capture
void stop_packet_capture() {
    if (capture_running) {
        printf("Stopping packet capture...\n");
        pcap_breakloop(handle);  // Stop the packet capture loop
        pcap_close(handle);      // Close the pcap handle
        capture_running = 0;
    }
}

