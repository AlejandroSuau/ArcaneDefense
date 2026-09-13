# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project is designed to demonstrate Gameplay Programmer and Systems Programmer skills through a small but complete wave-defense experience combining target-based spell combat, Unreal Engine's Gameplay Ability System, enemy AI, wave spawning, progression systems, UI, testing, and profiling.

---

# Project Goal

The target experience is an **8–12 minute wave-defense match** where the player protects a central objective while using spells and defensive systems against progressively more difficult enemy waves.

The final vertical slice is planned to include:

* Third-person player movement.
* Enemy target selection.
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
* Gameplay timers.
* C++ and Blueprint delegates.
* Git.
* Git LFS.

Planned technologies include:

* Behavior Trees when enemy decision-making becomes sufficiently complex.
* Additional data-driven enemy definitions.
* Talent Data Assets.
* Unreal Automation Tests.
* Unreal Insights.

---

# Development Principles

## C++ owns gameplay rules

Core gameplay behavior is implemented in C++.

This currently includes:

* Character inheritance.
* Player movement.
* Target validation.
* Ability execution.
* Attribute handling.
* Character death.
* Enemy navigation.
* Enemy objective attacks.
* Objective defeat.
* Wave execution.
* Enemy lifetime tracking.
* Wave completion.
* Victory and defeat rules.

## Blueprint owns configuration and presentation

Blueprint is primarily used for:

* Character meshes.
* Animation assets.
* Input asset assignment.
* Gameplay Effect configuration.
* Gameplay Ability configuration.
* Widget composition.
* Target indicators.
* Prototype presentation.
* Game result presentation.

## Gameplay values are data-driven

Configurable gameplay values are stored outside gameplay implementation whenever appropriate.

Examples include:

* Initial Health.
* Initial Mana.
* Spell damage.
* Spell mana cost.
* Cast time.
* Spell range.
* Objective damage.
* Spawn quantities.
* Spawn intervals.
* Spawn points.
* Group delays.
* Delay between waves.

## Systems have limited responsibilities

Each gameplay system solves a specific problem.

```text
ADTargetingComponent
    → Which enemy is selected?

ADCastComponent
    → What cast should presentation systems display?

GameplayAbility
    → Can an ability execute and what does it do?

ADEnemyAIController
    → How does an enemy navigate toward its destination?

ADDefenseObjective
    → What happens to the defended actor?

ADWaveDataAsset
    → What should a wave contain?

ADSpawnPoint
    → Where can enemies appear?

ADWaveDirector
    → When should enemies spawn and when is a wave complete?

ADGameMode
    → What is the result of the match?
```

## Abstractions are introduced only when necessary

The project deliberately avoids creating generic frameworks before a real requirement exists.

Currently there is:

* No custom `UADAbilitySystemComponent`.
* No Behavior Tree.
* No custom Asset Manager.
* No object pooling framework.
* No global event bus.
* No multiplayer abstraction.

---

# Source Structure

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

The source tree grows incrementally as systems acquire real responsibilities.

---

# Current Content Structure

```text
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   └── GA_ArcaneBolt
    │
    ├── Characters/
    │   ├── BP_ADPlayerCharacter
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
    │   ├── DA_Wave_01
    │   ├── DA_Wave_02
    │   └── DA_Wave_03
    │
    ├── BP_ADDefenseObjective
    ├── BP_ADGameMode
    └── BP_ADPlayerController
```

Exact asset locations may evolve as content organization is refined.

---

# Character Architecture

## `AADCharacterBase`

Shared base class for living player and enemy characters.

Current responsibilities:

* Own the Ability System Component.
* Own the shared Attribute Set.
* Implement `IAbilitySystemInterface`.
* Initialize Ability System Actor Info.
* Apply initial attribute Gameplay Effects.
* Expose Health and Mana.
* Listen to out-of-health events.
* Maintain generic character death state.
* Disable movement and collision when dead.

It deliberately contains no player input or enemy AI logic.

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

Current responsibilities:

* Third-person movement.
* Third-person camera.
* Jumping.
* Enhanced Input bindings.
* Ability input.
* Target-selection input.
* Right-mouse-button camera control.
* Ownership of `UADTargetingComponent`.
* Ownership of `UADCastComponent`.
* Initial Gameplay Ability granting.
* Movement-based cast interruption.
* Preventing appropriate spell casting while airborne.

