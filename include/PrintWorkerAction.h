/**
 * @file        PrintWorkerAction.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Concrete implementation of IWorkerAction that logs events.
 */

/*****************************************************************************/

/* Include Guard */

#pragma once

/*****************************************************************************/

/* Libraries */

#include <string>
#include <chrono>

#include "IWorkerAction.h"
#include "logger.h"

/*****************************************************************************/

/* Class IWorkerAction  */

template <typename T>
class PrintWorkerAction : public IWorkerAction<T>
{
    /******************************************************************/

    /* Public Methods */

    public:
        /**
         * @details Prints a message indicating that the "dato" was successfully
         * retrieved from the buffer and shows the "dato".
         */
        void trabajo(const std::string& workerName, const T& dato) override
        {
            Logger::info("[" + workerName + "] Data processed: " + std::to_string(dato));
        }

        /**
         * @details Prints a message indicating that the timeout retrieving
         * the "dato" from the buffer has passed and currently it is empty.
         */
        void colaVacia(const std::string& workerName, const std::chrono::seconds waitting_time) override
        {
            Logger::warn("[" + workerName + "] Cola empty after timeout of " + std::to_string(waitting_time.count()) + "s");
        }

        /**
         * @details Prints a message indicating that the Cola was shut down.
         */
        void colaApagada(const std::string& workerName) override
        {
            Logger::error("[" + workerName + "] Cola was shut down");
        }

        /**
         * @details Prints a message indicating that the worker finished its action.
         */
        void onStop(const std::string& workerName) override
        {
            Logger::info("[" + workerName + "] Finished.");
        }

    /******************************************************************/
};