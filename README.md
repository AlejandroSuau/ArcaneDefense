# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project is designed to demonstrate Gameplay Programmer and Systems Programmer skills through a small but complete wave-defense experience combining target-based spell combat, Unreal Engine's Gameplay Ability System, enemy AI, traps, wave spawning, progression, UI, testing, and profiling.

---

# Project Goal

The target experience is an **8–12 minute wave-defense match** where the player protects a central objective while casting spells and eventually placing defensive traps.

The final vertical slice is planned to include:

* Third-person character movement.
* Mouse-based and tab-based enemy targeting.
* Health and mana.
* Spell casting.
* Mana costs.
* Cooldowns.
* Four player abilities.
* Three enemy archetypes.
* Five data-driven enemy waves.
* A defense objective.
* Victory and defeat conditions.
* Two placeable traps.
* Experience and level progression.
* Talent selection.
* Functional gameplay UI.
* Automated tests.
* Performance profiling using Unreal Insights.
* A packaged playable build.

The project is intentionally scoped as a **portfolio vertical slice**, not as a full commercial game.

---

# Technology

The project currently uses:

* Unreal Engine 5.8.
* C++.
* Enhanced Input.
* Gameplay Ability System.
* Gameplay Attributes.
* Gameplay Effects.
* Gameplay Tags.
* Gameplay Ability Tasks.
* Unreal Navigation System.
* AI Controllers.
* UMG.
* Data Assets.
* Git.
* Git LFS.

Planned technologies include:

* Behavior Trees when enemy decision-making becomes sufficiently complex.
* Data-driven enemy definitions.
* Talent Data Assets.
* Unreal Automation Tests.
* Unreal Insights.

---

# Development Principles

The project follows several architectural rules.

## C++ owns gameplay rules

Core gameplay behavior is implemented in C++.

Examples:

* Character inheritance.
* Target validation.
* Ability execution.
* Attribute handling.
* Death rules.
* Enemy navigation.
* Wave execution.
* Objective damage.
* Match-state rules.

## Blueprint owns configuration and presentation

Blueprint is primarily used for:

* Character meshes.
* Animation assets.
* Input asset assignment.
* Gameplay Effect configuration.
* Ability configuration.
* Widget composition.
* Target indicators.
* Visual death feedback.
* Prototype asset composition.

## Gameplay values are data-driven

Values such as:

* Health.
* Mana.
* Spell damage.
* Spell cost.
* Cast time.
* Spawn quantity.
* Spawn interval.
* Spawn location.
* Enemy objective damage.

are intended to live in assets instead of being hardcoded into gameplay classes.

## Systems have limited responsibilities

Each system should solve one specific problem.

Examples:

```text
TargetingComponent
    → Which enemy is currently selected?

CastComponent
    → What cast is currently being presented?

GameplayAbility
    → Can the spell execute and what does it do?

WaveDirector
    → Which enemies should be spawned and when?

SpawnPoint
    → Where should enemies appear?

EnemyAIController
    → How does an enemy reach its current destination?

GameMode
    → What are the global match rules?
```

## Abstractions are introduced only when necessary

The project deliberately avoids creating generic frameworks before a real gameplay requirement exists.

For example:

* No custom `UADAbilitySystemComponent` yet.
* No Behavior Tree yet.
* No Asset Manager customization.
* No generic event bus.
* No object pooling.
* No multiplayer architecture.

---

# Current Source Structure

```text
Source/
└── ArcaneDefense/
    ├── AbilitySystem/
    │   ├── ADAttributeSet.h
    │   ├── ADAttributeSet.cpp
    │   ├── ADGameplayTags.h
    │   ├── ADGameplayTags.cpp
    │   │
    │   └── Abilities/
    │       ├── ADGameplayAbility.h
    │       ├── ADGameplayAbility.cpp
    │       ├── ADGA_TargetedDamage.h
    │       └── ADGA_TargetedDamage.cpp
    │
    ├── AI/
    │   ├── ADEnemyAIController.h
    │   └── ADEnemyAIController.cpp
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
    │   ├── ADTargetingComponent.cpp
    │   ├── ADCastComponent.h
    │   └── ADCastComponent.cpp
    │
    ├── Game/
    │   ├── ADGameMode.h
    │   ├── ADGameMode.cpp
    │   ├── ADPlayerController.h
    │   └── ADPlayerController.cpp
    │
    ├── Objective/
    │   ├── ADDefenseObjective.h
    │   └── ADDefenseObjective.cpp
    │
    ├── Waves/
    │   ├── ADWaveTypes.h
    │   ├── ADWaveDataAsset.h
    │   ├── ADSpawnPoint.h
    │   ├── ADSpawnPoint.cpp
    │   ├── ADWaveDirector.h
    │   └── ADWaveDirector.cpp
    │
    ├── ArcaneDefense.Build.cs
    ├── ArcaneDefense.cpp
    └── ArcaneDefense.h
```

