/**
 * @file        worker.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Consumer entity that processes data from a Cola instance.
 *
 * @details
 * The Worker repeatedly attempts to extract data from the queue
 * with a timeout. If data is available, it is "processed" (printed).
 * If the queue remains empty for the timeout duration, a warning
 * message is printed. The Worker runs indefinitely once started.
 */

/*****************************************************************************/

/* Include Guard */

#pragma once

/*****************************************************************************/

/* Libraries */

#include "cola.h"

/*****************************************************************************/

/* Class Worker */

class Worker {
    /******************************************************************/

    /* Public Methods */

    public:
        /**
         * @brief Construct a new Worker object.
         */
        Worker(Cola& c) : cola(c) {}

        /**
         * @brief Manage the Worker.
         */
        void run();

    /******************************************************************/

    /* Private Methods */

    private:
        /**
         * @brief Prints a message indicating that the empty buffer was detected.
         */
        void colaVacia();

        /**
         * @brief Prints a message with the data of the buffer extracted.
         * @param dato Data retrieved from the buffer.
         */
        void trabajo(int dato);

    /******************************************************************/

    /* Private Attributes */

    private:
        /**
         * @brief Cola used by the worker.
         */
        Cola& cola;

    /******************************************************************/
};