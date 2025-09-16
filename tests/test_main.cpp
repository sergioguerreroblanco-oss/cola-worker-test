/**
 * @file        test_main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Unit tests for the COLA-WORKER project.
 */

/*****************************************************************************/

/* Libraries */

#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>

#include "cola.h"

/*****************************************************************************/

/* Tests */

/**
 * @test KeepMaxBufferSize
 * @brief Ensures the Cola never grows beyond its maximum size.
 *
 * @details
 * After pushing 6 elements, only the last 5 should remain.
 * The oldest element must be discarded automatically.
 */
TEST(ColaTest, KeepMaxBufferSize) {
    Cola cola;
    int extracted_value;
    for (int i = 0; i < 6; i++) {
        cola.push(i);
    }
    EXPECT_EQ(cola.get_size(), 5u);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), Cola::PopResult::OK);  // The first value (0) was deleted
    EXPECT_EQ(extracted_value, 1);
}

/**
 * @test ExtractElements
 * @brief Validates FIFO behavior of Cola::pop().
 *
 * @details
 * Pushes two values and ensures they are retrieved in the same
 * order. Afterwards, the queue must be empty.
 */
TEST(ColaTest, ExtractElements) {
    Cola cola;
    int extracted_value;
    cola.push(10);
    cola.push(20);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), Cola::PopResult::OK);
    EXPECT_EQ(extracted_value, 10);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), Cola::PopResult::OK);
    EXPECT_EQ(extracted_value, 20);
    EXPECT_TRUE(cola.is_empty());
}

/**
 * @test ShutdownWakesUpImmediately
 * @brief Ensures that a waiting consumer is immediately released when shutdown() is called.
 *
 * @details
 * The test launches a thread blocked on pop() with a long timeout,
 * then calls shutdown() from the main thread. The waiting thread
 * must wake up immediately and return PopResult::SHUTDOWN.
 */
TEST(ColaTest, ShutdownWakesUpImmediately) {
    Cola cola;
    int extracted_value;
    std::atomic<bool> done{ false };
    std::thread t([&] {
        auto res = cola.pop(extracted_value, std::chrono::seconds(100));
        EXPECT_EQ(res, Cola::PopResult::SHUTDOWN);
        done = true;
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cola.shutdown();

    t.join();
    EXPECT_TRUE(done);
}