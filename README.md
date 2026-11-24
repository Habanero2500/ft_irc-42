*This project has been created as part of the 42 curriculum by jmetayer, opapouts and malaimo*

# ft_irc-42
Development of an IRC server to understand standardized protocols used by internet. 

## Description : 

The ft_irc project aims to build an Internet Relay Chat which was at the origin of all the app for messaging service using internet. It works using a server and on which the clients (the users) plug themselves to connect with others. The protocol used to deliver the messages from a client is TCP with IPV4.

TCP stands for Transmission Control Protocol and is the forth layer of the OSI model which is the model used for any transmission on internet. TCP can be compared to another protocol called User Datagram Protocol. The main difference between them is the reliability and the speed. TCP is way longer because of all the checks realized but the data will be received well sorted and the delivery is insured.  

IPv4 was learnt firstly to realize the netpractice project. It stands for Internet Protocol version 4. The adresses are computed on 32 bits separated in four 8 bits adresses. 

### The `poll()` Event Loop

At the core of the server lies a single-threaded **I/O multiplexing** loop powered by `poll()`. Instead of spawning a separate thread or process per user, the server monitors all active sockets simultaneously through a dynamic array (`std::vector<struct pollfd>`).

1. **Initialization:** The server's listening socket is registered in the `pollfd` array to watch for incoming connections (`POLLIN`).
2. **Blocking Wait:** `poll()` suspends execution until at least one monitored file descriptor becomes ready for I/O, updating its `revents` bitmask.
3. **Event Dispatching:** Once `poll()` returns, the server iterates through the array and inspects each entry's `revents` flags:
   * **Server Socket (`POLLIN`):** A new client is trying to connect. The server accepts the connection, configures the client socket in non-blocking mode, and appends its `pollfd` structure to the array.
   * **Client Socket (`POLLIN`):** An existing client has sent data. The server reads the incoming bytes into the client's buffer and executes any complete IRC commands.
   * **Client Socket (`POLLOUT`):** The socket is ready for writing, allowing the server to flush queued replies without blocking.
   * **Disconnection or Error (`POLLHUP` / `POLLERR` / `POLLNVAL`):** The client closed the connection or encountered a socket error; the server closes the file descriptor and removes it from the array.


## Instructions 

### Commands

#### Join :

#### Mode :



## Ressources
- The bible : https://www.rfc-editor.org/info/rfc1459/
- The importance of IRC : https://www.youtube.com/watch?v=6UbKenFipjo&t=509s
- Explanation of sockets : https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