This structure will continue growing incrementally as systems are actually implemented.

---

# Current Content Structure

The current content architecture is approximately:

```text
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   └── GA_ArcaneBolt
    │
    ├── Characters/
    │   ├── BP_ADPlayerCharacter
    │   │
    │   └── Enemies/
    │       └── BP_ADEnemyDummy
    │
    ├── Effects/
    │   ├── Attributes/
    │   │   ├── GE_InitializePlayerAttributes
    │   │   ├── GE_InitializeEnemyDummyAttributes
    │   │   └── GE_InitializeDefenseObjectiveAttributes
    │   │
    │   ├── Costs/
    │   │   └── GE_ArcaneBolt_Cost
    │   │
    │   └── Damage/
    │       ├── GE_ArcaneBolt_Damage
    │       └── GE_EnemyObjectiveDamage
    │
    ├── Input/
    │   ├── IA_Move
    │   ├── IA_Look
    │   ├── IA_Jump
    │   ├── IA_SelectTarget
    │   ├── IA_CameraLook
    │   ├── IA_Ability1
    │   └── IMC_Player
    │
    ├── Maps/
    │   └── L_Prototype
    │
    ├── Materials/
    │   └── M_TargetIndicator
    │
    ├── UI/
    │   ├── WBP_PlayerHUD
    │   └── WBP_CastBar
    │
    ├── Waves/
    │   └── DA_Wave_01
    │
    ├── BP_ADDefenseObjective
    ├── BP_ADGameMode
    └── BP_ADPlayerController
```

Exact asset locations may change as the Content folder is reorganized.

---

# Core Gameplay Architecture

## `AADCharacterBase`

Base class shared by player and enemy characters.

Current responsibilities:

* Own the Ability System Component.
* Own the shared Attribute Set.
* Implement `IAbilitySystemInterface`.
* Initialize Ability System Actor Info.
* Apply the initial attribute Gameplay Effect.
* Expose health and mana values.
* Listen for the shared out-of-health event.
* Maintain the generic character death state.
* Disable character movement and collision when dead.

It intentionally does not contain:

* Player input.
* Player camera logic.
* Enemy AI logic.
* Target selection.
* Wave logic.

Inheritance:

```text
ACharacter
    ↓
AADCharacterBase
    ├── AADPlayerCharacter
    └── AADEnemyCharacter
```

---

# Player Character

## `AADPlayerCharacter`

Represents the locally controlled player character.

Current responsibilities:

* Third-person movement.
* Third-person camera.
* Jumping.
* Enhanced Input bindings.
* Ability input.
* Target-selection input.
* Right-mouse-button camera control.
* Ownership of the targeting component.
* Ownership of the cast presentation component.
* Initial ability granting.
* Movement-based cast interruption.

Current composition:

```text
AADPlayerCharacter
├── CapsuleComponent
├── CharacterMovement
├── Mesh
├── AbilitySystemComponent
├── ADAttributeSet
├── ADTargetingComponent
├── ADCastComponent
└── CameraBoom
    └── FollowCamera
```

The player Blueprint configures assets and visual properties while C++ owns gameplay behavior.

---

# Enemy Character

## `AADEnemyCharacter`

Base class for current and future enemy archetypes.

Current responsibilities:

* Reuse the shared character Ability System and attributes.
* Maintain its selected-target visual state.
* Expose Blueprint events for visual targeting feedback.
* Store a navigation destination.
* Communicate movement targets to its AI Controller.
* React when the AI reaches its destination.
* Attack the defense objective periodically.
* Apply objective damage through Gameplay Effects.
* Stop navigation and attack timers when killed.
* Destroy itself shortly after death.

The current dummy:

```text
BP_ADEnemyDummy
```

is a Blueprint child used for prototype testing.

Future enemy types are expected to reuse this C++ class whenever possible:

```text
AADEnemyCharacter
├── BP_ADGrunt
├── BP_ADRunner
└── BP_ADTank
```

Separate C++ subclasses will only be introduced if an enemy requires genuinely different behavior.

---

# Gameplay Ability System

## Ability System Component

An Unreal `UAbilitySystemComponent` is currently owned directly by `AADCharacterBase`.

For the current single-player vertical slice:

```text
Owner Actor  = Character
Avatar Actor = Character
```

The defense objective also owns its own Ability System Component.

