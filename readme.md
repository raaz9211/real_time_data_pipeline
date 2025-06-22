# Real-Time Data Processing Pipeline (C++)

A high-performance, multithreaded real-time data pipeline written in modern C++ for ingesting, processing, storing, and streaming data.

---

## ✨ Architecture Diagram

```text
                    +------------------------+
                    |  Command Line Parser   |
                    +------------------------+
                              |
                              v
+------------+      +-------------------+     +-------------------+
| Data Source|----->| ThreadSafe/RingBuf|<----|   Data Processor  |
| (File/API) |      |       Queue       |     +-------------------+
+------------+      +--------+----------+              |
                              |                         v
                    +---------v----------+     +--------------------+
                    | Output Writer      |<----|   Metrics Collector |
                    | (File / DB / WS)   |     +--------------------+
                    +---------+----------+
                              |
                              v
                 +---------------------------+
                 | Prometheus + WebSocket UI |
                 +---------------------------+
```

---

## 💪 Features

* Multi-threaded with `ThreadPool`
* `ThreadSafeQueue` and `RingBufferQueue` for concurrency control
* File and API data ingestion
* Modular DataProcessor with strategy pattern
* SQLite storage via `DatabaseWriter`
* Real-time streaming via `WebSocketClient`
* Prometheus-style metrics server
* GoogleTest framework for unit tests

---

## ⚙️ Build & Run

### Prerequisites

* g++ (C++17+)
* CMake >= 3.15
* libcurl
* sqlite3
* GoogleTest (for tests)

Install dependencies on Ubuntu:

```bash
sudo apt update
sudo apt install g++ cmake libcurl4-openssl-dev libsqlite3-dev
```

### Build

```bash
git clone https://github.com/yourname/real_time_data_pipeline.git
cd real_time_data_pipeline
mkdir build && cd build
cmake ..
make
```

### Run

```bash
./main_pipeline --config ../config/config.json --data_source file --verbose
```

### Test

```bash
./test_runner
```

---

## 📊 Example

### Input (example file)

```
123,Temp=36.4C,Status=OK
124,Temp=37.0C,Status=Warning
```

### Output

```
[Processed] {"id":123,"temp":"36.4C","status":"OK"}
[Processed] {"id":124,"temp":"37.0C","status":"Warning"}
```

Also streamed to WebSocket + stored in SQLite DB + logged to file.

---

## 🛠️ Technologies Used

* **C++17**: Modern language features, smart pointers
* **CMake**: Build system
* **libcurl**: HTTP API support
* **SQLite**: Embedded database
* **pthread**: Multithreading
* **Prometheus-style Metrics**: Simple HTTP endpoint
* **ixwebsocket / websocketpp**: Real-time WebSocket communication
* **GoogleTest**: Unit testing

---

## 📦 Directory Structure

```
real_time_data_pipeline/
├── src/                   # All core logic modules
├── include/               # Header files
├── third_party/           # External libs (ixwebsocket)
├── data/                  # Input and output data
├── config/                # Config files
├── logs/                  # Logs
├── tests/                 # Unit tests
└── CMakeLists.txt
```

---

## 🔍 Shell Scripts (Optional)

#### `build.sh`

```bash
#!/bin/bash
mkdir -p build && cd build
cmake .. && make
```

#### `run.sh`

```bash
#!/bin/bash
./build/main_pipeline --config config/config.json --data_source=<file/api> --verbose
```

Make executable:

```bash
chmod +x build.sh run.sh
```

---

## 🚀 Future Improvements

* Kafka/MQ ingestion
* Real-time dashboard with charts
* Docker deployment
* REST endpoint to query results

---

## ✨ Maintainer

**Raj Kumar Singh**

---

## 📅 License

MIT License.

---
