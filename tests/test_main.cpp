/**
 * @file        test_main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Unit tests for the COLA-WORKER project.
 */

/*****************************************************************************/

/* Standard libraries */

#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

/* Project libraries */

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
    Cola<int> cola;
    int extracted_value;

    // Given: a queue with max size 5
    for (int i = 0; i < 6; i++) {
        cola.push(i);
    }

    // When: pushing beyond capacity
    // Then: the size is capped and the oldest element discarded
    EXPECT_EQ(cola.get_size(), 5u);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)),
              Cola<int>::PopResult::OK);  // The first value (0) was deleted
    EXPECT_EQ(extracted_value, 1);
}

/**
 * @test ExtractsInOrder
 * @brief Validates FIFO behavior of Cola::pop().
 *
 * @details
 * Pushes two values and ensures they are retrieved in the same
 * order. Afterwards, the queue must be empty.
 */
class ColaFifoTest : public ::testing::TestWithParam<std::vector<int>> {
   protected:
    Cola<int> cola;
};

TEST_P(ColaFifoTest, ExtractsInOrder) {
    auto values = GetParam();
    int extracted_value;

    // Given: a queue with multiple pushes
    for (auto v : values) {
        cola.push(v);
    }

    // When & Then: pop retrieves elements in the same order
    for (auto expected : values) {
        EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), Cola<int>::PopResult::OK);
        EXPECT_EQ(extracted_value, expected);
    }
    EXPECT_TRUE(cola.is_empty());
}

// Instanciación con diferentes secuencias
INSTANTIATE_TEST_SUITE_P(MultipleSequences, ColaFifoTest,
                         ::testing::Values(std::vector<int>{10, 20},
                                           std::vector<int>{1, 2, 3, 4, 5}, std::vector<int>{42}));

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
    Cola<int> cola;
    int extracted_value;
    std::atomic<bool> done{false};

    // Given: a consumer thread waiting on pop() with a long timeout
    std::thread t([&] {
        auto res = cola.pop(extracted_value, std::chrono::seconds(100));
        EXPECT_EQ(res, Cola<int>::PopResult::SHUTDOWN);
        done = true;
    });

    // When: shutdown is triggered from the main thread
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cola.shutdown();

    // Then: the waiting consumer must be released immediately
    t.join();
    EXPECT_TRUE(done);
}

/**
 * @test PopReturnsTimeout
 * @brief Ensures that pop() returns TIMEOUT if the queue remains empty during the wait period.
 *
 * @details
 * Perform an attempt to retrieve data from an empty cola
 * and after the timeout is finished it returns the timeout
 * result of the operation.
 */
TEST(ColaTest, PopReturnsTimeout) {
    Cola<int> cola;
    int extracted_value;

    // Given: an empty queue
    // (nothing pushed)

    // When: attempting to pop with a short timeout
    auto result = cola.pop(extracted_value, std::chrono::seconds(1));

    // Then: the call must return TIMEOUT
    EXPECT_EQ(result, Cola<int>::PopResult::TIMEOUT);
}