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

#include "cola.h"

/*****************************************************************************/

/* Tests */

/**
 * @test MantieneMaximoCincoElementos
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
    EXPECT_EQ(cola.get_size(), 5);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), true);  // The first value (0) was deleted
    EXPECT_EQ(extracted_value, 1);
}

/**
 * @test ExtraerElementos
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
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), true);
    EXPECT_EQ(extracted_value, 10);
    EXPECT_EQ(cola.pop(extracted_value, std::chrono::seconds(5)), true);
    EXPECT_EQ(extracted_value, 20);
    EXPECT_TRUE(cola.is_empty());
}