A custom `UADAbilitySystemComponent` has intentionally not been created yet because there is currently no project-specific ASC behavior that justifies one.

---

# Attributes

## `UADAttributeSet`

Current shared attributes:

```text
Health
MaxHealth

Mana
MaxMana
```

Responsibilities:

* Store gameplay attributes.
* Clamp Health between zero and MaxHealth.
* Clamp Mana between zero and MaxMana.
* Ensure maximum attributes remain valid.
* Broadcast when Health reaches zero.

The Attribute Set deliberately does **not** decide what death means.

Instead:

```text
UADAttributeSet
        ↓
OnOutOfHealth
        ↓
        ├── Character → HandleDeath()
        │
        └── Objective → HandleOutOfHealth()
```

This allows the same Attribute Set to be reused by different gameplay actors.

---

# Gameplay Effects

Gameplay Effects currently handle data-driven attribute modifications.

## Attribute initialization

```text
GE_InitializePlayerAttributes
GE_InitializeEnemyDummyAttributes
GE_InitializeDefenseObjectiveAttributes
```

They initialize attributes instead of hardcoding starting values into constructors.

Example player configuration:

```text
MaxHealth = 100
Health    = 100

MaxMana   = 100
Mana      = 100
```

## Arcane Bolt mana cost

```text
GE_ArcaneBolt_Cost
```

Currently modifies:

```text
Mana += -20
```

## Arcane Bolt damage

```text
GE_ArcaneBolt_Damage
```

Currently modifies:

```text
Health += -25
```

## Enemy objective damage

```text
GE_EnemyObjectiveDamage
```

Currently modifies:

```text
Health += -10
```

The C++ gameplay code applies effects but does not own these balance values.

---

# Gameplay Abilities

## `UADGameplayAbility`

Base C++ Gameplay Ability class.

Current instancing policy:

```text
Instanced Per Actor
```

This provides a shared foundation for future Arcane Defense abilities.

---

## `UADGA_TargetedDamage`

Reusable C++ implementation for a targeted damaging spell.

Current responsibilities:

* Validate the selected target.
* Reject dead targets.
* Validate spell range.
* Reject activation while the player is falling.
* Capture the target when casting begins.
* Start the cast presentation.
* Wait asynchronously using an Ability Task.
* Revalidate the target after casting.
* Commit the ability.
* Apply the mana cost.
* Apply the configured damage Gameplay Effect.
* Finish or cancel the ability correctly.

It contains configurable properties such as:

```text
Damage Effect Class
Max Range
Cast Time
Cast Display Name
```

---

## `GA_ArcaneBolt`

Blueprint configuration of `UADGA_TargetedDamage`.

Current prototype configuration:

```text
Cast Time       = 1.5 seconds
Max Range       = 2000 cm
Mana Cost       = 20
Damage          = 25
```

Input:

```text
Keyboard 1
```

Current flow:

```text
Select Enemy
    ↓
Press 1
    ↓
CanActivateAbility()
    ↓
Target valid?
In range?
Character grounded?
Enough mana?
    ↓
Start Cast
    ↓
Wait 1.5 seconds
    ↓
Revalidate target
    ↓
CommitAbility()
    ↓
Spend mana
    ↓
Apply damage
    ↓
EndAbility()
```

---

# Gameplay Tags

Current native tags include:

```text
State.Casting

Ability.CancelOnMovement
```

## `State.Casting`

Applied while an appropriate Gameplay Ability is actively casting.

It allows other systems to query whether the character is currently casting.

## `Ability.CancelOnMovement`

Identifies abilities that should be interrupted when movement input begins.

The player therefore does not use:

```text
CancelAllAbilities()
```

when moving.

Instead, only abilities carrying the corresponding cancellation tag are cancelled.

This leaves room for future instant abilities or abilities that may be used while moving.

---

# Targeting System

## `UADTargetingComponent`

Owned by `AADPlayerCharacter`.

Current responsibilities:

* Trace under the mouse cursor.
* Detect enemy candidates.
* Validate maximum selection distance.
* Maintain the currently selected enemy.
* Clear the previous target.
* Clear selection when clicking outside a valid target.
* Clear the target if the selected actor is destroyed.
* Broadcast target-change events.

The selected enemy is stored using a non-owning weak reference:

```text
TWeakObjectPtr<AADEnemyCharacter>
```

because the targeting component does not own the enemy.

Current targeting flow:

```text
Left Mouse Button
        ↓
IA_SelectTarget
        ↓
TrySelectTargetUnderCursor()
        ↓
Visibility Trace
        ↓
AADEnemyCharacter?
        ↓
Range valid?
        ↓
SetCurrentTarget()
```

---

# Target Presentation

