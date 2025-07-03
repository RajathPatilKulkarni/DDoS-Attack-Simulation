# 🛡️ DDoS Attack Simulation in C++

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green)](LICENSE)
[![Simulated](https://img.shields.io/badge/Simulated-DDoS%20Attack-critical)](#)
[![Mitigation](https://img.shields.io/badge/Mitigation-Rate%20Limiting%2C%20DPI-lightgrey)](#)

This project simulates a Distributed Denial-of-Service (DDoS) attack in a network of nodes using C++. It models legitimate and malicious traffic, processes packets, and applies mitigation strategies to analyze how a target system handles attack scenarios.

> ⚠️ For educational and ethical use only. This simulation is intended solely for research and learning purposes.

---

## 📌 Features

- Node-based virtual network simulation
- Simulates attacker vs legitimate traffic sources
- Supports various DDoS mitigation techniques:
  - ✅ Rate Limiting
  - ✅ IP Filtering
  - ✅ Deep Packet Inspection (DPI)
  - ✅ Traffic Pattern Analysis
- Packet-level processing and drop stats
- Modular design to easily add more techniques

---

## 🖥️ How It Works

- Nodes generate packets targeting a specific node (victim).
- Attackers generate high-volume traffic based on configurable intensity.
- The simulator evaluates the effect of each mitigation strategy across multiple time steps.
- Statistics on packets processed, dropped, and load on the target node are logged for each step.

---

## 🛠️ Setup & Run

### 🧾 Prerequisites
- C++17 compatible compiler (e.g. g++, clang++)
- Linux/MacOS/Windows Terminal

### 🧪 Compile

```bash
g++ src/main.cpp -o ddos_simulation
```

### ▶️ Run

```bash
./ddos_simulation
```

---

## 📊 Sample Output

```
=== DDoS Attack Simulation ===
Network configuration: 50 nodes, 10 attackers, target node: 0

=== Without Mitigation ===
Time step: 0
Packets processed: 400 (Legitimate: 100, Attack: 300)
Packets dropped: 600 (Legitimate: 0, Attack: 600)
Target node load: 1000/1000
----------------------------------
```

---

## 📁 Project Structure

```
ddos-attack-simulation/
├── src/
│   └── main.cpp           # Main simulation code
├── README.md              # Project documentation
├── LICENSE                # MIT License
```

---

## 🧠 Topics

- `c++`
- `ddos`
- `network-simulation`
- `cybersecurity`
- `network-security`
- `mitigation-strategies`
- `ethical-hacking`
- `packet-analysis`

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.

---

## 👨‍💻 Author

**Rajath Patil Kulkarni**

[GitHub](https://github.com/RajathPatilKulkarni) · [LinkedIn](https://www.linkedin.com/in/rajathpatilkulkarni)
