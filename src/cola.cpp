/**
 * @file        cola.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Thread-safe bounded queue for integer values.
 */

/*****************************************************************************/

/* Libraries */

#include "cola.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details Constructor of the Cola taking the maximum size 
 * of the internal buffer.
 */
Cola::Cola(size_t max_size) 
    : max_size(max_size),
    shutting_down(false)
{}

/**
* @details Manages to stop using the Cola and notifies all the consumers.
*/
void Cola::shutdown()
{
    std::unique_lock<std::mutex> lock(mtx);
    shutting_down = true;
    lock.unlock();
    cv.notify_all();
}

/**
 * @details Manages the addition of a new data value into the buffer
 * which could mean to delete the eldest data when the buffer is full.
 */
void Cola::push(int dato) 
{
    std::unique_lock<std::mutex> lock(mtx);
    if (buffer.size() >= max_size) 
    {
        buffer.pop_front(); // Take out the eldest value
    }
    buffer.push_back(dato);
    cv.notify_one(); // notify the waiting worker
}

/**
 * @details Manages retrieval of data from the buffer but if the buffer
 * is empty waits for new data until a timeout is triggered.
 */
Cola::PopResult Cola::pop(int& out, std::chrono::seconds timeout)
{
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until new data is added or until time is out
    if (!cv.wait_for(lock, timeout, [this] { return shutting_down || !buffer.empty(); }))
    {
        return PopResult::TIMEOUT;
    }

    if (shutting_down)
    {
        return PopResult::SHUTDOWN;
    }

    out = buffer.front();
    buffer.pop_front();
    return PopResult::OK;
}

/**
 * @details Returns the size of the buffer.
 */
size_t Cola::get_size(void) const
{
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.size();
}

/**
 * @details Indicates if the buffer is empty or not.
 */
bool Cola::is_empty(void) const
{
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.empty();
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/