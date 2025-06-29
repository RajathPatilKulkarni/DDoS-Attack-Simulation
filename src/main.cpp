#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>

// Network node representation
class Node {
public:
    int id;
    int capacity;         // Maximum packets per second this node can process
    int currentLoad;      // Current load on this node
    bool isAttacker;      // Flag to mark if this is an attacker node
    
    Node(int _id, int _capacity, bool _isAttacker = false) :
        id(_id), capacity(_capacity), currentLoad(0), isAttacker(_isAttacker) {}
    
    bool canHandlePacket() {
        return currentLoad < capacity;
    }
    
    void processPacket() {
        currentLoad++;
    }
    
    void resetLoad() {
        currentLoad = 0;
    }
};

// Packet representation
class Packet {
public:
    int sourceId;
    int destinationId;
    bool isLegitimate;
    int timestamp;
    std::string signature;  // For mitigation techniques that use packet signatures
    
    Packet(int src, int dst, bool legitimate, int time, std::string sig = "") :
        sourceId(src), destinationId(dst), isLegitimate(legitimate),
        timestamp(time), signature(sig) {}
};

// Network simulator
class NetworkSimulator {
private:
    std::vector<Node> nodes;
    std::vector<std::vector<int>> connections;  // Adjacency list for network topology
    std::queue<Packet> packetQueue;
    int timeStep;
    
    // Mitigation strategies
    bool rateLimit;
    bool ipFiltering;
    bool deepPacketInspection;
    bool trafficPatternAnalysis;
    
    // Tracking data for mitigation
    std::unordered_map<int, int> sourcePacketCount;
    std::unordered_map<std::string, int> signatureCount;
    
    // Random number generator
    std::mt19937 rng;
    
public:
    NetworkSimulator(int numNodes, int targetNodeId, int numAttackers) :
        timeStep(0),
        rateLimit(false),
        ipFiltering(false),
        deepPacketInspection(false),
        trafficPatternAnalysis(false) {
        
        // Seed the random number generator
        std::random_device rd;
        rng = std::mt19937(rd());
        
        // Initialize nodes
        for (int i = 0; i < numNodes; i++) {
            // Determine if this node is an attacker
            bool isAttacker = (i < numAttackers);
            
            // Set capacity - target node has higher capacity
            int capacity = (i == targetNodeId) ? 1000 : 500;
            
            nodes.push_back(Node(i, capacity, isAttacker));
        }
        
        // Initialize connections (fully connected network for simplicity)
        connections.resize(numNodes);
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                if (i != j) {
                    connections[i].push_back(j);
                }
            }
        }
    }
    
    // Enable different mitigation strategies
    void enableRateLimiting(bool enable) { rateLimit = enable; }
    void enableIPFiltering(bool enable) { ipFiltering = enable; }
    void enableDeepPacketInspection(bool enable) { deepPacketInspection = enable; }
    void enableTrafficPatternAnalysis(bool enable) { trafficPatternAnalysis = enable; }
    
    // Generate traffic (both legitimate and attack)
    void generateTraffic(int targetNodeId, double attackIntensity, int legitimateTraffic) {
        // Generate legitimate traffic
        for (int i = 0; i < legitimateTraffic; i++) {
            // Pick a random source node that's not an attacker
            int sourceId = -1;
            do {
                sourceId = std::uniform_int_distribution<>(0, nodes.size() - 1)(rng);
            } while (nodes[sourceId].isAttacker);
            
            packetQueue.push(Packet(sourceId, targetNodeId, true, timeStep, "legitimate"));
        }
        
        // Generate attack traffic
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].isAttacker) {
                int attackPackets = static_cast<int>(attackIntensity * nodes[i].capacity);
                for (int j = 0; j < attackPackets; j++) {
                    std::string signature = "attack_" + std::to_string(i);
                    packetQueue.push(Packet(i, targetNodeId, false, timeStep, signature));
                }
            }
        }
    }
    
    // Process packets with mitigation techniques
    void processTraffic() {
        int packetsDropped = 0;
        int legitimateDropped = 0;
        int attackDropped = 0;
        int packetsProcessed = 0;
        int legitimateProcessed = 0;
        int attackProcessed = 0;
        
        // Reset node loads
        for (auto& node : nodes) {
            node.resetLoad();
        }
        
        // Process current packet queue
        int queueSize = packetQueue.size();
        for (int i = 0; i < queueSize; i++) {
            Packet packet = packetQueue.front();
            packetQueue.pop();
            
            bool dropPacket = false;
            
            // Apply mitigation techniques
            if (ipFiltering && !packet.isLegitimate) {
                sourcePacketCount[packet.sourceId]++;
                // Simple threshold-based filtering
                if (sourcePacketCount[packet.sourceId] > 100) {
                    dropPacket = true;
                }
            }
            
            if (deepPacketInspection && !dropPacket) {
                // Count signature occurrences for potential blocking
                signatureCount[packet.signature]++;
                // Block if signature appears too frequently
                if (packet.signature.find("attack") != std::string::npos &&
                    signatureCount[packet.signature] > 50) {
                    dropPacket = true;
                }
            }
            
            if (rateLimit && !dropPacket) {
                // Implement token bucket algorithm by checking if destination can handle packet
                if (!nodes[packet.destinationId].canHandlePacket()) {
                    dropPacket = true;
                }
            }
            
            if (trafficPatternAnalysis && !dropPacket) {
                // Simple pattern analysis - if too many packets from one source in short time
                if (timeStep - packet.timestamp < 5 && sourcePacketCount[packet.sourceId] > 200) {
                    dropPacket = true;
                }
            }
            
            // Process or drop the packet
            if (dropPacket) {
                packetsDropped++;
                if (packet.isLegitimate) legitimateDropped++;
                else attackDropped++;
            } else {
                nodes[packet.destinationId].processPacket();
                packetsProcessed++;
                if (packet.isLegitimate) legitimateProcessed++;
                else attackProcessed++;
            }
        }
        
        // Print statistics
        std::cout << "Time step: " << timeStep << std::endl;
        std::cout << "Packets processed: " << packetsProcessed
                  << " (Legitimate: " << legitimateProcessed
                  << ", Attack: " << attackProcessed << ")" << std::endl;
        std::cout << "Packets dropped: " << packetsDropped
                  << " (Legitimate: " << legitimateDropped
                  << ", Attack: " << attackDropped << ")" << std::endl;
        std::cout << "Target node load: " << nodes[0].currentLoad
                  << "/" << nodes[0].capacity << std::endl;
        std::cout << "----------------------------------" << std::endl;
        
        // Increment time
        timeStep++;
    }
    
    // Run the simulation
    void runSimulation(int steps, int targetNodeId, double attackIntensity, int legitimateTraffic) {
        for (int i = 0; i < steps; i++) {
            generateTraffic(targetNodeId, attackIntensity, legitimateTraffic);
            processTraffic();
        }
    }
};

