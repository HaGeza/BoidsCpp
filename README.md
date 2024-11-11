# Boids

This is a simple [Boids Simulation](https://en.wikipedia.org/wiki/Boids), written in C++ 17.

![Example 1](https://github.com/HaGeza/BoidsCpp/blob/main/images/sfml_example1.png)
![Example 2](https://github.com/HaGeza/BoidsCpp/blob/main/images/sfml_example2.png)

## Usage

The application was tested on MacOS Sonoma version 14.6.1, with clang 15.0.0.

To compile the project, navigate to `build` and run:
```sh
cmake ..; make
```
Then to run the simulation, use:
```sh
./boids
```

The following arguments are available:
- `--display ...`: Controls how the simulation is displayed. One of `ascii` and `sfml` (default).
  To use [SFML](https://www.sfml-dev.org/) on MacOS, with brew installed run: `brew install sfml`.
- `--periodic`: Whether to use periodic boundaries. This tends to result in
  less interesting simulations.
- `--numBoids ...`: Number of boids, default is 50.
- `--weights [cohesion],[alignment],[separation]`: Un-normalized strengths of the forces
  being applied. By default all are equal.
- `--speed [min],[max]`: range of speeds for the boids, with `0.01` and `0.015` as defaults.
- `--radii [inner],[outer]`: radii for calculating forces on each boid. `inner` is used for
  separation and has default of `0.1`, i.e. 10% of the map. `outer` is used for cohesion and
  alignment and has default of `0.2`.
