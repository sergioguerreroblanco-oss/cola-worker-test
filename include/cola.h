/**
 * @file        cola.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Thread-safe bounded queue template.
 *
 * @details
 * `Cola<T>` is a generic, thread-safe queue with a fixed maximum size.
 * - Implements the producer-consumer pattern with synchronization
 *   using a mutex and condition variable.
 * - When the queue reaches its maximum size, the oldest element is discarded.
 * - Provides timeout-based retrieval (`pop`) and a shutdown mechanism
 *   to gracefully stop consumers.
 *
 * The class is safe for concurrent use by multiple producer and consumer threads.
 */

/*****************************************************************************/

/* Include Guard */
#pragma once

/*****************************************************************************/

/* Standard libraries */

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>

/*****************************************************************************/

/**
 * @class Cola
 * @brief Thread-safe bounded queue.
 * @tparam T Type of elements stored in the queue.
 *
 * This class implements a fixed-size, thread-safe FIFO queue
 * with a maximum capacity (default: 5). When the queue is full,
 * the oldest element is discarded.
 */
template <typename T>
class Cola {
    /******************************************************************/

    /* Public Data Types */

   public:
    /**
     * @brief Description of the different scenarios when
     * popping values from the buffer.
     */
    enum class PopResult { OK, TIMEOUT, SHUTDOWN };

    /******************************************************************/

    /* Public Methods */

   public:
    /**
     * @brief Constructor of the Cola class.
     * @param max_size Maximum number of elements of the buffer, by default 5.
     */
    explicit Cola(size_t max_size = 5);

    /**
     * @brief Destructor of the Cola class.
     */
    ~Cola() = default;

    /**
     * @brief Disable copy constructor.
     * Cola cannot be copied because it manages synchronization primitives
     * (std::mutex, std::condition_variable) which are non-copyable.
     */
    Cola(const Cola&) = delete;

    /**
     * @brief Disable copy assignment operator.
     * Cola cannot be assigned by copy for the same reason: it manages
     * synchronization primitives that are non-copyable.
     */
    Cola& operator=(const Cola&) = delete;

    /**
     * @brief Disable move constructor.
     * Cola cannot be moved because synchronization primitives
     * are not safely movable between instances.
     */
    Cola(Cola&&) = delete;

    /**
     * @brief Disable move assignment operator.
     * Cola cannot be move-assigned because synchronization primitives
     * cannot be safely transferred between instances.
     */
    Cola& operator=(Cola&&) = delete;

    /**
     * @brief Manages to stop using the Cola and notifies all the consumers.
     */
    void shutdown();

    /**
     * @brief Push the data into the buffer.
     * @param dato Data to introduce in the buffer.
     */
    void push(T dato);

    /**
     * @brief Removes the oldest element from the buffer.
     * @param out Data retrieved from the buffer.
     * @param timeout Time given to retrieve data from the buffer.
     * @return OK Value succesfully retrieved from the buffer.
     * @return TIMEOUT The buffer is empty and triggered a timeout.
     * @return SHUTDOWN The buffer is stopped.
     */
    PopResult pop(T& out, std::chrono::seconds timeout);

    /**
     * @brief Getter of the buffer's size.
     * @return Size of the buffer.
     */
    size_t get_size(void) const;

    /**
     * @brief Indicates if the buffer is empty or not.
     * @return true The buffer is empty.
     * @return false The buffer is not empty.
     */
    bool is_empty(void) const;

    /******************************************************************/

    /* Private Attributes */

   private:
    /**
     * @brief FIFO buffer.
     */
    std::deque<T> buffer;

    /**
     * @brief Mutex.
     */
    mutable std::mutex mtx;

    /**
     * @brief Condition variable.
     */
    std::condition_variable cv;

    /**
     * @brief Maximum size of the buffer.
     */
    size_t max_size;

    /**
     * @brief Indicates if the Cola was stopped.
     */
    bool shutting_down;

    /******************************************************************/
};

#include "cola.ipp"