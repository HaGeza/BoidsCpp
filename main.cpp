#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "BoidManager.hpp"
#include "display/Display.hpp"

// Enum for display type
enum class DisplayType { ASCII, SFML, UNKNOWN };

namespace DisplayTypeToStr {
str ASCII = "ascii";
str SFML = "sfml";

str getStr(DisplayType displayType) {
    switch (displayType) {
        case DisplayType::ASCII:
            return ASCII;
        case DisplayType::SFML:
            return SFML;
        default:
            return "unknown";
    }
}
}  // namespace DisplayTypeToStr

// Function to convert string to DisplayType enum
DisplayType stringToDisplayType(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   ::tolower);
    if (lowerStr == DisplayTypeToStr::ASCII) return DisplayType::ASCII;
    if (lowerStr == DisplayTypeToStr::SFML) return DisplayType::SFML;
    return DisplayType::UNKNOWN;
}

int main(int argc, char* argv[]) {
    // Default values
    DisplayType displayType = DisplayType::SFML;
    int numBoids = 10;

    // Parse command-line arguments as key-value pairs
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc - 1; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    // Process display type
    if (args.find("--display") != args.end()) {
        str displayTypeStr = args["--display"];
        displayType = stringToDisplayType(displayTypeStr);
        if (displayType == DisplayType::UNKNOWN) {
            std::cerr << "Invalid display type: " << displayTypeStr
                      << std::endl;
            return 1;
        }
    }

    // Process number of boids
    if (args.find("--numBoids") != args.end()) {
        str numBoidsStr = args["--numBoids"];
        try {
            numBoids = std::stoi(numBoidsStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number of boids: " << numBoidsStr
                      << std::endl;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number of boids out of range: " << numBoidsStr
                      << std::endl;
            return 1;
        }
    }

    std::cout << "Display Type: " << DisplayTypeToStr::getStr(displayType)
              << std::endl;
    std::cout << "Number of Boids: " << numBoids << std::endl;

    // Run simulation
    IDisplay* display;
    switch (displayType) {
        case DisplayType::ASCII:
            display = new ASCIIDisplay(80, 32);
            break;
        case DisplayType::SFML:
            display = new SFMLDisplay(800, 600);
            break;
        default:
            return 1;
    }

    BoidManager boidManager(display, numBoids);

    boidManager.start();

    // TODO: implement some stopping mechanism

    return 0;
}
