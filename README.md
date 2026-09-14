# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project demonstrates Gameplay Programmer and Systems Programmer skills through a wave-defense experience combining target-based spell combat, Gameplay Ability System, enemy AI, data-driven waves, progression, UI, testing, and profiling.

## Project Goal

The target experience is an 8–12 minute match where the player protects a central objective against progressively harder enemy waves.

The planned vertical slice includes four player abilities, two placeable traps, three enemy archetypes, five waves, in-match progression, talents, victory and defeat conditions, functional UI, automated testing, profiling, and a packaged playable build.

## Technology

The project currently uses:

* Unreal Engine 5.8.
* C++.
* Enhanced Input.
* Gameplay Ability System.
* Gameplay Attributes.
* Gameplay Effects.
* Instant Gameplay Effects.
* Duration Gameplay Effects.
* Periodic Gameplay Effects.
* Gameplay Effect stacking.
* Gameplay Tags.
* Gameplay Ability Tasks.
* Projectile Movement.
* Navigation.
* AI Controllers.
* UMG.
* Data Assets.
* Gameplay timers.
* C++ and Blueprint delegates.
* Git.
* Git LFS.

## Development Principles

Core gameplay rules belong in C++.

Blueprint and Data Assets are primarily responsible for configuration, composition, presentation, UI, animation, visual effects, and balance values.

Systems remain narrowly focused:

```text
ADTargetingComponent
    → Current selected target

ADCastComponent
    → Cast presentation state

GameplayAbility
    → Ability validation and execution

ADProjectile
    → Transport an effect through the world

GameplayEffect
    → Modify attributes and represent persistent effects

ADEnemyAIController
    → Enemy navigation

ADDefenseObjective
    → Defended-object behavior

ADWaveDataAsset
    → Wave configuration

ADSpawnPoint
    → Spawn location

ADWaveDirector
    → Wave execution and lifecycle

ADGameMode
    → Global match result
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

Day 12 required no new native gameplay class. The existing targeted ability infrastructure was deliberately reused.

## Current Content Structure

```text
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   ├── GA_ArcaneBolt
    │   ├── GA_Burn
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
    │   │
    │   ├── Costs/
    │   │   ├── GE_ArcaneBolt_Cost
    │   │   └── GE_Burn_Cost
    │   │
    │   └── Damage/
    │       ├── GE_ArcaneBolt_Damage
    │       ├── GE_Burn_DoT
    │       └── GE_EnemyObjectiveDamage
    │
    ├── Input/
    │   ├── IA_Move
    │   ├── IA_Look
    │   ├── IA_Jump
    │   ├── IA_SelectTarget
    │   ├── IA_CameraLook
    │   ├── IA_Ability1
    │   ├── IA_Ability2
    │   └── IMC_Player
    │
    ├── Maps/
    │   └── L_Prototype
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

`AADCharacterBase` is shared by player and enemy Characters.

It owns the Ability System Component and shared Attribute Set, initializes GAS, applies initial attributes and implements generic death behavior.

```text
ACharacter
    ↓
AADCharacterBase
    ├── AADPlayerCharacter
    └── AADEnemyCharacter
```

`AADPlayerCharacter` currently owns movement, camera, Enhanced Input bindings, ability activation, targeting and cast interruption.

The player currently supports two explicitly bound ability inputs.

```text
1 → GA_ArcaneBolt
2 → GA_Burn
```

`AADEnemyCharacter` owns enemy-specific navigation, objective attacks, targeting presentation and enemy-defeat events.

## Gameplay Ability System

Characters own a standard `UAbilitySystemComponent` directly.

The Defense Objective owns its own ASC.

No custom `UADAbilitySystemComponent` currently exists because no project-specific ASC behavior has yet justified one.

## Attributes

`UADAttributeSet` currently contains:

```text
Health
MaxHealth

Mana
MaxMana
```

It handles attribute validation and broadcasts `OnOutOfHealth`.

Death behavior remains outside the Attribute Set.

```text
OnOutOfHealth
    ├── Character → HandleDeath()
    └── Defense Objective → HandleOutOfHealth()
```

This architecture works equally for instant and periodic changes to Health.

## Gameplay Effects

Current effects include:

```text
GE_InitializePlayerAttributes
GE_InitializeEnemyDummyAttributes
GE_InitializeDefenseObjectiveAttributes

GE_ArcaneBolt_Cost
GE_ArcaneBolt_Damage

GE_Burn_Cost
GE_Burn_DoT

GE_EnemyObjectiveDamage
```

Three Gameplay Effect patterns are currently represented.

### Instant initialization

Initial attribute effects use Instant Gameplay Effects.

### Instant damage

Arcane Bolt applies an instant Health modifier when its projectile reaches its target.

### Duration and periodic damage

Burn applies `GE_Burn_DoT`, which remains active for a configured duration and executes periodically.

