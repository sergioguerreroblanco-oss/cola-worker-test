/**
 * @file        worker.ipp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Implementation of the Worker<T> template class.
 *
 * @details
 * This file provides the definitions of all Worker<T> methods.
 *
 * The Worker is responsible for running in its own thread, consuming
 * elements from a thread-safe queue (`Cola<T>`), and delegating the
 * handling of events (element retrieved or timeout) to a
 * strategy defined via the IWorkerAction<T> interface.
 *
 * Keeping the implementation in a separate `.ipp` file, included at the
 * end of `worker.h`, ensures that template definitions are visible at
 * compile time while keeping headers organized and readable.
 */

/*****************************************************************************/

/* Project libraries */

#include "worker.h"

// Definition required for constexpr static data members of non-integral types
template <typename T>
constexpr std::chrono::seconds Worker<T>::DEFAULT_WAIT_TIMEOUT;

/*****************************************************************************/

/* Public Methods */

/**
 * @details Implementation of the Worker constructor.
 *          Initializes references to the queue and action, and sets the worker name.
 */
template <typename T>
Worker<T>::Worker(Cola<T>& cola, IWorkerAction<T>& action, const std::string& name)
    : cola(cola), action(action), name(name), running(false) {}

/**
 * @details Ensures the worker thread has finished
 *          before destruction (joins the thread if needed)
 */
template <typename T>
Worker<T>::~Worker() {
    stop();
    if (thread.joinable()) {
        thread.join();
    }
    action.onStop(name);
}

/**
 * @details Starts the worker by setting the running flag to true
 *          and launching a dedicated thread that executes the run() loop.
 */
template <typename T>
void Worker<T>::start() {
    running = true;
    thread = std::thread(&Worker<T>::run, this);
}

/**
 * @details Stops the worker activity by setting its running flag to false.
 *          The worker thread will complete its current loop iteration and then exit.
 *          This does not affect the underlying queue.
 * @note stop() does not interrupt pop() immediately.
 *       If the worker is waiting in pop(), it will exit
 *       only after the configured timeout (5s by default).
 *       This simplifies the design and avoids pushing
 *       shutdown logic into the queue.
 */
template <typename T>
void Worker<T>::stop() {
    if (!running) return;

    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

/**
 * @details Main worker loop.
 *          Attempts to pop elements from the queue with a timeout.
 *           - If an element is retrieved, it delegates processing to `action.trabajo()`.
 *           - If the queue is empty and the timeout expires, it calls `action.colaVacia()`.
 */
template <typename T>
void Worker<T>::run() {
    while (running) {
        auto extracted_data = cola.pop(DEFAULT_WAIT_TIMEOUT);
        if (!extracted_data) {
            action.colaVacia(name, DEFAULT_WAIT_TIMEOUT);
        } else {
            action.trabajo(name, *extracted_data);
        }
    }
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/