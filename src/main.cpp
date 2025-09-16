/**
 * @file        main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief        Main source file of the COLA-WORKER project.
 *
 * @details
 * Demonstrates the producer-consumer pattern using Cola and Worker.
 * A Worker is launched in a separate thread, while the main thread
 * pushes integer values into the queue. The Worker consumes them
 * asynchronously until the program terminates.
 */

 /*****************************************************************************/

/* Libraries */

#include <thread>
#include <chrono>

#include "cola.h"
#include "worker.h"
#include "logger.h"

/* Namespace */

using namespace std::chrono_literals;

int main() 
{
    Logger::set_min_level(Logger::Level::INFO);

    constexpr size_t maxQueueSize = 5;
    constexpr int maxValues = 15;
    constexpr auto pushInterval = 200ms;
    constexpr auto mainSleep = 10s;

    Cola cola(maxQueueSize);

    Worker worker1(cola, "Worker1");
    Worker worker2(cola, "Worker2");
    Worker worker3(cola, "Worker3");

    worker1.start();
    worker2.start();
    worker3.start();

    /**
     * NOTE: In a real world scenario, production would be driven by external events
     * (e.g. hardware input, network messages, sensors, etc.). 
     * Here we simulate a steady production rate with sleep_for to clearly demonstrate 
     * the producer-consumer mechanism without adding unnecessary complexity.
     */
    for (int i = 0; i < maxValues; ++i)
    {
        cola.push(i);
        std::this_thread::sleep_for(pushInterval);
    }

    std::this_thread::sleep_for(mainSleep);

    cola.shutdown();

    worker1.stop();
    worker2.stop();
    worker3.stop();

    return 0;
}