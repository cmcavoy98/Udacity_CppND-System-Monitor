#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds){ 
  long int minutes, hours;

  minutes = seconds / 60;
  hours = minutes / 60;

  // Formatting to ensure correct display of single digit values 
  std::string rtr;
  (hours < 10) ? rtr.append("0" + std::to_string(hours) + ".") : rtr.append(std::to_string(hours) + ".");
  (minutes < 10) ? rtr.append("0" + std::to_string(minutes) + ".") : rtr.append(std::to_string(minutes) + ".");
  ((seconds % 60) < 10) ? rtr.append("0" + std::to_string(seconds % 60)) : rtr.append(std::to_string(seconds % 60));

  return rtr;     
}
