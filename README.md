# Queuing System Simulation – DS 2026

A comprehensive **discrete-event queuing system simulator** implemented in C++17. Demonstrates fundamental data structures (queues, linked lists) and real-world queueing theory applied to multi-server systems.

---

## 📋 Overview

This project simulates a **multi-server queue** (like a bank teller line or help desk) with:
- **Multiple parallel servers** handling customers
- **Arrival process** with configurable probability
- **Service times** drawn from a range
- **Queue management** tracking wait times, server utilization, and system performance

Two build targets:
- **Console (CLI)** – Text-based interface with detailed logging
- **GUI (ImGui)** – Interactive graphical interface with live visualization

---

## ✨ Features

### Core Simulation
- Configurable number of servers (1–10)
- Adjustable simulation duration (1–1000 ticks)
- Probabilistic customer arrivals (1–100% per tick)
- Randomized service times (configurable min/max)
- Automatic queue management and server dispatch

### Statistics & Metrics
- Total customers arrived and served
- Average wait time per customer
- Average service time
- Average and maximum queue lengths
- Server utilization tracking
- Detailed event logging

### User Interfaces
| **Console** | **GUI (ImGui + OpenGL)** |
|---|---|
| Text menu-driven | Real-time interactive sliders |
| Log file export | Live server/queue visualization |
| Detailed event transcript | Animated queue length chart |
| No external dependencies | ImGui + GLFW + OpenGL 3 |

---

## 🏗️ Project Architecture

### Class Hierarchy

```
Queue<T>  (template, linked-list based)
  └─ Used by: serverListType, main simulators

customerType
  ├─ Properties: number, arrivalTime, waitingTime, transactionTime
  └─ Methods: getters/setters, increment tracking

serverType
  ├─ Properties: currentCustomer, status (free/busy), transactionTime
  ├─ Methods: isFree(), setBusy/setFree(), decrement timers
  └─ Used by: serverListType

serverListType
  ├─ Properties: array of serverType
  ├─ Methods: updateServers(), getFreeServerID(), setServerBusy()
  └─ Manages: dispatcher, timer updates, release logic

Simulation (main.cpp / imgui_main.cpp)
  ├─ Coordinates: customers, servers, queues
  ├─ Implements: event loop, statistics collection
  └─ Outputs: log files, UI rendering, reports
```

### Data Flow

```
┌─────────────────────────────────┐
│  Customer Arrival (probability) │
└────────────┬────────────────────┘
             │
             ▼
     ┌───────────────┐
     │ Free Server?  │
     └───┬───────┬───┘
         │ Yes   │ No
         ▼       ▼
       Serve   Enqueue
       (busy)   (queue)
         │       │
         │   ┌───┴─────────────────┐
         │   │ When server freed?   │
         │   │ (timer expires)      │
         │   └───────┬──────────────┘
         │           ▼
         │   ┌──────────────────┐
         │   │ Dequeue waiting? │
         │   │ (dispatch logic) │
         │   └──────────────────┘
         │           │
         └─────┬─────┘
               ▼
        ┌─────────────────┐
        │ Update Clocks   │
        │ & Statistics    │
        └─────────────────┘
```

### Queue Implementation

A **generic template queue** using a singly-linked list:
- `enqueue(item)` – O(1) append to back
- `dequeue()` – O(1) remove from front
- `front()` – O(1) peek at next item
- `isEmpty()`, `size()` – O(1) status checks

---

## 📦 Directory Structure

```
queuing-sim/
├── README.md                    # This file
├── CMakeLists.txt               # CMake build config (GUI)
├── Makefile                     # Make build config (Console)
├── imgui_main.cpp               # GUI entry point
├── imgui/                        # Dear ImGui library (prepackaged)
│   ├── imgui.h / imgui.cpp
│   ├── imgui_draw.cpp, imgui_tables.cpp, imgui_widgets.cpp
│   └── backends/
│       ├── imgui_impl_glfw.h/.cpp
│       └── imgui_impl_opengl3.h/.cpp
└── src/
    ├── main.cpp                 # Console entry point
    ├── customerType.h/.cpp      # Customer class
    ├── serverType.h/.cpp        # Server class
    ├── serverListType.h/.cpp    # Server pool manager
    ├── Queue.h                  # Template queue (header-only)
    ├── validation.h/.cpp        # Input validation
    ├── errorHandler.h/.cpp      # Error printing utilities
    └── tests/
        ├── testRunner.cpp/.h
        └── test_*.h             # Unit tests
```

---

## 🚀 Installation & Build

### Prerequisites

#### Console (CLI only)
- **C++17 compiler** (`g++`, `clang++`, or MSVC)
- Standard build tools (`make` or CMake)

**Install on Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install build-essential
```

#### GUI (ImGui + OpenGL)
- Everything above, plus:
- **GLFW 3** – Window and input handling
- **OpenGL 3** – Graphics rendering
- **CMake 3.15+** – Build system

**Install on Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev libx11-dev
```

