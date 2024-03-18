# Asteroid Space Shooter Game Documentation

Welcome to the Asteroid Space Shooter Game, an engaging 2D space adventure where players navigate a spaceship through asteroid fields, dodging and destroying obstacles with well-timed projectiles. This project leverages the Simple and Fast Multimedia Library (SFML) to provide a rich graphical experience and responsive gameplay.

## Table of Contents

- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
- [Building and Running](#building-and-running)
- [Gameplay Overview](#gameplay-overview)
- [Key Components](#key-components)
    - [Spaceship](#spaceship)
    - [Asteroid](#asteroid)
    - [GameManager](#gamemanager)
    - [ScoreManager](#scoremanager)
    - [GameRendering](#gamerendering)
- [Main Game Loop](#main-game-loop)
- [Features](#features)
    - [Collision Detection](#collision-detection)
    - [Scoring and High Scores](#scoring-and-high-scores)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Before you begin, ensure you have the following software installed on your system:

- C++ Compiler supporting C++11 or later (GCC, Clang, MSVC)
- SFML Library (version 2.5 or later)

### Installation

1. **Clone the repository**

   ```
   git clone https://github.com/kshashkina/AsteroidsGame
   ```

2. **Navigate to the project directory**

   ```
   cd AsteroidsGame
   ```

3. **Ensure SFML is installed and correctly set up**

   SFML installation varies by operating system. Please follow the [official SFML setup tutorial](https://www.sfml-dev.org/tutorials/).

## Building and Running

After installing all prerequisites and configuring SFML, follow these steps to compile and run the game:

1. **Compile the project**

   Using GCC:

   ```
   g++ -std=c++11 -o AsteroidGame main.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```

   Adjust the command according to your compiler and setup.

2. **Run the game**

   ```
   ./AsteroidGame
   ```

## Gameplay Overview

In Asteroid Space Shooter, players control a spaceship navigating through space filled with asteroids. The goal is to avoid or destroy these asteroids using projectiles and survive as long as possible to achieve high scores.

## Key Components

### Spaceship

- **Functionality**: Manages movement, rotation, projectile shooting, and life count.
- **Key Attributes**: Position, rotation, textures, and projectile management.

### Asteroid

- **Functionality**: Handles asteroid generation, movement, and rendering.
- **Key Attributes**: Textures, spawn intervals, and movement patterns.

### GameManager

- **Functionality**: Manages game logic including collisions and game state transitions.
- **Key Attributes**: Game over state and collision checks.

### ScoreManager

- **Functionality**: Tracks current score, high scores, and handles their persistence.
- **Key Attributes**: Current score and file management for high scores.

### GameRendering

- **Functionality**: Manages all rendering tasks, including drawing sprites, UI components, and managing game states.
- **Key Attributes**: Fonts and custom cursors.

## Main Game Loop

The game loop is the heart of the game, handling events, updating game states, and rendering frames. It processes input, updates positions, checks collisions, updates scores, and redraws the screen.

## Features

### Collision Detection

Implements AABB collision detection to manage interactions between the spaceship, asteroids, and projectiles, updating game states based on these interactions.

### Scoring and High Scores

Points are awarded for destroying asteroids, with scores saved to track high scores across game sessions.

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


## Acknowledgments

- The SFML Team for the fantastic multimedia library.
- The open-source community for continuous inspiration and support.