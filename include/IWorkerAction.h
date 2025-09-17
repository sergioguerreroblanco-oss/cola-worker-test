/**
 * @file        IWorkerAction.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Abstract interface for Worker actions.
 *
 * @details 
 * Defines the abstract contract that a Worker uses to delegate behavior
 * when consuming data from a queue, handling timeouts, reacting to
 * shutdown, and notifying lifecycle end (stop).
 *
 * This interface is templated to support any element type (Cola<T>).
 */

/*****************************************************************************/

/* Include Guard */

#pragma once

/*****************************************************************************/

/* Libraries */

#include <chrono>

/*****************************************************************************/

/* Class IWorkerAction  */

template <typename T>
class IWorkerAction
{
    /******************************************************************/

    /* Public Methods */

    public:

        virtual ~IWorkerAction() {}

        /**
         * @brief Action executed when data is successfully retrieved.
         * @param dato Data retrieved from the queue.
         */
        virtual void trabajo(const std::string& workerName, const T& dato) = 0;

        /**
         * @brief Action executed when the queue is empty after timeout.
         * @param timeout Time waited before considering Cola empty.
         */
        virtual void colaVacia(const std::string& workerName, const std::chrono::seconds timeout) = 0;

        /**
         * @brief Action executed when the queue is shut down.
         */
        virtual void colaApagada(const std::string& workerName) = 0;

        /**
         * @brief Action executed when the queue is shut down.
         */
        virtual void onStop(const std::string& workerName) = 0;

    /******************************************************************/
};