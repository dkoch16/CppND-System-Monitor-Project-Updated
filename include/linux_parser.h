#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Filter Keys
const std::string kFilterProcesses("processes");
const std::string kFilterRunningProcesses("procs_running");
const std::string kFilterMemTotal("MemTotal");
const std::string kFilterMemFree("MemFree");
const std::string kFilterCpu("cpu");
const std::string kFilterUID("Uid");
const std::string kFilterProcMem("VmSize");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<long> CpuUtilization();
long Jiffies(const std::vector<long>& jiffies);
long ActiveJiffies(const std::vector<long>& jiffies);
long ActiveJiffies(int pid);
long IdleJiffies(const std::vector<long>& jiffies);

// Processes
std::string Command(int pid);
int Ram(int pid);
int Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif