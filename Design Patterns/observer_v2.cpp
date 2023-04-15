#include "pch.h"
#include <vector>
#include <memory>

// Abstract base class for observing CPU/GPU usage
class Observer
{
public:
    virtual ~Observer() = default;

    // Pure virtual function to receive updates from the subject
    virtual void update(const double& cpuUsage, const double& gpuUsage) = 0;
};

// Subject class for CPU/GPU usage
class CPUGPUUsage
{
public:
    // Add an observer to the list of observers
    void attach(std::weak_ptr<Observer> observer)
    {
        m_observers.push_back(observer);
    }

    // Remove an observer from the list of observers
    void detach(std::weak_ptr<Observer> observer_to_remove)
    {
        for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
        {
            if (it->lock() == observer_to_remove.lock())
            {
                m_observers.erase(it);
                break;
            }
        }
    }

    // Notify all observers of updated CPU/GPU usage
    void notifyObservers(const double& cpuUsage, const double& gpuUsage)
    {
        // auto self = shared_from_this();
        for (const auto& observer : m_observers)
        {
            auto shared_observer = observer.lock();
            if (shared_observer)
            {
                shared_observer->update(cpuUsage, gpuUsage);
            }
        }
    }

    // Other member functions and data members...

private:
    std::vector<std::weak_ptr<Observer>> m_observers;
};

