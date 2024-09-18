#include "input/InputParsing.hpp"

#include <iostream>
#include <sstream>

str InputParsing::displayToStr(DisplayType displayType) {
    switch (displayType) {
        case DisplayType::ASCII:
            return ASCII;
        case DisplayType::SFML:
            return SFML;
        default:
            return "unknown";
    }
}

DisplayType InputParsing::stringToDisplayType(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   ::tolower);
    if (lowerStr == InputParsing::ASCII) return DisplayType::ASCII;
    if (lowerStr == InputParsing::SFML) return DisplayType::SFML;
    return DisplayType::UNKNOWN;
}

uint parseUnsignedInt(umap<str, str> args, str key, str name, uint def) {
    uint val = def;
    str upperName = name;
    upperName[0] = std::toupper(upperName[0]);
    std::stringstream ss;

    if (args.find(key) != args.end()) {
        str argStr = args[key];
        try {
            val = std::stoi(argStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid " << name << ": " << argStr << std::endl;

            ss << "Invalid " << name;
            throw std::invalid_argument(ss.str());
        } catch (const std::out_of_range& e) {
            std::cerr << upperName << " out of range: " << argStr << std::endl;

            ss << upperName << " out of range";
            throw std::invalid_argument(ss.str());
        }
    }

    return val;
}

dd parseNonDecreasingPositiveDoublePair(umap<str, str> args, str key, str name,
                                        dd def) {
    dd pair = def;
    str upperName = name;
    upperName[0] = std::toupper(upperName[0]);
    std::stringstream ss;

    if (args.find(key) != args.end()) {
        str speedStr = args[key];
        try {
            size_t pos = 0;
            str token;
            pos = speedStr.find(",");
            if (pos != str::npos) {
                token = speedStr.substr(0, pos);
                if (!token.empty()) pair.first = std::stod(token);
                speedStr.erase(0, pos + 1);
            }
            if (!speedStr.empty()) pair.second = std::stod(speedStr);

            if (pair.first > pair.second) {
                std::cerr << "Invalid " << name << ": " << speedStr
                          << std::endl;

                ss << "Invalid " << name;
                throw std::invalid_argument(ss.str());
            }
            if (pair.first <= 0 || pair.second <= 0) {
                std::cerr << upperName
                          << " must be strictly positive: " << speedStr
                          << std::endl;

                ss << upperName << " must be strictly positive";
                throw std::invalid_argument(ss.str());
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid " << name << ": " << speedStr << std::endl;
            ss << "Invalid " << name;
            throw std::invalid_argument(ss.str());
        } catch (const std::out_of_range& e) {
            std::cerr << upperName << " out of range: " << speedStr
                      << std::endl;
            ss << upperName << " out of range";
            throw std::invalid_argument(ss.str());
        }
    }
    return pair;
}

Arguments InputParsing::parseArguments(int argc, char* argv[], bool verbose) {
    // Default values
    auto def = Arguments();
    vec<double> weights = {std::get<0>(def.boidForceWeights),
                           std::get<1>(def.boidForceWeights),
                           std::get<2>(def.boidForceWeights)};

    // Parse command-line arguments as key-value pairs
    umap<str, str> args;
    for (int i = 1; i < argc - 1; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    // Process display type
    if (args.find("--display") != args.end()) {
        str displayTypeStr = args["--display"];
        def.displayType = stringToDisplayType(displayTypeStr);
        if (def.displayType == DisplayType::UNKNOWN) {
            std::cerr << "Invalid display type: " << displayTypeStr
                      << std::endl;
            throw std::invalid_argument("Invalid display type");
        }
    }

    // Process number of boids
    def.numBoids =
        parseUnsignedInt(args, "--num-boids", "number of boids", def.numBoids);

    // Process random seed
    def.randomSeed =
        parseUnsignedInt(args, "--seed", "random seed", def.randomSeed);

    // Process weights
    if (args.find("--weights") != args.end()) {
        str weightsStr = args["--weights"];
        try {
            size_t pos = 0, i = 0;
            str token;
            std::cout << "Weights: " << weightsStr << std::endl;
            while ((pos = weightsStr.find(",")) != str::npos) {
                token = weightsStr.substr(0, pos);
                std::cout << "Pos: " << pos << " ; Read: " << token
                          << std::endl;

                if (!token.empty()) weights[i] = std::stod(token);
                weightsStr.erase(0, pos + 1);
                if (++i >= weights.size() - 1) break;
            }
            if (!weightsStr.empty()) weights[i] = std::stod(weightsStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid weights: " << weightsStr << std::endl;
            throw std::invalid_argument("Invalid weights");
        } catch (const std::out_of_range& e) {
            std::cerr << "Weights out of range: " << weightsStr << std::endl;
            throw std::invalid_argument("Weights out of range");
        }
    }

    // Process speed range
    def.boidSpeed = parseNonDecreasingPositiveDoublePair(
        args, "--speed", "speed range", def.boidSpeed);

    // Process radii
    def.boidRadii = parseNonDecreasingPositiveDoublePair(
        args, "--radii", "radii", def.boidRadii);

    if (verbose) {
        std::cout << "Display Type: " << displayToStr(def.displayType)
                  << std::endl;
        std::cout << "Number of Boids: " << def.numBoids << std::endl;
        std::cout << "Weights: ";
        for (double weight : weights) {
            std::cout << weight << " ";
        }
        std::cout << std::endl;
        std::cout << "Speed Range: " << def.boidSpeed.first << " - "
                  << def.boidSpeed.second << std::endl;
    }

    def.boidForceWeights = {weights[0], weights[1], weights[2]};
    return {def.displayType, def.numBoids,  def.randomSeed,
            def.boidRadii,   def.boidSpeed, def.boidForceWeights};
}
