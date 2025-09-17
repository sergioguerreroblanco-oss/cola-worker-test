# Cola & Worker

Cola & Worker is a C++ project that demonstrates a producer–consumer pattern with a bounded, thread-safe queue and multiple worker threads.
It has been designed as part of a technical test, but structured in a professional way, including **templates, abstract interfaces, unit tests, logging, Docker support and CI/CD pipelines.**

---

## ✨ Features

- **Thread-safe bounded queue** (```Cola<T>```) implemented as a C++14 template with configurable maximum size (Default: 5 elements).
- **Multiple workers** (```Worker<T>```) running in independent threads, consuming data concurrently.
- **Abstract interface** (```IWorkerAction```) to decouple worker logic from concrete actions.
- **Concrete action** (```PrintWorkerAction```) that logs worker events.
- **Logger** utility for thread-safe logs with levels (INFO, WARN, ERROR).
- **Graceful shutdown mechanism**: workers wake up and exit cleanly.
- **Unit tests** with GoogleTest, integrated into CMake.
- **Reproducible builds** with Docker.
- **Continuous Integration** with GitHub Actions (build + test + Docker validation).

---

## 🏗 Architecture
The following diagram illustrates the internal architecture of the project.
- **Main Thread** simulates a producer pushing values into the queue at fixed intervals.
- **Cola<T>** is a thread-safe bounded queue that stores up to 5 elements and synchronizes access among threads.
- **Worker<T>** consumes values from the queue and delegates the actual handling of events to an action (strategy pattern).
- **IWorkerAction** defines the contract for worker actions.
- **PrintWorkerAction** implements this contract by logging messages.
```mermaid
flowchart LR
    subgraph Main [Main Thread]
        P[Producer - push values every 200ms]
    end

    subgraph Cola [Thread-Safe Queue]
        Q[(Cola<T> - max 5 elements)]
    end

    subgraph Workers [Worker Threads]
        W1[Worker<T> - thread]
        W2[Worker<T> - thread]
        W3[Worker<T> - thread]
    end

    subgraph Actions [Worker Actions]
        A1[IWorkerAction<T>]
        A2[PrintWorkerAction<T>]
    end

    P -->|"push()"| Q
    Q -->|"pop()"| W1
    Q -->|"pop()"| W2
    Q -->|"pop()"| W3

    W1 --> A2
    W2 --> A2
    W3 --> A2
    A2 -->|implements| A1


```

---

## 🗂 Class Diagram

```mermaid
classDiagram
    class Cola~T~ {
        -deque<T> buffer
        -mutex mtx
        -condition_variable cv
        -size_t max_size
        -bool shutting_down
        +Cola(size_t max_size = 5)
        +void push(T dato)
        +PopResult pop(T& out, chrono::seconds timeout)
        +void shutdown()
        +size_t get_size() const
        +bool is_empty() const
    }

    class Worker~T~ {
        -Cola<T>& cola
        -string name
        -thread thread
        -atomic<bool> running
        -IWorkerAction<T>* action
        +Worker(Cola<T> c, IWorkerAction<T> a, string name="Worker")
        +void start()
        +void stop()
        -void run()
    }

    class IWorkerAction~T~ {
        <<interface>>
        +void onWork(T dato)
        +void onQueueEmpty(string workerName)
        +void onShutdown(string workerName)
        +void onStop(string workerName)
    }

    class PrintWorkerAction~T~ {
        +void onWork(T dato)
        +void onQueueEmpty(string workerName)
        +void onShutdown(string workerName)
        +void onStop(string workerName)
    }

    class Logger {
        -static mutex mtx
        -static Level minLevel
        +static void set_min_level(Level lvl)
        +static void info(const string& msg)
        +static void warn(const string& msg)
        +static void error(const string& msg)
        +static void log(Level lvl, const string& msg)
    }

    Cola <.. Worker : uses
    Worker --> IWorkerAction : delegates
    PrintWorkerAction ..|> IWorkerAction
    PrintWorkerAction ..> Logger : logs to


```

