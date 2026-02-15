# AGENTS

Repository maintenance notes for any coding agent working on this project.

## Release/version hygiene

When shipping notable gameplay/UI/system changes:

1. Update `CHANGELOG.md` using Keep a Changelog format.
   - Keep `## [Unreleased]` at the top.
   - Add or update a dated version section (for example: `## [0.2.1] - 2026-02-20`).
   - Keep entries concise and player-facing.

2. Update the main menu version string in `src/GameViewPlayer.cpp`.
   - Change `kGameVersion` to the same version used in `CHANGELOG.md`.
   - The menu renders this in the lower-left as `v<version>`.

3. Keep these in sync in the same change:
   - `CHANGELOG.md` version header
   - `kGameVersion` value in `src/GameViewPlayer.cpp`

If only internal refactors happen and no player-facing behavior changes, keep them in `Unreleased` until a release-worthy version is created.