`AADEnemyCharacter` owns the logical targeted state.

Blueprint controls its appearance.

Current flow:

```text
UADTargetingComponent
        ↓
Enemy.SetTargeted(true)
        ↓
ReceiveTargetedStateChanged()
        ↓
BP_ADEnemyDummy
        ↓
TargetIndicator visible
```

This keeps visual implementation outside the targeting gameplay system.

---

# Casting System

## `UADCastComponent`

The cast component exists primarily as a bridge between Gameplay Abilities and presentation systems.

It does **not** control when an ability executes.

Responsibilities:

* Store whether a cast is currently active.
* Store cast display name.
* Store cast duration.
* Store cast start time.
* Calculate normalized progress.
* Calculate remaining time.
* Broadcast cast-start events.
* Broadcast cast-end events.

It does not use Actor Component Tick.

Cast progress is calculated from world time whenever requested.

The actual waiting logic remains inside GAS through:

```text
UAbilityTask_WaitDelay
```

---

# Cast Interruption

Movement input checks whether:

```text
State.Casting
```

is active.

If so, abilities tagged:

```text
Ability.CancelOnMovement
```

are cancelled.

Current rule:

```text
Casting
   +
movement input
      ↓
Cancel matching ability
      ↓
EndAbility(cancelled)
      ↓
CastComponent.EndCast(true)
```

Current targeted spells also reject activation while the Character Movement Component reports that the character is falling.

---

# UI Architecture

## `WBP_PlayerHUD`

Root gameplay HUD widget.

Currently contains:

```text
WBP_PlayerHUD
└── WBP_CastBar
```

Future HUD elements will be composed here.

---

## `WBP_CastBar`

Displays:

* Current cast name.
* Current cast progress.

It listens to:

```text
UADCastComponent::OnCastStarted
UADCastComponent::OnCastEnded
```

The widget may update its visual progress each frame, but gameplay timing itself does not depend on the widget or widget Tick.

---

# Player Controller

## `AADPlayerController`

Represents local-player-level input/UI concerns.

Current responsibilities:

* Show the mouse cursor.
* Enable click events.
* Configure Game + UI input mode.
* Create the local HUD widget.

A Blueprint child:

```text
BP_ADPlayerController
```

assigns:

```text
Player HUD Class = WBP_PlayerHUD
```

The Character therefore does not create its own HUD.

---

# Enemy AI

## `AADEnemyAIController`

Controls navigation for enemy characters.

Current responsibilities:

* Possess `AADEnemyCharacter`.
* Retrieve the enemy's movement target.
* Request navigation using `MoveToActor`.
* Configure an acceptance radius.
* React to completed movement.
* Inform the enemy when the destination has been reached.

Enemies are configured with:

```text
AI Controller Class
    = ADEnemyAIController

Auto Possess AI
    = Placed In World Or Spawned
```

This supports both prototype enemies and Wave Director spawned enemies.

---

# Enemy Navigation

Enemies do not know specifically what a crystal or portal is.

Instead, they expose:

```text
SetMoveTarget(AActor*)
GetMoveTarget()
```

Therefore:

```text
AADEnemyCharacter
        ↓
MoveTarget = some Actor
```

During early prototyping this actor was a `TargetPoint`.

It has since been replaced by:

```text
BP_ADDefenseObjective
```

without changing the enemy movement API.

This is intentional decoupling.

---

# Behavior Tree Decision

A Behavior Tree has deliberately not been introduced yet.

Current enemy logic is only:

```text
Move toward configured objective
    ↓
Reach objective
    ↓
Attack objective
```

`AAIController::MoveToActor()` is sufficient for this behavior.

A Behavior Tree will become useful when enemies require decisions such as:

```text
Am I stunned?
Am I rooted?
Am I dead?
Should I attack?
Should I move?
Do I have another target?
Should I prioritize the player?
```

Introducing one before these requirements exist would add unnecessary architectural complexity.

---

# Defense Objective

## `AADDefenseObjective`

Represents the actor the player must protect.

It derives directly from:

```text
AActor
```

and implements:

```text
IAbilitySystemInterface
```

Composition:

```text
AADDefenseObjective
├── SceneRoot
├── AbilitySystemComponent
└── ADAttributeSet
```

Current responsibilities:

* Initialize its Ability System.
* Apply initial attributes.
* Expose current and maximum Health.
* Broadcast health changes.
* Detect when Health reaches zero.
* Broadcast objective defeat.
* Notify the Game Mode.

Blueprint child:

```text
BP_ADDefenseObjective
```

provides its visual representation.

---

# Enemy Objective Attacks

When the AI Controller successfully reaches its movement destination:

