# Boids Simulation

A real-time 2D flocking simulation written in C++ using Raylib, based on Craig Reynolds' boids algorithm.

---

## What are Boids

Boids is an artificial life simulation developed by Craig Reynolds in 1986. It models the collective motion of animals such as birds, fish, and insects using a set of simple local rules applied to each individual agent. Despite the simplicity of the rules, the emergent behavior closely resembles real-world flocking.

Each boid has no knowledge of the global state of the flock. It only perceives other boids within a fixed radius and reacts accordingly. The complex group behavior you see is entirely emergent from individual interactions.

---

## The Three Rules

### Separation

Each boid steers away from neighbors that are too close, preventing overlap and crowding. The repulsion force is weighted by inverse distance, meaning boids that are very close exert a much stronger push than those near the edge of the separation radius.

### Alignment

Each boid steers toward the average velocity of its neighbors within the perception radius. This causes boids to gradually match the direction and speed of those around them, producing the streaking, directional motion characteristic of flocks.

### Cohesion

Each boid steers toward the average position of its neighbors, pulling individuals back toward the group. Combined with separation, this keeps the flock together without boids colliding.

All three forces are computed each frame, weighted, clamped to a maximum force, and accumulated into the boid's acceleration. The boid's velocity is then updated and clamped to a minimum and maximum speed to ensure natural-looking motion.

---

## Build Instructions

### Requirements

- macOS (Apple Silicon or Intel)
- CMake 3.20 or later
- Raylib 5.5 (via Homebrew)

### Install dependencies

```bash
brew install raylib
```

### Clone and build

```bash
git clone https://github.com/yourusername/boids.git
cd boids
mkdir build && cd build
cmake ..
make
./boids
```

---

## Configuration

The following constants in `main.cpp` can be tuned to change the simulation behavior:

| Constant          | Description                              |
|-------------------|------------------------------------------|
| `MAX_SPEED`       | Maximum boid velocity                    |
| `MIN_SPEED`       | Minimum boid velocity                    |
| `MAX_FORCE`       | Maximum steering force per frame         |
| `RADIUS_OF_PRECEP`| Neighborhood radius for cohesion/alignment |
| `RADIUS_OF_SEP`   | Radius within which separation activates |
| `SEP_WEIGHT`      | Multiplier for separation force strength |
