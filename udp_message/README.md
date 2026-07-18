# UDP Message

A UDP messaging tool with three modes: listen, send, and bidirectional.

## What it does

Plain `AF_INET`/`SOCK_DGRAM` sockets, unicast only. There's no broadcast, multicast, or raw socket support; see Future Improvements below for what's not implemented yet.

## Building

```bash
gcc -o udp udp.c -Wall
```

## Usage

### Listen mode

```bash
./udp -l <port>
```

Binds to the port and prints every message it receives, along with the sender's address:

```
$ ./udp -l 8080
Listening on port 8080...
From 192.168.1.50:54321 -> Hello from client!
```

### Send mode

```bash
./udp -d <ip> -p <port>
```

Reads lines from stdin and sends each one as a UDP datagram to the given address:

```
$ ./udp -d 192.168.1.100 -p 8080
Sending to 192.168.1.100:8080
Message: Hello from client!
Sent 19 bytes
```

Exit with Ctrl+D.

### Complete mode (bidirectional)

```bash
./udp -d <remote_ip> -l <port>
```

Combines both: it binds locally and, using `select()`, listens for incoming datagrams while reading stdin to send. Both ends need to use the same port number and point at each other's address.

```
$ ./udp -d 192.168.1.50 -l 8080
Listening on port 8080...
Ready to communicate with 192.168.1.50:8080
Message: Hey, can you hear me?

From 192.168.1.50:8080 -> Yes, loud and clear!
```

## Technical details

- Protocol: UDP, `AF_INET`, `SOCK_DGRAM`
- Buffer size: 256 bytes per message

## Future improvements

None of these are implemented; they're just ideas.

- Multi-threaded listener for multiple simultaneous clients
- Message logging to a file
- Encryption
- IPv6 support
- Broadcast/multicast
- Message acknowledgment
- A GUI
- Config file support
- Hostname resolution (currently IP addresses only)
- Packet fragmentation handling for messages larger than the buffer
- NAT traversal

## License

MIT. See `LICENSE`.
