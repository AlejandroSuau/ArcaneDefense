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

Systems are separated by responsibility rather than by individual content item.

New abstractions are introduced only when concrete gameplay requirements justify them.

---

# Source Structure

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

---

# Gameplay Ability System

The project uses Unreal Engine's Gameplay Ability System for combat state, player resources, abilities, costs, cooldowns, damage and temporary effects.

The player owns a single `UAbilitySystemComponent`.

Different responsibilities are represented by separate Attribute Sets rather than separate player ASCs.

## Combat Attributes

`UADAttributeSet` currently contains:

```text
Health
MaxHealth
Mana
MaxMana
```

It is shared by applicable gameplay Actors and handles combat-attribute validation and out-of-health behavior.

## Player Resources

`UADPlayerResourceAttributeSet` contains:

```text
Coins
```

Coins are player-specific and intentionally separated from the shared combat Attribute Set.

The player ASC conceptually contains:

```text
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

```text
GE_InitializePlayerResources
```

Coin transactions use:

```text
GE_ModifyCoins
```

with:

```text
Data.Resource.Coins
```

as a SetByCaller Gameplay Tag.

The same Gameplay Effect can therefore support purchases, future sales and future enemy rewards.

---

# Player Ability Kit

The current combat kit is:

```text
1 → Arcane Bolt
2 → Burn
3 → Frost Nova
4 → Meteor
```

## Arcane Bolt

Targeted cast-time spell delivered through a reusable homing projectile.

## Burn

Targeted duration Gameplay Effect that periodically damages its target.

## Frost Nova

Instant radial crowd-control ability that applies `State.Rooted`.

## Meteor

Ground-targeted cast-time area spell.

All current player abilities use GAS costs and independent cooldowns.

---

# Targeting Architecture

Actor targeting and ground targeting remain separate systems.

```text
UADTargetingComponent
    → enemy Actor selection

UADGroundTargetingComponent
    → world-position targeting
```

Trap placement is also kept separate from both.

This prevents unrelated interaction systems from becoming a single monolithic targeting component.

---

# Casting

`UADCastComponent` exposes cast presentation state.

Gameplay Abilities and Ability Tasks remain responsible for actual ability execution and timing.

Movement can interrupt abilities tagged for movement cancellation.

Cost and cooldown are committed only when a cast successfully completes.

---

# Projectile Architecture

`AADProjectile` provides reusable homing-projectile behavior.

The projectile transports a prepared Gameplay Effect Spec from the source Ability System to the target Ability System.

Visuals and impact presentation remain Blueprint responsibilities.

---

# Enemy Architecture

`AADEnemyCharacter` derives from the shared character architecture and participates in GAS combat.

`AADEnemyAIController` owns navigation toward the Defense Objective.

Temporary GAS states can affect movement.

Example:

```text
State.Rooted
    ↓
stop movement
    ↓
effect expires
    ↓
resume navigation
```

---

# Defense Objective

`AADDefenseObjective` owns an Ability System Component and uses the shared Health Attribute architecture.

Enemies reaching it periodically apply damage.

Zero Health produces match defeat.

---

# Wave Architecture

Wave responsibilities are separated into:

```text
UADWaveDataAsset
    → WHAT / WHEN

AADSpawnPoint
    → WHERE

AADWaveDirector
    → EXECUTION / LIFECYCLE
```

`AADWaveDirector` distinguishes spawning completion from actual wave completion.

A wave finishes only after:

```text
all configured enemies have spawned
+
all tracked enemies have been defeated
```

The current prototype supports multiple sequential waves and final victory.

---

# Match Result

`AADGameMode` owns the authoritative match result:

```text
InProgress
Victory
Defeat
```

Gameplay systems should avoid unnecessary direct dependencies on `GameMode`.

Trap placement currently validates its own placement conditions instead of querying `GameMode` every frame.

---

# Construction Economy

The player receives an initial amount of Coins.

Current and planned flow:

```text
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

```text
During waves      YES
Between waves     YES
```

Selling is planned to be allowed:

```text
During waves      NO
Between waves     YES
```

---

# Trap Data

`UADTrapDataAsset` describes configurable trap content.

Current data includes:

```text
Display Name
Icon

Trap Actor Class
Placement Preview Class

Construction Cost
Sell Refund Ratio

Activation Range

Can Place On Floor
Can Place On Wall
```