```text
AADEnemyAIController
        ↓
OnMoveCompleted()
        ↓
AADEnemyCharacter::HandleReachedMoveTarget()
```

If the target is an `AADDefenseObjective`, the enemy begins attacking.

Current flow:

```text
Reach Objective
      ↓
Attack immediately
      ↓
Start recurring timer
      ↓
GE_EnemyObjectiveDamage
      ↓
Objective AbilitySystemComponent
      ↓
Health decreases
```

Enemy attacks use `FTimerManager`, not Tick.

The attack timer is stopped when:

* The enemy dies.
* The objective becomes invalid.
* The objective has already been defeated.

---

# Game Mode

## `AADGameMode`

Owns high-level match rules.

Current responsibility:

```text
HandleGameDefeat()
```

When the objective reaches zero Health:

```text
AADDefenseObjective
        ↓
HandleOutOfHealth()
        ↓
AADGameMode::HandleGameDefeat()
```

The Game Mode stores:

```text
bGameDefeated
```

to ensure defeat is processed only once.

Blueprint child:

```text
BP_ADGameMode
```

currently provides provisional presentation feedback.

Later it will coordinate:

* Wave victory.
* Final defeat.
* Match completion.
* Restart/exit flows.

---

# Wave System

The current wave system was introduced on Day 9.

Its objective is to separate:

```text
WHAT should spawn
WHEN it should spawn
WHERE it should spawn
```

---

# Wave Types

## `FADWaveSpawnGroup`

Defines one sequential enemy group.

Current fields:

```text
EnemyClass

Quantity

SpawnInterval

SpawnPointId

DelayAfterGroup
```

Example:

```text
Enemy Class       = BP_ADEnemyDummy
Quantity          = 5
Spawn Interval    = 1.0
Spawn Point Id    = Main
Delay After Group = 0
```

---

# Wave Data Asset

## `UADWaveDataAsset`

Stores:

```text
TArray<FADWaveSpawnGroup>
```

It contains configuration only and does not execute spawning itself.

Current prototype asset:

```text
DA_Wave_01
```

Using a Data Asset allows the wave to be modified without C++ recompilation.

For example:

```text
Quantity:
5 → 10

Spawn Interval:
1.0 → 0.4
```

requires no code change.

A regular `UDataAsset` is currently sufficient.

`UPrimaryDataAsset` and Asset Manager integration have deliberately not been introduced because asynchronous asset discovery/loading is not currently required.

---

# Spawn Points

## `AADSpawnPoint`

Represents a named spawn location inside a level.

Composition:

```text
AADSpawnPoint
├── SceneRoot
└── DirectionArrow
```

It exposes:

```text
SpawnPointId
```

Example:

```text
SP_Main
    ↓
SpawnPointId = Main
```

The arrow provides editor visualization for position and orientation.

---

# Why Waves Use Spawn Point IDs

The Wave Data Asset does not directly reference level actors.

Instead:

```text
DA_Wave_01
    ↓
SpawnPointId = Main
```

The Wave Director contains:

```text
SP_Main
    ↓
SpawnPointId = Main
```

and resolves them at runtime.

Therefore the Data Asset remains independent of a specific level instance.

Conceptually:

```text
DA_Wave_01
"Main"
    ↓
WaveDirector
    ↓
search configured SpawnPoints
    ↓
SP_Main
```

This makes wave definitions more reusable between maps.

---

# Wave Director

## `AADWaveDirector`

Executes wave definitions.

Current responsibilities:

* Store the available Wave Data Assets.
* Store level Spawn Points.
* Store the Defense Objective.
* Start a requested wave.
* Execute spawn groups sequentially.
* Spawn enemies according to configured intervals.
* Handle delays between groups.
* Resolve Spawn Point IDs.
* Configure the enemy's Defense Objective.
* Detect when the wave has finished **spawning**.

Important distinction:

```text
Wave finished spawning
```

currently means:

> Every enemy configured for the wave has been created.

It does **not yet mean**:

> Every enemy in the wave has been defeated.

Enemy lifetime tracking will be introduced in the next milestone.

---

# Deferred Enemy Spawning

The Wave Director currently uses deferred spawning.

Conceptually:

```text
SpawnActorDeferred()
        ↓
Enemy exists but spawn is not complete
        ↓
Enemy.SetMoveTarget(DefenseObjective)
        ↓
FinishSpawningActor()
        ↓
BeginPlay / AI initialization
```

This lets the director provide required gameplay data before the actor fully enters gameplay.

It avoids this race:

```text
Spawn Enemy
    ↓
AI possesses enemy
    ↓
AI asks for MoveTarget
    ↓
MoveTarget has not been assigned yet
```

