#include <stdio.h>
#include <netinet/ip.h>  // For struct ip
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

    // Ensure packet index is valid
    if (packet_index <= 0) {
        printf("No packets captured for analysis.\n");
        pthread_mutex_unlock(&capture_mutex);
        return;
    }

    // Debug: print packet index
    printf("Captured %d packets for analysis.\n", packet_index);

    // Iterate over the packets to count the packets from each IP
    for (int i = 0; i < packet_index; i++) {
        if (packet_data[i] == NULL) {
            printf("Skipping NULL packet at index %d\n", i);  // Debug: print skipping information
            continue;  // Skip invalid packets
        }

        // Debug: Print the start address of packet_data[i] to check validity
        printf("Packet %d address: %p\n", i, packet_data[i]);

        struct ether_header *eth_header = (struct ether_header *)packet_data[i];
        
        // Ensure we can safely read the Ethernet header
        if (eth_header == NULL) {
            printf("Error accessing Ethernet header for packet %d\n", i);
            continue;
        }

        // Check EtherType to ensure it's an IP packet
        if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
            printf("Skipping non-IP packet at index %d\n", i);  // Debug: print skipping non-IP packet
            continue;
        }

        struct ip *ip_header = (struct ip *)(packet_data[i] + sizeof(struct ether_header));  // Correct offset to IP header

        // Debug: Check if IP header is correctly accessible
        if (ip_header == NULL) {
            printf("Error accessing IP header for packet %d\n", i);
            continue;
        }

        in_addr_t src_ip = ip_header->ip_src.s_addr;

        // Debug: print source IP of the packet
        printf("Packet %d: Source IP: %s\n", i, inet_ntoa((struct in_addr){src_ip}));

        // Count packets from the same source IP
        int packet_count = 0;
        for (int j = 0; j < packet_index; j++) {
            if (packet_data[j] == NULL) {
                printf("Skipping NULL packet at inner loop index %d\n", j);  // Debug: print skipping information
                continue;  // Skip invalid packets
            }

            struct ether_header *eth_header_inner = (struct ether_header *)packet_data[j];
            
            if (ntohs(eth_header_inner->ether_type) == ETHERTYPE_IP) {
                struct ip *ip_header_inner = (struct ip *)(packet_data[j] + sizeof(struct ether_header));
                if (ip_header_inner->ip_src.s_addr == src_ip) {
                    packet_count++;
                }
            }
        }

        // If packet count exceeds threshold, print DDoS warning
        if (packet_count > THRESHOLD) {
            printf("DDoS Attack detected from IP: %s with %d packets\n", inet_ntoa((struct in_addr){src_ip}), packet_count);
        }
    }

    // Unlock the mutex after processing
    pthread_mutex_unlock(&capture_mutex);
}
