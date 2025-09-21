/**
 * @file        main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief        Entry point of the COLA-WORKER project.
 *
 * @details
 * This program demonstrates the producer-consumer pattern using:
 *  - A thread-safe generic queue (`Cola<T>`) with bounded capacity.
 *  - Multiple worker threads (`Worker<T>`) that process elements
 *    through an injected action (`IWorkerAction<T>` implementation).
 *
 * In this example:
 *  - The main thread produces integer values at fixed intervals.
 *  - Three worker threads consume and process the values concurrently.
 *  - Processing is delegated to a `PrintWorkerAction`, which logs
 *    the results with timestamps and severity levels.
 *
 * The application illustrates thread synchronization, dependency
 * injection for worker behavior, and a clean stop of workers.
 */

/*****************************************************************************/

/* Standard libraries */

#include <chrono>
#include <string>
#include <thread>

/* Project libraries */

#include "cola.h"
#include "logger.h"
#include "print_worker_action.h"
#include "worker.h"

/*****************************************************************************/

// Forward declaration
void production(Cola<int>& cola);

int main() {
    constexpr size_t maxQueueSize = 5;
    const std::string WORKER1_NAME = "Worker1";
    const std::string WORKER2_NAME = "Worker2";
    const std::string WORKER3_NAME = "Worker3";

    Logger::set_min_level(Logger::Level::INFO);

    Cola<int> cola(maxQueueSize);

    PrintWorkerAction<int> action;

    Worker<int> worker1(cola, action, WORKER1_NAME);
    Worker<int> worker2(cola, action, WORKER2_NAME);
    Worker<int> worker3(cola, action, WORKER3_NAME);

    worker1.start();
    worker2.start();
    worker3.start();

    /**
     * NOTE: In a real world scenario, production would be driven by external events
     * (e.g. hardware input, network messages, sensors, etc.).
     * Here we simulate a steady production rate with sleep_for to clearly demonstrate
     * the producer-consumer mechanism without adding unnecessary complexity.
     */
    production(cola);

    worker1.stop();
    worker2.stop();
    worker3.stop();

    return 0;
}

/**
 * @brief Producer function that periodically pushes integers into the queue.
 * Simulates steady production by generating sequential integers
 * and sleeping between pushes.
 * @param cola Reference to the queue where integers are inserted.
 */
void production(Cola<int>& cola) {
    using namespace std::chrono_literals;

    constexpr int maxValues = 15;
    constexpr auto pushInterval = 200ms;
    constexpr auto mainSleep = 10s;

    for (int i = 0; i < maxValues; ++i) {
        cola.push(i);
        std::this_thread::sleep_for(pushInterval);
    }

    std::this_thread::sleep_for(mainSleep);
}