---

# Current Wave Flow

The current prototype behaves as follows:

```text
BeginPlay
    ↓
WaveDirector
    ↓
StartWave(0)
    ↓
DA_Wave_01
    ↓
Group 0
    ↓
Spawn Enemy 1
    ↓
wait SpawnInterval
    ↓
Spawn Enemy 2
    ↓
...
    ↓
Last enemy spawned
    ↓
FinishSpawningCurrentWave()
```

Each spawned enemy independently performs:

```text
Spawn
    ↓
Receive DefenseObjective
    ↓
AI Controller possession
    ↓
MoveToActor()
    ↓
Reach objective
    ↓
Attack objective
```

---

# Current Gameplay Loop

The current prototype supports:

```text
Player movement
        ↓
Enemy wave spawning
        ↓
Enemy navigation toward objective
        ↓
Player selects enemy
        ↓
Player starts Arcane Bolt cast
        ↓
Cast bar appears
        ↓
Movement can interrupt cast
        ↓
Successful cast spends mana
        ↓
Enemy takes damage
        ↓
Enemy dies
```

Simultaneously:

```text
Enemy reaches objective
        ↓
Enemy attacks periodically
        ↓
Objective loses Health
        ↓
Objective Health reaches zero
        ↓
GameMode receives defeat
```

---

# Input

Current Enhanced Input configuration:

```text
IMC_Player
├── IA_Move
├── IA_Look
├── IA_Jump
├── IA_SelectTarget
├── IA_CameraLook
└── IA_Ability1
```

Current controls:

```text
WASD
    → movement

Mouse movement while Right Mouse Button is held
    → camera

Space
    → jump

Left Mouse Button
    → select target

Right Mouse Button
    → enable camera look

1
    → Arcane Bolt
```

---

# Current Data Flow Overview

A simplified gameplay dependency flow is:

```text
Input
  │
  ▼
AADPlayerCharacter
  │
  ├──────────────► UADTargetingComponent
  │                       │
  │                       ▼
  │                AADEnemyCharacter
  │
  ▼
AbilitySystemComponent
  │
  ▼
GA_ArcaneBolt
  │
  ├──────────────► UADCastComponent ─────► UI
  │
  ├──────────────► GE_ArcaneBolt_Cost
  │
  └──────────────► GE_ArcaneBolt_Damage
                           │
                           ▼
                    Enemy AttributeSet
                           │
                           ▼
                       Health == 0
                           │
                           ▼
                     Enemy death
```

Wave side:

```text
DA_Wave_01
    │
    ▼
AADWaveDirector
    │
    ├──► AADSpawnPoint
    │
    └──► Spawn AADEnemyCharacter
                │
                ▼
         AADEnemyAIController
                │
                ▼
         AADDefenseObjective
                │
                ▼
         Objective Health == 0
                │
                ▼
             AADGameMode
                │
                ▼
              Defeat
```

---

# Current Blueprint / C++ Boundary

## C++

Currently responsible for:

* Character hierarchy.
* Movement implementation.
* Camera behavior.
* Enhanced Input bindings.
* Ability System initialization.
* Attribute storage and validation.
* Health depletion events.
* Generic character death.
* Target tracing and validation.
* Target lifetime handling.
* Gameplay Ability execution.
* Cast lifecycle.
* Cast interruption.
* Enemy navigation.
* Enemy objective attack lifecycle.
* Defense Objective gameplay behavior.
* Game defeat rules.
* Wave execution.
* Runtime enemy spawning.

## Blueprint / Data Assets

Currently responsible for:

* Character meshes.
* Animation classes.
* Input Action assets.
* Input Mapping Context.
* Initial Attribute Gameplay Effects.
* Damage Gameplay Effects.
* Mana Cost Gameplay Effect.
* Ability configuration.
* Target indicator.
* Cast bar visuals.
* HUD composition.
* Defense objective visuals.
* Game Mode configuration.
* Wave definitions.

---

# Current Technical Decisions

## GAS lives on characters

The player and enemies inherit their ASC from `AADCharacterBase`.

This is sufficient for the current single-player project.

Moving the player's ASC to `PlayerState` is intentionally deferred because the project currently has:

* No respawn requirement.
* No character switching.
* No multiplayer.
* No requirement for the ASC to survive Pawn destruction.

---

## The Defense Objective also uses GAS

The objective uses the same Attribute Set and Gameplay Effect infrastructure.

This avoids creating a completely separate health system.

```text
Characters
    └── GAS Health

Defense Objective
    └── GAS Health
```

---

## Experience and Level are not currently GAS attributes

They are planned to belong to the progression system.

