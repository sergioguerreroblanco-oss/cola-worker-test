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
 * handling of events (element retrieved, timeout, or shutdown) to a
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
 * Initializes references to the queue and action, and sets the worker name.
 */
template <typename T>
Worker<T>::Worker(Cola<T>& cola, IWorkerAction<T>& action, const std::string& name)
    : cola(cola), action(action), name(name), running(false) {}

/**
 * @details Ensures the worker thread has finished
 * before destruction (joins the thread if needed)
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
 * @details Launches the worker activity setting the running flag to true
 * and getting the thread for it.
 */
template <typename T>
void Worker<T>::start() {
    running = true;
    thread = std::thread(&Worker<T>::run, this);
}

/**
 * @details Stops the worker activity by setting its running flag to false.
 * The thread will exit gracefully after the current iteration
 */
template <typename T>
void Worker<T>::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

/**
 * @details The process method of the worker that manages how
 * the data from the buffer is handled.
 */
template <typename T>
void Worker<T>::run() {
    while (running) {
        T extracted_data;
        typename Cola<T>::PopResult result = cola.pop(extracted_data, DEFAULT_WAIT_TIMEOUT);
        switch (result) {
            case Cola<T>::PopResult::OK:
                action.trabajo(name, extracted_data);
                break;
            case Cola<T>::PopResult::TIMEOUT:
                action.colaVacia(name, DEFAULT_WAIT_TIMEOUT);
                break;
            case Cola<T>::PopResult::SHUTDOWN:
                action.colaApagada(name);
                return;
            default:
                return;
        }
    }
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/