There is no longer a separate activation radius and effect radius.

The same configured area is used both to trigger the trap and to determine which enemies are affected when it executes.

---

# Standard Trap Footprint

Arcane Defense currently uses one fixed trap footprint.

```text
Base tile
    100 × 100 cm

Trap slot
    200 × 200 cm
    = 2 × 2 floor tiles

Trap
    approximately 200 × 200 cm
    = exactly one placement slot
```

Variable trap sizes are deliberately not supported.

There are no runtime trap footprints, automatic grids or dynamic mesh-scaling rules.

Trap assets and placement slots are authored around this fixed project convention.

---

# Manual Trap Placement Slots

Trap locations are explicitly authored by the level designer.

The placement system does not use:

```text
free-form placement

automatic runtime grids

grid snapping

generated cells

variable footprints

runtime rotation controls
```

Instead, the level contains manually placed:

```text
AADTrapPlacementSlot
```

Actors.

Each slot represents exactly one legal trap location.

Conceptually:

```text
Floor

┌───────┐   ┌───────┐
│ SLOT  │   │ SLOT  │
│ 2×2   │   │ 2×2   │
└───────┘   └───────┘
```

The exact number and location of possible traps is therefore controlled directly through level design.

---

# Placement Slot Architecture

`AADTrapPlacementSlot` owns:

```text
Slot Type
    Floor / Wall

Slot Half Size
    currently 100 × 100 cm

Placement Clearance

Exact Transform

Current Occupying Trap
```

Because half-size is 100×100 cm, the resulting slot is:

```text
200 × 200 cm
```

The Actor itself remains at:

```text
Scale = 1,1,1
```

The level designer manually positions and rotates each slot.

---

# Placement Orientation Convention

Every slot follows one important coordinate convention:

```text
Local X / Y
    → trap surface

Local +Z
    → away from supporting geometry
```

For floor traps:

```text
     +Z
      ↑

   RANGE
      ↑
   ┌────┐
   │TRAP│
───┴────┴─── FLOOR
```

For wall traps:

```text
WALL
████│TRAP → +Z → RANGE
████│
████│
```

Because trap range always extends along local `+Z`, the runtime code does not need separate floor/wall range logic.

The slot transform already determines the correct orientation.

---

# Slot Occupancy

A slot can contain at most one trap.

```text
Slot free
    → candidate placement

Slot occupied
    → invalid placement
```

Trap-to-trap exclusion is controlled through slot ownership rather than general trap collision.

This deliberately allows adjacent manually authored slots to contain adjacent traps.

```text
┌─────────┬─────────┐
│ Trap A  │ Trap B  │
│ Slot A  │ Slot B  │
└─────────┴─────────┘
```

If both slots were authored by the level designer, both positions are treated as valid.

When a trap is destroyed or later sold, `AADTrapBase::EndPlay()` releases its slot automatically.

---

# Physical Placement Blocking

Slot occupancy and physical blocking solve different problems.

Slot occupancy answers:

```text
Does another trap already own this slot?
```

The custom collision channel:

```text
TrapPlacementBlocker
```

can answer:

```text
Is something temporarily or physically preventing
construction in this otherwise valid slot?
```

Typical blockers can include:

```text
Player
Enemy
Defense Objective
Blocking props
Specific level geometry
```

Placed traps themselves do not use a generic placement-blocking volume.

Their relationship with placement is represented by slot ownership.

Trap visual meshes can remain `NoCollision` unless a concrete gameplay requirement later justifies physical collision.

---

# Trap Placement Component

`UADTrapPlacementComponent` owns the player's interactive placement process.

Responsibilities:

```text
Select TrapData

Spawn preview

Find placement slot under cursor

Move preview exactly to slot transform

Check Floor / Wall compatibility

Check slot occupancy

Check configured physical blockers

Check player Coins

Display valid / invalid feedback

Confirm placement

Spend Coins

Spawn trap

Cancel placement
```

The component only ticks while Placement Mode is active.

Outside placement it has no active Tick.

---

# Placement Input

Current provisional controls are:

```text
5
    enter / exit Trap 1 placement

Left Mouse Button
    confirm placement

Right Mouse Button
    cancel placement
```

