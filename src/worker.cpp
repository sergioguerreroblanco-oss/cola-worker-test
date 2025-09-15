/**
 * @file        worker.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Consumer entity that processes data from a Cola instance.
 */

/*****************************************************************************/

/* Libraries */

#include <iostream>
#include <chrono>

#include "worker.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details The process method of the worker that manages how
 * the data from the buffer is handled.
 */
void Worker::run() {
    while (true) {
        int dato;
        bool ok = cola.pop(dato, std::chrono::seconds(5));
        if (ok) {
            trabajo(dato);
        }
        else {
            colaVacia();
        }
    }
}

/**
 * @details Prints a message indicating that the timeout retrieving
 * data from the buffer has passed and currently it is empty.
 */
void Worker::colaVacia()
{
    std::cout << "[Worker] Cola empty after waiting 5s.\n";
}

/**
 * @details Prints a message indicating that the data was successfully
 * retrieved from the buffer and shows the data.
 */
void Worker::trabajo(int dato)
{
    std::cout << "[Worker] Data processed: " << dato << "\n";
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/