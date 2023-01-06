#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu(); 
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel() const;               
  std::string OperatingSystem() const;      

 private:
  void removeProcesses();
  void addProcess();
  const std::string _os;
  const std::string _kernel;
  Processor _cpu = {};
  std::vector<Process> _processes = {};
};

#endif