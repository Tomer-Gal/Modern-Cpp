#include "pch.h"
#include "ImageFilterFactoryWithRegistry.h"

using namespace image_filter_factory_with_registry;

TEST(Factory, create_filters)
{
    auto cpuFilter = ImageFilterFactory::createFilter("CPU");
    auto gpuFilter = ImageFilterFactory::createFilter("GPU");
    auto unknownFilter = ImageFilterFactory::createFilter("Unknown");

    // Check if the filters are created correctly
    ASSERT_NE(cpuFilter, nullptr);
    ASSERT_NE(gpuFilter, nullptr);
    ASSERT_EQ(unknownFilter, nullptr);

    // Check if the created filters have the correct types
    EXPECT_TRUE(dynamic_cast<CPUImageFilter*>(cpuFilter.get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<GPUImageFilter*>(gpuFilter.get()) != nullptr);
}

TEST(Factory, apply_filters)
{
    auto cpuFilter = ImageFilterFactory::createFilter("CPU");
    auto gpuFilter = ImageFilterFactory::createFilter("GPU");

    Image testImage{
        nullptr, // No actual image data is provided for this example
        640,     // width
        480      // height
    };

    // Call the apply functions for both filters (assuming the filter implementations are provided)
    ASSERT_NO_THROW(cpuFilter->apply(testImage));
    ASSERT_NO_THROW(gpuFilter->apply(testImage));
}