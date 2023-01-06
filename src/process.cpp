#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

/**
 * @brief Construct Process object for appropriate process id
 * 
 * @param[in] id A Process Id  
 **/
Process::Process(const int id)
: _id(id)
, _user(LinuxParser::User(id))
, _command(LinuxParser::Command(id))
{
    // update cpu usage needed for sort
    CpuUtilization();
}

/**
 * @brief Return this process's ID
 **/
int Process::Pid() const { return _id; }

/**
 * @brief Return this process's CPU utilization
 **/
float Process::CpuUtilization() 
{ 
   const long systemUpTime     = LinuxParser::UpTime();
   const long processUpTime    = UpTime();

   const long totalTimeActive  = LinuxParser::ActiveJiffies(_id)/ sysconf(_SC_CLK_TCK);
   _cpuUsage = static_cast<float>(totalTimeActive) / static_cast<float>(systemUpTime-processUpTime);
   return _cpuUsage; 
}

/**
 * @brief Return the command that generated this process
 **/
string Process::Command() const { return _command; }

/**
 * @brief Return this process's memory utilization
 **/
int Process::Ram() 
{ 
    return LinuxParser::Ram(_id); 
}

/**
 * @brief Return the user (name) that generated this process
 **/
string Process::User() const { return _user; }

/**
 * @brief Return the age of this process (in seconds)
 **/
long int Process::UpTime() 
{ 
   return LinuxParser::UpTime(_id);
}

/**
 * @brief "less than" comparison operator for Process objects
 * 
 * @param[in] other A reference to other Process instance  
 * @return bool of comparison
 **/
bool Process::operator<(Process const& other) const 
{ 
   return  other._cpuUsage < this->_cpuUsage;
}