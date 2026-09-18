# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project demonstrates Gameplay Programmer and Systems Programmer skills through a wave-defense experience combining spell combat, target selection, ground targeting, Gameplay Ability System, enemy AI, objective defense, data-driven waves, player economy, traps, progression, UI, testing, and profiling.

## Project Goal

The target experience is an 8–12 minute wave-defense match where the player protects a central objective against progressively harder enemy waves.

The final vertical slice is planned to contain four player spells, two placeable traps, three enemy archetypes, five waves, a construction economy, in-match progression, talents, victory and defeat conditions, functional UI, automated testing, profiling, and a packaged playable build.

## Development Philosophy

Core gameplay rules are implemented in C++.

Blueprint and Data Assets primarily own presentation, asset composition, tuning and content configuration.

Systems are separated according to responsibility rather than according to individual content items.

New abstractions are introduced only after concrete gameplay requirements justify them.

## Source Structure

```text
Source/
└── ArcaneDefense/
    ├── AbilitySystem/
    │   ├── ADAttributeSet.h
    │   ├── ADAttributeSet.cpp
    │   ├── ADPlayerResourceAttributeSet.h
    │   ├── ADPlayerResourceAttributeSet.cpp
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
    ├── Traps/
    │   ├── ADTrapBase.h
    │   ├── ADTrapBase.cpp
    │   └── ADTrapDataAsset.h
    │
    └── Waves/
        ├── ADWaveTypes.h
        ├── ADWaveDataAsset.h
        ├── ADSpawnPoint.h
        ├── ADSpawnPoint.cpp
        ├── ADWaveDirector.h
        └── ADWaveDirector.cpp
```

## Gameplay Ability System

The player and enemies use Unreal Engine's `UAbilitySystemComponent`.

The Defense Objective also owns an ASC.

The player still owns one Ability System Component. Player-only economic resources are represented by an additional Attribute Set rather than by another ASC.

## Combat Attributes

`UADAttributeSet` contains combat attributes shared between applicable gameplay Actors:

```text
Health
MaxHealth
Mana
MaxMana
```

It handles validation and out-of-health events.

## Player Resource Attributes

`UADPlayerResourceAttributeSet` contains resources exclusive to the player:

```text
Coins
```

Coins represent the construction economy used to buy traps.

They are deliberately separated from `UADAttributeSet` because enemies and the Defense Objective do not participate in the player economy.

The player's ASC therefore conceptually contains:

```text
AbilitySystemComponent
├── UADAttributeSet
│     Health
│     MaxHealth
│     Mana
│     MaxMana
│
└── UADPlayerResourceAttributeSet
      Coins
```

Coins are prevented from becoming negative.

Initial resources are currently configured through:

```text
GE_InitializePlayerResources
```

with a provisional starting value of 500 Coins.

Future Gameplay Effects will modify Coins for enemy rewards, trap purchases and trap sales.

## Player Ability Kit

The current combat kit is:

```text
1 → Arcane Bolt
2 → Burn
3 → Frost Nova
4 → Meteor
```

Arcane Bolt demonstrates targeted projectile delivery.

Burn demonstrates periodic Gameplay Effects.

Frost Nova demonstrates radial crowd control through Gameplay Tags.

Meteor demonstrates ground-targeted area damage.

All four use GAS costs and independent GAS cooldowns.

## Targeting

`UADTargetingComponent` owns enemy Actor selection.

`UADGroundTargetingComponent` owns interactive world-location selection.

The two systems remain separate because actor targeting and spatial placement have different responsibilities.

## Casting

`UADCastComponent` exposes presentation state for cast-time abilities.

Actual ability execution and timing remain controlled by Gameplay Abilities and Gameplay Ability Tasks.

## Enemy AI

`AADEnemyAIController` handles navigation toward an assigned target Actor.

Temporary states such as `State.Rooted` can stop enemy movement and navigation resumes when the state expires.

## Defense Objective

`AADDefenseObjective` uses the shared GAS Health architecture.

Enemies periodically damage it.

Reaching zero Health produces match defeat.

## Wave Architecture

`UADWaveDataAsset` stores ordered spawn-group definitions.

`AADSpawnPoint` represents named locations in the level.

`AADWaveDirector` executes configured waves, tracks living enemies, distinguishes spawning completion from actual wave completion, progresses through multiple waves and triggers victory after the final one.

## Player Economy

The player has an in-match construction resource represented by the GAS `Coins` Attribute.

Economy rules are:

```text
Match start
    → initial Coins

Enemy defeated
    → future Coin reward

Trap placed
    → future Coin cost

Trap sold
    → future Coin refund
```

Trap placement will be allowed throughout the active match when the player can afford the trap and the world position is valid.

Trap selling will be restricted to the inter-wave state.

The actual price paid for a trap is preserved on the runtime trap Actor so future talents or modifiers cannot produce incorrect sale refunds.

## Trap Data Architecture

`UADTrapDataAsset` is the data-driven description of a trap.

It currently contains:

```text
Display Name
Icon
Trap Actor Class
Placement Preview Class

Construction Cost
Sell Refund Ratio

Trigger Box Extent
Placement Box Extent

Can Place On Floor
Can Place On Wall

Maximum Floor Slope
Surface Height Tolerance
```

The Data Asset contains configuration only.

It does not implement placement or runtime effects.

## Runtime Trap Architecture

`AADTrapBase` is the shared base Actor for placed traps.