Composition:

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

---

# Enemy Character

## `AADEnemyCharacter`

Current responsibilities:

* Reuse shared GAS attributes.
* Maintain target-selection presentation state.
* Store a navigation destination.
* Communicate its destination to the AI Controller.
* React when navigation reaches the destination.
* Attack the Defense Objective.
* Apply damage through Gameplay Effects.
* Stop navigation when killed.
* Stop objective-attack timers when killed.
* Broadcast `OnEnemyDefeated`.
* Destroy its Actor after death presentation.

Current prototype:

```text
BP_ADEnemyDummy
```

Future planned data/configuration variants:

```text
AADEnemyCharacter
├── BP_ADGrunt
├── BP_ADRunner
└── BP_ADTank
```

Separate native subclasses will only be introduced if behavior genuinely differs.

---

# Gameplay Ability System

The player and enemy characters inherit a regular:

```text
UAbilitySystemComponent
```

from `AADCharacterBase`.

For the current single-player design:

```text
Owner Actor  = Character
Avatar Actor = Character
```

The Defense Objective owns a separate Ability System Component.

A custom `UADAbilitySystemComponent` has deliberately not been introduced because no project-specific ASC behavior currently requires one.

---

# Attributes

## `UADAttributeSet`

Current attributes:

```text
Health
MaxHealth

Mana
MaxMana
```

Responsibilities:

* Store shared combat attributes.
* Clamp Health.
* Clamp Mana.
* Validate maximum values.
* Broadcast `OnOutOfHealth`.

It does not decide what zero Health means.

```text
UADAttributeSet
      ↓
OnOutOfHealth
      ↓
      ├── Character
      │       ↓
      │   HandleDeath()
      │
      └── DefenseObjective
              ↓
          HandleOutOfHealth()
```

This allows one Attribute Set to be reused across different gameplay actor types.

---

# Gameplay Effects

Current Gameplay Effects include:

```text
GE_InitializePlayerAttributes
GE_InitializeEnemyDummyAttributes
GE_InitializeDefenseObjectiveAttributes

GE_ArcaneBolt_Cost
GE_ArcaneBolt_Damage

GE_EnemyObjectiveDamage
```

Gameplay implementation applies these effects while balance values remain configurable in assets.

---

# Gameplay Abilities

## `UADGameplayAbility`

Base native Gameplay Ability used by project abilities.

Current instancing:

```text
Instanced Per Actor
```

## `UADGA_TargetedDamage`

Reusable native targeted spell implementation.

Current responsibilities:

* Validate target.
* Reject dead targets.
* Validate range.
* Reject inappropriate airborne activation.
* Capture target at cast start.
* Start cast presentation.
* Wait asynchronously.
* Revalidate target.
* Commit ability.
* Apply mana cost.
* Apply damage.
* Complete or cancel correctly.

## `GA_ArcaneBolt`

Current prototype configuration:

```text
Cast Time = 1.5 seconds
Max Range = 2000 cm
Mana Cost = 20
Damage = 25
```

Flow:

```text
Select Target
    ↓
Press 1
    ↓
Validate
    ↓
Start Cast
    ↓
Wait
    ↓
Revalidate
    ↓
Commit
    ↓
Spend Mana
    ↓
Apply Damage
```

---

# Gameplay Tags

Current native tags:

```text
State.Casting
Ability.CancelOnMovement
```

`State.Casting` identifies an active cast.

`Ability.CancelOnMovement` identifies abilities that movement may interrupt.

Movement cancels matching abilities rather than indiscriminately cancelling every active ability.

---

# Targeting System

## `UADTargetingComponent`

Current responsibilities:

* Perform cursor traces.
* Detect enemy candidates.
* Validate selection range.
* Maintain a weak selected-target reference.
* Clear previous selection.
* Clear selection when clicking elsewhere.
* Handle selected target destruction.
* Broadcast target-change events.

Current flow:

```text
Left Mouse Button
        ↓
IA_SelectTarget
        ↓
Cursor Trace
        ↓
Enemy?
        ↓
Range valid?
        ↓
CurrentTarget
```

---

# Casting System

## `UADCastComponent`

Presentation-facing representation of current casting state.

