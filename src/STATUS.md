# crossplay v26 — status (2026-11-10, worker; difftest step 11c)

Work dir: /mnt/dow2bench/dev/native_run/macfix/v26 (v25 sources + scatfix.c, scatfix_st.inc, t26/ (c26.h, w26.c, m26.c), reg26.sh,
pkg26.sh). Outputs: v26/o26/. Previous status: crossplay_v25/STATUS.md (v25 work dir).

## scatfix (part 32) - WeaponUtil::GetEntitiesInScatterArea angle test
Target: angelgate 2025-12-13 Low F1335 / High H1130: burst group of entity 16633 (burstenemygroupsize Mac 3 / Win 2): candidate
16493 accepted on the Mac only. GetEntitiesInScatterArea = Mac 0x101c98a30 / DOW2.exe 0x98e370 (caller
RangedWeapon::GetPossibleBurstTargets Mac 0x101c77b80 / DOW2.exe 0x9abf00).
Cause: c = dot(d * r, dir) summed Mac 0x101c9922b (y+x)+z vs DOW2.exe 0x98e857/0x98e85b (z+y)+x -> 1 ulp near 1 -> ~270 ulp of acos ->
other side of the half-angle. acos itself is already Windows' (mathfix hooks _acosf -> (float)ms_acos).
Fix: 16 bytes rewritten in place (mulss xmm0,[rbp-0x18e4]; addss xmm0,xmm3; addss xmm0,xmm4); self-test 256 DOW2.exe cases through
the patched code (entry 0x101c99181, landing on the _acosf call); DOW2_NOSCATFIX=1 off. t26 1M: port 211527 -> v26 0.

## Regressions (reg26.sh, BUILT v26 dylib, o26/reg26.log + reg26_t20.log, 10.11): identical to reg25, every Mac output
byte-identical to v25's; t26 scat 211527 -> 0.

## Replays (Low, difftest runs/v26): angelgate 2025-12-13 (v25 F1335) now to the end; the 9 control replays to the end.
3 replays that hit the time limit in v25all: runs/v26to (DIFFTEST_TIMEOUT_MUL=4) - all 3 to the end.
