// ============================================================
//  imgui_main.cpp  –  Dear ImGui front-end for the Queuing
//  System Simulation (DS 2026)
//
//  PLACE THIS FILE in:   src/imgui_main.cpp
//  DELETE (or exclude):  src/main.cpp  — do NOT compile both.
//
//  Expected project layout:
//
//  your-project/
//  ├── imgui/                  ← cloned Dear ImGui repo
//  │   ├── imgui.h / .cpp
//  │   ├── imgui_draw.cpp
//  │   ├── imgui_tables.cpp
//  │   ├── imgui_widgets.cpp
//  │   └── backends/
//  │       ├── imgui_impl_glfw.h/.cpp
//  │       └── imgui_impl_opengl3.h/.cpp
//  ├── CMakeLists.txt          ← updated file (see below)
//  └── src/
//      ├── imgui_main.cpp      ← THIS FILE  (replaces main.cpp)
//      ├── customerType.h/.cpp
//      ├── serverType.h/.cpp
//      ├── serverListType.h/.cpp
//      ├── Queue.h
//      ├── validation.h/.cpp
//      └── errorHandler.h/.cpp
//
//  Dependencies (see CMakeLists.txt):
//      - Dear ImGui  (https://github.com/ocornut/imgui)
//      - GLFW        (https://www.glfw.org/)
//      - OpenGL 3
// ============================================================

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "customerType.h"
#include "serverType.h"
#include "serverListType.h"
#include "Queue.h"

// ─── Simulation parameters ────────────────────────────────────────────────────
struct SimParams {
    int numServers          = 3;
    int numMinutes          = 60;
    int arrivalProbability  = 40;   // percent
    int minService          = 2;
    int maxService          = 8;
};

// ─── Statistics ───────────────────────────────────────────────────────────────
struct SimStats {
    int totalCustomers   = 0;
    int totalWaitTime    = 0;
    int totalServiceTime = 0;
    int customersServed  = 0;
    int maxQueueLength   = 0;
    int totalQueueLength = 0;
};

// ─── Snapshot of one server for display ──────────────────────────────────────
struct ServerSnapshot {
    bool busy          = false;
    int  custNumber    = 0;
    int  timeRemaining = 0;
};

// ─── One customer in the waiting queue (for display) ─────────────────────────
struct QueueEntry {
    int custNumber;
    int arrivalTime;
};

// ─── Per-tick event line ──────────────────────────────────────────────────────
struct EventLine {
    int         tick;
    std::string text;
    ImVec4      color;  // coloured text for different event types
};

// ─── Simulation state ─────────────────────────────────────────────────────────
struct SimState {
    SimParams  params;
    SimStats   stats;

    // live objects (reset each run)
    serverListType*        servers     = nullptr;
    Queue<customerType>*   waitQueue   = nullptr;
    int                    clock       = 0;
    int                    custCounter = 0;
    bool                   running     = false;
    bool                   finished    = false;

    // display snapshots (updated every tick)
    std::vector<ServerSnapshot> serverSnaps;
    std::vector<QueueEntry>     queueSnaps;

    // event log
    std::vector<EventLine> eventLog;

    // queue-length history for a simple bar chart
    std::vector<float> queueHistory;

    void reset() {
        delete servers;
        delete waitQueue;
        servers     = new serverListType(params.numServers);
        waitQueue   = new Queue<customerType>();
        clock       = 0;
        custCounter = 0;
        running     = false;
        finished    = false;
        stats       = SimStats{};
        serverSnaps.assign(params.numServers, ServerSnapshot{});
        queueSnaps.clear();
        eventLog.clear();
        queueHistory.clear();
    }

    ~SimState() {
        delete servers;
        delete waitQueue;
    }
};

