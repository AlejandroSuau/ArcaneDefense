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
    → attribute/state changes

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
    │   └── WBP_CastBar
    │
    ├── Waves/
    │   ├── DA_Wave_01
    │   ├── DA_Wave_02
    │   └── DA_Wave_03
    │
    ├── Maps/
    │   └── L_Prototype
    ├── BP_ADDefenseObjective
    ├── BP_ADGameMode
    └── BP_ADPlayerController
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

`AADPlayerCharacter` owns player input integration and the local gameplay components required for targeting and casting.

Current ability inputs:

```text
1 → Arcane Bolt
2 → Burn
3 → Frost Nova
4 → Meteor
```

## Targeting Architecture

The project now explicitly distinguishes two targeting concepts.

### Actor Targeting

`UADTargetingComponent` answers:

```text
Which enemy is selected?
```

It maintains a weak `AADEnemyCharacter` reference and is used by Arcane Bolt and Burn.

### Ground Targeting

`UADGroundTargetingComponent` answers:

```text
Which world location has the player selected?
```

It performs cursor collision queries against ground-compatible world geometry, maintains a live preview, validates range and surface slope, and stores a confirmed location for a ground-targeted ability.

The component only ticks while targeting is active.

Ground targeting does not activate abilities itself.

## Gameplay Ability Architecture

Current native hierarchy:

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

Each specialization represents a meaningfully different delivery model rather than one ability-specific class.

## Arcane Bolt

Arcane Bolt is a cast-time targeted homing projectile.

```text
Input        = 1
Cast Time    = 1.5 s
Range        = 2000 cm
Mana Cost    = 20
Damage       = 25
```

Damage occurs on projectile impact.

## Burn

Burn is a targeted duration effect.

```text
Input        = 2
Cast Time    = 1.0 s
Range        = 2000 cm
Mana Cost    = 15
Duration     = 5 s
Period       = 1 s
```

Periodic execution belongs to GAS rather than a project timer.

## Frost Nova

Frost Nova is an instant radial crowd-control effect centered on the player.

```text
Input         = 3
Radius        = 600 cm
Mana Cost     = 25
Root Duration = 3 s
```

It applies `State.Rooted` through a duration Gameplay Effect.

Enemy movement reacts to that Gameplay Tag rather than an independent rooted boolean.

## Meteor

Meteor is the first ground-targeted ability.

Current prototype configuration:

```text
Input           = 4
Targeting Range = 2500 cm
Effect Radius   = 400 cm
Cast Time       = 2.0 s
Mana Cost       = 30
Damage          = 40
```

Flow:

```text
Press 4
    ↓
Ground targeting mode
    ↓
Preview under cursor
    ↓
Confirm location
    ↓
Activate GA_Meteor
    ↓
Cast
    ↓
Commit ability
    ↓
Sphere overlap at target location
    ↓
Apply GE_Meteor_Damage
```

Target selection and ability activation are deliberately separate phases.

Movement is permitted while choosing the target location.

Movement during the actual cast cancels Meteor.

## Ground Preview

`BP_ADGroundTargetPreview` is presentation-only.

It uses a decal to communicate the area selected by Meteor.

Gameplay validity remains entirely inside `UADGroundTargetingComponent`.

The preview actor has no gameplay collision.

## Gameplay Ability System

Characters own standard `UAbilitySystemComponent` instances directly.

The Defense Objective owns its own ASC.

A custom ASC remains unnecessary at the current stage.

## Attributes

`UADAttributeSet` currently contains Health, MaxHealth, Mana and MaxMana.

It validates attributes and broadcasts `OnOutOfHealth`.

Attribute logic does not depend on the damage source.

Arcane Bolt, Burn and Meteor therefore share the same death pipeline.

## Gameplay Tags

Current native tags include:

```text
State.Casting
State.Rooted

Ability.CancelOnMovement

Effect.Root
```

Gameplay Tags represent meaningful gameplay state rather than duplicated booleans.

## Projectile System

`AADProjectile` transports a previously created Gameplay Effect Spec toward a captured target using `UProjectileMovementComponent`.

The projectile is not Arcane-Bolt-specific.

## Casting System

`UADCastComponent` exposes cast presentation state.

Actual cast execution uses GAS Ability Tasks.

Arcane Bolt, Burn and Meteor use the same cast-bar infrastructure.

Frost Nova is instant.

## Enemy AI and Crowd Control

`AADEnemyAIController` owns enemy navigation.

