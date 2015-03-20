//
// Copyright 2015 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Unit tests for HandleAllocator.
//

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "libANGLE/HandleAllocator.h"

namespace
{

TEST(HandleAllocatorTest, ReservationsWithGaps)
{
    gl::HandleAllocator allocator;

    std::set<GLuint> allocationList;
    for (GLuint id = 2; id < 50; id += 2)
    {
        allocationList.insert(id);
    }

    for (GLuint id : allocationList)
    {
        allocator.reserve(id);
    }

    std::set<GLuint> allocatedList;
    for (size_t allocationNum = 0; allocationNum < allocationList.size() * 2; ++allocationNum)
    {
        GLuint handle = allocator.allocate();
        EXPECT_EQ(0, allocationList.count(handle));
        EXPECT_EQ(0, allocatedList.count(handle));
        allocatedList.insert(handle);
    }
}

TEST(HandleAllocatorTest, Random)
{
    gl::HandleAllocator allocator;

    std::set<GLuint> allocationList;
    for (size_t iterationCount = 0; iterationCount < 40; ++iterationCount)
    {
        for (size_t randomCount = 0; randomCount < 40; ++randomCount)
        {
            GLuint randomHandle = (rand() % 1000) + 1;
            if (allocationList.count(randomHandle) == 0)
            {
                allocator.reserve(randomHandle);
                allocationList.insert(randomHandle);
            }
        }

        for (size_t normalCount = 0; normalCount < 40; ++normalCount)
        {
            GLuint normalHandle = allocator.allocate();
            EXPECT_EQ(0, allocationList.count(normalHandle));
            allocationList.insert(normalHandle);
        }
    }
}

TEST(HandleAllocatorTest, Reallocation)
{
    // Note: no current test for overflow
    gl::HandleAllocator limitedAllocator(10);

    for (GLuint count = 1; count < 10; count++)
    {
        GLuint result = limitedAllocator.allocate();
        EXPECT_EQ(count, result);
    }

    for (GLuint count = 1; count < 10; count++)
    {
        limitedAllocator.release(count);
    }

    for (GLuint count = 2; count < 10; count++)
    {
        limitedAllocator.reserve(count);
    }

    GLint finalResult = limitedAllocator.allocate();
    EXPECT_EQ(finalResult, 1);
}

}