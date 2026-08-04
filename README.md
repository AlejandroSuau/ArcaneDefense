# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using Unreal Engine 5.8.

The project is designed to demonstrate gameplay and systems programming skills through a small but complete wave-defense experience.

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

The goal is not to build a complete commercial game, but to create a polished and technically documented portfolio project for Gameplay Programmer and Systems Programmer positions.

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

Core gameplay logic is implemented in C++.

Blueprints are primarily used for:

* Actor composition.
* Asset configuration.
* User interfaces.
* Visual feedback.
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
* Visual Studio 2022 or JetBrains Rider with Unreal Engine support.
* A compatible C++ compiler toolchain.
* Git.
* Git LFS.

## Opening the Project

1. Clone the repository:

   ```bash
   git clone <repository-url>
   ```

2. Enter the project directory:

   ```bash
   cd ArcaneDefense
   ```

3. Initialize Git LFS:

   ```bash
   git lfs install
   git lfs pull
   ```

4. Generate the project files if necessary.

   On Windows, right-click `ArcaneDefense.uproject` and select:

   ```text
   Generate Visual Studio project files
   ```

5. Open the generated solution or the `.uproject` file using the configured IDE.

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
* Added the required Gameplay Ability System module dependencies.
* Created the prototype map.
* Configured Git.
* Configured Git LFS for Unreal binary assets.
* Added the project `.gitignore`.
* Added the initial project documentation.

### Day 2 — Player Character

Completed:

* Added a shared C++ character base class.
* Added a C++ player character class.
* Implemented third-person movement.
* Implemented camera controls.
* Implemented jumping.
* Configured Enhanced Input actions.
* Configured the player input mapping context.
* Created the player character Blueprint.
* Configured the custom player character as the default pawn.

### Day 3 — Ability System and Attributes

Completed:

* Added an Ability System Component to the base character.
* Implemented `IAbilitySystemInterface`.
* Added a shared C++ Attribute Set.
* Added health and maximum health attributes.
* Added mana and maximum mana attributes.
* Implemented basic attribute validation and clamping.
* Created a data-driven initial attributes Gameplay Effect.
* Initialized the player's Ability System Actor Info.
* Applied initial attributes through Gameplay Effects.

### Day 4 — Target Selection

Completed:

* Added the base enemy character class.
* Added a reusable targeting component.
* Implemented enemy selection under the mouse cursor.
* Added maximum target-selection distance validation.
* Added automatic target cleanup when an enemy is destroyed.
* Added Blueprint-driven target indicators.
* Added a custom player controller with mouse cursor support.
* Added right-mouse-button camera control.
* Created an enemy dummy for targeting tests.

## Current Gameplay

The current prototype allows the player to:

* Move through the level using third-person controls.
* Rotate the camera while holding the right mouse button.
* Jump.
* Select an enemy using the left mouse button.
* Change the selected target.
* Clear the selection by clicking outside an enemy.
* See a visual indicator on the selected target.
* Automatically clear the target when the selected enemy is destroyed.

The player and enemy base classes also contain initialized Ability System Components and gameplay attributes.

Combat abilities and damage have not been implemented yet.

## Current C++ Architecture

```text
Source/
└── ArcaneDefense/
    ├── AbilitySystem/
    │   ├── ADAttributeSet.h
    │   └── ADAttributeSet.cpp
    │
    ├── Characters/
    │   ├── ADCharacterBase.h
    │   ├── ADCharacterBase.cpp
    │   ├── ADPlayerCharacter.h
    │   ├── ADPlayerCharacter.cpp
    │   ├── ADEnemyCharacter.h
    │   └── ADEnemyCharacter.cpp
    │
    ├── Combat/
    │   ├── ADTargetingComponent.h
    │   └── ADTargetingComponent.cpp
    │
    ├── Game/
    │   ├── ADPlayerController.h
    │   └── ADPlayerController.cpp
    │
    ├── ArcaneDefense.Build.cs
    ├── ArcaneDefense.cpp
    └── ArcaneDefense.h
```

The structure will grow incrementally as new gameplay systems are implemented.

## Current Content Structure

