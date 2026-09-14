# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project demonstrates Gameplay Programmer and Systems Programmer skills through a wave-defense experience combining target-based spell combat, Gameplay Ability System, enemy AI, data-driven waves, progression, UI, testing, and profiling.

## Project Goal

The target experience is an 8–12 minute match where the player protects a central objective against progressively harder enemy waves.

The planned vertical slice includes four player abilities, two placeable traps, three enemy archetypes, five waves, in-match progression, talents, victory and defeat conditions, functional UI, automated testing, profiling, and a packaged playable build.

## Technology

The project currently uses Unreal Engine 5.8, C++, Enhanced Input, Gameplay Ability System, Gameplay Attributes, Gameplay Effects, Gameplay Tags, Gameplay Ability Tasks, Projectile Movement, Navigation, AI Controllers, UMG, Data Assets, gameplay timers, C++/Blueprint delegates, Git, and Git LFS.

Behavior Trees, additional data-driven definitions, automated tests, and Unreal Insights are planned for later milestones.

## Development Principles

Core gameplay rules belong in C++. Blueprint is primarily responsible for asset configuration, composition, UI, animation and presentation.

Gameplay values such as health, mana, damage, costs, cast time, projectile configuration and wave composition are data-driven whenever practical.

Systems have narrow responsibilities:

```text
ADTargetingComponent
    → current selected target

ADCastComponent
    → cast presentation state

GameplayAbility
    → ability rules and execution

ADProjectile
    → transport an effect to a target

ADEnemyAIController
    → enemy navigation

ADDefenseObjective
    → defended-object behavior

ADWaveDataAsset
    → wave configuration

ADSpawnPoint
    → spawn location

ADWaveDirector
    → wave execution and lifecycle

ADGameMode
    → global match result
```

Abstractions are introduced only when a concrete gameplay requirement justifies them.

## Source Structure

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
    │       ├── ADGA_TargetedDamage.cpp
    │       ├── ADGA_TargetProjectile.h
    │       └── ADGA_TargetProjectile.cpp
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
    │   ├── ADCastComponent.cpp
    │   ├── ADProjectile.h
    │   └── ADProjectile.cpp
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

## Content Structure

```text
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   ├── GA_ArcaneBolt
    │   └── Projectiles/
    │       └── BP_ADProjectile_ArcaneBolt
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
    │   ├── Costs/
    │   │   └── GE_ArcaneBolt_Cost
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

## Character Architecture

`AADCharacterBase` is the shared base class for player and enemy characters. It owns the Ability System Component and shared Attribute Set, initializes GAS, applies initial attributes, exposes Health and Mana and implements the generic character death state.

```text
ACharacter
    ↓
AADCharacterBase
    ├── AADPlayerCharacter
    └── AADEnemyCharacter
```

`AADPlayerCharacter` owns player movement, camera, Enhanced Input integration, ability activation, targeting, casting state and movement-based cast interruption.

`AADEnemyCharacter` owns enemy-specific gameplay state, navigation destination, objective attacks, target presentation state and the `OnEnemyDefeated` event.

## Gameplay Ability System

Characters currently own a standard `UAbilitySystemComponent` directly.

The project deliberately does not yet contain a custom `UADAbilitySystemComponent`, because no project-specific ASC behavior currently justifies one.

The Defense Objective owns its own Ability System Component.

## Attributes

`UADAttributeSet` currently contains:

```text
Health
MaxHealth
Mana
MaxMana
```

It validates/clamps attribute values and broadcasts `OnOutOfHealth`.

The Attribute Set does not decide what zero Health means.

```text
OnOutOfHealth
    ├── Character → HandleDeath()
    └── Defense Objective → HandleOutOfHealth()
```

## Gameplay Effects

Current Gameplay Effects include:

```text
GE_InitializePlayerAttributes
GE_InitializeEnemyDummyAttributes
GE_InitializeDefenseObjectiveAttributes

GE_ArcaneBolt_Cost
GE_ArcaneBolt_Damage

GE_EnemyObjectiveDamage
```

Gameplay implementation determines when effects are applied; the assets contain configurable balance values.

## Gameplay Ability Architecture

`UADGameplayAbility` is the native base for project abilities and currently uses `InstancedPerActor`.

`UADGA_TargetedDamage` contains shared targeted-cast behavior:

```text
Target validation
Range validation
Airborne validation
Target capture
Cast lifecycle
Post-cast target validation
Cost commit
Payload execution
```

It exposes `ExecuteAbilityPayload()` as a specialization point.

Its default implementation applies the configured Gameplay Effect directly.

`UADGA_TargetProjectile` specializes that payload by creating a homing projectile instead of directly applying the Gameplay Effect.

## Arcane Bolt

`GA_ArcaneBolt` currently derives from:

```text
ADGA_TargetProjectile
```

Current prototype values:

```text
Cast Time = 1.5 seconds
Max Range = 2000 cm
Mana Cost = 20
Damage = 25
Projectile = BP_ADProjectile_ArcaneBolt
```

Current execution:

```text
Select target
    ↓