---

## 🛠 Build Instructions

### Windows (Visual Studio 2022)

1. Open **Visual Studio 2022**.  
2. *Open folder* → select the project root (`cola-worker-test/`).  
3. Choose a configuration (`x64-Debug` or `Release`).  
4. Build the project (Ctrl+Shift+B).  
5. Run the generated binary `cola_worker.exe`.  

---

### Linux / WSL

Install required tools:
```
sudo apt update
sudo apt install -y build-essential cmake git
```

Build project:
```
git clone https://github.com/sergioguerreroblanco-oss/cola-worker-test.git
cd cola-worker-test
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./cola_worker
```

---

## 🧪 Unit Tests

Unit tests are implemented with GoogleTest and integrated into the CMake build system via CTest.
They validate the main behavior of the queue (Cola<T>):
- Maximum size enforcement.
- FIFO ordering.
- Timeout behavior.
- Shutdown behavior.

### Running Tests (Linux / Docker)

After building the project, run the following command inside the build directory:

```ctest --output-on-failure```

This will automatically discover and execute all registered GoogleTest cases.

### Running Tests (Windows)

On Windows, the project has been validated with Visual Studio 2022.
Tests can be executed directly from Test Explorer:
```Menu → Test → Run All Tests```

Alternatively, if CMake is installed and available in your PATH, you can also run from PowerShell:
```ctest --output-on-failure```

Example output:
```
Test project /app/build
    Start 1: ColaTest.KeepMaxBufferSize
1/3 Test #1: ColaTest.KeepMaxBufferSize ........ Passed
    Start 2: ColaTest.ExtractElements
2/3 Test #2: ColaTest.ExtractElements .......... Passed
    Start 3: ColaTest.ShutdownWakesUpImmediately
3/3 Test #3: ColaTest.ShutdownWakesUpImmediately Passed
100% tests passed, 0 tests failed out of 3
```

---

## 🐳 Docker

This project includes a Dockerfile to provide a reproducible build and test environment.

Build image

```docker build -t cola-worker:dev .```

Run tests inside container

```docker run --rm cola-worker:dev```

Run main binary

```docker run --rm cola-worker:dev ./build/cola_worker```

---

## 🔄 Continuous Integration

A GitHub Actions workflow (```.github/workflows/ci.yml```) is provided. It performs:
- Build and run unit tests on ```ubuntu-latest``` using CMake and g++.
- Build Docker image and execute the binary/tests inside the container.

This ensures that:
- The code always compiles on a clean environment.
- All unit tests pass successfully on each push/pull request.
- The project works both in native Linux and inside a reproducible Docker container.

---

## 📂 Project Structure

```
cola-worker-test/
│
├── CMakeLists.txt           # Build configuration
├── Dockerfile               # Docker build context
├── .dockerignore
├── .gitignore
├── README.md                # Project documentation
│
├── include/                 # Headers and template implementations
│   ├── cola.h
│   ├── cola.ipp
│   ├── worker.h
│   ├── worker.ipp
│   ├── IWorkerAction.h
│   ├── PrintWorkerAction.h
│   └── logger.h
│
├── src/                     # Source files
│   ├── main.cpp
│   └── logger.cpp
│
├── tests/                   # Unit tests
│   └── test_main.cpp
│
└── .github/workflows/
    └── ci.yml               # GitHub Actions CI/CD
```

---

## 📌 Notes

- C++ Standard: C++14 (set(CMAKE_CXX_STANDARD 14)).
- Thread Safety: Managed with std::mutex, std::condition_variable, and std::atomic.
- Extensibility: Queue and workers implemented as templates (Cola<T>, Worker<T>).
- Interfaces: Worker behavior decoupled via IWorkerAction.
- Logging: Centralized Logger utility with severity levels.
- Cross-Platform: Builds on Windows (MSVC), Linux (g++) and Docker.

---

## ❤️ Acknowledgements

This project was developed as a technical test and extended to include professional practices such as **templates, interfaces, logging, unit testing, CI/CD and containerization.**