**Install on macOS (Homebrew):**
```bash
brew install cmake glfw
# OpenGL comes with Xcode Command Line Tools
```

**Install on Windows (MSVC + vcpkg):**
```powershell
vcpkg install glfw3:x64-windows
```

---

## 🛠️ Building

### Option 1: Console Version (Simple)

**Using Make:**
```bash
cd /path/to/queuing-sim
make
./queuing_sim
```

**Clean build:**
```bash
make clean
make
```

**Manual compile (if no Make):**
```bash
cd /path/to/queuing-sim
g++ -std=c++17 -Wall -Wextra -O2 -o queuing_sim \
  src/main.cpp src/customerType.cpp src/serverType.cpp \
  src/serverListType.cpp src/validation.cpp src/errorHandler.cpp
./queuing_sim
```

### Option 2: GUI Version (CMake)

**From project root:**
```bash
mkdir -p build
cd build
cmake ..
make
./QueueSim
```

**Rebuild after changes:**
```bash
cd build
make
```

**Clean build:**
```bash
cd build
cmake ..
make clean
make
```

**On Windows (MSVC):**
```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
.\Release\QueueSim.exe
```

---

## 💻 Usage

### Console Mode

Run the executable:
```bash
./queuing_sim
```

**Interactive prompts:**
1. **Number of servers** – [1–10]
2. **Simulation duration** – Clock ticks [1–1000]
3. **Arrival probability** – Percentage per tick [1–100]
4. **Min service time** – [1–50] ticks
5. **Max service time** – [min–100] ticks
6. **Save log file?** – (y/n) → outputs `simulation_log.txt`

**Example session:**
```
╔══════════════════════════════════════════╗
║   QUEUING SYSTEM SIMULATION  (DS 2026)   ║
╚══════════════════════════════════════════╝

Enter number of servers         [1-10]  : 3
Enter simulation duration (ticks)[1-1000]: 100
Enter arrival probability %      [1-100] : 50
Enter min service time (ticks)   [1-50]  : 2
Enter max service time (ticks)   [1-100] : 8

Save detailed log to file? (y/n): y

[Simulation runs... displays per-tick events...]

╔══════════════════════════════════════════╗
║        SIMULATION SUMMARY REPORT         ║
╚══════════════════════════════════════════╝
  Number of servers       : 3
  Simulated minutes       : 100
  ...
  Avg waiting time        : 2.45 ticks
  Avg service time        : 5.12 ticks
  Avg queue length        : 0.89 customers
  Max queue length        : 12

Detailed log saved to: simulation_log.txt
```

### GUI Mode

Run the executable:
```bash
./QueueSim
```

**Layout:**

| Section | Features |
|---------|----------|
| **Left Panel** | Config sliders, ▶ Run/⏹ Stop/↺ Reset buttons, progress bar, statistics |
| **Top-Right** | Server status badges, waiting queue list |
| **Bottom-Right** | Live event log with color-coded messages (arrivals, departures, etc.) |
| **Bottom Chart** | Queue length over time (visual histogram) |

**Interactive workflow:**
1. Adjust sliders (servers, duration, arrival %, service range)
2. Click **▶ Run** to start
3. Watch real-time events and statistics update
4. Charts update automatically
5. Click **⏹ Stop** anytime; click **↺ Reset** to reconfigure

**Color coding in event log:**
- 🟢 **Green** – Customer departure
- 🔵 **Blue** – Customer arrival
- 🟡 **Yellow** – Customer joined queue
- 🟣 **Purple** – Customer dispatched to server
- ⚪ **Grey** – System info

---

## 📖 API Reference

### `customerType`

```cpp
customerType();
customerType(int custNum, int arrivalT, int waitT, int transT);

void setCustomerInfo(int custNum = 0, int arrivalT = 0, 
                     int waitT = 0, int transT = 0);

int  getCustomerNumber()   const;
int  getArrivalTime()      const;
int  getWaitingTime()      const;
int  getTransactionTime()  const;

void setWaitingTime(int t);
void incrementWaitingTime();
```

### `serverType`

```cpp
serverType();

bool isFree()              const;
void setBusy();
void setFree();

void setTransactionTime(int t);      // Manual set
void setTransactionTime();            // Use currentCustomer's time

int  getRemainingTransactionTime() const;
void decreaseTransactionTime();

void setCurrentCustomer(customerType c);
int  getCurrentCustomerNumber()      const;
int  getCurrentCustomerArrivalTime() const;
int  getCurrentCustomerWaitingTime() const;
```

### `serverListType`

```cpp
serverListType(int numServers);
~serverListType();

void updateServers(std::ostream& log);  // Tick servers, release finished

int  getFreeServerID()           const;  // Returns first free server ID, or -1
void setServerBusy(int id, customerType c);  // Assign customer to server
```

### `Queue<T>` (Template)

```cpp
template <typename T>
class Queue {
    bool isEmpty()     const;
    int  size()        const;
    
    void enqueue(const T& item);
    T    front()       const;
    void dequeue();
};
```