Start Arcane Bolt
    ↓
Validate target/range/state
    ↓
Cast 1.5 seconds
    ↓
Commit ability
    ↓
Spend mana
    ↓
Construct GameplayEffectSpec
    ↓
Spawn projectile
    ↓
Projectile follows target
    ↓
Impact
    ↓
Apply GameplayEffectSpec
    ↓
Enemy Health decreases
```

Cast completion and spell impact are now separate events.

## Projectile System

`AADProjectile` is a reusable Actor responsible for transporting a Gameplay Effect to a target.

Composition:

```text
AADProjectile
├── SphereCollision
└── ProjectileMovement
```

It receives during initialization:

```text
Source AbilitySystemComponent
Target Actor
GameplayEffectSpecHandle
```

It does not know which spell created it.

Homing movement is handled by `UProjectileMovementComponent`, not custom Actor Tick.

A targeted projectile ignores overlapping Pawns other than its captured target.

World geometry can block and destroy the projectile.

If its target disappears before impact, the projectile safely destroys itself.

The Blueprint child `BP_ADProjectile_ArcaneBolt` contains Arcane Bolt-specific presentation only.

## Gameplay Tags

Current native tags include:

```text
State.Casting
Ability.CancelOnMovement
```

Movement only cancels abilities explicitly marked as movement-interruptible.

## Targeting

`UADTargetingComponent` performs cursor targeting, validates selection distance, maintains a weak reference to the selected enemy and automatically clears invalid/destroyed targets.

Target visuals remain Blueprint-driven through `AADEnemyCharacter`.

## Casting

`UADCastComponent` exposes presentation state for the current cast.

Actual asynchronous waiting remains inside GAS using `UAbilityTask_WaitDelay`.

The component itself does not execute abilities.

## UI

`WBP_PlayerHUD` is the root local-player HUD.

Current composition:

```text
WBP_PlayerHUD
└── WBP_CastBar
```

`WBP_CastBar` listens to `UADCastComponent` events and displays cast name and progress.

## Player Controller

`AADPlayerController` owns local-player-level concerns including cursor configuration, input mode and HUD creation.

`BP_ADPlayerController` provides Blueprint configuration.

## Enemy AI

`AADEnemyAIController` handles navigation through `MoveToActor`, using NavMesh pathfinding.

Enemies expose a generic:

```text
SetMoveTarget(AActor*)
```

rather than depending specifically on the Defense Objective.

No Behavior Tree exists yet because current AI behavior remains linear.

## Defense Objective

`AADDefenseObjective` is a GAS-enabled Actor containing its own Ability System Component and the shared Attribute Set.

It broadcasts Health changes and objective defeat.

When Health reaches zero it informs `AADGameMode`.

## Enemy Objective Attacks

After successfully reaching the objective, enemies periodically apply:

```text
GE_EnemyObjectiveDamage
```

through GAS.

Gameplay timers control attack intervals instead of Actor Tick.

## Game Mode

`AADGameMode` owns the match result:

```text
EADGameResult
├── InProgress
├── Victory
└── Defeat
```

This prevents contradictory combinations of match-state booleans.

## Wave Data

`FADWaveSpawnGroup` currently contains:

```text
EnemyClass
Quantity
SpawnInterval
SpawnPointId
DelayAfterGroup
```

`UADWaveDataAsset` stores ordered arrays of these groups.

Current prototype assets:

```text
DA_Wave_01
DA_Wave_02
DA_Wave_03
```

## Spawn Points

`AADSpawnPoint` defines named spawn transforms in the map.

Wave assets reference logical `FName` identifiers rather than direct references to level actors, keeping wave definitions map-independent.

## Wave Director

`AADWaveDirector` owns the lifecycle of the current wave sequence.

Responsibilities include spawning enemies, resolving Spawn Points, assigning the Defense Objective, tracking active enemies, detecting wave completion, scheduling subsequent waves, stopping after defeat and triggering victory after the final wave.

## Enemy Lifetime Tracking

The Wave Director registers every enemy it creates.

```text
Spawn
  ↓
ActiveEnemies.Add()
  ↓
Bind OnEnemyDefeated
```

Death produces:

```text
OnEnemyDefeated
  ↓
ActiveEnemies.Remove()
  ↓
TryCompleteCurrentWave()
```

No repeated world scans are necessary.

## Wave Completion

The project explicitly separates:

```text
Finished spawning
```

from:

```text
Wave completed
```

A wave completes only when:

```text
bIsSpawningWave == false
AND
ActiveEnemies.Num() == 0
```

## Current Wave Progression

```text
Wave 1
    ↓
