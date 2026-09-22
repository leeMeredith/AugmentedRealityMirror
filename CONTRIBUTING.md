# Contributing to ARM

The immediate goal is a repeatable development build and an accurate demonstration of the existing mirror behavior. Start with [BUILD.md](docs/BUILD.md) and [ROADMAP.md](docs/ROADMAP.md).

## Useful contributions

- Recover and document a working SDK, compiler, operating system, and Kinect combination.
- Resolve a specific build or runtime failure with reproduction steps.
- Provide a synthetic demonstration or a clearly labeled historical example.
- Review setup, usability, and evaluation questions with rehabilitation practitioners.

For a build report, include the ARM commit, openFrameworks version, add-on versions, OS/architecture, compiler, Kinect model, steps taken, and the relevant error text. State which checks in BUILD.md actually passed.

## Changes

Keep changes focused so their effects can be assessed. When restoring compatibility, preserve the mirroring behavior until it can be compared with a reference demonstration. Explain any changes to coordinates, timing, data formats, or camera controls.

Use invented participant information in fixtures and screenshots. Keep local session records out of commits. Before sharing a change, inspect `git diff --check` and `git status --short`, and document what you could and could not run.

The project license has not yet been selected. Resolve contribution and reuse terms with the maintainer before accepting substantial outside contributions. Preserve the existing acknowledgments and record the origin of any new asset.
