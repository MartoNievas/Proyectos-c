# UDP Chat Tool

A simple UDP-based messaging tool written in C that allows bidirectional communication between devices over a network. This lightweight utility can operate in two modes: listen mode (server) and send mode (client).

## Project Overview

This project implements a basic UDP chat application that demonstrates socket programming fundamentals in C. Unlike TCP, UDP is connectionless and doesn't guarantee delivery, making it suitable for scenarios where speed is prioritized over reliability. This tool can be used for:

- Quick peer-to-peer messaging over a local network
- Testing network connectivity
- Learning UDP socket programming
- Educational purposes for understanding network protocols

## Features

- **Listen Mode**: Acts as a server, receiving messages from any client
- **Send Mode**: Acts as a client, sending messages to a specified IP address and port
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

## Example Session

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

## Technical Details

- **Protocol**: UDP (User Datagram Protocol)
- **Socket Type**: SOCK_DGRAM
- **Address Family**: AF_INET (IPv4)
- **Buffer Size**: 256 bytes
- **Dependencies**: Standard POSIX socket libraries

## Future Improvements

- **Bidirectional Communication**: Allow the listener to respond to messages
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
