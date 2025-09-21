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
 * @details Constructor of Cola, setting the maximum buffer size.
 */
template <typename T>
Cola<T>::Cola(size_t max_size) : max_size(max_size) {}

/**
 * @details Inserts a new element into the buffer.
 *          If the buffer is full, the oldest element is removed before inserting the new one.
 */
template <typename T>
void Cola<T>::push(T dato) {
    std::unique_lock<std::mutex> lock(mtx);
    if (buffer.size() >= max_size) {
        buffer.pop_front();  // Take out the eldest "dato"
    }
    buffer.push_back(std::move(dato));
    cv.notify_one();  // notify the waiting worker
}

/**
 * @details Retrieves the oldest element from the buffer.
 *          If the buffer is empty, waits up to the specified timeout for new data.
 * @return An `optional<T>` containing the retrieved element,
 *         or `nonstd::nullopt` if the timeout expires.
 */
template <typename T>
nonstd::optional<T> Cola<T>::pop(std::chrono::seconds timeout) {
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until new data is added or until time is out
    if (!cv.wait_for(lock, timeout, [this] { return !buffer.empty(); })) {
        return nonstd::nullopt;
    }

    T out = std::move(buffer.front());
    buffer.pop_front();
    return out;
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
 * @details Checks whether the buffer is empty.
 * @return true if empty, false otherwise.
 */
template <typename T>
bool Cola<T>::is_empty(void) const {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.empty();
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/