Current prototype Burn configuration:

```text
Duration = 5 seconds
Period = 1 second
Health change per period = -5
Execute on application = false
```

This represents a total target value of approximately 25 damage over the full effect duration.

## Gameplay Effect Stacking

Burn currently uses source-based stacking with a stack limit of one.

Conceptually:

```text
same caster
    ↓
reapplies Burn
    ↓
existing Burn refreshed
```

rather than:

```text
Burn instance 1
+
Burn instance 2
+
Burn instance 3
```

This behavior is configured through the Gameplay Effect rather than custom C++.

## Gameplay Ability Architecture

`UADGameplayAbility` is the native base for project abilities.

`UADGA_TargetedDamage` contains the reusable targeted-cast flow:

```text
Validate target
Validate range
Validate movement state
Capture target
Start cast
Wait
Revalidate
Commit ability
Execute payload
```

Its default payload applies the configured Gameplay Effect directly.

`UADGA_TargetProjectile` overrides only the payload delivery mechanism and spawns an `AADProjectile`.

This allows two current ability patterns:

```text
GA_ArcaneBolt
    ↓
ADGA_TargetProjectile
    ↓
Projectile
    ↓
Instant Gameplay Effect
```

and:

```text
GA_Burn
    ↓
ADGA_TargetedDamage
    ↓
Duration Gameplay Effect
    ↓
Periodic executions
```

## Arcane Bolt

Current prototype configuration:

```text
Input = 1
Cast Time = 1.5 seconds
Range = 2000 cm
Mana Cost = 20
Damage = 25
Projectile = BP_ADProjectile_ArcaneBolt
```

Flow:

```text
Cast
 ↓
Spend Mana
 ↓
Create Effect Spec
 ↓
Spawn Projectile
 ↓
Homing travel
 ↓
Impact
 ↓
Apply instant damage
```

## Burn

Current prototype configuration:

```text
Input = 2
Cast Time = 1.0 second
Range = 2000 cm
Mana Cost = 15
Effect = GE_Burn_DoT
```

Flow:

```text
Select Target
    ↓
Cast Burn
    ↓
Validate target
    ↓
Spend Mana
    ↓
Apply GE_Burn_DoT
    ↓
Active Gameplay Effect
    ↓
Periodic Health modification
    ↓
Effect expires
```

Burn does not use a projectile in its current implementation.

## Projectile System

`AADProjectile` is a reusable world Actor that carries a `FGameplayEffectSpecHandle` from a source ASC toward a captured target.

It uses `UProjectileMovementComponent` for homing movement.

The projectile does not know which specific spell created it.

World geometry can block the projectile and destroying its target safely invalidates the projectile.

## Gameplay Tags

Current native gameplay tags include:

```text
State.Casting
Ability.CancelOnMovement
```

Movement cancels only appropriately tagged abilities.

Additional effect-state tags such as Burn will be introduced only when gameplay or presentation systems need to query them.

## Targeting

`UADTargetingComponent` performs cursor-based enemy targeting, validates distance and maintains a weak target reference.

Target presentation remains Blueprint-driven.

## Casting

`UADCastComponent` represents UI-facing casting state.

Actual waiting remains inside GAS through Ability Tasks.

Both Arcane Bolt and Burn reuse this system.

## UI

`WBP_PlayerHUD` remains the root local HUD.

Current composition:

```text
WBP_PlayerHUD
└── WBP_CastBar
```

The Cast Bar automatically works with both current cast-time abilities because it listens to `UADCastComponent`, not individual spell classes.

## Enemy AI

`AADEnemyAIController` controls NavMesh navigation through `MoveToActor`.

Enemy movement destinations remain generic Actor references.

No Behavior Tree has been introduced because current enemy decision complexity does not justify one.

## Defense Objective

`AADDefenseObjective` is a GAS-enabled Actor sharing the Health Attribute Set architecture with Characters.

Enemies apply objective damage through a Gameplay Effect.

Reaching zero Health produces match defeat.

## Game Mode

`AADGameMode` owns exactly one match result:

```text
InProgress
Victory
Defeat
```

Victory and defeat presentation remain provisional.

## Wave Architecture

`FADWaveSpawnGroup` represents an ordered spawn group.

`UADWaveDataAsset` stores wave configuration.

`AADSpawnPoint` represents named level spawn locations.

`AADWaveDirector` executes waves, tracks spawned enemies, detects true wave completion, schedules subsequent waves and produces victory after the final configured wave.

## Enemy Lifetime Tracking

Wave enemies are registered when spawned.

```text
Spawn
 ↓
ActiveEnemies.Add()
 ↓
OnEnemyDefeated
 ↓
ActiveEnemies.Remove()
```

No recurring world scans are required.

An enemy killed by Burn follows exactly the same death path as an enemy killed by Arcane Bolt.

