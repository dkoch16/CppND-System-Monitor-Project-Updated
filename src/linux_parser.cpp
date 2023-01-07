#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;


/**
 * @brief Read a value for the appropriated key from a file
 * @param[in] filePath Full path to file which should be read in
 * @param[in] searchedKey Key for which should be read value 
 * @param[in] defaultVaL  Default value if key not found   
 * 
 * @return A value for the appropriated key in file, if not found return default
 **/
template <typename T>
T readValueFromFile(const string& filePath, const string& searchedKey, T defaultVal)
{
  T value = defaultVal;
  string key, line;
  std::ifstream fileStream(filePath);
  if (fileStream.is_open()) 
  {
    while (std::getline(fileStream, line))
    {
      // remove some separators 
      std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream lineStream(line);
      lineStream >> key;
      if (key == searchedKey)
      {
        lineStream >> value; 
        break;
      }
    }
  }
  fileStream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream fileStream(kOSPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  fileStream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

/**
 * @brief Read and return the system memory utilization
 * 
 * @return Used memory in percent  
 **/
float LinuxParser::MemoryUtilization()
{ 
  const string filePath = kProcDirectory + kMeminfoFilename;
  const int memTotal    = readValueFromFile<int>(filePath, kFilterMemTotal, 0);
  const int memFree     = readValueFromFile<int>(filePath, kFilterMemFree, 0);
  
  // return relative usage of memory
  const float memUsed   = (memTotal == 0)? 0.0 : static_cast<float>(memTotal - memFree) / static_cast<float>(memTotal); 
  return memUsed; 
}

/**
 * @brief Read and return the system uptime
 * 
 * @return Uptime in seconds 
 **/
long LinuxParser::UpTime() 
{ 
  long uptime = 0.0;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) 
  {
    stream >> uptime;
  }
  stream.close();
  return uptime; 
}

/**
 * @brief Read and return the number of jiffies for the system
 * 
 * @param[in] jiffies A vector with jiffies values from cpu
 * 
 * @return A total number of jiffies 
 **/
long LinuxParser::Jiffies(const vector<long>& jiffies) 
{ 
  return ActiveJiffies(jiffies) + IdleJiffies(jiffies);
}

/**
 * @brief Read and return the number of active jiffies for a PID
 * 
 * @param[in] pid 
 * 
 * @return A active number of jiffies 
 **/ 
long LinuxParser::ActiveJiffies(int pid) 
{ 
  long uTime, sTime, cuTime, csTime;

  // define lambda function for skipping from reading determined number values 
  const auto skip = [](std::ifstream& fs, int numberSkip)
  {
    string unused;
    for (int k = 1; k <= numberSkip; k++)
    {
      fs >> unused;
    } 
  };
  
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);

  if (fileStream.is_open()) 
  {
    // skip reading of #1-13 values in file
    skip(fileStream, 13);
    // read #14-17
    fileStream >> uTime >> sTime >> cuTime >> csTime;
  }

  long total_time = uTime + sTime + cuTime + csTime;  
  return total_time;
}

/**
 * @brief Read and return the number of active jiffies for the system
 * 
 * @param[in] jiffies A vector with jiffies values from cpu
 * 
 * @return A number of active jiffies 
 **/
long LinuxParser::ActiveJiffies(const vector<long>& jiffies) 
{ 
  return jiffies[kUser_] + jiffies[kNice_] + jiffies[kSystem_] + jiffies[kIRQ_] + jiffies[kSoftIRQ_] + jiffies[kSteal_];
}


/**
 * @brief Read and return the number of idle jiffies for the system
 * 
 * @param[in] jiffies A vector with jiffies values from cpu
 * 
 * @return A number of idle jiffies 
 **/
long LinuxParser::IdleJiffies(const vector<long>& jiffies) 
{ 
  return jiffies[kIdle_] + jiffies[kIOwait_]; 
}

/**
 * @brief Read and return CPU utilization
 * 
 * @return A vector with CPU utilization values (jiffies)
 **/
vector<long> LinuxParser::CpuUtilization() 
{ 
  string key;
  vector<long> values;

  string line;

  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {

      std::istringstream lineStream(line);
      lineStream >> key; 

      if (key == kFilterCpu)
      {
        std::copy(
          std::istream_iterator<long>(lineStream), 
          std::istream_iterator<long>(), 
          std::back_inserter(values));
        break;         
      }
    }
  }
  fileStream.close();
  return values;  
}

/**
 * @brief Read and return the total number of processes
 * 
 * @return Total number of processes 
 **/
int LinuxParser::TotalProcesses() 
{ 
  const string filePath = kProcDirectory + kStatFilename;
  const int totalNumber  = readValueFromFile<int>(filePath, kFilterProcesses, 0);

  return totalNumber; 
}

/**
 * @brief Read and return the number of running processes
 * 
 * @return Running number of processes 
 **/
int LinuxParser::RunningProcesses() 
{ 
  const string filePath    = kProcDirectory + kStatFilename;
  
  const int runningNumber  = readValueFromFile<int>(filePath, kFilterRunningProcesses, 0);

  return runningNumber; 
}

/**
 * @brief Read and return the command associated with a process
 *  
 * @param[in] pid  
 * @return command full path of associated process 
 **/
string LinuxParser::Command(int pid) 
{ 
  string cmd;

  std::ifstream stream(kProcDirectory +  to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    stream >> cmd;
  }  
  stream.close();
  return cmd; 
}

/**
 * @brief Read and return the memory used by a process
 *
 * @param[in] pid   
 * @return used ram in mb 
 **/
int LinuxParser::Ram(int pid) 
{ 
  const string filePath = kProcDirectory +  to_string(pid) + kStatusFilename;
  const int sizeInKB    = readValueFromFile<int>(filePath, kFilterProcMem, 0);
  const int sizeInMB    = (int)( (sizeInKB / 1024.0) + 0.5);
  return sizeInMB; 
}

/**
 * @brief Read and return the user ID associated with a process
 * 
 * @param[in] pid  
 * @return user id
 **/
int LinuxParser::Uid(int pid) 
{ 
  const string filePath = kProcDirectory +  to_string(pid) + kStatusFilename;
  const int uid      = readValueFromFile<int>(filePath, kFilterUID, 0);
  return uid; 
}

/**
 * @brief Read and return the user associated with a process
 * 
 * @param[in] pid  
 * @return user id name
 **/
string LinuxParser::User(int pid) 
{ 
  string user, passwd; 
  int id;
  string line;
  int uid = LinuxParser::Uid(pid);
  
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) 
  {
    while (std::getline(fileStream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');    

      std::istringstream lineStream(line);
      lineStream >> user >> passwd >> id;
      if (uid == id)
      {
        break;
      }
    }
  }

  fileStream.close();
  return user; 
}

/**
 * @brief Read and return the uptime of a process
 * 
 * @param[in] pid   
 * @return uptime in seconds
 **/
long LinuxParser::UpTime(int pid) 
{ 
  int uptime;

  // define lambda function for skipping from reading determined number values 
  const auto skip = [](std::ifstream& fs, int numberSkip)
  {
    string unused;
    for (int k = 1; k <= numberSkip; k++)
    {
      fs >> unused;
    } 
  };

  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);

  if (fileStream.is_open()) 
  {
    // skip reading of #1-#21 in file
    skip(fileStream, 21);
    // read #22 
    fileStream >> uptime;
  }
  fileStream.close();

  return (uptime / sysconf(_SC_CLK_TCK)); 
}
