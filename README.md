# IRC Server Project

This project is an implementation of an IRC (Internet Relay Chat) server in C++. The objective is to develop a functional and efficient server that handles multiple clients, channels, and message exchanges while adhering to the IRC protocol.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Features](#features)
- [Troubleshooting](#troubleshooting)
- [Author](#author)

## Prerequisites
Before running the project, ensure you have the following:
- C++ compiler (GCC or Clang recommended)
- Make for building the project
- A terminal or command-line interface

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/M4HDIOUI/irc-server.git
   cd irc-server
   ```
2. Build the project:
   ```sh
   make
   ```

## Project Structure
```
.
├── .git
├── Channel.cpp
├── Channel.hpp
├── Makefile
├── client.cpp
├── client.hpp
├── invite.cpp
├── join.cpp
├── kick.cpp
├── list.cpp
├── main.cpp
├── mode.cpp
├── numeric_replies.hpp
├── part.cpp
├── privmsg.cpp
├── quit.cpp
├── registration.cpp
├── server.cpp
├── server.hpp
├── topic.cpp
├── utils.cpp
├── utils.hpp
├── README.md
```

## Usage
- **Start the server**: `./irc_server <port> <password>`
- **Connect with an IRC client**: Use a compatible IRC client (e.g., LimeChat, HexChat, irssi) and enter the server details.
- **Join a channel**: `/join #channel`
- **Send messages**: `/msg #channel Hello!`

## Features
- Multi-client support
- Channel management (topic, join, mode, kick)
- User authentication with a password
- Basic IRC commands support
- Real-time message broadcasting

## Troubleshooting
- Ensure the port is not already in use.
- Check server logs for error messages.
- Verify the firewall allows incoming connections.
- Restart the server if clients fail to connect.

## Author
Project created by **Omar Mahdioui**, **Meryem Mesbahi** as part of the 42 curriculum.

---