## Wave Completion

A wave completes only when:

```text
bIsSpawningWave == false

AND

ActiveEnemies.Num() == 0
```

Damage source is irrelevant to wave tracking.

## Event-Driven Architecture

Current event integration includes:

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

Periodic Burn damage itself is scheduled internally by GAS rather than by a project-level event or timer.

## Timer Policy

Project timers currently handle:

```text
Enemy objective attacks
Enemy spawning
Spawn-group delays
Inter-wave delays
```

No project timer is used for Burn.

Continuous projectile movement belongs to `UProjectileMovementComponent`.

Periodic Gameplay Effect execution belongs to GAS.

## C++ / Blueprint Boundary

C++ currently owns:

* Character hierarchy.
* Player input integration.
* GAS initialization.
* Attribute rules.
* Targeting.
* Casting lifecycle.
* Ability execution.
* Projectile transport.
* AI navigation.
* Enemy attacks.
* Objective logic.
* Wave execution.
* Enemy tracking.
* Match rules.

Blueprint and data assets currently own:

* Visual assets.
* Input Actions and mappings.
* Initial attribute values.
* Mana costs.
* Damage values.
* Cast configuration.
* Projectile visual configuration.
* Gameplay Effect duration.
* Gameplay Effect periods.
* Gameplay Effect stacking.
* Wave composition.
* UI composition.
* Prototype presentation.

## Important Technical Decisions

The player ASC remains on the Character because multiplayer, respawn persistence and character switching are outside the current requirements.

The Defense Objective uses GAS instead of implementing a second Health system.

Experience and Level remain outside GAS until gameplay requirements justify effect-driven modification.

The Cast Component describes presentation while Gameplay Abilities remain authoritative over execution.

Target relationships use weak references when ownership is not implied.

Wave definitions do not directly reference level Actors.

Enemy death and Actor destruction remain separate concepts.

Projectiles transport already-built Gameplay Effect Specs.

Periodic damage uses GAS rather than manual timers.

Burn stacking behavior is configured in the Gameplay Effect rather than hardcoded into its ability.

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
```

## Current Prototype

The current prototype can:

1. Start a complete wave sequence.
2. Spawn enemies from Data Assets.
3. Navigate enemies toward the objective.
4. Select enemy targets.
5. Cast Arcane Bolt.
6. Launch a homing projectile.
7. Damage enemies when projectiles impact.
8. Cast Burn.
9. Apply duration-based periodic damage.
10. Kill enemies through instant or periodic effects.
11. Interrupt casts through movement.
12. Spend Mana through Gameplay Effects.
13. Track living enemies.
14. Complete waves automatically.
15. Damage the Defense Objective.
16. Produce defeat.
17. Produce victory.

## Current Limitations

The prototype does not yet include:

* Area-of-effect player abilities.
* Crowd control.
* Ground-targeted abilities.
* Cooldowns.
* Full ability-bar UI.
* Gameplay Cue presentation for Burn.
* Final enemy archetypes.
* Traps.
* Experience.
* Levels.
* Talents.
* Final five-wave balance.
* Behavior Trees.
* Final visual/audio presentation.
* Automated testing.
* Profiling.

## Next Milestone — Day 13

Day 13 introduces the first area crowd-control spell: **Frost Nova**.

Target behavior:

```text
Press Frost Nova
    ↓
Area around player
    ↓
Find nearby enemies
    ↓
Apply crowd-control effect
    ↓
Enemies temporarily stop or become rooted
    ↓
Effect expires
    ↓
Navigation resumes
```

This will introduce our first meaningful gameplay state effect and begin using tags such as:

```text
State.Rooted
Effect.Root
```

rather than implementing crowd control through ad-hoc booleans.

## Roadmap

Week 1 — Foundation and Targeting: **Completed**

Week 2 — Casting and Wave Defense: **Completed**

Week 3 — Abilities and Effects: **In Progress**

Current Week 3 progress:

```text
Day 11 — Targeted Projectile   Completed
Day 12 — Damage Over Time      Completed
Day 13 — Frost Nova            Next
Day 14 — Meteor
Day 15 — Cooldowns and Ability UI
```

Week 4 covers traps.

Week 5 covers progression and talents.

Week 6 covers final enemies and wave content.

Week 7 covers testing and profiling.

Week 8 covers polish, packaging and portfolio presentation.

## Out of Scope

The initial vertical slice excludes multiplayer, matchmaking, backend services, inventory, equipment, loot, crafting, shops, campaign, multiple maps, multiple playable classes, complex talent trees, and original modelling/animation production.

## Definition of Done

The vertical slice is complete when it contains a packaged 8–12 minute match, four spells, two traps, three enemy types, five waves, progression, talents, victory/defeat, functional UI, automated tests, documented profiling and optimization, technical documentation, and a short gameplay presentation video.

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
