# Arcane Defense

**Arcane Defense** is a technical gameplay vertical slice developed primarily in C++ using **Unreal Engine 5.8**.

The project is designed as a Gameplay / Systems Programmer portfolio piece and combines target-based spell combat, Unreal's Gameplay Ability System, enemy AI, objective defense, data-driven waves, construction economy, manually authored trap placement, progression, testing and profiling.

## Project Goal

The target experience is an 8–12 minute wave-defense match where the player protects a central objective against progressively harder enemy waves.

The final vertical slice is planned to contain:

* Four player spells.
* Two placeable traps.
* Three enemy archetypes.
* Five final waves.
* A trap construction economy.
* Experience and in-match levels.
* Talent choices.
* Victory and defeat conditions.
* Functional combat and economy UI.
* Automated gameplay tests.
* Unreal Insights profiling.
* A packaged playable build.
* Technical documentation and portfolio material.

## Development Philosophy

Core gameplay rules are implemented in C++.

Blueprints and Data Assets primarily own presentation, asset composition, VFX, UI and configurable gameplay values.

Systems are separated by responsibility instead of by individual content item.

Abstractions are introduced only when the current gameplay requirements justify them.

## Current Source Structure

```text id="uf39es"
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
    │   ├── ADTrapCollision.h
    │   ├── ADTrapDataAsset.h
    │   ├── ADTrapPlacementComponent.h
    │   ├── ADTrapPlacementComponent.cpp
    │   ├── ADTrapPlacementPreview.h
    │   ├── ADTrapPlacementPreview.cpp
    │   ├── ADTrapPlacementSlot.h
    │   └── ADTrapPlacementSlot.cpp
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

The project uses Unreal Engine's Gameplay Ability System for combat state, resource state, abilities, costs, cooldowns, damage and temporary effects.

The player owns a single `UAbilitySystemComponent`.

Multiple Attribute Sets separate different responsibilities instead of introducing additional Ability System Components.

## Combat Attributes

`UADAttributeSet` currently contains:

```text id="o7y26o"
Health
MaxHealth
Mana
MaxMana
```

It is shared by applicable gameplay Actors and handles Health/Mana validation and out-of-health behavior.

## Player Resource Attributes

`UADPlayerResourceAttributeSet` contains player-specific economic resources:

```text id="ca25kf"
Coins
```

Coins are intentionally separated from the combat Attribute Set because enemies and the Defense Objective do not participate in the player's construction economy.

The player Ability System therefore conceptually contains:

```text id="nijynl"
AbilitySystemComponent
├── UADAttributeSet
│   ├── Health
│   ├── MaxHealth
│   ├── Mana
│   └── MaxMana
│
└── UADPlayerResourceAttributeSet
    └── Coins
```

Initial Coins are configured through:

```text id="wm6rh7"
GE_InitializePlayerResources
```

Coin transactions use:

```text id="zp7c5u"
GE_ModifyCoins
```

with the SetByCaller Gameplay Tag:

```text id="l9pwbt"
Data.Resource.Coins
```

The same Gameplay Effect can therefore support:

```text id="ovl65u"
Trap purchase
    → negative magnitude

Trap sale
    → positive magnitude

Enemy reward
    → positive magnitude
```

The player exposes resource operations such as:

```text id="9pwmnr"
CanAffordCoins()
SpendCoins()
AddCoins()
```

while GAS remains responsible for modifying the actual Attribute.

## Player Ability Kit

The current player combat kit is:

```text id="a92uwk"
1 → Arcane Bolt
2 → Burn
3 → Frost Nova
4 → Meteor
```

### Arcane Bolt

Targeted cast-time spell delivered through a reusable homing projectile.

### Burn

Targeted duration Gameplay Effect that periodically damages the target.

### Frost Nova

Instant radial crowd-control ability that applies `State.Rooted`.

### Meteor

Ground-targeted cast-time AoE spell using an interactive world-location targeting mode.

All four abilities currently use independent GAS costs and cooldowns.

## Targeting Architecture

Actor targeting and ground targeting are deliberately separate systems.

```text id="6vkeh0"
UADTargetingComponent
    → selected enemy Actor