```text
Content/
└── ArcaneDefense/
    ├── Characters/
    │   ├── BP_ADPlayerCharacter
    │   └── Enemies/
    │       └── BP_ADEnemyDummy
    │
    ├── Effects/
    │   └── Attributes/
    │       ├── GE_InitializePlayerAttributes
    │       └── GE_InitializeEnemyDummyAttributes
    │
    ├── Input/
    │   ├── IA_Move
    │   ├── IA_Look
    │   ├── IA_Jump
    │   ├── IA_SelectTarget
    │   ├── IA_CameraLook
    │   └── IMC_Player
    │
    ├── Maps/
    │   └── L_Prototype
    │
    └── Materials/
        └── M_TargetIndicator
```

Some asset locations may change as the project structure evolves.

## Gameplay Architecture

### `AADCharacterBase`

Shared base class for player and enemy characters.

Current responsibilities:

* Own the Ability System Component.
* Own the shared Attribute Set.
* Initialize Ability System Actor Info.
* Apply initial attribute Gameplay Effects.
* Expose common health and mana values.

Player input and enemy AI behavior do not belong in this class.

### `AADPlayerCharacter`

Player-controlled character.

Current responsibilities:

* Third-person movement.
* Camera setup.
* Enhanced Input bindings.
* Jump input.
* Target-selection input.
* Right-mouse-button camera control.
* Ownership of the targeting component.

### `AADEnemyCharacter`

Base class for enemy characters.

Current responsibilities:

* Share character attributes and Ability System functionality.
* Store whether the enemy is currently selected.
* Notify Blueprint when its targeted state changes.

Enemy AI and combat behavior will be added later.

### `UADAttributeSet`

Stores shared combat attributes:

* `Health`.
* `MaxHealth`.
* `Mana`.
* `MaxMana`.

It also validates and clamps attribute values.

### `UADTargetingComponent`

Handles player target selection.

Current responsibilities:

* Perform cursor traces.
* Validate enemy targets.
* Validate target-selection distance.
* Store a non-owning reference to the current target.
* Clear the previous target when selecting another enemy.
* Clear the selection when clicking outside an enemy.
* React safely when the selected enemy is destroyed.
* Broadcast target-change events for future UI and gameplay systems.

### `AADPlayerController`

Current responsibilities:

* Display the mouse cursor.
* Enable mouse click events.
* Configure the initial game and UI input mode.

## C++ and Blueprint Responsibilities

### C++

C++ is used for:

* Gameplay rules.
* Character inheritance.
* Character movement.
* Camera behavior.
* Input bindings.
* Ability System initialization.
* Attribute storage and validation.
* Target validation.
* Target lifetime management.
* Reusable gameplay components.

### Blueprint

Blueprint is used for:

* Character meshes.
* Animation Blueprints.
* Input asset assignment.
* Initial attribute effect assignment.
* Target indicator composition.
* Visual response to target-state changes.
* Asset configuration.

## Development Principles

* Core gameplay rules are implemented in C++.
* Blueprint is used for configuration and presentation.
* Configurable gameplay values should not be hardcoded.
* Gameplay systems should have clear and limited responsibilities.
* Shared behavior belongs in reusable base classes or components.
* Player-specific input does not belong in shared character classes.
* References to actors not owned by a system should not prevent their destruction.
* Tick should only be used when continuous updates are genuinely required.
* New abstractions should only be introduced when there is a concrete requirement.
* Every development session should end with a verifiable result.
* Every completed task should be committed to version control.
* Features outside the defined vertical slice should not delay the main deliverables.

## Next Milestone

### Day 5 — First Targeted Spell

The next milestone will implement the first complete combat interaction:

1. Select an enemy.
2. Activate a targeted Gameplay Ability.
3. Validate the selected target.
4. Validate available mana.
5. Commit the ability cost.
6. Apply damage through a Gameplay Effect.
7. Reduce the enemy's health.
8. Detect when the enemy reaches zero health.
9. Destroy or disable the defeated enemy.
10. Clear the selected target safely.

This will complete the first playable combat loop:

```text
Select target → Cast spell → Spend mana → Deal damage → Defeat enemy
```

## Planned Roadmap

The project is planned as an eight-week vertical slice:

1. Project foundation and target selection.
2. Spell casting and wave defense.
3. Player abilities and gameplay effects.
4. Placeable traps.
5. Experience, levels, and talents.
6. Enemy types and complete wave configuration.
7. Testing, profiling, and optimization.
8. Presentation, packaging, and portfolio documentation.

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