---

## 🧪 Testing

Run unit tests (if compiled):
```bash
./testRunner
```

Tests cover:
- Customer creation and property updates
- Server state transitions
- Queue FIFO operations
- Input validation edge cases

---

## 📊 Output Examples

### Console Log (`simulation_log.txt`)

```
====== QUEUING SYSTEM SIMULATION LOG ======

-- Clock: 1 --
  Customer 1 arrived & assigned to Server 1
  Customer 2 arrived & assigned to Server 2

-- Clock: 2 --
  Customer 1 departed from Server 1
  Customer 4 arrived & assigned to Server 1
  Customer 5 arrived & joined queue (queue size: 1)

-- Clock: 3 --
  Customer 6 arrived & joined queue (queue size: 2)
  Customer 2 departed from Server 2
  Customer 5 moved from queue to Server 2 (waited 1 ticks)

[Simulation ended — 0 customer(s) still in queue]
```

### GUI Statistics Panel

- **Total arrived:** 47
- **Served:** 47
- **Still waiting:** 0
- **Avg wait:** 2.34 ticks
- **Avg service:** 5.18 ticks
- **Max queue len:** 8

---

## 🔧 Configuration & Customization

### Adjust Simulation Parameters

Edit constants in `src/main.cpp` or `imgui_main.cpp`:

```cpp
struct SimParams {
    int numServers          = 3;      // Default: 3 servers
    int numMinutes          = 60;     // Default: 60 ticks
    int arrivalProbability  = 40;     // Default: 40% per tick
    int minService          = 2;      // Default: 2 ticks
    int maxService          = 8;      // Default: 8 ticks
};
```

### Modify Simulation Logic

- **Arrival process:** Edit random roll in `tickSimulation()` or `runSimulation()`
- **Service distribution:** Change `randRange(minService, maxService)`
- **Statistics tracking:** Add new fields to `SimStats` struct

### Customize GUI Theme

In `imgui_main.cpp`, modify the style:

```cpp
ImGui::StyleColorsDark();  // or: StyleColorsLight(), StyleColorsClassic()
ImGuiStyle& style = ImGui::GetStyle();
style.WindowRounding = 4.f;
style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
```

---

## 🐛 Troubleshooting

### Build Issues

| Problem | Solution |
|---------|----------|
| `error: cannot find customerType.cpp` | Run `cmake ..` again; ensure `src/` subdirectory exists |
| `GLFW not found` | Install `libglfw3-dev` (Linux) or use `vcpkg` (Windows) |
| `OpenGL not found` | Install `libgl1-mesa-dev` and `libx11-dev` (Linux) |
| `make: command not found` | Install `build-essential` (Linux) or use CMake instead |
| Compile error on Windows | Use MSVC and run `cmake -G "Visual Studio 16 2019"` |

### Runtime Issues

| Problem | Solution |
|---------|----------|
| **GUI crashes at startup** | Ensure GLFW window manager is available; test with console version |
| **Queue appears empty** | Check arrival probability is > 0; increase duration to 100+ ticks |
| **Log file not created** | Ensure write permissions in project directory; check disk space |
| **Slow simulation** | Reduce `ticksPerSecond` in `imgui_main.cpp` (default: 10) |

### On macOS/Linux

If you see **"GLFW error: X11 connection failed"**:
```bash
# Ensure X11/Wayland is available
export DISPLAY=:0
./QueueSim
```

---

## 📚 Concepts Demonstrated

This project illustrates:

- **Data Structures:** Linked lists, queues, arrays
- **Simulation:** Discrete-event modeling, random processes
- **Queueing Theory:** Multi-server queue, arrival/service processes
- **C++ Features:** Templates, OOP design, file I/O
- **UI Development:** Immediate-mode GUI (ImGui), OpenGL rendering
- **Statistics:** Mean, max, aggregation; running totals

**Real-world applications:**
- Bank teller lines
- Help desk ticket systems
- Hospital emergency room staffing
- Supermarket checkout optimization
- Server load balancing

---

## 📄 License

See `imgui/LICENSE.txt` for Dear ImGui licensing (MIT).  
Project source code: **Educational use – DS 2026**.

---

## 👤 Author & Contact

**Project:** Queuing System Simulation (DS 2026)  
**Framework:** Dear ImGui, GLFW, OpenGL 3

For questions or issues, refer to the source code comments or rebuild with `cmake -DCMAKE_BUILD_TYPE=Debug` for verbose output.

---

## 🔗 Quick Start Cheat Sheet

```bash
# Console version (fastest)
make && ./queuing_sim

# GUI version
mkdir -p build && cd build && cmake .. && make && ./QueueSim

# Clean everything
make clean && rm -rf build/

# Manual compile (no build system)
g++ -std=c++17 -O2 -o queuing_sim \
  src/{main,customerType,serverType,serverListType,validation,errorHandler}.cpp
```

Enjoy your simulation! 🎉
