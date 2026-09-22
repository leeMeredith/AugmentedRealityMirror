# Restoration roadmap

The first milestone is a reproducible technical demonstration that others can inspect. Decisions about a larger rewrite or any participant-facing evaluation would follow from that work.

## Completed in the September 2026 cleanup

- Rewrote the README around the implemented prototype and its known limits; retained the demo link and acknowledgments.
- Added the dependency manifest, build/restoration notes, contribution guidance, and project context.
- Removed tracked Finder `.DS_Store` files and normalized C++ source/header permissions.
- Added ignore rules for desktop metadata, build products, per-user IDE state, and generated session records.

The source now contains focused openFrameworks 0.12.1 compatibility changes. Bundled media was preserved.

Verification: the Release target builds on Apple silicon. The pre-JSON version launched, loaded runtime assets and audio, and created/reopened XML records. A Kinect v1 displayed a live ARM view from Xcode after clearing a stale process and reconnecting the sensor. Pressing `h` switched the mirrored side, and moving the mouse changed the viewpoint while capture stayed live. The JSON version builds and passes storage roundtrip tests; a new live run, recording/playback, longer stability, and mirror alignment remain pending.

## 1. Recover a working demonstration

| Work item | Evidence / reason | Done when |
| --- | --- | --- |
| Establish build environment | **Complete for this Mac:** openFrameworks 0.12.1, Xcode 27.0, Apple silicon | A fresh checkout can be built using documented versions and commands |
| Check the sensor independently | `testApp::setup()` initializes and opens a Kinect | SDK example and ARM both show stable capture on the documented device |
| Resolve startup audio | **Complete:** startup now uses bundled `heyYou.wav`; failed loads are guarded | The intended bundled audio loads, or optional audio is explicitly disabled |
| Handle output directories and bad files | **Partial:** output directories are created; malformed JSON is preserved by the storage layer, but the app's response needs checking | Empty/missing directories and malformed records produce predictable behavior |
| Audit record indexing | Camera playback reads multiple position arrays and updates record addresses | Empty, one-entry, and boundary navigation cases are exercised without invalid access |
| Document mirror geometry | `drawPointCloud()` assumes 640×480, uses half-image selection, and hard-coded translations | Both sides and viewpoint changes match a recorded reference and coordinate units are documented |

The latter items are review targets from source inspection, not all confirmed runtime defects.

## 2. Prepare a shareable demonstration

- Record a short current video identifying the hardware and showing input, reflected output, side selection, and viewpoint movement.
- Measure frame rate and capture-to-display delay on the actual system; do not claim a latency result based on the requested frame rate.
- Prepare one synthetic session and a brief list of observed limitations.
- Record the scope of earlier PT/physician involvement accurately before describing the project to others.
- Select the project license and inventory the origin and redistribution terms of code, fonts, icons, and audio. No license was added during cleanup.

Completion: another developer can repeat the demo, and an outside reviewer can understand what ARM does and what remains untested.

## 3. Identify questions for external review

Possible clinical questions include which user group, task, and comparison would be meaningful. Any discussion with clinicians or researchers could assess the visual experience and workflow before considering whether a supervised feasibility study is appropriate and what review it would require.

Potential engineering work includes a replaceable depth-camera interface, explicit mirror calibration, predictable recording, and simpler controls. Those priorities can be refined as technical and clinical feedback becomes available.

Completion: a clearly scoped possible next experiment with success criteria and an identified owner if it is pursued. Pain reduction, home suitability, and clinical effectiveness remain questions for evaluation.