Responsibilities:

* Store whether casting is active.
* Store display name.
* Store duration.
* Store starting time.
* Calculate normalized progress.
* Broadcast cast start.
* Broadcast cast completion/cancellation.

It does not control ability timing.

Actual asynchronous waiting belongs to GAS through:

```text
UAbilityTask_WaitDelay
```

---

# User Interface

## `WBP_PlayerHUD`

Current root HUD widget.

```text
WBP_PlayerHUD
└── WBP_CastBar
```

## `WBP_CastBar`

Displays:

* Ability name.
* Cast progression.

It listens to `UADCastComponent`.

Presentation can update each frame without gameplay timing depending on UI Tick.

---

# Player Controller

## `AADPlayerController`

Current responsibilities:

* Show the mouse cursor.
* Enable mouse interaction.
* Configure input mode.
* Create the local HUD.

Blueprint configuration:

```text
BP_ADPlayerController
    ↓
Player HUD Class
    ↓
WBP_PlayerHUD
```

---

# Enemy AI

## `AADEnemyAIController`

Current responsibilities:

* Possess enemy characters.
* Retrieve their movement destination.
* Request `MoveToActor`.
* Use NavMesh pathfinding.
* Detect movement completion.
* Inform the enemy when the destination is reached.

Enemies support both placed and dynamically spawned AI possession.

---

# Enemy Navigation

Enemies expose:

```text
SetMoveTarget(AActor*)
GetMoveTarget()
```

They do not depend specifically on a crystal or portal class.

Current destination:

```text
BP_ADDefenseObjective
```

This keeps movement logic decoupled from objective implementation.

---

# Behavior Tree Decision

No Behavior Tree currently exists.

The current behavior remains linear:

```text
Move toward objective
        ↓
Reach objective
        ↓
Attack objective
```

A Behavior Tree will be introduced only when enemy behavior requires meaningful decisions.

---

# Defense Objective

## `AADDefenseObjective`

The actor the player must protect.

Composition:

```text
AADDefenseObjective
├── SceneRoot
├── AbilitySystemComponent
└── ADAttributeSet
```

Responsibilities:

* Initialize GAS.
* Apply initial attributes.
* Expose Health.
* Broadcast Health changes.
* Detect zero Health.
* Broadcast objective defeat.
* Notify the Game Mode.

Blueprint child:

```text
BP_ADDefenseObjective
```

provides the visual representation.

---

# Enemy Objective Attacks

When navigation succeeds:

```text
AADEnemyAIController
        ↓
OnMoveCompleted()
        ↓
AADEnemyCharacter
        ↓
HandleReachedMoveTarget()
        ↓
StartAttackingObjective()
```

The enemy immediately attacks and then uses a gameplay timer for recurring attacks.

Damage is applied through:

```text
GE_EnemyObjectiveDamage
```

The timer stops when:

* The enemy dies.
* The objective becomes invalid.
* The objective is defeated.

---

# Match Rules

## `AADGameMode`

Owns the final match result.

Current result model:

```text
EADGameResult
├── InProgress
├── Victory
└── Defeat
```

Using one enum prevents contradictory combinations of independent boolean flags.

Current methods include:

```text
HandleGameVictory()
HandleGameDefeat()

IsGameOver()
IsGameVictorious()
IsGameDefeated()

GetGameResult()
```

The Blueprint child can provide presentation through:

```text
Game Victory
Game Defeat
```

---

# Wave Data

## `FADWaveSpawnGroup`

Defines one sequential spawning group.

Current fields:

```text
EnemyClass
Quantity
SpawnInterval
SpawnPointId
DelayAfterGroup
```

---

# `UADWaveDataAsset`

Contains:

```text
TArray<FADWaveSpawnGroup>
```

and stores configuration only.

Current assets:

```text
DA_Wave_01
DA_Wave_02
DA_Wave_03
```

All three use the same system; there is no separate code for individual waves.

---

# Spawn Points

## `AADSpawnPoint`

Defines a named spawn transform inside the current level.

Current composition:

```text
SceneRoot
DirectionArrow
```

Each point exposes:

```text
SpawnPointId
```

Wave Data Assets reference logical IDs instead of map actor references.

---

# Wave Director

## `AADWaveDirector`