UADGroundTargetingComponent
    → selected world location
```

This prevents enemy selection, ground-targeted spells and trap placement from becoming one monolithic targeting system.

## Casting

`UADCastComponent` exposes presentation state for cast-time abilities.

Gameplay Ability Tasks remain responsible for actual cast timing.

Movement can interrupt abilities tagged for movement cancellation.

Cost and cooldown are committed only when a cast successfully completes.

## Projectile Architecture

`AADProjectile` provides reusable homing projectile behavior.

The projectile transports a prepared Gameplay Effect Spec from the player's Ability System to the target's Ability System.

Projectile visuals and impact presentation remain Blueprint responsibilities.

## Enemy Architecture

`AADEnemyCharacter` derives from the shared character base and participates in GAS combat.

`AADEnemyAIController` owns navigation toward the Defense Objective.

Temporary GAS states can affect navigation.

For example:

```text id="llggir"
State.Rooted
    ↓
stop movement
    ↓
effect expires
    ↓
resume movement toward objective
```

## Defense Objective

`AADDefenseObjective` owns its own Ability System Component and uses the shared Health Attribute architecture.

Enemies reaching the objective periodically apply damage through Gameplay Effects.

Zero Health produces match defeat.

## Wave Architecture

Wave responsibilities are separated into:

```text id="zc2wsf"
UADWaveDataAsset
    → WHAT and WHEN to spawn

AADSpawnPoint
    → WHERE to spawn

AADWaveDirector
    → EXECUTION and lifecycle
```

`AADWaveDirector` distinguishes:

```text id="fxwqhe"
Finished spawning enemies
```

from:

```text id="4zgbn0"
Wave actually completed
```

A wave completes only after spawning has finished and all tracked enemies have been defeated.

The provisional implementation currently supports multiple consecutive waves and final victory.

## Match Result

`AADGameMode` owns the authoritative match result:

```text id="k65cr7"
InProgress
Victory
Defeat
```

Gameplay systems should not unnecessarily depend directly on `GameMode`.

In particular, local trap placement currently validates placement rules directly rather than querying `GameMode` every frame.

## Construction Economy

The player receives an initial amount of Coins.

Current and planned economy flow:

```text id="cnfk2h"
Match Start
    ↓
Initial Coins

Enemy Defeated
    ↓
future Coin reward

Trap Placement
    ↓
Coin cost

Trap Sale
    ↓
future Coin refund
```

Construction is allowed:

```text id="bizwhh"
During waves       YES
Between waves      YES
```

Trap selling is planned to be allowed:

```text id="34bs75"
During waves       NO
Between waves      YES
```

## Trap Data

`UADTrapDataAsset` describes configurable trap content.

Current properties include:

```text id="5c0fkk"
Display Name
Icon

Trap Actor Class
Placement Preview Class

Construction Cost
Sell Refund Ratio

Activation Radius
Effect Radius

Can Place On Floor
Can Place On Wall
```

The Data Asset contains configuration only.

Runtime behavior belongs to the trap Actor.

Placement interaction belongs to the placement system.

## Activation Radius vs Effect Radius

Trap range is intentionally split into two concepts.

### Activation Radius

```text id="vfmhak"
ActivationRadius
```

determines how close an enemy must come before the trap reacts.

At runtime this is represented by the persistent `USphereComponent` owned by `AADTrapBase`.

### Effect Radius

```text id="zbjf0v"
EffectRadius
```

determines how far the trap's actual effect reaches once activated.

It does not require a second permanent collision component.

Concrete traps can perform an area query only when their effect executes.

This allows designs such as:

```text id="4hhi5c"
Activation Radius = 100 cm
Effect Radius     = 200 cm
```

where one enemy triggers the trap and multiple nearby enemies can be affected.

## Runtime Trap Architecture

`AADTrapBase` represents a placed trap.

Its shared responsibilities are deliberately limited:

```text id="8z9ynu"
Own TrapData

