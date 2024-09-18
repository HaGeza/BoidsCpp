#ifndef INPUTPARSING_HPP
#define INPUTPARSING_HPP

#include "typedefs.hpp"

// Enum for display type
enum class DisplayType { ASCII, SFML, UNKNOWN };

struct Arguments {
    DisplayType displayType = DisplayType::SFML;
    uint numBoids = 10;
    uint randomSeed = 0;
    dd boidRadii = {0.1, 0.2};
    dd boidSpeed = {0.01, 0.015};
    ddd boidForceWeights = {1.0, 1.0, 1.0};
};

namespace InputParsing {
str ASCII = "ascii";
str SFML = "sfml";

str displayToStr(DisplayType displayType);

DisplayType stringToDisplayType(const std::string& str);

Arguments parseArguments(int argc, char* argv[], bool verbose = false);
}  // namespace InputParsing

#endif  // INPUTPARSING_HPP
