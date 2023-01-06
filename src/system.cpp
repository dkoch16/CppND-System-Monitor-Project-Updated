#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/**
 * @brief Construct System object
 **/
System::System()
: _os(LinuxParser::OperatingSystem())
, _kernel(LinuxParser::Kernel()) 
{


 const auto pIds = LinuxParser::Pids();
 for (const auto id : pIds)
 {
    _processes.emplace_back(Process(id)); 
 } 
}

/**
 * @brief Return the system's CPU
 **/
Processor& System::Cpu() { return _cpu; }

/**
 * @brief Return a container composed of the system's processes
 **/
vector<Process>& System::Processes() 
{ 
    const auto pIds = LinuxParser::Pids(); 
    
    // dismiss old data
    _processes = {};
    for (const auto id : pIds)
    {
        _processes.emplace_back(Process(id)); 
    } 
    std::sort(_processes.begin(), _processes.end());
    
    return _processes; 
}

/**
 * @brief Return the system's kernel identifier (string)
 **/
std::string System::Kernel() const { return _kernel; }

/**
 * @brief Return the system's memory utilization
 **/
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/**
 * @brief Return the operating system name
 **/
std::string System::OperatingSystem() const { return _os; }

/**
 * @brief Return the number of processes actively running on the system
 **/
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/**
 * @brief Return the total number of processes on the system
 **/
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/**
 * @brief Return the number of seconds since the system started running
 **/
long int System::UpTime() { return LinuxParser::UpTime(); }