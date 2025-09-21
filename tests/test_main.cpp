/**
 * @file        test_main.cpp
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     0.0.0
 *
 * @brief       Unit tests for the COLA-WORKER project.
 *
 * @details
 * These tests validate the behavior of the `Cola<T>` class:
 *  - Capacity limit and element eviction when the buffer is full.
 *  - FIFO order of extraction.
 *  - Timeout handling when attempting to pop from an empty queue.
 *
 * The tests use GoogleTest and rely on `nonstd::optional` to
 * represent the presence or absence of values.
 */

/*****************************************************************************/

/* Standard libraries */

#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <chrono>
#include <vector>

/* Third party libraries */

#include "third_party/optional.hpp"

/* Project libraries */

#include "cola.h"

/*****************************************************************************/

/* Tests */

/**
 * @test KeepMaxBufferSize
 * @brief Ensures the Cola never grows beyond its maximum size.
 *
 * @details
 * After pushing 6 elements into a queue with max size 5:
 *  - Only the last 5 remain.
 *  - The first inserted element (0) is discarded automatically.
 *  - The first `pop()` retrieves the second inserted value (1).
 */
TEST(ColaTest, KeepMaxBufferSize) {
    Cola<int> cola;

    // Given: a queue with max size 5
    for (int i = 0; i < 6; i++) {
        cola.push(i);
    }

    // When: pushing beyond capacity
    // Then: the size is capped and the oldest element discarded
    EXPECT_EQ(cola.get_size(), 5u);
    auto val = cola.pop(std::chrono::seconds(5));
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 1);  // The first value (0) was deleted
}

/**
 * @test ExtractsInOrder
 * @brief Validates FIFO behavior of Cola::pop().
 *
 * @details
 * Pushes a sequence of values and ensures:
 *  - They are retrieved in the exact same order.
 *  - The queue is empty afterwards.
 *
 * This test is parameterized with multiple input sequences
 * to validate consistency across different cases.
 */
class ColaFifoTest : public ::testing::TestWithParam<std::vector<int>> {
   protected:
    Cola<int> cola;
};

TEST_P(ColaFifoTest, ExtractsInOrder) {
    auto values = GetParam();

    // Given: a queue with multiple pushes
    for (auto v : values) {
        cola.push(v);
    }

    // When & Then: pop retrieves elements in the same order
    for (auto expected : values) {
        auto val = cola.pop(std::chrono::seconds(5));
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(val.value(), expected);
    }
    EXPECT_TRUE(cola.is_empty());
}

// Instanciación con diferentes secuencias
INSTANTIATE_TEST_SUITE_P(MultipleSequences, ColaFifoTest,
                         ::testing::Values(std::vector<int>{10, 20},
                                           std::vector<int>{1, 2, 3, 4, 5}, std::vector<int>{42}));

/**
 * @test PopReturnsTimeout
 * @brief Ensures that pop() returns an empty optional if the queue
 *        remains empty during the wait period.
 *
 * @details
 * Attempts to retrieve data from an empty queue with a short timeout.
 * Since no data is produced, `pop()` returns `nonstd::nullopt`,
 * indicating timeout.
 */
TEST(ColaTest, PopReturnsTimeout) {
    Cola<int> cola;

    // Given: an empty queue
    // (nothing pushed)

    // When: attempting to pop with a short timeout
    auto extracted_value = cola.pop(std::chrono::seconds(1));

    // Then: the call must return an empty optional
    EXPECT_EQ(extracted_value, nonstd::nullopt);
}