The Wave Director is responsible for the lifetime of a match's wave sequence.

Current responsibilities:

* Hold Wave Data Assets.
* Hold level Spawn Points.
* Hold the Defense Objective.
* Start waves.
* Spawn groups sequentially.
* Resolve Spawn Point IDs.
* Use deferred enemy spawning.
* Assign the objective before finishing enemy spawn.
* Track enemies belonging to the current wave.
* Track whether spawning is still occurring.
* Detect enemy defeat.
* Detect true wave completion.
* Schedule the next wave.
* Stop the wave system when the objective is defeated.
* Trigger victory after the final wave.

---

# Enemy Lifetime Tracking

Each dynamically spawned enemy is registered with the Wave Director.

```text
Spawn enemy
      ↓
ActiveEnemies.Add()
      ↓
bind OnEnemyDefeated
```

When the enemy dies:

```text
Enemy HandleDeath()
      ↓
OnEnemyDefeated
      ↓
WaveDirector
      ↓
ActiveEnemies.Remove()
```

The Wave Director does not scan the world for enemies.

This provides deterministic knowledge of which enemies belong to the active wave.

---

# Spawning vs Wave Completion

The system explicitly distinguishes:

```text
Finished Spawning
```

from:

```text
Wave Completed
```

A wave is complete only when:

```text
bIsSpawningWave == false

AND

ActiveEnemies.Num() == 0
```

This handles cases where previously spawned enemies are defeated before later enemies have appeared.

---

# Wave Progression

Current provisional flow:

```text
Wave 1
    ↓
Spawn configured enemies
    ↓
Defeat all enemies
    ↓
Wave 1 Complete
    ↓
Preparation Delay
    ↓
Wave 2
    ↓
Defeat all enemies
    ↓
Wave 2 Complete
    ↓
Preparation Delay
    ↓
Wave 3
```

Current default preparation delay:

```text
3 seconds
```

The full trap-building preparation phase will be implemented later.

---

# Victory

When:

```text
Final wave finished spawning

AND

No tracked enemies remain alive
```

the Wave Director invokes:

```text
AADGameMode::HandleGameVictory()
```

The result becomes:

```text
EADGameResult::Victory
```

Victory can only occur if the Defense Objective has not already been defeated.

---

# Defeat

When the Defense Objective reaches zero Health:

```text
ADDefenseObjective
        ↓
OnObjectiveDefeated
        ├──────────────► ADGameMode
        │                    ↓
        │                  Defeat
        │
        └──────────────► ADWaveDirector
                             ↓
                       Stop Wave System
```

Pending spawning, group-delay, and next-wave timers are stopped.

---

# Deferred Enemy Spawning

Enemies are created using a deferred spawn flow:

```text
SpawnActorDeferred()
        ↓
SetMoveTarget(DefenseObjective)
        ↓
Register with WaveDirector
        ↓
FinishSpawningActor()
```

This ensures important gameplay state exists before complete Actor initialization and AI possession.

---

# Timer Policy

Gameplay timers are currently used for discrete scheduled actions:

```text
Enemy objective attacks
Wave spawn intervals
Delays between spawn groups
Delays between waves
```

Tick is not used for these systems.

---

# Event-Driven Architecture

The project increasingly uses explicit gameplay events rather than polling.

Current examples:

```text
AttributeSet
    → OnOutOfHealth

Enemy
    → OnEnemyDefeated

TargetingComponent
    → OnTargetChanged

CastComponent
    → OnCastStarted
    → OnCastEnded

DefenseObjective
    → OnHealthChanged
    → OnObjectiveDefeated

WaveDirector
    → OnWaveStarted
    → OnWaveCompleted
    → OnAliveEnemyCountChanged

GameMode
    → OnGameResultChanged
```

These events create clean integration points for future UI and gameplay systems.

---

# Enhanced Input

Current mapping:

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
    → Move

Space
    → Jump

Left Mouse Button
    → Select target

Right Mouse Button + Mouse
    → Camera look

1
    → Arcane Bolt
```

---

# Current Gameplay Loop

```text
Match starts
      ↓
Wave 1 starts
      ↓
Enemies spawn
      ↓
Enemies navigate toward objective
      ↓
Player selects enemies
      ↓
Player casts Arcane Bolt
      ↓
