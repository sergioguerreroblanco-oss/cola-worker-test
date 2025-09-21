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
 * - Provides timeout-based retrieval (`pop`) using `nonstd::optional`.
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

/* Third party libraries */

#include "third_party/optional.hpp"

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
     *        Cola cannot be copied because it manages synchronization primitives
     *        (std::mutex, std::condition_variable), which are non-copyable.
     */
    Cola(const Cola&) = delete;

    /**
     * @brief Disable copy assignment operator.
     *        Cola cannot be copy-assigned for the same reason:
     *        synchronization primitives cannot be duplicated safely.
     */
    Cola& operator=(const Cola&) = delete;

    /**
     * @brief Disable move constructor.
     *        Although std::mutex and std::condition_variable technically
     *        have move operations deleted, even if they were movable,
     *        transferring them between Cola instances would break
     *        synchronization guarantees. To enforce strict ownership,
     *        movement is disabled.
     */
    Cola(Cola&&) = delete;

    /**
     * @brief Disable move assignment operator.
     *        Move assignment is also disabled to avoid transferring
     *        synchronization primitives between instances, which could
     *        lead to undefined behavior.
     */
    Cola& operator=(Cola&&) = delete;

    /**
     * @brief Push a new element into the buffer.
     *        If the buffer is full, the oldest element is discarded.
     * @param dato Data to insert in the buffer.
     */
    void push(T dato);

    /**
     * @brief Removes the oldest element from the buffer, waiting up to a timeout.
     * @param timeout Maximum time to wait for data.
     * @return An `optional<T>` containing the retrieved value if available.
     *         Returns `nonstd::nullopt` if the timeout expires without data.
     * @note The queue does not support immediate shutdown.
     *       If a Worker calls stop() while blocked on pop(),
     *       the call will only return after the timeout expires.
     *       This design keeps the queue "dumb" (responsible only
     *       for synchronization and storage, without shutdown logic).
     */
    nonstd::optional<T> pop(std::chrono::seconds timeout);

    /**
     * @brief Getter of the buffer size.
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

    /******************************************************************/
};

#include "cola.ipp"