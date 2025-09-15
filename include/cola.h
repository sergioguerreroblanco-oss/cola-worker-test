/**
 * @file        cola.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Thread-safe bounded queue for integer values.
 *
 * @details
 * This class implements a producer-consumer buffer with a fixed
 * maximum size. When the buffer is full, the oldest element is
 * discarded. Synchronization is guaranteed through a mutex and
 * condition variable, making it safe to use across multiple threads.
 */

/*****************************************************************************/

/* Include Guard */
#pragma once

/*****************************************************************************/

/* Libraries */

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

/*****************************************************************************/

/* Class Cola */

class Cola {
    /******************************************************************/

    /* Private Constants */

    private:

        /**
         * @brief Maximun size of the buffer.
         */
        const size_t max_size = 5;

    /******************************************************************/

    /* Public Methods */

    public:
        /**
         * @brief Push the data into the buffer.
         * @param dato Number to introduce in the buffer.
         */
        void push(int dato);

        /**
         * @brief Takes out of the buffer its eldest value .
         * @param out Number retrieved from the buffer.
         * @param timeout Time given to retrieve data from the buffer.
         * @return true Value succesfully retrieved from the buffer.
         * @return false The buffer is empty and triggered a timeout.
         */
        bool pop(int& out, std::chrono::seconds timeout);

        /**
         * @brief Getter of the buffer's size.
         * @return Size of the buffer.
         */
        int get_size(void);

        /**
         * @brief Indicates if the buffer is empty or not.
         * @return true The buffer is empty.
         * @return false The buffer is not empty.
         */
        bool is_empty(void);

    /******************************************************************/

    /* Private Attributes */

    private:
        /**
         * @brief FIFO buffer.
         */
        std::queue<int> buffer;

        /**
         * @brief Mutex.
         */
        mutable std::mutex mtx;

        /**
         * @brief Condition variable.
         */
        std::condition_variable cv;

    /******************************************************************/
};