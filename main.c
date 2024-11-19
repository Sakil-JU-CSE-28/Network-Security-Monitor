#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "capture.h"
#include "ddos.h"

void print_menu() {
    printf("NetSec Monitor - Main Menu\n");
    printf("1. Start Packet Capture\n");
    printf("2. Detect DDoS Attacks\n");
    printf("3. Detect MITM Attacks\n");
    printf("4. Detect Unauthorized Access\n");
    printf("5. View Alerts\n");
    printf("6. Log Suspicious Packets\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    pthread_t capture_thread;
    
    while(1) {
        print_menu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                // Start packet capture in a separate thread
                pthread_create(&capture_thread, NULL, start_packet_capture, NULL);
                printf("Capturing finshed\n");
                break;
            case 2:
                // Detect DDoS attacks
                // for testing this i use hping3
                // using sudo hping3 -i u1000 -S -p 80 127.0.0.1
                // command i send the packet and detect the attack
                detect_ddos_attack();
                break;
            case 3:
                // Detect MITM attacks
                //detect_mitm_attack();
                break;
            case 4:
                // Detect unauthorized access
                //detect_unauthorized_access();
                break;
            case 5:
                // View real-time alerts
                //view_alerts();
                break;
            case 6:
                // Log suspicious packets
                //log_suspicious_packets();
                break;
            case 7:
                printf("Exiting the program.\n");
                exit(0);
                break;
            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    }
    return 0;
}
