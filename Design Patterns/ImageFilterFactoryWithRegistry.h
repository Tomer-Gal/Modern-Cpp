#pragma once

#include <memory>  // for std::unique_ptr
#include <string>
#include <unordered_map>
#include <functional>

namespace image_filter_factory_with_registry
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
        virtual ~ImageFilter() = default;

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
        using FilterCreator = std::function<std::unique_ptr<ImageFilter>()>;
        using FilterRegistry = std::unordered_map<std::string, FilterCreator>;

        // Static member function to create an image filter
        static std::unique_ptr<ImageFilter> createFilter(const std::string& type) 
        {
            const auto& registry = getRegistry();
            auto it = registry.find(type);
            if (it != registry.end()) 
            {
                return it->second();
            }
            else 
            {
                // Return a null pointer if the type is unknown
                return nullptr;
            }
        }

        static bool registerFilter(const std::string& type, FilterCreator creator) 
        {
            return getRegistry().emplace(type, std::move(creator)).second;
        }

    private:
        static FilterRegistry& getRegistry() 
        {
            static FilterRegistry registry;
            return registry;
        }
    };

    // Register filters in factory
    static bool cpuFilterRegistered = ImageFilterFactory::registerFilter("CPU", []() 
        {
        return std::make_unique<CPUImageFilter>();
        });

    static bool gpuFilterRegistered = ImageFilterFactory::registerFilter("GPU", []() 
        {
        return std::make_unique<GPUImageFilter>();
        });
}