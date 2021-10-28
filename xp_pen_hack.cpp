#include <fstream>
#include <iostream>
#include <linux/input.h>
#include <optional>
#include <regex>
#include <time.h>
#include <unistd.h>

std::optional<std::string> find_event_number() {
  std::ifstream devices{std::string("/proc/bus/input/devices"), std::ios::in};
  if (!devices) {
    std::cerr << "Could not open device file!" << std::endl;
    return {};
  }

  for (std::string line; getline(devices, line);) {
    if (line.find("XP-PEN DECO 02") == std::string::npos) {
        continue;
    }

    for (; line.rfind("H", 0) != 0; getline(devices, line)) {
    };

    std::regex event_nr_rx("event([0-9]+)");
    std::smatch matches;
    std::string number;
    if (std::regex_search(line, matches, event_nr_rx)) {
      number = matches[1].str();
    } else {
      continue;
    }

    for (; line.rfind("B: KEY", 0) != 0 && line.rfind("I", 0) != 0;
         getline(devices, line)) {
    };

    if (line.rfind("B: KEY=c03", 0) == 0)
      return number;
  }

  return {};
};

int main() {
  auto event_nr = find_event_number();
  if (!event_nr) {
      std::cout << "can't find tablet";
      return -1;
  }

  const auto event_dev = std::string("/dev/input/event") + *event_nr;
  std::cout << "Using device: " << event_dev << std::endl;

  std::ifstream events{event_dev,
                           std::ios::in | std::ios::binary};
  if (!events) {
      std::cerr << "Could not open device!" << std::endl;
          return -1;
      }

      input_event event;

      while (events.read((char *)&event, sizeof(event))) {
          if (event.type == 1 && event.code == 321) {
              if(event.value == 1)
                  system("xdotool key shift+ctrl+e");
              else
                  system("xdotool key shift+ctrl+p");
          }
      }

      std::cout << "Good bye!" << std::endl;

      return 0;
}
