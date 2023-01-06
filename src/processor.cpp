#include "processor.h"
#include "linux_parser.h"

#include <vector>

/**
 * @brief Construct Processor object
 **/
Processor::Processor()
{
    const std::vector<long> jiffies = LinuxParser::CpuUtilization();
    _prevIdleJiffies   = LinuxParser::IdleJiffies(jiffies);   
    _prevTotalJiffies  = LinuxParser::Jiffies(jiffies);
}

/**
 * @brief Return the aggregate CPU utilization
 **/
float Processor::Utilization() 
{ 
    // get current jiffies from file 
    const std::vector<long> jiffies = LinuxParser::CpuUtilization();
    const auto currentIdleJiffies   = LinuxParser::IdleJiffies(jiffies);
    const auto currentTotalJiffies  = LinuxParser::Jiffies(jiffies);  
    
    // differentiate: actual value minus the previous one
    auto deltaTotal = currentTotalJiffies - _prevTotalJiffies;
    auto deltaIdle  = currentIdleJiffies  - _prevIdleJiffies; 
    
    // save jiffies
    _prevIdleJiffies   = currentIdleJiffies;
    _prevTotalJiffies  = currentTotalJiffies;
     
    // check to avoid division with zero
    float utilization = 0.0;
    if  (deltaTotal > 0)
    {
        utilization = static_cast<float>(deltaTotal - deltaIdle) / static_cast<float>(deltaTotal); 
    }
    return utilization;

}