int main() {
    int numNodes = 50;       // Total number of nodes in the network
    int numAttackers = 10;   // Number of attacker nodes
    int targetNodeId = 0;    // ID of the target node
    int simSteps = 10;       // Number of simulation steps
    double attackIntensity = 2.0;  // Attack traffic intensity (multiplier of node capacity)
    int legitimateTraffic = 100;   // Amount of legitimate traffic per step
    
    std::cout << "=== DDoS Attack Simulation ===" << std::endl;
    std::cout << "Network configuration: " << numNodes << " nodes, "
              << numAttackers << " attackers, target node: " << targetNodeId << std::endl;
    
    // Run simulation without mitigation
    std::cout << "\n=== Without Mitigation ===" << std::endl;
    NetworkSimulator simNoMitigation(numNodes, targetNodeId, numAttackers);
    simNoMitigation.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    // Run simulation with rate limiting
    std::cout << "\n=== With Rate Limiting ===" << std::endl;
    NetworkSimulator simRateLimit(numNodes, targetNodeId, numAttackers);
    simRateLimit.enableRateLimiting(true);
    simRateLimit.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    // Run simulation with IP filtering
    std::cout << "\n=== With IP Filtering ===" << std::endl;
    NetworkSimulator simIPFiltering(numNodes, targetNodeId, numAttackers);
    simIPFiltering.enableIPFiltering(true);
    simIPFiltering.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    // Run simulation with Deep Packet Inspection
    std::cout << "\n=== With Deep Packet Inspection ===" << std::endl;
    NetworkSimulator simDPI(numNodes, targetNodeId, numAttackers);
    simDPI.enableDeepPacketInspection(true);
    simDPI.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    // Run simulation with Traffic Pattern Analysis
    std::cout << "\n=== With Traffic Pattern Analysis ===" << std::endl;
    NetworkSimulator simTPA(numNodes, targetNodeId, numAttackers);
    simTPA.enableTrafficPatternAnalysis(true);
    simTPA.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    // Run simulation with all mitigation techniques
    std::cout << "\n=== With All Mitigation Techniques ===" << std::endl;
    NetworkSimulator simAll(numNodes, targetNodeId, numAttackers);
    simAll.enableRateLimiting(true);
    simAll.enableIPFiltering(true);
    simAll.enableDeepPacketInspection(true);
    simAll.enableTrafficPatternAnalysis(true);
    simAll.runSimulation(simSteps, targetNodeId, attackIntensity, legitimateTraffic);
    
    return 0;
} 
