#include <stdio.h>
#include <netinet/ip.h>  // Include this header for struct ip
#include <net/ethernet.h>  // For struct ether_header
#include <arpa/inet.h>
#include <pcap.h>
#include "capture.h"
#include "ddos.h"

#define THRESHOLD 1000  // DDoS packet threshold

void detect_ddos_attack() {
    printf("Detecting DDoS attacks...\n");

    // Lock the mutex to safely access the packet buffer
    pthread_mutex_lock(&capture_mutex);

    for (int i = 0; i < packet_index; i++) {
        struct ip *ip_header = (struct ip *)(packet_data[i] + sizeof(struct ether_header));  // Correct offset to IP header
        in_addr_t src_ip = ip_header->ip_src.s_addr;

        // Simple logic to detect DDoS (packet count threshold per second for an IP)
        int packet_count = 0;
        for (int j = 0; j < packet_index; j++) {
            struct ip *ip_header_inner = (struct ip *)(packet_data[j] + sizeof(struct ether_header));
            if (ip_header_inner->ip_src.s_addr == src_ip) {
                packet_count++;
            }
        }

        if (packet_count > THRESHOLD) {
            printf("DDoS Attack detected from IP: %s with %d packets\n", inet_ntoa((struct in_addr){src_ip}), packet_count);
        }
    }

    // Unlock the mutex after processing
    pthread_mutex_unlock(&capture_mutex);
}
