# Network-Security-Monitor

## How to run this project
- clone this repository from your linux machine
- Go to terminal
- From terminal go to this directory
- Write command 'make'
- Run using 'sudo ./my_program'
- After completing remove file using 'make clear'

## Project Documentation

### Features
- **Packet Sniffing**: Capture network traffic in real-time using `libpcap`.
- **DDoS Attack Detection**: Analyze traffic for abnormal packet rates.
- **MITM Detection**: Monitor for ARP spoofing or unusual traffic patterns.
- **Unauthorized Access Detection**: Flag suspicious login attempts or abnormal traffic.
- **Real-Time Alerts**: Print alerts to the console for detected malicious activities.

### Tools
- **libpcap**: For capturing and analyzing network packets.
- **pthread**: For multithreading the capture and analysis tasks.
- **C Programming Language**: For implementing core functionality.
- **Command Line Interface (CLI)**: For interaction with the system.
### Testing
- **hping3**: For packet sending and testing ddos attack
