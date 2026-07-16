# Arcane Defense

**Arcane Defense** is a technical vertical slice developed primarily in C++ using Unreal Engine 5.8.

The project is intended to demonstrate gameplay and systems programming skills through a small but complete playable experience.

## Project Goal

Create an 8–12 minute wave-defense game that combines:

* Target-based spell combat.
* Unreal Engine's Gameplay Ability System.
* Enemy AI and navigation.
* Placeable defensive traps.
* Wave-based progression.
* Experience, levels, and talent choices.
* Data-driven gameplay configuration.
* Automated testing and performance profiling.

The goal is not to build a complete commercial game, but to create a polished and technically documented portfolio project.

## Core Features

The planned vertical slice includes:

* A third-person playable character.
* Click and tab target selection.
* Health and mana attributes.
* Spell casting, mana costs, cooldowns, and interruptions.
* Four different player abilities.
* Three enemy archetypes.
* Five configurable enemy waves.
* A defense objective with victory and defeat conditions.
* Two placeable traps.
* Experience and in-match level progression.
* A talent selection system.
* Functional gameplay UI.
* Automated tests.
* Performance analysis using Unreal Insights.
* A packaged playable build.

## Technical Approach

The main gameplay logic will be implemented in C++.

Blueprints will primarily be used for:

* Actor composition.
* Asset configuration.
* User interfaces.
* Visual effects.
* Audio.
* Animation.
* Data-driven tuning.

The project uses or plans to use:

* Unreal Engine 5.8.
* C++.
* Enhanced Input.
* Gameplay Ability System.
* Gameplay Tags.
* Gameplay Effects.
* Data Assets and Data Tables.
* Behavior Trees.
* Unreal Motion Graphics.
* Unreal Insights.
* Unreal Automation Tests.

## Requirements

* Unreal Engine 5.8.
* Visual Studio 2022 with C++ game development support.
* Git.
* Git LFS.

## Opening the Project

1. Clone the repository.

   ```bash
   git clone <repository-url>
   ```

2. Enter the project directory.

   ```bash
   cd ArcaneDefense
   ```

3. Initialize Git LFS.

   ```bash
   git lfs install
   git lfs pull
   ```

4. Generate the Visual Studio project files if necessary.

   On Windows, right-click `ArcaneDefense.uproject` and select:

   ```text
   Generate Visual Studio project files
   ```

5. Open the generated Visual Studio solution.

6. Select the following build configuration:

   ```text
   Development Editor | Win64
   ```

7. Build the `ArcaneDefense` project.

8. Open `ArcaneDefense.uproject`.

## Current Project Status

### Day 1 — Project Setup

Completed:

* Created the Unreal Engine 5.8 C++ project.
* Configured the project using the Third Person template.
* Enabled Enhanced Input.
* Enabled Gameplay Ability System.
* Added the required GAS module dependencies.
* Created the prototype map.
* Configured Git.
* Configured Git LFS for Unreal binary assets.
* Added the project `.gitignore`.
* Added the initial project documentation.

## Project Structure

The initial source structure will grow incrementally as each gameplay system is implemented.

```text
Source/
└── ArcaneDefense/
    ├── Characters/
    ├── AbilitySystem/
    ├── Combat/
    ├── AI/
    ├── Waves/
    ├── Traps/
    ├── Progression/
    ├── Objective/
    ├── UI/
    ├── Game/
    ├── Data/
    └── Tests/
```

Unreal assets are stored under:

```text
Content/
└── ArcaneDefense/
```

Generated folders such as `Binaries`, `Intermediate`, `Saved`, and `DerivedDataCache` are not tracked by Git.

## Development Principles

* Core gameplay rules are implemented in C++.
* Blueprint is used for configuration and presentation.
* Configurable gameplay values should not be hardcoded.
* Gameplay systems should have clear and limited responsibilities.
* New abstractions should only be introduced when there is a real requirement.
* Every development session should end with a verifiable result.
* Every completed task should be committed to version control.
* Features outside the defined vertical slice should not delay the main deliverables.

## Out of Scope

The initial version will not include:

* Multiplayer.
* Matchmaking.
* Backend services.
* Inventory.
* Equipment.
* Loot.
* Crafting.
* Shops.
* A campaign.
* Multiple maps.
* Multiple playable classes.
* A complex talent tree.
* Custom character modelling.
* Custom animation production.

## Development Roadmap

The project is planned as an eight-week vertical slice:

1. Project foundation and target selection.
2. Spell casting and wave defense.
3. Player abilities and gameplay effects.
4. Placeable traps.
5. Experience, levels, and talents.
6. Enemy types and complete wave configuration.
7. Testing, profiling, and optimization.
8. Presentation, packaging, and portfolio documentation.

## Definition of Done

The vertical slice will be considered complete when it includes:

* A playable packaged build.
* A complete 8–12 minute match.
* Four player spells.
* Two placeable traps.
* Three enemy types.
* Five enemy waves.
* Experience and level progression.
* Between six and nine talents.
* Victory and defeat conditions.
* Functional gameplay UI.
* Automated tests.
* A documented profiling and optimization pass.
* Technical project documentation.
* A short gameplay presentation video.

## License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
