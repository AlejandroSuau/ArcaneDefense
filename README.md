# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project demonstrates Gameplay Programmer and Systems Programmer skills through a wave-defense experience combining target-based spell combat, Gameplay Ability System, crowd control, enemy AI, data-driven waves, progression, UI, testing, and profiling.

## Project Goal

The target experience is an 8–12 minute match where the player protects a central objective against progressively harder enemy waves.

The planned vertical slice includes four player abilities, two placeable traps, three enemy archetypes, five waves, in-match progression, talents, victory and defeat conditions, functional UI, automated testing, profiling, and a packaged playable build.

## Technology

The project currently uses Unreal Engine 5.8, C++, Enhanced Input, Gameplay Ability System, Gameplay Attributes, Gameplay Effects, Gameplay Tags, Gameplay Effect Components, Gameplay Ability Tasks, Projectile Movement, Navigation, AI Controllers, UMG, Data Assets, gameplay timers, delegates, Git, and Git LFS.

## Development Principles

Core gameplay rules belong in C++.

Blueprint and Data Assets primarily own configuration, asset composition, visuals, UI, animation and balance values.

Gameplay state should have a single authoritative representation. GAS Gameplay Tags are used when a temporary Gameplay Effect creates a meaningful state such as `State.Rooted`.

Systems remain narrowly focused:

```text
ADTargetingComponent
    → Current selected target

ADCastComponent
    → Cast presentation

GameplayAbility
    → Validation and execution

GameplayEffect
    → Attribute or persistent-state change

ADProjectile
    → Effect transport through the world

ADEnemyAIController
    → Navigation

ADDefenseObjective
    → Defended-object behavior

ADWaveDataAsset
    → Wave configuration

ADWaveDirector
    → Wave lifecycle

ADGameMode
    → Match result
```

Abstractions are introduced only when a concrete gameplay requirement requires them.

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
    │       └── ADGA_RadialEffect.cpp
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
    │   └── Projectiles/
    │       └── BP_ADProjectile_ArcaneBolt
    │
    ├── Effects/
    │   ├── Attributes/
    │   │   ├── GE_InitializePlayerAttributes
    │   │   ├── GE_InitializeEnemyDummyAttributes
    │   │   └── GE_InitializeDefenseObjectiveAttributes
    │   ├── Costs/
    │   │   ├── GE_ArcaneBolt_Cost
    │   │   ├── GE_Burn_Cost
    │   │   └── GE_FrostNova_Cost
    │   ├── Damage/
    │   │   ├── GE_ArcaneBolt_Damage
    │   │   ├── GE_Burn_DoT
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

`AADCharacterBase` is shared by the player and enemy Characters. It owns the ASC and shared Attribute Set, initializes GAS, applies initial attributes and implements generic death behavior.

```text
ACharacter
    ↓
AADCharacterBase
    ├── AADPlayerCharacter
    └── AADEnemyCharacter
```

`AADPlayerCharacter` owns movement, camera, input integration and activation of the currently configured ability slots.

Current inputs:

```text
1 → GA_ArcaneBolt
2 → GA_Burn
3 → GA_FrostNova
```

`AADEnemyCharacter` owns enemy-specific navigation, objective attacks, selection presentation, defeat notification and reactions to gameplay states such as `State.Rooted`.

## Gameplay Ability System

Characters currently own standard `UAbilitySystemComponent` instances directly.

The Defense Objective owns its own ASC.

A custom ASC is intentionally deferred until project-specific ASC behavior warrants one.

## Attributes

`UADAttributeSet` contains:

```text
Health
MaxHealth
Mana
MaxMana
```

It validates values and broadcasts `OnOutOfHealth`.

The Attribute Set does not determine the meaning of death.

## Gameplay Tags

Current native tags include:

```text
State.Casting
State.Rooted

Ability.CancelOnMovement

Effect.Root
```

`State.Casting` represents an active cast.

`State.Rooted` represents inability to move.

