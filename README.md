# Magshimim Trivia Client-Server Game – Team Project

This repository contains a full **client–server trivia game** developed as a **team project** in the  
**Magshimim Cyber Program (2024–2025)**.

The project is implemented as a classic **client–server architecture**:
- A **C++ server** that manages users, rooms, questions and game flow.
- A **C# WPF desktop client** that provides a graphical interface for players.

The project was developed in pairs as part of the Magshimim curriculum.

---

## 🎯 Project Goals

- Design and implement a complete **networked trivia game**.
- Use **object-oriented design** in both server and client.
- Implement a modular **request/handler** architecture on the server.
- Manage users, rooms and games concurrently.
- Build a **desktop GUI** using WPF for a smooth player experience.
- Practice teamwork, version control and clean code conventions.

---

## 🧱 Architecture Overview

### ✅ Server (C++)

The server is responsible for:

- Accepting client connections and handling requests.
- User registration and login.
- Lobby and room management (create, join, leave, start game).
- Game flow: questions, answers, timing and scoring.
- Returning statistics and personal results to the client.
- Serializing and deserializing JSON messages.

Some of the core server components (based on file names):

- `Communicator` – manages sockets and network communication.
- `GameManager` – tracks active games and their state.
- `GameRequestHandler`, `MenuRequestHandler`, `RoomAdminRequestHandler`, etc. –  
  implement the request/handler pattern for different stages of the app.
- `IDatabase` – abstraction for the database layer.
- `JsonRequestPacketDeserializer` / `…Serializer` – converts between JSON and binary protocol.
- `Helper`, `Codes`, and various models and structs used across the server.

---

### ✅ Client (C# WPF – `TriviaClientGUI`)

The client is a WPF desktop application that lets the user:

- Sign up / log in.
- Navigate through the main menu and lobby.
- Join or create trivia rooms.
- View room lobby, player list and room settings.
- Play and answer questions during the game.
- View statistics and personal results.

Key GUI components (based on file names):

- `LoginWindow`, `SignupWindow`, `WelcomeWindow`
- `MainMenuWindow`, `RoomLobbyWindow`
- `PersonalStatsWindow`, `StatisticsWindow`
- `TriviaClientGUI.csproj`, `TriviaClientGUI.sln`

The GUI communicates with the server over TCP, following the same protocol  
implemented on the C++ side.

---

## 🏗️ Features

### Implemented

- User registration and login
- Lobby and room system
- Game start & end logic
- Question sending and answer handling
- Basic statistics and personal results
- WPF-based GUI client

### Known Issues

- Some graphical issues remain in the WPF client (layout / navigation / states).  
  The underlying logic and protocol are stable and working.

---

## 🛠️ Technologies Used

- **C++** (server logic, networking, JSON handling)
- **C# WPF** (.NET desktop client)
- **Visual Studio** (server & client projects)
- **JSON** for message formats
- **Sockets / TCP** for networking

---

## 📁 Suggested Project Structure in This Repo

```text
/Server
    Codes.h
    Communicator.cpp / .h
    GameManager.cpp / .h
    GameRequestHandler.cpp / .h
    Helper.cpp / .h
    IDatabase.cpp / .h
    IRequestHandler.cpp / .h
    JsonRequestPacketDeserializer.cpp / .h
    JsonRequestPacketSerializer.cpp / .h
    ...
    json.hpp
    (other C++ header and source files)

/Client
    TriviaClientGUI.sln
    TriviaClientGUI.csproj
    *.xaml
    *.xaml.cs
    App.xaml / App.xaml.cs
    Window classes (LoginWindow, MainMenuWindow, RoomLobbyWindow, etc.)

README.md
.gitignore
```

---

## 🚀 How to Run (High Level)

1. **Server (C++):**
   - Open the server solution/project in Visual Studio.
   - Build the project (Debug or Release).
   - Run the server executable so it listens for clients.

2. **Client (C# WPF):**
   - Open `TriviaClientGUI.sln` in Visual Studio.
   - Restore NuGet packages if needed.
   - Build and run the WPF application.
   - Configure the client (if required) to point to the server IP/port.

> Note: Exact build/run steps may depend on the original Magshimim template and configuration.

---

## 👥 Authors

**Nir Ben Saadon & partner**  
Magshimim Cyber Program – Trivia Project (Client–Server)

---

## ⚠️ Disclaimer

This project was developed for **educational purposes** as part of the Magshimim Cyber Program.  
Any templates, starter code or graphics originally provided by Magshimim remain their intellectual property  
and are used here only as part of the learning project.