`State.Rooted` temporarily stops navigation and Character Movement.

When the effect expires, navigation resumes.

Root does not prevent an enemy already attacking the objective from continuing its attack.

No Behavior Tree exists yet because current behavior remains sufficiently linear.

## Defense Objective

`AADDefenseObjective` uses the shared GAS attribute architecture and triggers defeat when Health reaches zero.

Enemies periodically apply objective damage through Gameplay Effects.

## Wave Architecture

`UADWaveDataAsset` contains data-driven spawn definitions.

`AADSpawnPoint` represents named level locations.

`AADWaveDirector` executes waves, tracks living enemies, detects real wave completion, progresses between waves and triggers victory after the final configured wave.

Enemy deaths are independent of which player ability caused them.

## Match Rules

`AADGameMode` stores one result:

```text
InProgress
Victory
Defeat
```

Defeat stops pending wave execution.

Completing the final wave produces victory.

## Event-Driven Architecture

Important events currently include:

```text
AttributeSet
    → OnOutOfHealth

ASC
    → State.Rooted changed

Enemy
    → OnEnemyDefeated

TargetingComponent
    → OnTargetChanged

GroundTargetingComponent
    → OnTargetConfirmed
    → OnTargetCancelled

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

## Tick and Timer Policy

Gameplay timers are used for discrete scheduled actions such as wave spawning and objective attacks.

Periodic Burn execution belongs to GAS.

Root lifetime belongs to GAS.

Projectile movement belongs to `UProjectileMovementComponent`.

Ground-target preview is one of the few systems intentionally allowed to tick because its world location must follow continuously changing cursor input. The tick is enabled only while targeting.

## C++ / Blueprint Boundary

C++ owns gameplay rules, validation, targeting state, ability execution, collision queries, AI behavior, waves and match rules.

Blueprint/Data Assets own presentation and configurable content including meshes, animations, materials, VFX hooks, spell balance, Gameplay Effects, wave definitions and UI.

Ground-target validity is C++ gameplay logic.

The ground-target decal is Blueprint presentation.

## Important Technical Decisions

Actor targeting and location targeting are separate systems.

Ground targeting resolves a position before the corresponding Gameplay Ability activates, allowing `State.Casting` to represent the actual cast phase rather than the preview phase.

Confirmed locations are revalidated by the Gameplay Ability before execution.

Gameplay abilities remain responsible for cost and effect execution.

Ground previews never apply gameplay effects.

AoE damage uses overlap queries rather than world-wide Actor scans.

Meteor currently resolves gameplay damage immediately when its cast finishes. A falling meteor visual can be added as presentation without forcing a gameplay projectile architecture unless world interaction requires one.

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
```

## Current Prototype

The project now supports four distinct spell-delivery models:

```text
Arcane Bolt
    → targeted projectile

Burn
    → targeted periodic effect

Frost Nova
    → caster-centered AoE state effect

Meteor
    → ground-targeted AoE damage
```

The prototype also supports data-driven waves, enemy navigation, defense-objective attacks, wave progression, victory and defeat.

## Current Limitations

The project does not yet contain cooldowns, a complete ability bar, final VFX/audio, final enemy archetypes, traps, progression, talents, five final balanced waves, automated tests or profiling.

## Next Milestone — Day 15

Day 15 closes the current ability milestone by introducing:

```text
Cooldowns
Ability UI
Mana feedback
Ability availability
```

The objective will be to stop treating keys `1–4` as invisible mechanics and display the actual combat kit to the player.

The UI should be driven by GAS/gameplay state rather than maintaining independent cooldown timers.

After Day 15, the project moves into the trap system.

## Roadmap

Week 1 — Foundation and targeting: Completed.

Week 2 — Casting and wave defense: Completed.

Week 3 — Abilities and effects: In progress.

```text
Day 11 — Targeted Projectile   Completed
Day 12 — Damage Over Time      Completed
Day 13 — Frost Nova / Root     Completed
Day 14 — Meteor                Completed
Day 15 — Cooldowns + UI        Next
```

Week 4 covers traps.

Week 5 covers progression and talents.

Week 6 covers final enemies and waves.

Week 7 covers testing and profiling.

Week 8 covers polish, packaging and portfolio presentation.

## Out of Scope

The initial vertical slice excludes multiplayer, matchmaking, backend services, inventory, equipment, loot, crafting, shops, campaign, multiple maps, multiple playable classes, complex talent trees, and original character modelling/animation production.

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
