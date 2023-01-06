#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);
std::string Ram(int ram);
std::string Pid(int pid);  
std::string Command(std::string command, int maxSize = 100);  
};                                    // namespace Format

#endif