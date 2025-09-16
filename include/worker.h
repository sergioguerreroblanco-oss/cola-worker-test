/**
 * @file        worker.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Consumer entity that processes data from a Cola instance.
 *
 * @details
 * The Worker repeatedly attempts to extract data from the queue
 * with a timeout. If data is available, it is "processed" (printed).
 * If the queue remains empty for the timeout duration, a warning
 * message is printed.
 */

/*****************************************************************************/

/* Include Guard */

#pragma once

/*****************************************************************************/

/* Libraries */

#include <atomic>
#include <thread>
#include <string>
#include <mutex>

#include "cola.h"

/*****************************************************************************/

/* Class Worker */

class Worker {
    /******************************************************************/
    
    /* Private Constants */

    private:

        /**
         * @brief Time to wait new values enter into the Cola when it is empty.
         */
        const std::chrono::seconds wait_timeout{ 5 };
    
    /******************************************************************/

    /* Public Methods */

    public:
        /**
         * @brief Construct a new Worker object.
         * @param cola The "cola" to get values from.
         * @param name The name of the worker, by default "Worker".
         */
        explicit Worker(Cola& cola, std::string name = "Worker");

        /**
         * @brief Destruct a Worker object.
         */
        ~Worker();

        /**
         * @brief Disable copy and move semantics since Worker manages a std::thread, which
         * cannot be safely copied or moved. Each Worker only owns its thread.
         */
        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;
        Worker(Worker&&) = delete;
        Worker& operator=(Worker&&) = delete;

        /**
         * @brief Starts the Worker.
         */
        void start();

        /**
         * @brief Stops the Worker.
         */
        void stop();

    /******************************************************************/

    /* Private Methods */

    private:
        /**
         * @brief Manage the Worker actions.
         */
        void run();

        /**
         * @brief Prints a message indicating that the empty buffer was detected.
         */
        void colaVacia() const;

        /**
         * @brief Prints a message with the data of the buffer extracted.
         * @param dato Data retrieved from the buffer.
         */
        void trabajo(int dato) const;

        /**
         * @brief Prints a message indicating that the worker has detected that
         * the buffer was shut down.
         */
        void colaApagada() const;

    /******************************************************************/

    /* Private Attributes */

    private:
        /**
         * @brief Cola used by the worker.
         */
        Cola& cola;

        /**
         * @brief Name of the worker.
         */
        std::string name;

        /**
         * @brief Thread used by the worker.
         */
        std::thread thread;

        /**
         * @brief Indicator of the worker that can be modified by other threads.
         */
        std::atomic<bool> running;

    /******************************************************************/
};