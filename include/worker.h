/**
 * @file        worker.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Generic worker that consumes data from a Cola<T> instance.
 *
 * @details
 * The Worker class runs in its own thread and repeatedly attempts to
 * extract data from a thread-safe queue (`Cola<T>`). For each retrieved
 * element, or in case of timeout/shutdown, the Worker delegates the
 * handling of events to a user-defined action (via the IWorkerAction<T>
 * interface).
 *
 * This design decouples the worker’s concurrency logic from the specific
 * behavior applied to each element, making it possible to plug in
 * different actions (e.g., logging, processing, testing) without
 * modifying the Worker itself.
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
#include "IWorkerAction.h"

/*****************************************************************************/

/* Class Worker */

template <typename T>
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
        explicit Worker(Cola<T>& cola, IWorkerAction<T>& action, const std::string& name = "Worker");


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

    /******************************************************************/

    /* Private Attributes */

    private:
        /**
         * @brief Cola used by the worker.
         */
        Cola<T>& cola;

        /**
         * @brief The worker action interface.
         */
        IWorkerAction<T>& action;

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

#include "worker.ipp"