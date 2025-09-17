/**
 * @file        worker.ipp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Implementation of the Worker<T> template class.
 */

/*****************************************************************************/

/* Libraries */

#include "worker.h"

/*****************************************************************************/

/* Public Methods */

/**
 * @details Constructor of the Worker that also initializes the running flag to false.
 */
template <typename T>
Worker<T>::Worker(Cola<T>& cola, IWorkerAction<T>& action, const std::string& name)
    : cola(cola), 
    action(action), 
    name(name), 
    running(false) 
{}

/**
 * @details Ensures the worker thread has finished 
 * before destruction (joins the thread if needed)
 */
template <typename T>
Worker<T>::~Worker() 
{
    stop();
    if (thread.joinable()) 
    {
        thread.join();
    }
    action.onStop(name);
}

/**
 * @details Launches the worker activity setting the running flag to true
 * and getting the thread for it.
 */
template <typename T>
void Worker<T>::start() 
{
    running = true;
    thread = std::thread(&Worker<T>::run, this);
}

/**
 * @details Stops the worker activity by setting its running flag to false. 
 * The thread will exit gracefully after the current iteration
 */
template <typename T>
void Worker<T>::stop() 
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
template <typename T>
void Worker<T>::run() 
{
    while (running) 
    {
        T extracted_data;
        typename Cola<T>::PopResult result = cola.pop(extracted_data, wait_timeout);
        switch (result) 
        {
            case typename Cola<T>::PopResult::OK:
                action.trabajo(name, extracted_data);
                break;
            case typename Cola<T>::PopResult::TIMEOUT:
                action.colaVacia(name, wait_timeout);
                break;
            case typename Cola<T>::PopResult::SHUTDOWN:
                action.colaApagada(name);
                return;
        }
    }
}

/*****************************************************************************/

/* Private Methods */

/*****************************************************************************/