/**
 * @file        cola.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Thread-safe bounded queue for integer values.
 */

/*****************************************************************************/

/* Libraries */

#include "cola.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details Manages the addition of a new data value into the buffer
 * which could mean to delete the eldest data when the buffer is full.
 */
void Cola::push(int dato) 
{
    std::unique_lock<std::mutex> lock(mtx);
    if (buffer.size() >= max_size) 
    {
        buffer.pop(); // Take out the eldest value
    }
    buffer.push(dato);
    cv.notify_one(); // notify the waitting worker
}

/**
 * @details Manages retrieval of data from the buffer but if the buffer
 * is empty waits for new data until a timeout is triggered.
 */
bool Cola::pop(int& out, std::chrono::seconds timeout) 
{
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until new data is added or until time is out
    if (!cv.wait_for(lock, timeout, [this] { return !buffer.empty(); })) 
    {
        return false;
    }

    out = buffer.front();
    buffer.pop();
    return true;
}

/**
 * @details Returns the size of the buffer.
 */
int Cola::get_size(void)
{
    return buffer.size();
}

/**
 * @details Indicates if the buffer is empty or not.
 */
bool Cola::is_empty(void)
{
    return buffer.empty();
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/