Current responsibilities are intentionally limited:

```text
Own TrapData
Remember source AbilitySystemComponent
Remember actual PurchasePrice
Own enemy TriggerVolume
Detect valid enemy entry
Detect valid enemy exit
Expose currently overlapping enemies
```

`AADTrapBase` has no custom Tick.

It has no generic cooldown or rearm system.

It does not own an Ability System Component.

These are deliberate architectural decisions.

## Trap GAS Ownership

Placing a trap does not create another Ability System.

Instead:

```text
Player ASC
    ↓
places trap
    ↓
AADTrapBase stores weak Source ASC reference
```

Concrete trap effects can later use the player's ASC as the Gameplay Effect source when interacting with enemy ASCs.

This preserves player attribution while avoiding an unnecessary ASC on every world trap.

## Trap Detection

`AADTrapBase` owns a `UBoxComponent` configured for Pawn overlaps.

Runtime enemy detection uses overlap events and filters `AADEnemyCharacter`.

Only the Character Capsule is treated as the authoritative overlap component, preventing multiple components on the same enemy from generating duplicate logical entries.

The base exposes separate extension points for enemy entry and exit.

This allows future trap implementations to have different semantics.

For example:

```text
Spike Trap
    Enemy enters
        → damage

Slow Trap
    Enemy enters
        → apply slow
    Enemy exits
        → remove/expire slow
```

No assumption is made that all traps must share the same trigger lifecycle.

## Trap Sale Model

Every placed trap can store the exact `PurchasePrice` used when it was built.

Its expected refund is:

```text
PurchasePrice × SellRefundRatio
```

rather than:

```text
TrapData.ConstructionCost × SellRefundRatio
```

This allows future cost modifiers and talents without creating an economy exploit.

## Placement Requirements

Actual trap placement is not implemented yet.

The next placement system will validate:

```text
Explicitly buildable surface
Floor/wall compatibility
Surface slope
Surface flatness across the footprint
Available physical space
No blocking obstacle
Player affordability
```

A placement preview will communicate validity visually:

```text
Green → valid
Red   → invalid
```

C++ will determine validity.

Blueprint will determine presentation.

## Timer and Tick Policy

GAS owns spell cooldowns, persistent status durations and periodic effects.

Gameplay timers are used only where a scheduled world behavior actually requires one.

`AADTrapBase` currently uses neither Tick nor a cooldown timer.

This may change in concrete trap subclasses only if their actual design requires timed behavior.

## C++ / Blueprint Boundary

C++ owns gameplay rules, economy state, placement validity, trap detection, spell behavior, AI, waves and match rules.

Blueprint and Data Assets own trap meshes, placement-preview visuals, materials, VFX, UI and configurable balance values.

## Development Status

```text
Day 1  — Project Setup                         Completed
Day 2  — Player Character                      Completed
Day 3  — Ability System and Attributes         Completed
Day 4  — Target Selection                      Completed
Day 5  — First Targeted Ability                Completed
Day 6  — Spell Casting                         Completed
Day 7  — Enemy Navigation                      Completed
Day 8  — Defense Objective                     Completed
Day 9  — Data-Driven Wave Spawning             Completed
Day 10 — Wave Completion and Victory           Completed
Day 11 — Reusable Homing Projectile            Completed
Day 12 — Periodic Burn Gameplay Effect         Completed
Day 13 — Frost Nova / Root Crowd Control       Completed
Day 14 — Meteor / Ground Targeting             Completed
Day 15 — Cooldowns / Combat Ability UI         Completed
Day 16 — Player Economy + Trap Foundation      Completed
```

## Current Prototype

The project currently supports a complete four-spell combat kit, data-driven waves, enemy AI, objective defense, victory and defeat, and the foundational architecture for the construction economy and runtime traps.

## Next Milestone — Day 17

Day 17 implements trap placement.

Target flow:

```text
Select Trap
    ↓
Enter Placement Mode
    ↓
Spawn Preview
    ↓
Trace under cursor
    ↓
Validate explicitly buildable surface
    ↓
Validate floor/wall compatibility
    ↓
Sample surface flatness
    ↓
Validate PlacementBoxExtent
    ↓
Check Coins
    ↓
Preview Green / Red
    ↓
Left Click
    ↓
Valid?
    ├── no → remain in placement
    └── yes
          ↓
       pay Coins
          ↓
       spawn TrapActorClass
          ↓
       InitializeTrap(
           TrapData,
           Player ASC,
           ActualPricePaid
       )
```

Trap construction will be possible during active waves as well as between waves.

Selling will be implemented later and permitted only during the inter-wave state.

## Remaining Roadmap

```text
Day 17 — Trap Placement + Green/Red Validation
Day 18 — Spike Trap
Day 19 — Slow Trap
Day 20 — Coin Rewards, Purchase/Sale Flow, Intermission Economy

Days 21–25 — Experience, Levels and Talents
Days 26–30 — Final Enemies, Five Waves and Refactor
Days 31–35 — Tests, Profiling and Optimization
Days 36–40 — Polish, Packaging and Portfolio
```

## Definition of Done

The final vertical slice requires a packaged 8–12 minute match, four player spells, two placeable traps, three enemy archetypes, five waves, construction economy, progression, talents, victory/defeat, functional UI, automated tests, documented profiling and optimization, technical documentation and portfolio material.

## License

This project is intended for educational and portfolio purposes.

Third-party assets remain subject to their respective licenses.
