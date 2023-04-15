#include "pch.h"
#include <vector>  // for std::vector
#include <memory>

// Abstract base class for observing CPU/GPU usage
class Observer
{
public:
    virtual ~Observer() {}

    // Pure virtual function to receive updates from the subject
    virtual void update(double cpuUsage, double gpuUsage) = 0;
};

// Subject class for CPU/GPU usage
class CPUGPUUsage
{
public:
    // Add an observer to the list of observers
    void attach(Observer* observer)
    {
        m_observers.push_back(observer);
    }

    // Remove an observer from the list of observers
    void detach(Observer* observer_to_remove)
    {
        for (auto it = m_observers.begin(); it <= m_observers.end(); ++it)
        {
            if (*it == observer_to_remove)
            {
                m_observers.erase(it);
                break;
            }

        }
    }

    // Notify all observers of updated CPU/GPU usage
    void notifyObservers(double cpuUsage, double gpuUsage)
    {
        for (const auto& observer : m_observers)
        {
            observer->update(cpuUsage, gpuUsage);
        }
    }

    // Other member functions and data members...

private:
    std::vector<Observer*> m_observers;
};