Remember Source AbilitySystemComponent

Remember actual PurchasePrice

Remember PlacementSlot

Own ActivationVolume

Detect living enemies entering/leaving ActivationRadius
```

`AADTrapBase`:

```text id="s8v7l0"
does NOT Tick

does NOT own its own ASC

does NOT define generic damage

does NOT define a generic cooldown

does NOT calculate placement coordinates
```

Concrete trap classes implement concrete gameplay behavior.

## Trap Ability System Ownership

A placed trap does not receive another Ability System Component.

Instead:

```text id="7wlb71"
Player ASC
    ↓
places trap
    ↓
Trap stores weak Source ASC
```

Future trap Gameplay Effects can therefore use the player's ASC as their source when applying effects to enemy ASCs.

This preserves gameplay attribution without adding an unnecessary ASC to every placed trap.

## Manual Trap Placement Slots

Trap placement uses explicitly authored level-design positions.

The system does **not** use:

```text id="bz4dz4"
Free-form world placement
Automatic world grid
Runtime grid snapping
Generated cells
Trap footprints
Free rotation
```

Instead, the level designer manually places:

```text id="ktmhip"
AADTrapPlacementSlot
```

Actors throughout the map.

Each slot represents exactly one legal trap position.

Conceptually:

```text id="v0aacy"
Floor

□   □   □   □
□   □   □   □
```

and:

```text id="5ic8jw"
Wall

□   □   □
□   □   □
```

This makes trap placement deterministic, easy to author and easy to reason about.

## Placement Slot Architecture

`AADTrapPlacementSlot` owns:

```text id="iysf4a"
Slot Type
    Floor / Wall

Slot Half Size

Placement Clearance

Exact world transform

Current occupying trap
```

A slot can contain at most one trap.

```text id="gl6a5i"
Slot free
    → potential placement

Slot occupied
    → invalid placement
```

Slots are manually positioned and rotated by the level designer.

Their local `+Z` axis points away from the supporting surface.

For floor slots:

```text id="7gppki"
+Z
 ↑
─●──────── floor
```

For wall slots:

```text id="ntzbuz"
wall │● → +Z
```

The slot transform therefore defines both the exact placement position and exact trap orientation.

## Slot Occupancy

Placed traps do not use collision against neighboring traps to determine whether another trap may be constructed.

Trap-to-trap occupancy is controlled exclusively through the slot:

```text id="ohvizs"
Slot A occupied
    → Slot A invalid

Slot B free
    → Slot B remains valid
```

This intentionally allows manually authored slots to sit directly beside one another.

If the level designer places two adjacent slots, the placement system trusts that both locations are legitimate construction positions.

When a trap is destroyed or later sold, `AADTrapBase::EndPlay()` releases its slot automatically.

## Physical Placement Blocking

Slot occupancy and temporary physical blocking solve different problems.

Slot occupancy prevents:

```text id="jdqrrl"
two traps using the same slot
```

The `TrapPlacementBlocker` collision channel can prevent placement because of temporary or environmental blockers such as:

```text id="z61dw5"
Player
Enemy
Defense Objective
Blocking props
Level geometry that should prevent construction
```

Placed traps themselves do not use a generic `PlacementBlockingVolume`.

This prevents a trap placed in one manually authored slot from incorrectly invalidating an adjacent slot.

Trap visual meshes can remain presentation-only with `NoCollision` unless a concrete gameplay requirement later needs physical collision.

## Trap Placement Preview

`AADTrapPlacementPreview` is presentation-only.

While the player is placing a trap, C++ supplies:

```text id="spb5cm"
Exact slot transform

Placement validity

Activation Radius

Effect Radius

Slot dimensions
```

Blueprint owns the visual representation.

The current convention is:

```text id="uolfwn"
Trap Ghost

Green
    → placement valid

Red
    → placement invalid
```

The preview also displays both gameplay ranges.

```text id="26lg9r"
Activation Range
    → visual representation of ActivationRadius

Effect Range
    → visual representation of EffectRadius
