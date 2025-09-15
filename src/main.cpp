/**
 * @file        main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
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
#include <iostream>

#include "cola.h"
#include "worker.h"

int main() {
    Cola cola;
    Worker worker(cola);

    std::thread t(&Worker::run, &worker);

    for (int i = 0; i < 10; ++i) {
        cola.push(i);
    }

    t.join(); // Infinite worker (This must not be reached ever).
    return 0;
}