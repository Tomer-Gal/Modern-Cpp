#pragma once
#include <memory>  // for std::unique_ptr
#include <string>

namespace image_filter_factory
{
    struct Image
    {
        unsigned char* data{};
        unsigned int width{}, height{};
    };

    // Abstract base class for image filters
    class ImageFilter
    {
    public:
        virtual ~ImageFilter() {}

        // Pure virtual function to apply the filter to an image
        virtual void apply(const Image& image) = 0;
    };

    // Concrete class for filtering images on the CPU
    class CPUImageFilter : public ImageFilter
    {
    public:
        // Override the apply function to apply the filter on the CPU
        void apply(const Image& image) override
        {
            // Implement the filter algorithm on the CPU...
        }
    };

    // Concrete class for filtering images on the GPU
    class GPUImageFilter : public ImageFilter
    {
    public:
        // Override the apply function to apply the filter on the GPU
        void apply(const Image& image) override
        {
            // Implement the filter algorithm on the GPU...
        }
    };

    // Factory class for creating image filters
    class ImageFilterFactory
    {
    public:
        // Static member function to create an image filter
        static std::unique_ptr<ImageFilter> createFilter(const std::string& type) 
        {
            if (type == "CPU") 
            {
                // Return a unique pointer to a CPUImageFilter object
                return std::make_unique<CPUImageFilter>();
            }
            else if (type == "GPU") 
            {
                // Return a unique pointer to a GPUImageFilter object
                return std::make_unique<GPUImageFilter>();
            }
            else 
            {
                // Return a null pointer if the type is unknown
                return nullptr;
            }
        }
    };
}