```

These remain visible before the player spends Coins, allowing the player to evaluate the usefulness of a slot.

Preview range visuals have no gameplay collision.

## Trap Placement Component

`UADTrapPlacementComponent` owns interactive trap placement.

Its responsibilities are:

```text id="bnvtke"
Select TrapData

Spawn preview

Trace slot under cursor

Move preview to exact slot transform

Check Floor / Wall compatibility

Check slot occupancy

Check temporary physical blockers

Check player Coins

Display valid / invalid state

Confirm placement

Charge Coins

Spawn runtime trap

Cancel placement
```

The component only ticks while placement mode is active.

Outside placement mode its Tick is disabled.

## Placement Transaction

A successful trap purchase follows:

```text id="k4bqbl"
Valid slot
    ↓
Revalidate slot / blockers / Coins
    ↓
SpawnActorDeferred
    ↓
Slot.TryOccupy()
    ↓
Spend Coins through GAS
    ↓
Trap.InitializeTrap(
    TrapData,
    Player ASC,
    Actual Purchase Price,
    Placement Slot
)
    ↓
FinishSpawningActor
```

The placed trap therefore knows:

```text id="m2siwy"
what trap definition created it

who placed it

how much was actually paid

which slot it owns
```

## Trap Purchase Price

Placed traps preserve the exact purchase price.

Future refunds will therefore use:

```text id="dxsf0s"
Actual PurchasePrice
×
SellRefundRatio
```

instead of recalculating from the Data Asset base cost.

This prevents economy exploits when future talents or effects modify trap prices.

## Placement Input

Current provisional controls:

```text id="atci5x"
5
    Enter / exit Trap 1 placement mode

Left Mouse Button
    Confirm trap placement

Right Mouse Button
    Cancel trap placement
```

There is no trap rotation input.

The placement slot itself defines the final orientation.

After successful placement, placement mode remains active so the player can quickly place additional traps.

Spell activation is disabled while trap placement mode is active.

## Collision Channels

The trap placement system currently uses dedicated collision channels for two different queries.

```text id="f93o6q"
TrapPlacementSlot
    → cursor selection of placement slots

TrapPlacementBlocker
    → temporary/environmental placement blockers
```

Slot selection and placement blocking remain separate responsibilities.

## C++ / Blueprint Boundary

C++ owns:

```text id="smosjl"
Economy transactions

Slot occupancy

Placement validation

Cursor-to-slot selection

Trap spawning

Activation detection

Runtime trap ownership

Gameplay-effect source attribution
```

Blueprint / Data Assets own:

```text id="emrnzo"
Trap meshes

Slot editor visualization

Preview ghost mesh

Valid/invalid materials

Activation-range visualization

Effect-range visualization

Icons

VFX

Configurable balance values
```

Blueprint presentation cannot override C++ placement validity.

## Tick Policy

Gameplay Tick is avoided unless continuous frame updates are genuinely required.

Current examples:

```text id="4nxlkm"
UADTrapPlacementComponent
    → ticks only during interactive placement

UADGroundTargetingComponent
    → ticks only during Meteor targeting
```

`AADTrapBase`, placement slots and trap preview Actors do not require gameplay Tick.

## Current Content Structure

```text id="jf28md"
Content/
└── ArcaneDefense/
    ├── Abilities/
    │   ├── GA_ArcaneBolt
    │   ├── GA_Burn
    │   ├── GA_FrostNova
    │   └── GA_Meteor
    │
    ├── Effects/
    │   ├── Attributes/
    │   │   ├── GE_InitializePlayerAttributes
    │   │   └── GE_InitializePlayerResources
    │   │
    │   ├── Resources/
    │   │   └── GE_ModifyCoins
    │   │
    │   ├── Costs/
    │   ├── Cooldowns/
    │   ├── Damage/
    │   └── Status/
    │
    ├── Traps/
    │   ├── Blueprints/
    │   │   ├── BP_ADTrapBase_Debug
    │   │   └── BP_ADTrapPlacementSlot
    │   │
    │   ├── Data/
    │   │   └── DA_Trap_Debug
    │   │
    │   ├── Materials/
    │   │   ├── M_TrapPreview_Valid
    │   │   └── M_TrapPreview_Invalid
    │   │
    │   └── Previews/
    │       └── BP_ADTrapPreview_Debug
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

