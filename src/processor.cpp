#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
      
	std::vector<std::string> cpuInfo = LinuxParser::CpuUtilization();
    float thisUser, thisSteal, thisIdle, thisIowait, thisSystem, thisNice, thisIrq, thisSoftIrq;
    thisUser = std::stof(cpuInfo[0]);
	thisSteal = std::stof(cpuInfo[1]);
	thisIdle = std::stof(cpuInfo[2]);
	thisIowait = std::stof(cpuInfo[3]);
	thisSystem = std::stof(cpuInfo[4]);
	thisNice = std::stof(cpuInfo[5]);
	thisIrq = std::stof(cpuInfo[6]);
	thisSoftIrq = std::stof(cpuInfo[7]);
	
	float idle = thisIdle + thisIowait;
	float active = thisUser + thisSteal + thisSystem + thisNice + thisIrq + thisSoftIrq;
	float total = idle + active;
	
	return (total - idle) / total;
}