Enemies die
      ↓
Wave Director tracks deaths
      ↓
Wave completes
      ↓
Next wave
```

Meanwhile surviving enemies can:

```text
Reach objective
      ↓
Attack periodically
      ↓
Objective loses Health
      ↓
Health reaches zero
      ↓
Defeat
```

Completing the final wave instead produces:

```text
Victory
```

---

# Current C++ / Blueprint Boundary

## C++

C++ currently owns:

* Character hierarchy.
* Movement.
* Camera behavior.
* Input bindings.
* Ability System initialization.
* Attribute validation.
* Death rules.
* Targeting logic.
* Target lifetime handling.
* Gameplay Ability execution.
* Cast lifecycle.
* Movement interruption.
* Enemy navigation.
* Enemy objective attacks.
* Defense Objective rules.
* Match results.
* Wave execution.
* Enemy tracking.
* Wave completion.
* Victory and defeat.

## Blueprint / Data Assets

Blueprint and data currently own:

* Meshes.
* Animation setup.
* Input Action assets.
* Input Mapping Context.
* Initial attribute values.
* Spell cost.
* Spell damage.
* Objective damage.
* Cast configuration.
* Target visuals.
* Cast bar.
* HUD composition.
* Objective appearance.
* Wave composition.
* Spawn quantities.
* Spawn intervals.
* Spawn locations.
* Prototype victory/defeat presentation.

---

# Important Technical Decisions

## GAS remains on Characters

The player and enemies currently own their Ability System directly.

Moving the player ASC to Player State remains unnecessary because there is currently no multiplayer, respawn, or character-switching requirement.

## The Defense Objective also uses GAS

Characters and the objective share the same attribute/effect infrastructure instead of maintaining separate health implementations.

## No custom ASC yet

A project-specific Ability System Component will only be introduced when it has concrete responsibilities.

## Experience and Level are not GAS attributes yet

They are planned as progression data unless future gameplay rules demonstrate a need for Gameplay Effects to manipulate them.

## Cast execution remains inside Gameplay Abilities

`UADCastComponent` exposes presentation state but does not execute or time abilities.

## Non-owning gameplay relationships avoid ownership assumptions

Weak references are used for transient relationships where appropriate.

## Wave definitions do not reference level actors

Logical Spawn Point IDs separate reusable data assets from specific level instances.

## Wave completion is event-driven

The director does not repeatedly search the world for living enemies.

It already owns knowledge of the enemies it created.

## Death and destruction are separate concepts

Wave progression reacts to enemy defeat immediately rather than waiting for its Actor to disappear visually.

## Match result uses one enum

The match has exactly one logical result:

```text
InProgress
Victory
Defeat
```

instead of multiple potentially contradictory boolean flags.

## No Behavior Tree yet

Current enemy behavior remains simple enough to express clearly through `AADEnemyAIController`.

---

# Development Status

## Day 1 — Project Setup

Completed.

* Unreal Engine 5.8 C++ project.
* Git.
* Git LFS.
* Enhanced Input.
* Gameplay Ability System.
* Prototype map.
* Initial documentation.

## Day 2 — Player Character

Completed.

* Character inheritance.
* Player character.
* Movement.
* Camera.
* Jumping.
* Enhanced Input.

## Day 3 — Ability System and Attributes

Completed.

* Ability System Component.
* `IAbilitySystemInterface`.
* Attribute Set.
* Health.
* Mana.
* Initial Gameplay Effects.

## Day 4 — Target Selection

Completed.

* Enemy base class.
* Targeting Component.
* Cursor selection.
* Selection distance.
* Target cleanup.
* Target indicator.
* Player Controller.

## Day 5 — First Targeted Ability

Completed.

* Gameplay Ability base.
* Targeted-damage ability.
* Arcane Bolt.
* Mana cost.
* Damage.
* Enemy death.
* First combat loop.

## Day 6 — Spell Casting

Completed.

* Cast Component.
* Ability Task-based cast time.
* Movement interruption.
* Gameplay Tags.
* Cast bar.
* HUD.
* Post-cast validation.
* Airborne casting restriction.

## Day 7 — Enemy Navigation

Completed.

* Enemy AI Controller.
* Automatic AI possession.
* Navigation target.
* NavMesh pathfinding.
* Movement completion.
* Death/navigation integration.

## Day 8 — Defense Objective

Completed.

* GAS-enabled objective.
* Shared health architecture.
* Objective Health changes.
* Periodic enemy attacks.
* Objective damage Gameplay Effect.
* Game Mode.
* Defeat condition.

## Day 9 — Data-Driven Wave Spawning

Completed.

* Spawn groups.
* Wave Data Asset.
* Spawn Point actors.
* Wave Director.
* Sequential spawning.
* Spawn intervals.
* Group delays.
* Deferred spawning.
* Data-driven first wave.

## Day 10 — Wave Completion and Victory

Completed.

* Enemy defeat delegate.
* Living enemy tracking.
* Explicit distinction between spawning completion and wave completion.
* Automatic wave progression.
* Inter-wave delay.
* Three provisional waves.
* Wave status delegates.
* Wave cancellation after defeat.
* Match-result enum.
* Victory condition.
* Complete provisional wave-defense gameplay loop.

---

# Current Prototype Definition

At the end of Day 10, the prototype can:

1. Start a match.
2. Start the first configured wave.
3. Spawn enemies from Data Assets.
4. Navigate enemies toward the Defense Objective.
5. Let the player select enemies.
6. Cast Arcane Bolt.
7. Display casting progress.
8. Interrupt casting through movement.
9. Consume mana.
10. Damage enemies.
11. Defeat enemies.
12. Track living enemies belonging to the active wave.
13. Detect when a wave is genuinely complete.
14. Automatically start subsequent waves.
15. Allow surviving enemies to attack the objective.
16. Trigger defeat if the objective reaches zero Health.
17. Trigger victory if all configured waves are completed.

---

# Current Limitations

The current prototype intentionally does not yet include:

* Final five-wave balance.
* Final enemy archetypes.
* Projectiles.
* Damage-over-time spells.
* Area-of-effect abilities.
* Meteor targeting.
* Cooldowns.
* Ability-bar UI.
* Tab targeting.
* Formal preparation/build phase.
* Traps.
* Experience.
* Levels.
* Talents.
* Behavior Trees.
* Crowd avoidance.
* Final objective UI.
* Final victory/defeat UI.
* Audio and VFX polish.

---

# Next Milestone — Day 11

The next phase begins **Week 3: Abilities and Effects**.

Day 11 will replace Arcane Bolt's immediate target damage with a reusable targeted projectile.

Target architecture:

```text
GA_ArcaneBolt
     ↓
