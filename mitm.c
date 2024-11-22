#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <stdbool.h>
#include <stdlib.h>
#include "capture.h"
#include "mitm.h"

// Define a structure for IP-to-MAC mappings
typedef struct {
    char ip[16];
    char mac[18];
} IP_MAC_Map;

#define MAX_ENTRIES 100
IP_MAC_Map ip_mac_table[MAX_ENTRIES];
int map_index = 0;

// Function to check for duplicate IPs with different MACs
bool detect_duplicate_ip(const char *ip, const char *mac) {
    for (int i = 0; i < map_index; i++) {
        if (strcmp(ip_mac_table[i].ip, ip) == 0) {
            // Check if MAC is different for the same IP
            if (strcmp(ip_mac_table[i].mac, mac) != 0) {
                printf("MITM DETECTED: IP %s is associated with multiple MACs: %s and %s\n",
                       ip, ip_mac_table[i].mac, mac);
                return true;
            }
            return false; // IP-MAC matches; no MITM
        }
    }

    // New entry; add to the mapping table
    strcpy(ip_mac_table[map_index].ip, ip);
    strcpy(ip_mac_table[map_index].mac, mac);
    map_index++;
    return false;
}

void detect_mitm() {
    printf("Performing MITM detection on %d packets...\n", packet_index);

    for (int i = 0; i < packet_index; i++) {
        struct ether_header *eth_header = (struct ether_header *)packet_data[i];

        // Check if the packet is an ARP packet
        if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
            struct ether_arp *arp_header = (struct ether_arp *)(packet_data[i] + sizeof(struct ether_header));

            char sender_ip[16], sender_mac[18];
            inet_ntop(AF_INET, arp_header->arp_spa, sender_ip, sizeof(sender_ip));
            sprintf(sender_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                    arp_header->arp_sha[0], arp_header->arp_sha[1], arp_header->arp_sha[2],
                    arp_header->arp_sha[3], arp_header->arp_sha[4], arp_header->arp_sha[5]);

            printf("ARP Packet: Sender IP: %s, Sender MAC: %s\n", sender_ip, sender_mac);

            // Detect duplicate IPs
            detect_duplicate_ip(sender_ip, sender_mac);
        }
    }

    printf("MITM detection completed.\n");
}
