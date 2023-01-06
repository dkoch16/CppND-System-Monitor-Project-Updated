#include <string>

#include "format.h"

using std::string;

/**
 * @brief Format seconds to string time format HH:MM:SS
 * 
 * @param[in] seconds    
 * @return string time in format HH:MM:SS
 **/
string Format::ElapsedTime(long seconds) 
{  
    // calc hours bw 0..23
    long hours   = (seconds / (60 * 60)) % 24;
 
    // calc minutes bw 0..59 
    long minutes = (seconds / 60) % 60;
    
    // calc seconds bw 0..59
    seconds  = seconds % 60;
    
    // format to string in HH:MM:SS format
    std::string hh =  (hours   >= 10) ? std::to_string(hours)   : "0" + std::to_string(hours);
    std::string mm =  (minutes >= 10) ? std::to_string(minutes) : "0" + std::to_string(minutes);
    std::string ss =  (seconds >= 10) ? std::to_string(seconds) : "0" + std::to_string(seconds);
    return  hh + ":" +  mm + ":" + ss;
}

/**
 * @brief Format memory numbers to same char size in colum of length 8
 * 
 * @param[in] ram size of used memory      
 * @return formatted memory number as string
 **/
string Format::Ram(int ram) 
{  
    char ramBuffer[8];
    sprintf(ramBuffer, "%7d", ram);
    return string(ramBuffer); 
}

/**
 * @brief Format pids to same size in colum of length 7
 * 
 * @param[in] pid process ID     
 * @return formatted pid number as string 
 **/
string Format::Pid(int pid)
{
    char pidBuffer[7];
    sprintf(pidBuffer, "%6d", pid);
    return string(pidBuffer); 
}

/**
 * @brief Format command string to same size in column of max size left in column
 * 
 * @param[in] pid process ID 
 * @param[in] maxSize max size left in column        
 * @return formatted command as string 
 **/
string Format::Command(string command, int maxSize)
{
    char pidBuffer[maxSize];
    string format = "%-" + std::to_string(maxSize-1)+ "s";
    sprintf(pidBuffer, format.c_str(), command.substr(0, maxSize).c_str());
    return string(pidBuffer); 
}