Defeat all enemies
    ↓
Delay
    ↓
Wave 2
    ↓
Defeat all enemies
    ↓
Delay
    ↓
Wave 3
    ↓
Defeat all enemies
    ↓
Victory
```

If the Defense Objective reaches zero Health at any point:

```text
Defeat
    ↓
Stop Wave System
```

## Event-Driven Architecture

Current events include:

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

Gameplay systems react to events rather than repeatedly polling global state.

## Timer Policy

Gameplay timers currently control:

```text
Enemy objective attacks
Enemy spawning
Spawn-group delays
Inter-wave delays
```

Custom Tick is avoided for these discrete scheduled behaviors.

Projectile continuous movement is delegated to Unreal's dedicated Projectile Movement Component.

## C++ / Blueprint Boundary

C++ currently owns gameplay rules, character hierarchy, input bindings, GAS initialization, attribute rules, targeting, ability execution, projectile transport, cast lifecycle, AI navigation, enemy attacks, objective behavior, wave execution and match results.

Blueprint/Data Assets currently own meshes, animations, materials, effects, input assets, ability tuning, projectile visuals, widgets, wave composition and presentation.

## Technical Decisions

The player's ASC remains on the Character because the current project has no multiplayer, respawn persistence or character switching requirement.

The Defense Objective uses GAS instead of introducing a second health framework.

Experience and Level remain outside GAS until a concrete effect-driven requirement exists.

The cast component represents UI-facing cast state while Gameplay Abilities remain authoritative over execution.

Weak references are used for non-owned gameplay relationships.

Wave definitions contain no direct level Actor references.

Enemy death and Actor destruction are treated as different events.

The Wave Director reacts to death, not delayed visual destruction.

Targeted projectiles carry a `GameplayEffectSpec`, allowing the projectile actor to remain independent from a particular spell or damage value.

## Development Status

```text
Day 1  — Project Setup                       Completed
Day 2  — Player Character                    Completed
Day 3  — Ability System and Attributes       Completed
Day 4  — Target Selection                    Completed
Day 5  — First Targeted Ability              Completed
Day 6  — Spell Casting                       Completed
Day 7  — Enemy Navigation                    Completed
Day 8  — Defense Objective                   Completed
Day 9  — Data-Driven Wave Spawning           Completed
Day 10 — Wave Completion and Victory         Completed
Day 11 — Reusable Homing Projectile          Completed
```

## Current Prototype

The prototype can start a match, spawn data-driven waves, navigate enemies toward the objective, select enemies, cast Arcane Bolt, interrupt casting through movement, spend mana, launch a homing projectile, apply damage on projectile impact, kill enemies, track active enemies, complete waves automatically, damage the Defense Objective, trigger defeat and trigger victory after all configured waves.

## Current Limitations

The prototype does not yet include damage-over-time abilities, area spells, ground-targeted abilities, cooldowns, complete ability UI, final enemy archetypes, traps, progression, talents, final five-wave balance, Behavior Trees, final VFX/audio or profiling.

## Next Milestone — Day 12

Day 12 introduces the first damage-over-time ability.

Target architecture:

```text
Gameplay Ability
    ↓
Apply Gameplay Effect
    ↓
Duration-based Burn Effect
    ↓
Periodic damage
    ↓
Enemy Health decreases over time
```

This will introduce an important GAS distinction between:

```text
Instant Gameplay Effects
```

and:

```text
Duration + Periodic Gameplay Effects
```

## Roadmap

Week 1 — Foundation and targeting: **Completed**

Week 2 — Casting and wave defense: **Completed**

Week 3 — Abilities and effects: **In progress**

Upcoming work covers burn/DoT, Frost Nova, Meteor and cooldown/ability UI.

Week 4 covers traps.

Week 5 covers experience, levels and talents.

Week 6 covers final enemy archetypes and wave content.

Week 7 covers testing, profiling and optimization.

Week 8 covers presentation, packaging and portfolio material.

## Out of Scope

The initial vertical slice excludes multiplayer, matchmaking, backend services, inventory, equipment, loot, crafting, shops, campaign, multiple maps, multiple playable classes, complex talent trees and original character modelling/animation production.

## Definition of Done

The vertical slice is complete when it contains a packaged 8–12 minute match, four spells, two traps, three enemy types, five waves, progression, talents, victory/defeat, functional UI, automated tests, documented profiling and optimization, technical documentation and a short gameplay presentation video.

## Development Workflow

```text
Define verifiable objective
        ↓
Implement
        ↓
Compile
        ↓
Test normal case
        ↓
Test edge cases
        ↓
Review logs
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

## License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