`Effect.Root` identifies effects belonging to the root category but is not itself granted as the target's state.

Gameplay Effects are used to own the lifetime of temporary state tags whenever practical.

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

GE_FrostNova_Cost
GE_FrostNova_Root

GE_EnemyObjectiveDamage
```

The project currently demonstrates instant effects, duration effects, periodic effects and status effects that grant Gameplay Tags.

## Gameplay Ability Architecture

`UADGameplayAbility` is the native base for project abilities.

It now also provides common Character-level validation such as `IsAvatarGrounded()`.

`UADGA_TargetedDamage` owns targeted cast behavior and executes a configurable payload against one selected enemy.

`UADGA_TargetProjectile` specializes targeted payload delivery through a world projectile.

`UADGA_RadialEffect` implements immediate radial application of a Gameplay Effect to valid nearby enemies.

Current specialization:

```text
UADGameplayAbility
    ├── UADGA_TargetedDamage
    │       ├── GA_Burn
    │       └── UADGA_TargetProjectile
    │               └── GA_ArcaneBolt
    │
    └── UADGA_RadialEffect
            └── GA_FrostNova
```

## Arcane Bolt

Arcane Bolt is a cast-time targeted projectile.

```text
Input = 1
Cast Time = 1.5 seconds
Range = 2000 cm
Mana Cost = 20
Damage = 25
```

Its damage occurs on projectile impact rather than cast completion.

## Burn

Burn is a targeted cast that applies a duration-based periodic Gameplay Effect.

```text
Input = 2
Cast Time = 1 second
Range = 2000 cm
Mana Cost = 15
Duration = 5 seconds
Period = 1 second
```

No project-level timer is used for its periodic damage.

## Frost Nova

Frost Nova is an instant radial crowd-control ability.

```text
Input = 3
Radius = 600 cm
Mana Cost = 25
Root Duration = 3 seconds
```

It does not require a selected target.

Flow:

```text
Activate Frost Nova
      ↓
Sphere overlap around player
      ↓
Valid nearby enemies
      ↓
GE_FrostNova_Root
      ↓
State.Rooted
      ↓
Enemy movement disabled
```

When the Gameplay Effect expires:

```text
State.Rooted removed
      ↓
Enemy movement restored
      ↓
Navigation resumes
```

Root affects movement only. It deliberately does not prevent an enemy already in attack range from attacking the Defense Objective.

## Root State Architecture

The enemy does not store a separate `bIsRooted` gameplay variable.

Instead:

```text
GE_FrostNova_Root
      ↓
State.Rooted
      ↓
ASC tag-change event
      ↓
AADEnemyCharacter reaction
```

This keeps GAS as the authoritative state source.

`AADEnemyCharacter` listens for `State.Rooted` changes through its Ability System Component.

On root:

```text
Stop AI movement
Stop current velocity
Disable Character Movement
```

On removal:

```text
Restore Walking
Resume AI navigation if required
```

Dead enemies never resume movement when a previous root effect expires.

## Projectile System

`AADProjectile` carries a Gameplay Effect Spec from a source ASC toward a captured target.

Homing movement uses `UProjectileMovementComponent`.

The projectile is independent of Arcane Bolt-specific damage configuration.

## Targeting

`UADTargetingComponent` maintains cursor-based target selection and weak target references.

Targeted abilities use it; radial abilities such as Frost Nova do not.

## Casting

`UADCastComponent` contains UI-facing cast state.

Actual ability timing is handled through GAS Ability Tasks.

Instant abilities such as Frost Nova bypass the casting system.

## Enemy AI

`AADEnemyAIController` handles navigation.

`MoveToActor()` results now correctly treat `AlreadyAtGoal` as satisfying the destination, which is important when movement resumes after temporary crowd control.

No Behavior Tree exists yet because enemy behavior remains sufficiently linear.

## Defense Objective

`AADDefenseObjective` uses GAS Health and broadcasts defeat when Health reaches zero.

Enemies attack the objective periodically through Gameplay Effects.

## Game Mode

`AADGameMode` stores one match result:

```text
InProgress
Victory
Defeat
```

## Wave System

`UADWaveDataAsset` stores data-driven spawn groups.

`AADSpawnPoint` defines named locations.

`AADWaveDirector` spawns enemies, tracks their lifetime, detects true wave completion, starts subsequent waves and resolves victory/defeat interaction.

A wave is complete only when:

```text
bIsSpawningWave == false
AND
ActiveEnemies.Num() == 0
```

Enemy death works identically whether caused by Arcane Bolt, Burn or future damage abilities.

## Event-Driven Architecture

Important gameplay events currently include:

```text
AttributeSet
    → OnOutOfHealth

