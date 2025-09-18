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

/* Standard libraries */

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

/* Project libraries */

#include "IWorkerAction.h"
#include "cola.h"

/*****************************************************************************/

/**
 * @class Worker
 * @brief Worker thread that consumes data from a queue.
 * @tparam T Type of data consumed from the queue.
 *
 * Each Worker runs in its own thread, repeatedly calling `pop()` on the queue
 * and delegating the retrieved data to the associated IWorkerAction.
 * It supports graceful shutdown and explicit stop.
 */
template <typename T>
class Worker {
    /******************************************************************/

    /* Private Constants */

   private:
    /**
     * @brief Time to wait new values enter into the Cola when it is empty.
     */
    const std::chrono::seconds wait_timeout{5};

    /******************************************************************/

    /* Public Methods */

   public:
    /**
     * @brief Construct a worker bound to a queue and an action.
     * @param cola Reference to the queue from which the worker consumes data.
     * @param action Reference to the action strategy executed by the worker.
     * @param name Optional worker name for logging/identification.
     */
    explicit Worker(Cola<T>& cola, IWorkerAction<T>& action, const std::string& name = "Worker");

    /**
     * @brief Destruct a Worker object.
     */
    ~Worker();

    /**
     * @brief Disable copy constructor.
     * Worker cannot be copied because it manages a std::thread,
     * which cannot be safely duplicated.
     */
    Worker(const Worker&) = delete;

    /**
     * @brief Disable copy assignment operator.
     * Worker cannot be copy-assigned because it owns a std::thread,
     * which cannot be safely reassigned.
     */
    Worker& operator=(const Worker&) = delete;

    /**
     * @brief Disable move constructor.
     * Worker cannot be moved because transferring ownership of
     * an active std::thread is unsafe.
     */
    Worker(Worker&&) = delete;

    /**
     * @brief Disable move assignment operator.
     * Worker cannot be move-assigned because std::thread ownership
     * cannot be safely transferred.
     */
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