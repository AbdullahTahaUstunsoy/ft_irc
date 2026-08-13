*This project has been created as part of the 42 curriculum by austunso, udemirci and falakus.*

### ft_irc

### Description

First, let's talk about what IRC is.

IRC is a text-based communication protocol. IRC was developed in 1988 by Jarkko Oikarinen at the University of Oulu in Finland, in order to replace the MUT (MultiUser Talk) program. To understand IRC better, let's show where IRC is positioned in the network model. First, what is the TCP/IP model? It is a four-layer network model that makes the Internet and modern computer networks work, defining the rules for how data is transferred from one device to another.

- **Application Layer:** This is the layer that contains the protocols allowing user applications to communicate over the network.
- **Transport Layer:** Provides end-to-end communication and data transfer between applications. TCP works here.
- **Internet Layer:** Allows data to be routed between different networks using IP addresses. IP works here.
- **Network Access Layer:** Allows data to be transmitted over the physical network. Technologies such as Wi-Fi and Ethernet are found here.

```
        TCP/IP Model
      ┌────────────────┐
      │  Application   │-> IRC and HTTP are here
      │     Layer      │
      └───────┬────────┘
              │           (Sockets are between the Application Layer and the Transport Layer.)
              ▼
      ┌────────────────┐
      │  Transport     │-> TCP and UDP are here (IRC uses TCP by default.)
      └───────┬────────┘
              │
              ▼
      ┌────────────────┐
      │    Internet    │-> IP is here
      └───────┬────────┘
              │
              ▼
      ┌────────────────┐
      │ Network Access │-> Wi-Fi, Ethernet
      └────────────────┘
```

This project is about creating an IRC server by implementing IRC (Internet Relay Chat), a text-based communication protocol. The goal of this project is to make sure that clients can properly communicate with each other through the IRC server. The IRC clients were created, in the way the subject allows, either through nc (netcat) from the terminal or through an IRC client application, in order to test our IRC server (we used irssi). Thanks to the IRC server we developed in this project, clients can communicate with each other through the server, and they can join channels and interact. In order to understand the project better, let's talk about some features of the server and some concepts.

#### Requirements
- Only 1 poll() can be used to handle all operations.
- The server must be able to handle multiple clients simultaneously without hanging.
- All input/output operations must be non-blocking.

#### Concepts
- **Server:** The program that accepts incoming connections from clients and manages their communication.
- **Client:** The program that connects to the server and sends and receives data.
- **Port:** A 16-bit number (0–65535) that identifies a specific program running on a machine. The IP address delivers the packet to the correct machine; the port determines which program on that machine it will be delivered to.
- **Socket:** The communication endpoint that a program uses to send and receive data over the network. It is represented by a file descriptor (fd).

- **AF_INET:** Specifies that we will use the IPv4 address family.
- **INADDR_ANY:** Listen for connections coming from all network interfaces.
- **SOCK_STREAM:** Creates a TCP-based connection.
- **SO_REUSEADDR:** Allows another socket to bind again immediately after a socket is closed, without waiting for the timeout to expire.
- **SOL_SOCKET:** Used to specify options at the socket level.
- **SOMAXCONN:** Expresses the maximum pending connection queue size allowed by the operating system for listen().
- **POLLIN:** Indicates that there is readable data or a connection on a file descriptor.

- **poll():** Monitors multiple file descriptors at the same time and reports on which of them an event has occurred. It does not loop internally. Each call is a single question. What makes it repeat is the while.
- **accept():** Accepts a new connection arriving at the listening server socket and creates a new client socket.
- **recv():** The system call used to read data from a socket.
- **send():** Sends data to the other side through a socket.
- **htons():** The function that converts a number from our computer's byte order to the network's standard byte order.

- **SIGINT:** The signal sent by Ctrl+C that indicates a request to terminate the program.
- **SIGPIPE:** The signal that occurs when data is sent to a closed socket after the other side has closed the connection.
- **SIG_IGN:** The signal handler value used to ignore a signal.

### Instructions

### Resources