Cast Complete
     ↓
Spawn ADProjectile
     ↓
Projectile travels toward captured target
     ↓
Impact
     ↓
Apply Gameplay Effect
     ↓
Enemy Health decreases
```

The projectile system should support future abilities without coupling projectile movement to Arcane Bolt specifically.

---

# Planned Roadmap

## Week 1 — Foundation and Targeting

Status: **Completed**

## Week 2 — Casting and Wave Defense

Status: **Completed**

## Week 3 — Abilities and Effects

Next:

* Reusable targeted projectile.
* Damage over time.
* Frost Nova.
* Meteor.
* Cooldowns and ability UI.

## Week 4 — Traps

* Trap base.
* Placement.
* Spike Trap.
* Slow Trap.
* Construction cost.

## Week 5 — Progression

* Experience.
* Levels.
* Talent Data Assets.
* Talent selection.
* Initial talent set.

## Week 6 — Content and Architecture

* Grunt.
* Runner.
* Tank.
* Final five waves.
* Refactor.

## Week 7 — Testing and Profiling

* Attribute tests.
* Combat tests.
* Progression tests.
* Load scenario.
* Unreal Insights.
* Measured optimization.

## Week 8 — Presentation

* Visual polish.
* VFX.
* Audio.
* Bug fixing.
* Packaged build.
* Documentation.
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

Every development session should produce one verifiable gameplay result.

```text
Define Objective
      ↓
Implement
      ↓
Compile
      ↓
Test Normal Case
      ↓
Test Edge Cases
      ↓
Review Logs
      ↓
Update README
      ↓
Commit
```

Typical completion:

```bash
git status
git add .
git commit -m "<meaningful commit message>"
git status
```

Expected final state:

```text
nothing to commit, working tree clean
```

---

# License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
