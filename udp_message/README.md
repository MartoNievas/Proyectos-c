# UDP Chat Tool

A simple UDP-based messaging tool written in C that allows bidirectional communication between devices over a network. This lightweight utility can operate in three modes: listen mode (server), send mode (client), and complete mode (peer-to-peer).

## Project Overview

This project implements a basic UDP chat application that demonstrates socket programming fundamentals in C. Unlike TCP, UDP is connectionless and doesn't guarantee delivery, making it suitable for scenarios where speed is prioritized over reliability. This tool can be used for:

- Quick peer-to-peer messaging over a local network
- Testing network connectivity
- Learning UDP socket programming
- Educational purposes for understanding network protocols

## Features

- **Listen Mode**: Acts as a server, receiving messages from any client
- **Send Mode**: Acts as a client, sending messages to a specified IP address and port
- **Complete Mode**: Enables full bidirectional communication between two peers
- **Simple CLI Interface**: Easy-to-use command-line arguments
- **Real-time Communication**: Instant message transmission without connection overhead

## Compilation

To compile the program, use the following command:
```bash
gcc -o udp_chat udp_chat.c -Wall
```

Or with optimizations:
```bash
gcc -o udp_chat udp_chat.c -Wall -O2
```

## Usage

### Listen Mode (Server)

To start listening for incoming messages on a specific port:
```bash
./udp_chat -l <port>
```

**Example:**
```bash
./udp_chat -l 8080
```

This will start the server listening on port 8080 and display all received messages along with the sender's IP address and port.

### Send Mode (Client)

To send messages to a specific IP address and port:
```bash
./udp_chat -d <ip_address> -p <port>
```

**Example:**
```bash
./udp_chat -d 192.168.1.100 -p 8080
```

After running this command, you'll be prompted to enter messages. Type your message and press Enter to send it. Press Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) to exit.

### Complete Mode (Peer-to-Peer)

To establish a bidirectional chat session between two devices:
```bash
./udp_chat -d <remote_ip> -l <port>
```

**Example:**
```bash
./udp_chat -d 192.168.1.100 -l 8080
```

This mode combines both listening and sending capabilities, allowing you to both send messages and receive responses in real-time. Both peers must use the same port number and specify each other's IP addresses.

**Important**: For complete mode to work properly, both devices need to:
1. Use the same port number
2. Specify the other device's IP address
3. Start the application at approximately the same time

## Example Sessions

### Session 1: Listen and Send Mode

**Terminal 1 (Listener):**
```bash
$ ./udp_chat -l 8080
Listening on port 8080...
From 192.168.1.50:54321 -> Hello from client!
From 192.168.1.50:54321 -> How are you?
```

**Terminal 2 (Sender):**
```bash
$ ./udp_chat -d 192.168.1.100 -p 8080
Sending to 192.168.1.100:8080
Message: Hello from client!
Sent 19 bytes
Message: How are you?
Sent 13 bytes
```

### Session 2: Complete Mode (Bidirectional Chat)

**Device 1 (192.168.1.100):**
```bash
$ ./udp_chat -d 192.168.1.50 -l 8080
Listening on port 8080...
Ready to communicate with 192.168.1.50:8080
Message: Hey! Can you hear me?

From 192.168.1.50:8080 -> Yes, I can hear you!
Message: Great! This is working perfectly.
```

**Device 2 (192.168.1.50):**
```bash
$ ./udp_chat -d 192.168.1.100 -l 8080
Listening on port 8080...
Ready to communicate with 192.168.1.100:8080
Message: 

From 192.168.1.100:8080 -> Hey! Can you hear me?
Message: Yes, I can hear you!

From 192.168.1.100:8080 -> Great! This is working perfectly.
Message: 
```

## Technical Details

- **Protocol**: UDP (User Datagram Protocol)
- **Socket Type**: SOCK_DGRAM
- **Address Family**: AF_INET (IPv4)
- **Buffer Size**: 256 bytes
- **Dependencies**: Standard POSIX socket libraries

## Future Improvements

- **Multi-threading**: Enable the listener to handle multiple clients simultaneously
- **Message History**: Save conversation logs to a file
- **Encryption**: Implement end-to-end encryption for secure communication
- **IPv6 Support**: Add support for IPv6 addresses
- **Broadcast/Multicast**: Enable sending messages to multiple recipients
- **Message Acknowledgment**: Add optional ACK mechanism for reliability
- **GUI Interface**: Create a graphical user interface for easier use
- **Configuration File**: Support for loading settings from a config file
- **Hostname Resolution**: Allow using hostnames instead of just IP addresses
- **Better Error Handling**: More descriptive error messages and recovery mechanisms
- **Packet Fragmentation**: Handle messages larger than the buffer size
- **NAT Traversal**: Implement hole-punching techniques for communication across NATs

## License

This project is provided as-is for educational purposes.

## Contributing

Feel free to fork this project and submit pull requests with improvements or bug fixes.
