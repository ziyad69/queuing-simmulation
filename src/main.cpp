#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "customerType.h"
#include "serverType.h"
#include "serverListType.h"
#include "Queue.h"
#include "validation.h"
#include "errorHandler.h"

// ─── Simulation parameters (read from user) ──────────────────────────────────
struct SimParams {
    int numServers;
    int numMinutes;        // total clock ticks
    int arrivalProbability; // 1..100  (% chance a customer arrives each tick)
    int minService;        // minimum transaction time
    int maxService;        // maximum transaction time
};

// ─── Statistics collected during the run ─────────────────────────────────────
struct SimStats {
    int totalCustomers    = 0;
    int totalWaitTime     = 0;
    int totalServiceTime  = 0;
    int customersServed   = 0;
    int maxQueueLength    = 0;
    int totalQueueLength  = 0;   // accumulated every tick (for average)
};

// ─── Helper: random int in [lo, hi] ──────────────────────────────────────────
int randRange(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

// ─── Core simulation ──────────────────────────────────────────────────────────
void runSimulation(const SimParams& p, SimStats& stats, std::ostream& log) {
    serverListType servers(p.numServers);
    Queue<customerType> waitingQueue;

    int customerCounter = 0;

    log << "\n====== QUEUING SYSTEM SIMULATION LOG ======\n\n";

    for (int clock = 1; clock <= p.numMinutes; clock++) {
        log << "-- Clock: " << std::setw(4) << clock << " --\n";

        // 1. Update servers (decrement timers, release finished customers)
        servers.updateServers(log);

        // 2. Possibly add a new arrival
        int roll = randRange(1, 100);
        if (roll <= p.arrivalProbability) {
            customerCounter++;
            int serviceTime = randRange(p.minService, p.maxService);

            customerType newCust;
            newCust.setCustomerInfo(customerCounter, clock, 0, serviceTime);
            stats.totalCustomers++;
            stats.totalServiceTime += serviceTime;

            int freeID = servers.getFreeServerID();
            if (freeID != -1) {
                // Send directly to a free server
                log << "  Customer " << customerCounter
                    << " arrived & assigned to Server " << (freeID + 1) << "\n";
                servers.setServerBusy(freeID, newCust);
                stats.customersServed++;
            } else {
                // All servers busy → join queue
                waitingQueue.enqueue(newCust);
                log << "  Customer " << customerCounter
                    << " arrived & joined queue (queue size: "
                    << waitingQueue.size() << ")\n";
            }
        }

        // 3. Try to dispatch waiting customers to newly freed servers
        while (!waitingQueue.isEmpty()) {
            int freeID = servers.getFreeServerID();
            if (freeID == -1) break;

            customerType nextCust = waitingQueue.front();
            waitingQueue.dequeue();

            // Record waiting time
            int waited = clock - nextCust.getArrivalTime();
            nextCust.setWaitingTime(waited);
            stats.totalWaitTime += waited;

            log << "  Customer " << nextCust.getCustomerNumber()
                << " moved from queue to Server " << (freeID + 1)
                << " (waited " << waited << " ticks)\n";

            servers.setServerBusy(freeID, nextCust);
            stats.customersServed++;
        }

        // 4. Collect queue-length stat
        int qLen = waitingQueue.size();
        stats.totalQueueLength += qLen;
        if (qLen > stats.maxQueueLength) stats.maxQueueLength = qLen;
    }

    // Customers still in queue at end of simulation
    log << "\n[Simulation ended — "
        << waitingQueue.size() << " customer(s) still in queue]\n";
}

// ─── Report ───────────────────────────────────────────────────────────────────
void printReport(const SimParams& p, const SimStats& s, std::ostream& out) {
    double avgWait    = s.customersServed  > 0 ? (double)s.totalWaitTime  / s.customersServed : 0.0;
    double avgService = s.customersServed  > 0 ? (double)s.totalServiceTime / s.customersServed : 0.0;
    double avgQueue   = p.numMinutes > 0       ? (double)s.totalQueueLength / p.numMinutes : 0.0;

    out << "\n╔══════════════════════════════════════════╗\n";
    out <<   "║        SIMULATION SUMMARY REPORT         ║\n";
    out << "╚══════════════════════════════════════════╝\n";
    out << "  Number of servers       : " << p.numServers      << "\n";
    out << "  Simulated minutes       : " << p.numMinutes      << "\n";
    out << "  Arrival probability     : " << p.arrivalProbability << "%\n";
    out << "  Service time range      : [" << p.minService << ", " << p.maxService << "]\n";
    out << "\n";
    out << "  Total customers arrived : " << s.totalCustomers   << "\n";
    out << "  Customers served        : " << s.customersServed   << "\n";
    out << "  Customers still waiting : " << (s.totalCustomers - s.customersServed) << "\n";
    out << "\n";
    out << std::fixed << std::setprecision(2);
    out << "  Avg waiting time        : " << avgWait   << " ticks\n";
    out << "  Avg service time        : " << avgService << " ticks\n";
    out << "  Avg queue length        : " << avgQueue   << " customers\n";
    out << "  Max queue length        : " << s.maxQueueLength << "\n";
    out << "\n";
}

// ─── Input helpers ────────────────────────────────────────────────────────────
int readInt(const std::string& prompt, int lo, int hi) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;

        ValidationResult result = validate(input, lo, hi);

        if (result.code == ErrorCode::SUCCESS) return result.value;

        // Delegate error printing to ErrorHandler
        ErrorHandler::handle(result.code, lo, hi);
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║   QUEUING SYSTEM SIMULATION  (DS 2026)   ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n\n";

    SimParams p;
    p.numServers        = readInt("Enter number of servers         [1-10]  : ", 1, 10);
    p.numMinutes        = readInt("Enter simulation duration (ticks)[1-1000]: ", 1, 1000);
    p.arrivalProbability= readInt("Enter arrival probability %      [1-100] : ", 1, 100);
    p.minService        = readInt("Enter min service time (ticks)   [1-50]  : ", 1, 50);
    p.maxService        = readInt("Enter max service time (ticks)   [1-100] : ", p.minService, 100);

    std::cout << "\nSave detailed log to file? (y/n): ";
    char choice; std::cin >> choice;

    SimStats stats;

    if (choice == 'y' || choice == 'Y') {
        std::string filename = "simulation_log.txt";
        std::ofstream logFile(filename);
        if (!logFile) {
            std::cerr << "Cannot open " << filename << " — logging to console.\n";
            runSimulation(p, stats, std::cout);
        } else {
            runSimulation(p, stats, logFile);
            printReport(p, stats, logFile);
            logFile.close();
            std::cout << "\nDetailed log saved to: " << filename << "\n";
        }
    } else {
        runSimulation(p, stats, std::cout);
    }

    printReport(p, stats, std::cout);

    return 0;
}
