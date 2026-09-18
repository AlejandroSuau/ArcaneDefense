# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project demonstrates Gameplay Programmer and Systems Programmer skills through a wave-defense experience combining target-based spell combat, ground targeting, crowd control, Gameplay Ability System, enemy AI, data-driven waves, progression, UI, testing, and profiling.

## Project Goal

The target experience is an 8–12 minute wave-defense match where the player protects a central objective against progressively harder enemy waves.

The planned vertical slice includes four player abilities, two placeable traps, three enemy archetypes, five waves, in-match progression, talents, victory and defeat conditions, functional UI, automated testing, profiling, and a packaged playable build.

## Technology

The project currently uses Unreal Engine 5.8, C++, Enhanced Input, Gameplay Ability System, Gameplay Attributes, Gameplay Effects, Gameplay Tags, Gameplay Ability Tasks, Projectile Movement, collision queries, NavMesh navigation, AI Controllers, UMG, Data Assets, gameplay timers, delegates, Git, and Git LFS.

## Development Principles

Core gameplay rules belong in C++.

Blueprint and Data Assets primarily own configuration, asset composition, visuals, UI, animation, VFX and balance values.

Gameplay systems are separated by responsibility:

```text
ADTargetingComponent
    → selected enemy Actor

ADGroundTargetingComponent
    → selected world location

ADCastComponent
    → cast presentation state

GameplayAbility
    → validation and execution

GameplayEffect
    → attributes, persistent states and cooldowns

ADProjectile
    → effect transport through the world

ADEnemyAIController
    → navigation

ADDefenseObjective
    → defended-object behavior

ADWaveDataAsset
    → wave configuration

ADWaveDirector
    → wave execution/lifecycle

ADGameMode
    → match result
```

Abstractions are introduced only when concrete gameplay requirements justify them.

## Source Structure

```text
Source/
└── ArcaneDefense/
    ├── AbilitySystem/
    │   ├── ADAttributeSet.h
    │   ├── ADAttributeSet.cpp
    │   ├── ADGameplayTags.h
    │   ├── ADGameplayTags.cpp
    │   └── Abilities/
    │       ├── ADGameplayAbility.h
    │       ├── ADGameplayAbility.cpp
    │       ├── ADGA_TargetedDamage.h
    │       ├── ADGA_TargetedDamage.cpp
    │       ├── ADGA_TargetProjectile.h
    │       ├── ADGA_TargetProjectile.cpp
    │       ├── ADGA_RadialEffect.h
    │       ├── ADGA_RadialEffect.cpp
    │       ├── ADGA_GroundTargetedArea.h
    │       └── ADGA_GroundTargetedArea.cpp
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
    │   ├── ADGroundTargetingComponent.h
    │   ├── ADGroundTargetingComponent.cpp
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
    └── Waves/
        ├── ADWaveTypes.h
        ├── ADWaveDataAsset.h
        ├── ADSpawnPoint.h
        ├── ADSpawnPoint.cpp
        ├── ADWaveDirector.h
        └── ADWaveDirector.cpp
```

## Current Content Structure

```text
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   ├── GA_ArcaneBolt
    │   ├── GA_Burn
    │   ├── GA_FrostNova
    │   ├── GA_Meteor
    │   └── Projectiles/
    │       └── BP_ADProjectile_ArcaneBolt
    │
    ├── Effects/
    │   ├── Attributes/
    │   ├── Costs/
    │   │   ├── GE_ArcaneBolt_Cost
    │   │   ├── GE_Burn_Cost
    │   │   ├── GE_FrostNova_Cost
    │   │   └── GE_Meteor_Cost
    │   ├── Cooldowns/
    │   │   ├── GE_CD_ArcaneBolt
    │   │   ├── GE_CD_Burn
    │   │   ├── GE_CD_FrostNova
    │   │   └── GE_CD_Meteor
    │   ├── Damage/
    │   │   ├── GE_ArcaneBolt_Damage
    │   │   ├── GE_Burn_DoT
    │   │   ├── GE_Meteor_Damage
    │   │   └── GE_EnemyObjectiveDamage
    │   └── Status/
    │       └── GE_FrostNova_Root
    │
    ├── Input/
    │   ├── IA_Move
    │   ├── IA_Look
    │   ├── IA_Jump
    │   ├── IA_SelectTarget
    │   ├── IA_CameraLook
    │   ├── IA_Ability1
    │   ├── IA_Ability2
    │   ├── IA_Ability3
    │   ├── IA_Ability4
    │   └── IMC_Player
    │
    ├── Targeting/
    │   └── BP_ADGroundTargetPreview
    │
    ├── UI/
    │   ├── WBP_PlayerHUD
    │   ├── WBP_CastBar
    │   └── WBP_AbilitySlot
    │
    ├── Waves/
    │   ├── DA_Wave_01
    │   ├── DA_Wave_02
    │   └── DA_Wave_03
    │
    └── Maps/
        └── L_Prototype
```

