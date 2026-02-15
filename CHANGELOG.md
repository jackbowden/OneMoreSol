# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project aims to follow [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [2.0.0] - 2026-02-15

Version 1 includes all commits prior to `9dbaa79d` and is intentionally not enumerated here.

### Added
- Arcade coin acceptor integration.
- Credit count display in the game view.
- Health bar above Major Tom with dynamic color changes based on remaining health.
- Health bars for BikeBoss and TankBoss with dynamic color changes based on remaining health.
- Draining weapon indicator support.
- Arcade setup documentation in `ARCADE_SETUP.md`.

### Changed
- Reload/weapon-indicator update logic refactored for clarity and efficiency.
- Continue screen and menu UI layout/timing adjustments (including countdown duration).
- Play button text handling updated and related texture references cleaned up.
- CMake/project configuration updated to include new gameplay and coin-reader components.

### Removed
- Pause functionality and related pause UI elements.