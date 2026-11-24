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

### Compilation & Execution

Compile the project from the root directory using `make`:

```bash
make
```

Start the server by providing a listening port and a connection password:

```bash
./ircserv <port> <password>
```

---

### Connecting with a Client

#### Using `irssi` (Recommended)

Connect directly from your terminal:

```bash
irssi -c 127.0.0.1 -p 6667 -w mypassword
```

Or from within an active `irssi` session:

```text
/connect 127.0.0.1 6667 mypassword
```

#### Using `nc` (Raw Protocol Testing)

Connect with Netcat (the `-C` flag ensures `\r\n` line endings) and send the registration commands:

```bash
nc -C 127.0.0.1 6667
```

```text
PASS mypassword
NICK alice
USER alice 0 * :Alice Smith
```

### Commands

#### `PASS`

**Syntax:** `PASS <password>`

Sets the connection password. Must be sent before `NICK` and `USER` during registration and match the server's password.

#### `NICK`

**Syntax:** `NICK <nickname>`

Sets a nickname during registration or updates the client's current nickname. Fails if the format is invalid or the nickname is already in use.

#### `USER`

**Syntax:** `USER <username> <hostname> <servername> :<realname>`

Specifies the username and real name of a new user to complete client registration once `PASS` and `NICK` are validated.

#### `QUIT`

**Syntax:** `QUIT [:<reason>]`

Disconnects the client from the server, broadcasting an optional quit message to all channels the user was a member of.

#### `PART`

**Syntax:** `PART <channel>{,<channel>} [:<reason>]`

Removes the client from one or more active channels, optionally broadcasting a departure reason to remaining members.

#### `PRIVMSG`

**Syntax:** `PRIVMSG <target>{,<target>} :<message>`

Sends a private message to a specific user (by nickname) or broadcasts it to all members of a channel.

#### `JOIN`

**Syntax:** `JOIN <channel>{,<channel>} [<key>{,<key>}]`

Joins one or more channels (creating them if they do not exist, making the creator an operator). Enforces channel restrictions such as invite-only (`+i`), user limit (`+l`), and channel keys (`+k`).

#### `TOPIC`

**Syntax:** `TOPIC <channel> [:<topic>]`

Displays the current channel topic, or updates it when a new topic is provided (restricted to channel operators if mode `+t` is active).

#### `INVITE`

**Syntax:** `INVITE <nickname> <channel>`

Invites a user to a channel, allowing them to bypass invite-only (`+i`) restrictions (requires channel operator privileges when `+i` is active).

#### `KICK`

**Syntax:** `KICK <channel> <user> [:<comment>]`

Forcefully removes a user from a channel with an optional reason. Requires channel operator privileges.

#### `MODE`
**Syntax:** `MODE <channel> [<modes> [<mode params>]]`
Displays the active modes of a channel, or updates channel settings. Modifying modes requires channel operator privileges.
`ft_irc` supports the following channel modes:
* **`i` (Invite-only):** Restricts access to the channel so that only users invited via `INVITE` can join.
* **`t` (Topic protection):** Restricts the `TOPIC` command so that only channel operators can change the channel topic.
* **`k` (Channel key):** Sets (`+k <password>`) or removes (`-k <password>`) a password required to join the channel.
* **`o` (Operator privilege):** Grants (`+o <nickname>`) or revokes (`-o <nickname>`) channel operator privileges for a member.
* **`l` (User limit):** Sets (`+l <limit>`) or removes (`-l`) the maximum number of members allowed in the channel.

### Stopping ft_irc

Press `Ctrl + C` to stop the server and exit cleanly.

## Ressources
- The bible : https://www.rfc-editor.org/info/rfc1459/
- The importance of IRC : https://www.youtube.com/watch?v=6UbKenFipjo&t=509s
- Explanation of sockets : https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