They will only be added to GAS if future gameplay mechanics demonstrate a concrete need for Gameplay Effects to manipulate them.

---

## No custom ASC yet

The project still uses:

```text
UAbilitySystemComponent
```

directly.

A custom:

```text
UADAbilitySystemComponent
```

will only be introduced when project-specific ASC behavior appears.

---

## No CastComponent timer

The Cast Component represents cast state for UI.

Actual asynchronous ability execution stays inside GAS.

This prevents two systems from independently controlling the same cast.

---

## Weak references for non-owned actors

Systems that do not own another actor use weak references where appropriate.

Examples:

```text
TargetingComponent
    → selected enemy

GameplayAbility
    → captured enemy

Enemy
    → currently attacked objective
```

This prevents gameplay systems from assuming those actors will remain alive indefinitely.

---

## Timers instead of unnecessary Tick

Gameplay timers are used for discrete periodic behavior:

```text
Enemy attack interval
Wave spawn interval
Delay between spawn groups
```

Actor/Component Tick is deliberately avoided for these systems.

UI may still update presentation every frame where appropriate.

---

## No Behavior Tree yet

Current AI behavior is linear enough that an AI Controller is easier to understand and maintain.

Behavior Trees will be introduced only once decision complexity justifies them.

---

## Wave Data Assets contain no level actor references

Wave definitions identify spawn locations by logical `FName` IDs.

This keeps Data Assets independent from specific map instances.

---

## Deferred enemy spawning

Enemies receive critical initialization data before completing their spawn.

This is particularly important because automatic AI possession can occur during initialization.

---

# Current Project Status

## Day 1 — Project Setup

Completed:

* Created the Unreal Engine 5.8 C++ project.
* Configured Git.
* Configured Git LFS.
* Enabled Enhanced Input.
* Enabled Gameplay Ability System.
* Added required GAS modules.
* Created the prototype map.
* Added initial project documentation.

---

## Day 2 — Player Character

Completed:

* Added `AADCharacterBase`.
* Added `AADPlayerCharacter`.
* Implemented movement.
* Implemented third-person camera.
* Implemented jumping.
* Configured Enhanced Input.
* Created the player Blueprint.

---

## Day 3 — Ability System and Attributes

Completed:

* Added the Ability System Component.
* Implemented `IAbilitySystemInterface`.
* Added `UADAttributeSet`.
* Added Health and Mana.
* Added MaxHealth and MaxMana.
* Added attribute clamping.
* Added Gameplay Effect-based attribute initialization.

---

## Day 4 — Target Selection

Completed:

* Added `AADEnemyCharacter`.
* Added `UADTargetingComponent`.
* Added cursor-based enemy selection.
* Added target range validation.
* Added automatic destroyed-target cleanup.
* Added a Blueprint-driven target indicator.
* Added `AADPlayerController`.
* Added right-mouse camera control.

---

## Day 5 — First Targeted Ability

Completed:

* Added `UADGameplayAbility`.
* Added reusable targeted-damage ability logic.
* Added Arcane Bolt.
* Added mana costs.
* Added Gameplay Effect damage.
* Added death detection.
* Added enemy destruction.
* Completed the first combat loop.

---

## Day 6 — Spell Casting

Completed:

* Added `UADCastComponent`.
* Added configurable cast times.
* Added asynchronous casting using GAS Ability Tasks.
* Added cast interruption through movement.
* Added native Gameplay Tags.
* Added cast-bar UI.
* Added root player HUD.
* Added post-cast target validation.
* Prevented interrupted casts from spending mana.
* Prevented targeted casting while falling.

---

## Day 7 — Enemy Navigation

Completed:

* Added `AADEnemyAIController`.
* Configured automatic AI possession.
* Added enemy movement targets.
* Added NavMesh navigation.
* Added `MoveToActor` based movement.
* Added movement completion callbacks.
* Integrated enemy death with AI movement.

---

## Day 8 — Defense Objective

Completed:

* Added `AADDefenseObjective`.
* Added GAS attributes to a non-character gameplay actor.
* Decoupled out-of-health handling from character-specific logic.
* Added objective health events.
* Added periodic enemy attacks.
* Added Gameplay Effect-based objective damage.
* Added `AADGameMode`.
* Added the provisional defeat condition.
* Replaced the temporary navigation target with the real objective.

---

## Day 9 — Data-Driven Wave Spawning

Completed:

* Added `FADWaveSpawnGroup`.
* Added `UADWaveDataAsset`.
* Added `AADSpawnPoint`.
* Added named spawn locations.
* Added `AADWaveDirector`.
* Added sequential spawn groups.
* Added spawn intervals.
* Added delays between groups.
* Added deferred enemy spawning.
* Automatically configured spawned enemies with their defense objective.
* Removed the need to manually place enemies.
* Added `DA_Wave_01`.
* Verified wave configuration can change without recompiling C++.