## Character Architecture

`AADCharacterBase` provides the shared Ability System, Attribute Set, initialization and generic death behavior.

```text
ACharacter
    ↓
AADCharacterBase
    ├── AADPlayerCharacter
    └── AADEnemyCharacter
```

`AADPlayerCharacter` integrates player input, targeting, ground targeting and activation of four granted Gameplay Abilities.

Current controls:

```text
1 → Arcane Bolt
2 → Burn
3 → Frost Nova
4 → Meteor
```

## Gameplay Ability Architecture

```text
UADGameplayAbility
    ├── UADGA_TargetedDamage
    │       ├── GA_Burn
    │       └── UADGA_TargetProjectile
    │               └── GA_ArcaneBolt
    │
    ├── UADGA_RadialEffect
    │       └── GA_FrostNova
    │
    └── UADGA_GroundTargetedArea
            └── GA_Meteor
```

The base Gameplay Ability also exposes UI-facing metadata such as display name and icon.

## Arcane Bolt

```text
Targeting      = Enemy Actor
Cast Time      = 1.5 s
Mana Cost      = 20
Delivery       = Homing Projectile
Damage         = 25
Cooldown       = 2 s
```

## Burn

```text
Targeting      = Enemy Actor
Cast Time      = 1.0 s
Mana Cost      = 15
Delivery       = Duration Gameplay Effect
Duration       = 5 s
Period         = 1 s
Cooldown       = 4 s
```

## Frost Nova

```text
Targeting      = Radius around caster
Activation     = Instant
Mana Cost      = 25
Radius         = 600 cm
Root Duration  = 3 s
Cooldown       = 6 s
```

## Meteor

```text
Targeting      = Confirmed world location
Cast Time      = 2 s
Mana Cost      = 30
Range          = 2500 cm
AoE Radius     = 400 cm
Damage         = 40
Cooldown       = 8 s
```

## Cooldown Architecture

Cooldowns are represented by duration Gameplay Effects.

```text
Gameplay Ability
      ↓
CommitAbility()
      ↓
Cooldown Gameplay Effect
      ↓
Cooldown Tag on owner ASC
      ↓
Ability unavailable
      ↓
Effect expires
      ↓
Ability available
```

Current cooldown tags:

```text
Cooldown.Ability.ArcaneBolt
Cooldown.Ability.Burn
Cooldown.Ability.FrostNova
Cooldown.Ability.Meteor
```

Gameplay code does not maintain independent cooldown timers.

The combat UI queries GAS for remaining cooldown and duration.

## Invalid Gameplay States

Ability activation is explicitly prevented in incompatible states.

Casting grants `State.Casting`, and all player spells treat that state as activation-blocking.

Ground targeting is handled separately from spell activation and prevents unrelated spell inputs until the target is confirmed or cancelled.

Airborne validation prevents the current spell kit from being initiated while falling.

GAS remains authoritative over cost and cooldown validation.

A global cooldown is deliberately not implemented because it is optional and no current gameplay requirement justifies it.

## Targeting Architecture

`UADTargetingComponent` represents selected enemy Actors.

`UADGroundTargetingComponent` represents selected world positions.

The ground-target component only ticks while interactive location selection is active.

## Gameplay Effects

Gameplay Effects currently represent:

```text
Initial attributes
Mana costs
Instant damage
Periodic damage
Temporary crowd-control states
Ability cooldowns
Enemy objective damage
```

The project therefore uses GAS not merely as a damage framework but as the primary system for ability state, cost, persistent effects and cooldowns.

