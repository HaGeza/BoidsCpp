#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "boid/BoidManager.hpp"
#include "boid/neighborhood/QTreeBoidNeighborhoods.hpp"
#include "display/Display.hpp"
#include "input/InputParsing.hpp"

int main(int argc, char* argv[]) {
    Arguments args = InputParsing::parseArguments(argc, argv, true);

    // Run simulation
    IDisplay* display;
    switch (args.displayType) {
        case DisplayType::ASCII:
            display = new ASCIIDisplay(80, 32);
            break;
        case DisplayType::SFML:
            display = new SFMLDisplay(800, 600);
            break;
        default:
            return 1;
    }

    double boidRadius = 0.25;

    IBoidNeighborhoods* boidNeighborhoods =
        new QTreeBoidNeighborhoods(1.0, 2, boidRadius);

    BoidManager boidManager(display, args.numBoids, boidNeighborhoods,
                            args.boidRadii, args.boidSpeed,
                            args.boidForceWeights, args.randomSeed,
                            args.periodicBoundary);
    boidManager.run();

    return 0;
}