## Development Status

```text id="6k2xgw"
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
Day 17 — Manual Slot-Based Trap Placement      Completed
```

## Day 17 Final Result

The final placement architecture deliberately does not use a generated grid.

The level designer explicitly defines every valid trap location by placing `AADTrapPlacementSlot` Actors in the level.

The player selects one of those locations rather than selecting an arbitrary world coordinate.

Placement validity currently considers:

```text id="cu6rd7"
Slot exists

Trap supports slot type

Slot is not occupied

No configured physical blocker occupies the slot

Player can afford ConstructionCost
```

The preview snaps exactly to the slot transform and displays both the activation and effect ranges.

Adjacent manually authored slots can contain adjacent traps without incorrectly blocking one another.

## Current Prototype

The current prototype supports:

* Four distinct player spells.
* Targeted combat.
* Ground-targeted combat.
* Cast interruption.
* GAS costs and cooldowns.
* Homing projectiles.
* Periodic damage.
* Root crowd control.
* Enemy navigation.
* Defense Objective damage.
* Data-driven waves.
* Victory and defeat.
* GAS-based player Coins.
* Data-driven trap definitions.
* Manually authored floor/wall placement slots.
* Economy-backed trap purchases.
* Green/red trap placement preview.
* Activation and effect range preview.
* Multiple adjacent trap placements.

## Next Milestone — Day 18

Day 18 introduces the first concrete runtime trap:

```text id="ftnlcd"
AADSpikeTrap
```

The placement architecture should not require modification.

Expected gameplay flow:

```text id="f119ev"
Enemy enters ActivationRadius
        ↓
AADSpikeTrap reacts
        ↓
query enemies inside EffectRadius
        ↓
build GameplayEffectSpec
using Player Source ASC
        ↓
GE_SpikeTrap_Damage
        ↓
apply to Enemy ASC
        ↓
Health decreases
```

The Spike Trap will own its concrete activation behavior.

If the final Spike design requires a rearm delay, that behavior will belong to `AADSpikeTrap` rather than being imposed on all traps through `AADTrapBase`.

## Remaining Roadmap

```text id="rjhc29"
Day 18 — Spike Trap

Day 19 — Slow Trap

Day 20 — Enemy Coin Rewards
         Trap Selling
         Inter-Wave Economy
         Coin UI

Day 21 — Experience Component + Enemy XP
Day 22 — Levels + XP Curve
Day 23 — Talent Data + Talent System
Day 24 — Three-Choice Talent UI
Day 25 — Final Talent Set + Synergies

Day 26 — Final Grunt
Day 27 — Runner
Day 28 — Tank
Day 29 — Five Final Waves + Pacing
Day 30 — General Gameplay Refactor

Day 31 — Attribute / Damage / Death Tests
Day 32 — XP / Level / Talent Tests
Day 33 — Load Scenario
Day 34 — Unreal Insights Profiling
Day 35 — Optimization + Before/After Documentation

Day 36 — Replace Key Placeholders
Day 37 — VFX / Hit Feedback / Minimal Audio
Day 38 — Bugs + Packaging + Clean Install Test
Day 39 — Final Technical README
Day 40 — Gameplay Video + Screenshots + Portfolio Release
```

## Definition of Done

The project is considered complete when it contains:

* A packaged 8–12 minute playable match.
* Four player spells.
* Two usable trap types.
* Three enemy archetypes.
* Five final waves.
* Construction economy.
* XP and levels.
* Six to nine talent choices.
* Victory and defeat.
* Functional combat/economy UI.
* Automated gameplay tests.
* Documented profiling and optimization.
* Technical documentation.
* Gameplay presentation video.
* Portfolio-ready build.

## Development Workflow

```text id="v39iro"
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