## Combat HUD

`WBP_PlayerHUD` now contains:

```text
Cast Bar
Mana display
Ability Bar
```

The ability bar contains four reusable `WBP_AbilitySlot` instances.

Each slot knows which Gameplay Ability class it represents and displays:

```text
Input key
Ability icon
Cooldown remaining
Insufficient-mana state
```

Cooldown numbers reflect GAS state rather than widget-owned timers.

## Enemy AI and Crowd Control

`AADEnemyAIController` handles navigation toward the defense objective.

`State.Rooted` temporarily disables enemy movement and automatically restores navigation when its owning Gameplay Effect expires.

## Defense Objective

`AADDefenseObjective` is GAS-enabled and shares the Health Attribute architecture.

Enemies periodically damage it.

Zero Health produces defeat.

## Wave Architecture

`UADWaveDataAsset` stores spawn definitions.

`AADSpawnPoint` represents named world locations.

`AADWaveDirector` executes waves, tracks spawned enemies, distinguishes finished spawning from wave completion and automatically advances through the configured sequence.

## Match Rules

`AADGameMode` stores exactly one result:

```text
InProgress
Victory
Defeat
```

## Event-Driven Architecture

The current project uses events for attribute depletion, target changes, casts, crowd-control tags, enemy defeat, objective defeat, wave state and match result.

UI uses direct lightweight queries where continuous presentation information such as cooldown remaining time is needed.

## Timer and Tick Policy

Custom gameplay timers are used for discrete scheduled behavior such as enemy attacks and wave spawning.

GAS owns DoT periods, status durations and cooldowns.

`UProjectileMovementComponent` owns projectile movement.

Ground-target preview ticks only while targeting.

UI may tick for lightweight visual presentation of four cooldown slots but does not own gameplay timing.

## C++ / Blueprint Boundary

C++ owns gameplay rules, validation, queries, state reactions, targeting behavior, ability execution, AI, wave logic and match rules.

Blueprint/Data Assets own spell tuning, Gameplay Effect configuration, visuals, input assets, widgets and wave content.

## Important Technical Decisions

The standard `UAbilitySystemComponent` remains sufficient; a project-specific ASC is not created without a concrete responsibility.

Cooldowns use GAS rather than custom timers.

Cooldown UI reads GAS rather than duplicating timer state.

There is currently no global cooldown.

Actor targeting and ground targeting remain separate concepts.

Gameplay states such as `State.Rooted` and `State.Casting` remain tag-driven.

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
Day 12 — Periodic Burn Gameplay Effect       Completed
Day 13 — Frost Nova / Root Crowd Control     Completed
Day 14 — Meteor / Ground Targeting           Completed
Day 15 — Cooldowns / Combat Ability UI       Completed
```

## Current Prototype

The project now contains four functionally distinct player abilities with targeting, cost, cast rules, effects and cooldowns.

It also supports wave spawning, enemy AI, objective defense, victory and defeat.

Week 3's core ability milestone is complete.

## Next Milestone — Day 16

Day 16 begins the trap system.

The first objective is not trap placement yet.

It is to design the reusable trap runtime architecture:

```text
AADTrapBase
      ↓
Trap configuration
      ↓
Enemy detection
      ↓
Activation
      ↓
Cooldown
```

and a data definition that lets individual trap types configure their values without hardcoding them into the base class.

## Remaining Roadmap

```text
Week 4
Days 16–20
Traps and preparation phase

Week 5
Days 21–25
Experience, levels and talents

Week 6
Days 26–30
Final enemy archetypes, five waves and refactor

Week 7
Days 31–35
Tests, Unreal Insights and optimization

Week 8
Days 36–40
Polish, packaging, documentation and portfolio
```

## Out of Scope

The initial vertical slice excludes multiplayer, matchmaking, backend systems, inventory, equipment, loot, crafting, shops, campaigns, multiple maps, multiple playable classes, complex talent trees and original character modelling/animation production.

## Definition of Done

The project is complete when it contains a packaged 8–12 minute match, four spells, two traps, three enemy types, five waves, progression, talents, victory/defeat, functional UI, tests, documented profiling and optimization, technical documentation and a short gameplay presentation video.

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

## License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
