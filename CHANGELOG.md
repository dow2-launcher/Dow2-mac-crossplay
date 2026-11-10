# Changelog

Each version adds one or more dylib parts (`src/*fix.c`); every part can be switched off with `DOW2_NO<NAME>=1`.
Versions v8-v25 were developed in [Dow2-patches](https://github.com/dow2-launcher/Dow2-patches) (`mac/`); this repo starts at v26.

- **v26** (10.11.2026) - `scatfix` (part 32): WeaponUtil::GetEntitiesInScatterArea angle test summed `(z+y)+x` as DOW2.exe
  (burst target groups). 1M cases 211527 -> 0. **All 56 vanilla reference replays match Windows to the end (Low).**
- **v25** (09-10.11) - `rowfix` (part 31): RightOfWay CollectLowerPriorityEntitiesOnLine direction as `1/len*dx` (Windows) instead of `dx/len`.
- **v24** (09.11) - `protfix` (Feral anti-modification timers no longer end matches after 15-18 min), `artfix` (off-map artillery
  scatter points as x87), `cdownfix` (projectile stuck to an entity: matrix-product sum order).
- **v23** (09.11) - `recipfix` (WeaponBag damage/weight scaling by `1/d`), second copy of `holdfix`. 54/56 vanilla replays to the end.
- **v22** (08.11) - `homingfix` (initHoming random-number order as MSVC), `holdfix`.
- **v21** (08.11) - `costfix` (A* ActualCost sum order), `fowcellfix` (fog-of-war cell rounding as `fistp`).
- **v20** (08.11) - `gsortfix`: the port's `std::stable_sort` in the simulation replaced with STLport `std::sort` behaviour (12 sites).
- **v19** (08.11) - `sortfix`: FindWeaponTarget candidate sort as STLport.
- **v18** (08.11) - `walkfix`: CollisionModel::GetTriangleHeightNormal sums as SimEngine.dll.
- **v17** (07.11) - `knockfix`: knockback throw target / durations (StateKnockback::PlotThrow) as DOW2.exe.
- **v16** (07.11) - `meleefix`: special attack chance (`1 - pow` in double as msvcr80 `_CIpow`), `_Pow_int`, sync-kill transform.
- **v15** (03.11) - `terrhitfix`: terrain segment hit / QueryTerrain sums as DOW2.exe / SimEngine.dll.
- **v14** (03.11) - `weaponfix`: bursts-until-reload stops at 0 (signed compare as Windows).
- **v13** (03.11) - `aimfix`, `rangefix`, `sumfix`: weapon aiming, OBB distance, sum order and `x/c` as `x*(1/c)`; lifted Windows x87 code (`lift/`).
- **v12** (02.11) - `combatfix` + `crctrace` (per-frame sync CRC log for desync hunting).
- **v11** (02.11) - `projfix`: projectile transform (setProjectileTransform) sums as MSVC.
- **v10** (02.11) - `pathfix`: pathfinding arithmetic as DOW2.exe.
- **v9** (02.11) - `heapfix` (pathfinder heap order as STLport), `terrainfix`, `luanumfix` (Lua numbers parsed as x87 24-bit).
- **v8** (02.11) - `randfix` (sim RNG floats as Windows x87 PC_24), `mathfix` + `msmath` (21 math functions run msvcr80's code).
