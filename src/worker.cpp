/**
 * @file        worker.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Consumer entity that processes data from a Cola instance.
 */

/*****************************************************************************/

/* Libraries */

#include "worker.h"
#include "logger.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details Constructor of the Worker that also initializes the running flag to false.
 */
Worker::Worker(Cola& cola, std::string name)
    : cola(cola), 
    name(std::move(name)),
    running(false)
{}

/**
 * @details Ensures the worker thread has finished 
 * before destruction (joins the thread if needed)
 */
Worker::~Worker() 
{
    stop();
    if (thread.joinable()) 
    {
        thread.join();
    }
    Logger::info("[" + name + "] Finished.");
}

/**
 * @details Launches the worker activity setting the running flag to true
 * and getting the thread for it.
 */
void Worker::start() 
{
    running = true;
    thread = std::thread(&Worker::run, this);
}

/**
 * @details Stops the worker activity by setting its running flag to false. 
 * The thread will exit gracefully after the current iteration
 */
void Worker::stop() 
{
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

/**
 * @details The process method of the worker that manages how
 * the data from the buffer is handled.
 */
void Worker::run() 
{
    while (running) 
    {
        int extracted_value;
        Cola::PopResult result = cola.pop(extracted_value, wait_timeout);
        switch (result) 
        {
            case Cola::PopResult::OK:       
                trabajo(extracted_value); 
                break;
            case Cola::PopResult::TIMEOUT:  
                colaVacia();              
                break;
            case Cola::PopResult::SHUTDOWN:
                colaApagada();
                return;
        }
    }
}

/**
 * @details Prints a message indicating that the timeout retrieving
 * data from the buffer has passed and currently it is empty.
 */
void Worker::colaVacia() const
{
    Logger::warn("[" + name + "] Cola empty after waiting " +
        std::to_string(wait_timeout.count()) + "s");
}

/**
 * @details Prints a message indicating that the data was successfully
 * retrieved from the buffer and shows the data.
 */
void Worker::trabajo(int dato) const
{
    Logger::info("[" + name + "] Data processed: " + std::to_string(dato));
}

/**
 * @details Prints a message indicating that the buffer was shut down.
 */
void Worker::colaApagada() const
{
    Logger::error("[" + name + "] Cola was shut down");
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/