There is no runtime trap rotation.

The slot defines the final trap orientation.

After successfully constructing a trap, Placement Mode remains active so repeated placement is fast.

Player spell activation is blocked while Trap Placement Mode is active.

---

# Placement Transaction

Successful construction follows:

```text
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
InitializeTrap(
    TrapData,
    Player ASC,
    Actual Purchase Price,
    Placement Slot
)
    ↓
FinishSpawningActor
```

The placed trap therefore knows:

```text
its TrapData

its source Player ASC

its actual PurchasePrice

its owning PlacementSlot
```

---

# Purchase Price

Placed traps preserve the exact amount paid.

Future sale refunds will therefore use:

```text
Actual PurchasePrice
×
SellRefundRatio
```

rather than recalculating from the Data Asset base cost.

This prepares the system for future cost-changing talents or other modifiers.

---

# Trap Runtime Architecture

`AADTrapBase` represents the common runtime behavior of placed traps.

Its responsibilities are deliberately limited:

```text
Own TrapData

Remember Source AbilitySystemComponent

Remember actual PurchasePrice

Remember PlacementSlot

Own ActivationVolume

Detect enemies entering/leaving the active area
```

`AADTrapBase` does not:

```text
Tick

own another ASC

implement generic damage

implement generic cooldown

calculate placement coordinates

generate placement grids
```

Concrete trap subclasses own concrete gameplay behavior.

---

# Trap GAS Ownership

Placed traps do not own individual Ability System Components.

Instead:

```text
Player ASC
    ↓
places trap
    ↓
Trap stores Source ASC
```

Concrete traps can later build Gameplay Effect Specs using the player's ASC and apply them to enemy ASCs.

This keeps source attribution while avoiding unnecessary ASCs on every trap Actor.

---

# Trap Activation / Effect Volume

Trap activation and trap effect now use the **same physical area**.

The runtime area is represented by:

```text
UBoxComponent ActivationVolume
```

rather than a sphere.

The footprint of the volume matches the placement slot:

```text
200 × 200 cm
```

The third dimension is configured through:

```text
ActivationRange
```

For example:

```text
Slot / Trap
    200 × 200 cm

ActivationRange
    300 cm

Resulting volume
    200 × 200 × 300 cm
```

Since `UBoxComponent` uses half-extents, the runtime box becomes:

```text
X = 100
Y = 100
Z = 150
```

and its center is offset:

```text
Local Z = 150
```

so the volume begins at the trap surface and extends only outward.

---

# Floor Trap Range

For a floor slot:

```text
Local +Z
    → upward
```

The volume becomes:

```text
        200 cm
     ┌───────────┐
     │           │
     │           │
     │  ACTIVE   │
300  │   AREA    │
 cm  │           │
     │           │
     ├───────────┤
     │   TRAP    │
─────┴───────────┴──── FLOOR
        200 cm
```

Enemies entering this rectangular prism can activate the trap.

When the trap executes, enemies currently inside this same volume are candidates for its effect.

---

# Wall Trap Range

For a wall slot:

```text
Local +Z
    → horizontally away from the wall
```

The exact same local Box configuration therefore produces:

```text
                    ActivationRange

WALL      ┌───────────────────────────────┐
██████████│                               │
████ TRAP │          ACTIVE AREA          │
██████████│                               │
          └───────────────────────────────┘
```

No floor/wall conditional code is required for the range volume.

The slot orientation determines the result automatically.

---

# Activation and Effect Semantics

There is no longer a distinction between:

```text
Activation Radius
Effect Radius
```

The project instead uses:

```text
ActivationRange
```

and one common rectangular volume.

Runtime flow:

```text
Enemy enters ActivationVolume
        ↓
Concrete trap activates
        ↓
Trap queries valid enemies currently overlapping
the same ActivationVolume
        ↓
Trap effect is applied
```

For a Spike Trap this means one enemy can trigger the trap while all valid enemies currently inside the same box can receive damage.

---

# Trap Placement Preview

`AADTrapPlacementPreview` is presentation-only.

C++ supplies:

```text
Exact slot transform

Placement validity

ActivationRange

Slot Half Extent
```

Blueprint owns the visual representation.

The current preview contains conceptually:

```text
SceneRoot
├── TrapGhostMesh
└── RangeVisual
```

