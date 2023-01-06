#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(const int id);
    int Pid() const;                              
    std::string User() const;                      
    std::string Command() const;                   
    float CpuUtilization();                  
    int Ram();                       
    long int UpTime();                       
    bool operator<(Process const& other) const;  
 
 private:
    int _id;
    std::string _user;
    std::string _command;
    float _cpuUsage{0};
};

#endif