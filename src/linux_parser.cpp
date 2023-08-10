#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


vector<string> LinuxParser::ParseLineToLongVector(string filename) {

  string line, value;
  vector<string> parsedVector;

  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> value) {
        parsedVector.push_back(value);
      }
    }
  }
  filestream.close();
  return parsedVector;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = 0, memFree = 0;
  string line, memory, value;; 
  std::ifstream stream( kProcDirectory + kMeminfoFilename);
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> memory >> value;
    if (memory == "MemTotal:") {
        memTotal = stof(value);
    }
    else if (memory == "MemFree:") {
      memFree = stof(value);
      break;
    }
  }
  return (memTotal - memFree) / memTotal;

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, upTime, idleTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stringstream(line);
    stringstream >> upTime >> idleTime;
    return stol(upTime);
    }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, cpu, userT, niceT, systemT, idelT, iowaitT, irqT, softT, stealT, guestT, niceguestT;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stringstream(line);
    stringstream >> cpu >> userT >> niceT >> systemT >> idelT >> iowaitT >> irqT >> softT >> stealT >> guestT >> niceguestT;
    return stol(userT) + stol(niceT) + stol(systemT) + stol(idelT) + stol(stealT) + stol(guestT) + stol(niceguestT);
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> parsedData = ParseLineToLongVector(kProcDirectory + to_string(pid) + kStatFilename);
  if (parsedData.size() >= 16) {
    return stol(parsedData[13]) + stol(parsedData[14]) + stol(parsedData[15]) + stol(parsedData[16]);
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, cpu, userT, niceT, systemT, idelT, iowaitT, irqT, softT, stealT, guestT, niceguestT;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stringstream(line);
    stringstream >> cpu >> userT >> niceT >> systemT >> idelT >> iowaitT >> irqT >> softT >> stealT >> guestT >> niceguestT;
    return stol(userT) + stol(niceT) + stol(systemT);
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, cpu, userT, niceT, systemT, idelT, iowaitT, irqT, softT, stealT, guestT, niceguestT;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stringstream(line);
    stringstream >> cpu >> userT >> niceT >> systemT >> idelT >> iowaitT >> irqT >> softT >> stealT >> guestT >> niceguestT;
    return stoi(idelT) + stoi(iowaitT);
  }
  return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  string thisUser, thisSteal, thisIdle, thisIowait, thisSystem, thisNice, thisIrq, thisSoftIrq, cpuString;

  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> cpuString >> thisUser >> thisNice >> thisSystem >> thisIdle >> thisIowait >> thisIrq >> thisSoftIrq >> thisSteal) {
        if (cpuString == "cpu") {
          return vector<string>{thisUser, thisNice, thisSystem, thisIdle, thisIowait, thisIrq, thisSoftIrq, thisSteal};
        }
      }
    }
  }
  return {};
}

std::vector<std::string> LinuxParser::CpuUtilizationPid(int pid) {
  
  // information from https://man7.org/linux/man-pages/man5/proc.5.html
  string line, uTime, sTime, cuTime, csTime, startTime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream stringstream(line);
    string word;
    vector<string> cpuInfo, pasrsedLine;

    while (stringstream >> word) {
        pasrsedLine.push_back(word);
    }

    for (int item = 0; item < 22; item++) {
      switch (item) {
        case 13:
        case 14:
        case 15:
        case 16:
        case 21:
          // intended fallthrough 
          cpuInfo.push_back(pasrsedLine[item]);
        default:
          break;
      }
    }
    return cpuInfo;
  }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int num_processes = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.substr(0, 9) == "processes") {
        num_processes = stoi(line.substr(10));
      }
    }
  }
  return num_processes;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_processes = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.substr(0, 13) == "procs_running") {
        running_processes =  stoi(line.substr(14));
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    return line;
  }
  return "No Command"; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, value, data; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream stringstream(line);
      stringstream >> value >> data;
      if (value == "VmRSS:") {
        return data;
      }
    }
  }
  return "Na"; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, value, data; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> value >> data) {
        if (value == "Uid:") {
          return data;
        } 
      }
    }
  }
  return "Na"; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  
  string line, user, xValue, id;
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);

      while (stringstream >> user >> xValue >> id) {
        if (id == Uid(pid)) {
          return user;
        }
      }
    }
  }
  return "user"; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  vector<string> parsedVect = ParseLineToLongVector(kProcDirectory + to_string(pid) + kStatFilename);

  if (parsedVect.size() >= 21) {
    return (UpTime() - (stol(parsedVect[21]) / 60));
  }
  return 0; 
}