---

# Current Prototype Definition

At the end of Day 9 the game can:

1. Start a match.
2. Spawn enemies automatically from a Data Asset.
3. Navigate those enemies toward the central objective.
4. Allow the player to select an enemy.
5. Cast Arcane Bolt.
6. Display casting progress.
7. Interrupt casting by movement.
8. Spend mana.
9. Damage enemies.
10. Kill enemies.
11. Allow surviving enemies to reach the objective.
12. Apply periodic damage to the objective.
13. Trigger defeat if the objective reaches zero Health.

---

# Current Limitations

The following systems are intentionally incomplete:

* A wave is considered finished when all enemies have been **spawned**, not defeated.
* There is currently only one prototype enemy archetype.
* There is currently only one prototype wave definition.
* Victory has not been implemented.
* Enemy attacks have no final animation.
* The objective has no final health UI.
* Arcane Bolt has no projectile yet.
* Cooldowns are not implemented.
* Tab targeting is not implemented.
* Enemy Behavior Trees are not implemented.
* Crowd avoidance is not implemented.
* Traps are not implemented.
* Experience and talents are not implemented.

These are planned milestones rather than defects in the current architecture.

---

# Next Milestone — Day 10

The next milestone will complete the basic wave gameplay loop.

Planned work:

* Track living enemies belonging to the current wave.
* Detect enemy removal/death.
* Distinguish between:

```text
Finished Spawning
```

and:

```text
Wave Completed
```

* Add three provisional wave definitions.
* Automatically advance between waves.
* Detect completion of the final wave.
* Add a provisional victory condition.

Target flow:

```text
Start Wave
    ↓
Spawn enemies
    ↓
Finished Spawning
    ↓
Enemies remain alive
    ↓
Player defeats enemies
    ↓
AliveEnemies == 0
    ↓
Wave Complete
    ↓
Next Wave
```

Final provisional flow:

```text
Wave 1
    ↓
Wave 2
    ↓
Wave 3
    ↓
All enemies defeated
    ↓
Victory
```

---

# Planned Roadmap

## Week 1

* Project foundation.
* Player character.
* Gameplay Ability System.
* Target selection.
* First targeted spell.

Status: **Completed**

## Week 2

* Spell casting.
* Enemy navigation.
* Defense objective.
* Wave spawning.
* Wave completion and provisional victory.

Status: **In progress**

## Week 3

* Reusable projectile.
* Damage over time.
* Frost Nova.
* Meteor.
* Cooldowns and ability UI.

## Week 4

* Trap framework.
* Trap placement.
* Spike trap.
* Slow trap.
* Construction resource.

## Week 5

* Experience.
* Levels.
* Talent Data Assets.
* Talent selection.
* Initial talent set.

## Week 6

* Grunt.
* Runner.
* Tank.
* Final five waves.
* Architecture refactor and cleanup.

## Week 7

* Gameplay tests.
* Progression tests.
* Load scenario.
* Unreal Insights profiling.
* Documented optimization.

## Week 8

* Asset polish.
* VFX.
* Audio.
* Bug fixing.
* Packaging.
* Technical documentation.
* Portfolio video.

---

# Out of Scope

The initial vertical slice deliberately excludes:

* Multiplayer.
* Matchmaking.
* Backend services.
* Inventory.
* Equipment.
* Loot.
* Crafting.
* Shops.
* Campaign.
* Multiple maps.
* Multiple playable classes.
* Complex talent trees.
* Custom character modelling.
* Custom animation production.

---

# Definition of Done

The vertical slice will be considered complete when it contains:

* A packaged playable build.
* An 8–12 minute complete match.
* Four player spells.
* Two placeable traps.
* Three enemy types.
* Five enemy waves.
* Experience and levels.
* Between six and nine talents.
* Victory and defeat conditions.
* Functional gameplay UI.
* Automated tests.
* A documented Unreal Insights profiling pass.
* At least one measured optimization.
* Technical documentation.
* A short gameplay presentation video.

---

# Development Workflow

Each development session follows this structure:

```text
Define one verifiable objective
        ↓
Implement
        ↓
Compile frequently
        ↓
Test normal behavior
        ↓
Test edge cases
        ↓
Review Output Log
        ↓
Update documentation
        ↓
Commit
```

Every completed development day should end with:

```bash
git status
git add .
git commit -m "<meaningful commit message>"
git status
```

The final status should be:

```text
nothing to commit, working tree clean
```

---

# License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
