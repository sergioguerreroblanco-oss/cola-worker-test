/**
 * @file        cola.ipp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Implementation file for the template class Cola<T>.
 *
 * @details
 * This file provides the definitions of all methods declared
 * in `cola.h`.
 * Keeping template implementations in a separate `.ipp` file allows
 * for better code organization while ensuring that the compiler
 * can instantiate the templates as needed.
 */

/*****************************************************************************/

/* Project libraries */

#include "cola.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details Constructor of the Cola receiving the maximum size
 * for the buffer.
 */
template <typename T>
Cola<T>::Cola(size_t max_size) : max_size(max_size), shutting_down(false) {}

/**
 * @details Manages to stop using the Cola and notifies all the consumers.
 */
template <typename T>
void Cola<T>::shutdown() {
    std::unique_lock<std::mutex> lock(mtx);
    shutting_down = true;
    lock.unlock();
    cv.notify_all();
}

/**
 * @details Manages the addition of a new "dato" into the buffer
 * which could mean to delete the eldest "dato" when the buffer is full.
 */
template <typename T>
void Cola<T>::push(T dato) {
    std::unique_lock<std::mutex> lock(mtx);
    if (buffer.size() >= max_size) {
        buffer.pop_front();  // Take out the eldest "dato"
    }
    buffer.push_back(dato);
    cv.notify_one();  // notify the waiting worker
}

/**
 * @details Manages retrieval of data from the buffer but if the buffer
 * is empty waits for new data until a timeout is triggered.
 */
template <typename T>
typename Cola<T>::PopResult Cola<T>::pop(T& out, std::chrono::seconds timeout) {
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until new data is added or until time is out
    if (!cv.wait_for(lock, timeout, [this] { return shutting_down || !buffer.empty(); })) {
        return PopResult::TIMEOUT;
    }

    if (shutting_down) {
        return PopResult::SHUTDOWN;
    }

    out = buffer.front();
    buffer.pop_front();
    return PopResult::OK;
}

/**
 * @details Returns the size of the buffer.
 */
template <typename T>
size_t Cola<T>::get_size(void) const {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.size();
}

/**
 * @details Indicates if the buffer is empty or not.
 */
template <typename T>
bool Cola<T>::is_empty(void) const {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.empty();
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/