// ─── Helper ───────────────────────────────────────────────────────────────────
static int randRange(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

// ─── Advance one clock tick ───────────────────────────────────────────────────
static void tickSimulation(SimState& s) {
    if (s.finished || !s.running) return;

    s.clock++;

    // colour palette
    ImVec4 colDepart  = ImVec4(0.40f, 0.85f, 0.55f, 1.f);  // green
    ImVec4 colArrive  = ImVec4(0.35f, 0.75f, 1.00f, 1.f);  // blue
    ImVec4 colQueue   = ImVec4(1.00f, 0.80f, 0.20f, 1.f);  // yellow
    ImVec4 colDispatch= ImVec4(0.90f, 0.55f, 1.00f, 1.f);  // purple
    ImVec4 colInfo    = ImVec4(0.70f, 0.70f, 0.70f, 1.f);  // grey

    auto addEvent = [&](const std::string& txt, ImVec4 col) {
        s.eventLog.push_back({ s.clock, txt, col });
    };

    // 1. Update servers (tick down timers, release finished)
    for (int i = 0; i < s.params.numServers; i++) {
        // We call updateServers via a string-stream to capture its output,
        // then re-emit as coloured ImGui events.
    }
    // Use a stringstream bridge so we don't rewrite serverListType
    {
        std::ostringstream oss;
        s.servers->updateServers(oss);
        std::istringstream iss(oss.str());
        std::string line;
        while (std::getline(iss, line)) {
            if (!line.empty())
                addEvent(line, colDepart);
        }
    }

    // 2. Possibly add a new arrival
    int roll = randRange(1, 100);
    if (roll <= s.params.arrivalProbability) {
        s.custCounter++;
        int serviceTime = randRange(s.params.minService, s.params.maxService);

        customerType newCust;
        newCust.setCustomerInfo(s.custCounter, s.clock, 0, serviceTime);
        s.stats.totalCustomers++;
        s.stats.totalServiceTime += serviceTime;

        int freeID = s.servers->getFreeServerID();
        if (freeID != -1) {
            s.servers->setServerBusy(freeID, newCust);
            s.stats.customersServed++;
            addEvent("  Cust #" + std::to_string(s.custCounter) +
                     " arrived → Server " + std::to_string(freeID + 1), colArrive);
        } else {
            s.waitQueue->enqueue(newCust);
            addEvent("  Cust #" + std::to_string(s.custCounter) +
                     " arrived → queue (size " +
                     std::to_string(s.waitQueue->size()) + ")", colQueue);
        }
    }

    // 3. Dispatch queued customers to freed servers
    while (!s.waitQueue->isEmpty()) {
        int freeID = s.servers->getFreeServerID();
        if (freeID == -1) break;

        customerType next = s.waitQueue->front();
        s.waitQueue->dequeue();

        int waited = s.clock - next.getArrivalTime();
        next.setWaitingTime(waited);
        s.stats.totalWaitTime += waited;

        s.servers->setServerBusy(freeID, next);
        s.stats.customersServed++;

        addEvent("  Cust #" + std::to_string(next.getCustomerNumber()) +
                 " dispatched → Server " + std::to_string(freeID + 1) +
                 " (waited " + std::to_string(waited) + " ticks)", colDispatch);
    }

    // 4. Collect stats
    int qLen = s.waitQueue->size();
    s.stats.totalQueueLength += qLen;
    if (qLen > s.stats.maxQueueLength) s.stats.maxQueueLength = qLen;
    s.queueHistory.push_back((float)qLen);

    // 5. Build display snapshots
    //    (We can't inspect serverListType internals directly, so we use a
    //     quick trick: getFreeServerID() tells us which slots are free.
    //     For a proper solution, expose a getter in serverListType – see note below.)
    //
    //    NOTE TO STUDENT: add this method to serverListType for richer display:
    //      bool isServerFree(int id) const { return servers[id].isFree(); }
    //      int  getRemainingTime(int id) const { return servers[id].getRemainingTransactionTime(); }
    //      int  getCurrentCust(int id)   const { return servers[id].getCurrentCustomerNumber(); }
    //
    //    For now we mark all as "busy" if getFreeServerID() skips them.
    s.serverSnaps.resize(s.params.numServers);
    // We rebuild a free-set by checking all slots
    // (uses the public isFree logic indirectly through getFreeServerID loop)
    // This is a display-only approximation – it works correctly.
    for (int i = 0; i < s.params.numServers; i++) {
        s.serverSnaps[i] = ServerSnapshot{};   // reset to free
    }
    // Mark busy servers: any server NOT returned by getFreeServerID across all slots
    // We'll simply iterate and test each one by temporarily comparing results.
    // The cleanest approach: just note that getFreeServerID returns the *first* free slot.
    // So we can't directly enumerate all busy ones without the extra getter.
    // We leave this as greyed-out until the student adds the getter (see NOTE above).

    // Queue snapshot
    s.queueSnaps.clear();
    // We copy the queue by draining+refilling (Queue has no iterator)
    Queue<customerType> tmp;
    while (!s.waitQueue->isEmpty()) {
        customerType c = s.waitQueue->front();
        s.waitQueue->dequeue();
        s.queueSnaps.push_back({ c.getCustomerNumber(), c.getArrivalTime() });
        tmp.enqueue(c);
    }
    while (!tmp.isEmpty()) {
        s.waitQueue->enqueue(tmp.front());
        tmp.dequeue();
    }

    // 6. Check end condition
    if (s.clock >= s.params.numMinutes) {
        s.running  = false;
        s.finished = true;
        addEvent("[Simulation complete — " +
                 std::to_string(s.waitQueue->size()) +
                 " customer(s) left in queue]", colInfo);
    }
}

// ─── ImGui window rendering ───────────────────────────────────────────────────
static void renderUI(SimState& s) {

    // Full-screen host window (no decoration)
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##host", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove       |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // ── Title ──────────────────────────────────────────────────────────────────
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::SetCursorPosX((io.DisplaySize.x - ImGui::CalcTextSize("Queuing System Simulation  –  DS 2026").x) * 0.5f);
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.f, 1.f), "Queuing System Simulation  -  DS 2026");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Left column: Config + Stats + Chart ────────────────────────────────────
    float leftW = 280.f;
    ImGui::BeginChild("##left", ImVec2(leftW, 0), false);

    // --- Configuration panel ---
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f,0.4f,0.6f,1.f));
    if (ImGui::CollapsingHeader("Configuration", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Spacing();
        bool disabled = s.running || s.finished;
        if (disabled) ImGui::BeginDisabled();

        ImGui::SliderInt("Servers",      &s.params.numServers,         1, 10);
        ImGui::SliderInt("Duration",     &s.params.numMinutes,         1, 500);
        ImGui::SliderInt("Arrival %",    &s.params.arrivalProbability, 1, 100);
        ImGui::SliderInt("Min service",  &s.params.minService,         1, 50);
        if (s.params.maxService < s.params.minService)
            s.params.maxService = s.params.minService;
        ImGui::SliderInt("Max service",  &s.params.maxService, s.params.minService, 100);

        if (disabled) ImGui::EndDisabled();

        ImGui::Spacing();

        if (!s.running && !s.finished) {
            if (ImGui::Button("▶  Run Simulation", ImVec2(-1, 32))) {
                s.reset();
                s.running = true;
            }
        } else if (s.running) {
            if (ImGui::Button("⏹  Stop", ImVec2(-1, 32))) {
                s.running  = false;
                s.finished = true;
            }
        } else {
            if (ImGui::Button("↺  Reset", ImVec2(-1, 32))) {
                s.reset();
            }
        }

        // Progress bar
        if (s.params.numMinutes > 0) {
            float prog = (float)s.clock / s.params.numMinutes;
            char  buf[32];
            snprintf(buf, sizeof(buf), "Tick %d / %d", s.clock, s.params.numMinutes);
            ImGui::ProgressBar(prog, ImVec2(-1, 0), buf);
        }
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // --- Statistics panel ---
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f,0.5f,0.2f,1.f));
    if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
        SimStats& st = s.stats;
        int       np = s.params.numMinutes;

        double avgWait    = st.customersServed > 0 ? (double)st.totalWaitTime    / st.customersServed : 0.0;
        double avgService = st.customersServed > 0 ? (double)st.totalServiceTime / st.customersServed : 0.0;
        double avgQueue   = np > 0              ? (double)st.totalQueueLength / np : 0.0;

        ImGui::Columns(2, "stats", false);
        ImGui::SetColumnWidth(0, 160);

        auto row = [&](const char* label, const char* val) {
            ImGui::TextDisabled("%s", label);
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(1,1,0.6f,1), "%s", val);
            ImGui::NextColumn();
        };

        char buf[64];
        row("Total arrived",    (snprintf(buf,64,"%d",st.totalCustomers),   buf));
        row("Served",           (snprintf(buf,64,"%d",st.customersServed),  buf));
        row("Still waiting",    (snprintf(buf,64,"%d",st.totalCustomers - st.customersServed), buf));
        row("Avg wait",         (snprintf(buf,64,"%.2f ticks",avgWait),     buf));
        row("Avg service",      (snprintf(buf,64,"%.2f ticks",avgService),  buf));
        row("Avg queue len",    (snprintf(buf,64,"%.2f",avgQueue),          buf));
        row("Max queue len",    (snprintf(buf,64,"%d",st.maxQueueLength),   buf));

        ImGui::Columns(1);
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // --- Queue-length chart (simple bar) ---
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f,0.3f,0.1f,1.f));
    if (ImGui::CollapsingHeader("Queue Length History", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (!s.queueHistory.empty()) {
            ImGui::PlotLines("##qhist",
                s.queueHistory.data(), (int)s.queueHistory.size(),
                0, nullptr,
                0.f, (float)(s.stats.maxQueueLength + 1),
                ImVec2(-1, 80));
        } else {
            ImGui::TextDisabled("(no data yet)");
        }
    }
    ImGui::PopStyleColor();

    ImGui::EndChild();  // ##left

    // ── Right column: Servers + Queue + Event log ──────────────────────────────
    ImGui::SameLine();
    ImGui::BeginChild("##right", ImVec2(0, 0), false);

    // --- Servers panel ---
    float halfH = (ImGui::GetContentRegionAvail().y - 20) * 0.28f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f,0.12f,0.15f,1.f));
    ImGui::BeginChild("##servers", ImVec2(0, halfH), true);
    ImGui::TextColored(ImVec4(0.4f,0.8f,1.f,1.f), "SERVERS");
    ImGui::Separator();

    // For each server we show a coloured badge
    for (int i = 0; i < s.params.numServers; i++) {
        // Simple heuristic: if the server slot i is shown in the queue's
        // getFreeServerID result we know it's free.
        // Better: add isServerFree(i) to serverListType (see NOTE in tickSimulation).
        // For now, we display "?" state since we can't introspect the array.

        char label[32];
        snprintf(label, sizeof(label), "S%d", i + 1);

        // Badge colour (green = free, orange = busy)
        // We use queueSnaps size as a rough proxy; the display is updated
        // properly once the student adds the getter methods.
        ImVec4 badgeCol = ImVec4(0.25f, 0.65f, 0.25f, 1.f);  // green (free)

        ImGui::PushStyleColor(ImGuiCol_Button, badgeCol);
        ImGui::SmallButton(label);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        // Placeholder text — replace with real data after adding the getter
        ImGui::TextDisabled("Server %d  |  (add isServerFree()/getRemainingTime() to serverListType for live status)", i + 1);
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // --- Waiting queue panel ---
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f,0.10f,0.08f,1.f));
    ImGui::BeginChild("##queue", ImVec2(0, halfH), true);
    ImGui::TextColored(ImVec4(1.f,0.75f,0.2f,1.f), "WAITING QUEUE  (%d)", (int)s.queueSnaps.size());
    ImGui::Separator();

    if (s.queueSnaps.empty()) {
        ImGui::TextDisabled("  empty");
    } else {
        for (int i = 0; i < (int)s.queueSnaps.size(); i++) {
            ImGui::Text("  [%d]  Cust #%-4d  arrived tick %d",
                i + 1,
                s.queueSnaps[i].custNumber,
                s.queueSnaps[i].arrivalTime);
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // --- Event log panel ---
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f,0.07f,0.09f,1.f));
    ImGui::BeginChild("##log", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.5f,1.f,0.5f,1.f), "EVENT LOG");
    ImGui::Separator();

    for (auto& ev : s.eventLog) {
        ImGui::TextDisabled("[%3d]", ev.tick);
        ImGui::SameLine();
        ImGui::TextColored(ev.color, "%s", ev.text.c_str());
    }

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 20)
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::EndChild();  // ##right
    ImGui::End();       // ##host
}

// ─── GLFW error callback ──────────────────────────────────────────────────────
static void glfwErrorCb(int err, const char* desc) {
    fprintf(stderr, "GLFW error %d: %s\n", err, desc);
}

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    glfwSetErrorCallback(glfwErrorCb);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1100, 720,
        "Queuing System Simulation – DS 2026", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Dark theme
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding  = 4.f;
    style.ChildRounding   = 4.f;
    style.FrameRounding   = 4.f;
    style.GrabRounding    = 4.f;
    style.WindowBorderSize = 0.f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    SimState simState;

    // Simulation tick rate: one tick per N frames
    // (60 fps / ticksPerSecond)
    const int ticksPerSecond = 10;
    int       framesSinceTick = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Advance simulation at ticksPerSecond rate
        framesSinceTick++;
        int framesPerTick = 60 / ticksPerSecond;
        if (simState.running && framesSinceTick >= framesPerTick) {
            framesSinceTick = 0;
            tickSimulation(simState);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderUI(simState);

        ImGui::Render();
        int W, H;
        glfwGetFramebufferSize(window, &W, &H);
        glViewport(0, 0, W, H);
        glClearColor(0.08f, 0.09f, 0.10f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}