## Trap Ghost

The ghost represents the physical trap position.

Current convention:

```text
Green
    → placement valid

Red
    → placement invalid
```

Since every trap uses the same 200×200 cm footprint, the ghost is authored around that fixed size.

## Range Visual

`RangeVisual` represents the exact gameplay ActivationVolume.

It uses a translucent Cube with:

```text
Full X
    = SlotHalfExtent.X × 2

Full Y
    = SlotHalfExtent.Y × 2

Full Z
    = ActivationRange
```

For the current 200×200 slots and a 300 cm range:

```text
RangeVisual
    200 × 200 × 300 cm
```

Its relative location is:

```text
Z = ActivationRange / 2
```

so it begins at the trap surface and extends along the preview's local `+Z`.

Because the preview itself inherits the slot transform, the same Blueprint visualization works for both floor and wall traps.

The range visual communicates gameplay reach only.

It does not determine whether placement is valid.

---

# Collision Channels

The placement system currently uses:

```text
TrapPlacementSlot
    → cursor selection of placement slots

TrapPlacementBlocker
    → environmental / temporary blocking checks
```

These responsibilities remain intentionally separate.

---

# C++ / Blueprint Boundary

C++ owns:

```text
GAS resource changes

Trap purchasing

Slot occupancy

Cursor-to-slot selection

Placement validity

Trap runtime ownership

ActivationVolume

Enemy overlap detection

Gameplay Effect source attribution
```

Blueprint and Data Assets own:

```text
Trap meshes

Slot editor visualization

Trap ghost presentation

Valid / invalid materials

Range visualization

VFX

Icons

Balance values such as ActivationRange
```

Blueprint presentation cannot override C++ gameplay validity.

---

# Current Content Structure

```text
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
    │   │   ├── M_TrapPreview_Invalid
    │   │   └── M_TrapPreview_Range
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

---

# Development Status

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
Day 17 — Manual Slot-Based Trap Placement      Completed
```

---

# Day 17 Final Result

The final placement architecture uses manually authored trap positions rather than a generated placement grid.

Current design rules are:

```text
Base floor tile
    100 × 100 cm

Trap slot
    200 × 200 cm
    = 2 × 2 floor tiles

Trap footprint
    200 × 200 cm

One slot
    = maximum one trap
```

The level designer explicitly controls the number, position and orientation of all possible trap locations.

Adjacent authored slots can contain adjacent traps.

Trap placement is validated using:

```text
slot exists

trap supports Floor / Wall slot type

slot is not occupied

configured physical blockers are absent

player can afford ConstructionCost
```

The preview snaps exactly to the selected slot and displays the same rectangular volume that the runtime trap will later use for activation and effect application.

---

# Current Prototype

The project currently supports:

* Four player spells.
* GAS-based Health, Mana and Coins.
* Spell costs and cooldowns.
* Enemy Actor targeting.
* Ground targeting.
* Cast interruption.
* Homing projectiles.
* Periodic damage.
* Root crowd control.
* Enemy navigation.
* Defense Objective damage.
* Data-driven waves.
* Victory and defeat.
* Player construction economy.
* Data-driven trap definitions.
* Manually authored 200×200 trap slots.
* Floor and wall slot compatibility.
* Green/red placement feedback.
* Economy-backed trap purchases.
* Adjacent trap placement.
* Rectangular activation/effect range preview.
* Runtime rectangular trap activation volumes.

---

# Next Milestone — Day 18

Day 18 introduces the first concrete trap:

```text
AADSpikeTrap
```

The placement system should not require further redesign.

Expected runtime flow:

```text
Enemy enters ActivationVolume
        ↓
AADSpikeTrap activates
        ↓
Get all valid enemies currently overlapping
ActivationVolume
        ↓
build GameplayEffectSpec
using Player Source ASC
        ↓
GE_SpikeTrap_Damage
        ↓
apply to each Enemy ASC
        ↓
Health decreases
```

The Spike Trap will own its concrete activation behavior.

If Spike requires a rearm delay, that behavior will belong specifically to `AADSpikeTrap` rather than being imposed on every trap through `AADTrapBase`.

---

# Remaining Roadmap

```text
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

---

# Definition of Done

The final vertical slice requires:

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

---

# Development Workflow

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
