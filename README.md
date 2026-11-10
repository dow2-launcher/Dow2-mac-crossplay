# Dow2-mac-crossplay

`crossplay_fix.dylib` lets the native Mac (Feral) build of **Dawn of War II: Retribution** (3.19.10320) play in the same
match as Windows players. The dylib is injected into the Mac game with `DYLD_INSERT_LIBRARIES` and patches the simulation so it
computes bit-for-bit like Windows `DOW2.exe`: x87 PC_24 math, msvcr80 math functions, summation order, reciprocal vs divide,
sorts as STLport, RNG floats, and more. Each `src/*fix.c` is one part with a site table (`*_sites.h`) and a self-test
(`*_st*.inc`) that runs through the patched code at load. `protfix.c` stops Feral's anti-modification timer from quitting a
patched game 15-18 min after start.

## Status (v26, test build)
- **Vanilla: all 56 reference replays (Low sync) match Windows to the end.** All 1M side-by-side tests per fix: 0 differences.
- **Elite mod on Mac: NOT in sync yet** (step 12: 5 Elite replays still diverge). Do not use it for Elite matches.
- Not released to players in the launcher; pre-releases here are for testing.

## Install
1. Download `dow2mac_crossplay_v26.zip` from [Releases](https://github.com/dow2-launcher/Dow2-mac-crossplay/releases)
   (pre-release `mac-crossplay-v26`).
2. Unzip anywhere and double-click `dow2mac/run_vanilla.command` (it copies itself to `~/dow2mac`).
   If macOS blocks it: right click -> Open, or `xattr -dr com.apple.quarantine dow2mac`.
3. Details, sync test (`-sync_high`), log lines to expect and the `DOW2_NO<NAME>=1` switches: `README_RU.txt` in the zip.

## Verify a download
Every release file is built from `src/` by [`.github/workflows/build.yml`](.github/workflows/build.yml) on a GitHub macOS
runner (clang 19) and carries a build provenance attestation:

    gh attestation verify dow2mac_crossplay_v26.zip -R dow2-launcher/Dow2-mac-crossplay
    gh attestation verify crossplay_fix.dylib -R dow2-launcher/Dow2-mac-crossplay

`SHA256SUMS.txt` in the release lists the hashes.

## Layout
- `src/` - dylib sources, `VERSION`, `build_ci.sh` (CI/Mac build: dylib + player zip), `build.sh` (osxcross build used during
  development), `reg26.sh` (1M side-by-side tests against DOW2.exe under Wine), `pkg/` (player run scripts), `lift/` (lifted
  Windows x87 code used as reference), `difftest/` (replay checker `difftest.py`, `diagnose_high.py`, `HANDOFF.md` step log),
  `README_RU.txt` (player readme, goes into the zip).
- `CHANGELOG.md` - v8 ... v26.

Build locally on a Mac: `CC=/opt/homebrew/opt/llvm@19/bin/clang src/build_ci.sh` -> `src/out/`.

## History
v8-v25 were developed in [dow2-launcher/Dow2-patches](https://github.com/dow2-launcher/Dow2-patches) (`mac/`); that folder
stays there as history. Game binaries (Feral or Windows), replays, dumps and logs are not part of this repo.

---

# Dow2-mac-crossplay (RU)

`crossplay_fix.dylib` позволяет нативной Mac-версии (Feral) **Dawn of War II: Retribution** (3.19.10320) играть в одном матче с
Windows-игроками. Библиотека внедряется в игру через `DYLD_INSERT_LIBRARIES` и правит симуляцию так, чтобы она считала бит в бит
как Windows `DOW2.exe` (x87 24 бита, функции msvcr80, порядок сумм, умножение на обратное вместо деления, сортировки как STLport,
случайные числа и т. д.). Каждый `src/*fix.c` — отдельная часть с таблицей мест и самотестом при загрузке.

## Статус (v26, тестовая сборка)
- **Ваниль: все 56 эталонных реплеев (Low) сходятся с Windows до конца.** Проверки на миллион случаев по каждой части — 0 расхождений.
- **Elite на Mac пока НЕ сходится** (шаг 12: 5 реплеев Elite расходятся). Для матчей с Elite не использовать.
- Игрокам через лаунчер не выпущено; пре-релизы здесь — для тестов.

## Установка
1. Скачать `dow2mac_crossplay_v26.zip` из [Releases](https://github.com/dow2-launcher/Dow2-mac-crossplay/releases)
   (пре-релиз `mac-crossplay-v26`).
2. Распаковать и дважды щёлкнуть `dow2mac/run_vanilla.command` (скрипт сам скопируется в `~/dow2mac`).
   Если macOS не даёт запустить: правый клик -> Открыть, или `xattr -dr com.apple.quarantine dow2mac`.
3. Подробности, проверка синхронизации (`-sync_high`) и выключатели `DOW2_NO<ИМЯ>=1` — `README_RU.txt` в архиве.

## Проверка подлинности
Все файлы релиза собираются из `src/` workflow `.github/workflows/build.yml` на macOS-раннере GitHub (clang 19), с аттестацией:

    gh attestation verify <файл> -R dow2-launcher/Dow2-mac-crossplay

## История
v8–v25 делались в [Dow2-patches](https://github.com/dow2-launcher/Dow2-patches) (папка `mac/`, остаётся там как история);
список изменений — `CHANGELOG.md`. Бинарники игры, реплеи, дампы и логи в репо не входят.