AbilitySystemComponent
    → State.Rooted tag changed

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

## Timer Policy

Project timers handle discrete scheduled gameplay such as enemy attacks and wave spawning.

Burn periodic execution belongs to GAS.

Root duration belongs to GAS.

Projectile continuous movement belongs to `UProjectileMovementComponent`.

No custom Tick or timer is required for Frost Nova.

## C++ / Blueprint Boundary

C++ owns gameplay rules, state reactions, targeting, ability execution, movement behavior, AI navigation, objective logic, wave logic and match rules.

Blueprint/Data Assets own configurable spell values, Gameplay Effect lifetime/stacking, meshes, materials, VFX hooks, input assets, widgets and wave composition.

## Important Technical Decisions

The player ASC remains on the Character because persistent Pawn replacement and multiplayer are outside current requirements.

The Defense Objective shares GAS health infrastructure rather than having a separate health implementation.

Gameplay Tags represent temporary states when those states originate from Gameplay Effects.

State lifetime is owned by the effect that created it.

Root is intentionally separate from stun semantics.

Radial abilities use collision queries rather than scanning every enemy in the world.

No custom ASC, Behavior Tree or generic status-component framework has been introduced yet.

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
```

## Current Prototype

The prototype can run a data-driven wave sequence, navigate enemies, select targets, cast Arcane Bolt, launch homing projectiles, apply Burn damage over time, use Frost Nova to root multiple nearby enemies, kill enemies through multiple damage sources, track wave completion, damage the Defense Objective and resolve victory or defeat.

## Current Limitations

The prototype does not yet include a ground-targeting system, Meteor, cooldowns, final ability UI, Gameplay Cues, final enemy archetypes, traps, progression, talents, final five-wave balance, automated tests or profiling.

## Next Milestone — Day 14

Day 14 introduces **Meteor**, our first ground-targeted spell.

Target flow:

```text
Press 4
    ↓
Enter targeting mode
    ↓
Cursor traces against ground
    ↓
Ground preview
    ↓
Confirm location
    ↓
Cast
    ↓
Meteor impact
    ↓
Area damage
```

This will require a new concept that the previous three abilities do not have:

```text
Target Actor
    ≠
Target Location
```

We will therefore design ground targeting separately rather than forcing it through `UADTargetingComponent`, whose current responsibility is enemy selection.

## Roadmap

Week 1 — Foundation and Targeting: Completed.

Week 2 — Casting and Wave Defense: Completed.

Week 3 — Abilities and Effects: In progress.

```text
Day 11 — Targeted Projectile     Completed
Day 12 — Damage Over Time        Completed
Day 13 — Frost Nova / Root       Completed
Day 14 — Meteor                  Next
Day 15 — Cooldowns + Ability UI
```

Week 4 covers traps.

Week 5 covers progression and talents.

Week 6 covers final enemies and waves.

Week 7 covers testing and profiling.

Week 8 covers polish, packaging and portfolio material.

## Out of Scope

The vertical slice excludes multiplayer, matchmaking, backend services, inventory, equipment, loot, crafting, shops, campaign, multiple maps, multiple playable classes, complex talent trees, and original character modelling/animation production.

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
