# Restoration roadmap

The first milestone is a reproducible technical demonstration that others can inspect. Decisions about a larger rewrite or any participant-facing evaluation would follow from that work.

## Completed in the September 2026 cleanup

- Rewrote the README around the implemented prototype and its known limits; retained the demo link and acknowledgments.
- Added the dependency manifest, build/restoration notes, contribution guidance, and project context.
- Removed tracked Finder `.DS_Store` files and normalized C++ source/header permissions.
- Added ignore rules for desktop metadata, build products, per-user IDE state, and generated session records.

The source now contains focused openFrameworks 0.12.1 compatibility changes. Bundled media was preserved.

Verification: the Release and Debug targets build on Apple silicon. The JSON version passes storage roundtrip tests and displayed live Kinect color and depth data on September 24, 2026. It now starts on the Kinect screen, and its `c`/`o` close-and-reopen cycle was verified. Audio playback is disabled by default. Mirror-side switching and viewpoint movement worked on the earlier XML build; repeat checks, recording/playback, longer stability, and mirror alignment remain pending.

## 1. Recover a working demonstration

| Work item | Evidence / reason | Done when |
| --- | --- | --- |
| Establish build environment | **Complete for this Mac:** openFrameworks 0.12.1, Xcode 27.0, Apple silicon | A fresh checkout can be built using documented versions and commands |
| Check the sensor independently | `testApp::setup()` initializes and opens a Kinect | SDK example and ARM both show stable capture on the documented device |
| Park startup audio | **Complete:** audio playback is explicitly disabled by default; the file and code remain available for later work | Startup and camera controls remain silent |
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

## 3. Build comparison modes

These modes would make it possible to examine which parts of the current approach are useful. They are development and evaluation tools; no particular outcome is assumed.

### 3.1 Camera-only split screen

Create a mode that uses the live color camera without constructing a point cloud or using depth data for the display. Show the original camera image and its horizontally mirrored counterpart together in a split-screen view.

- Keep the two views synchronized and use the same crop, scale, and display timing.
- Make the divider and mirror side configurable so positioning can be varied deliberately.
- Add screen recording for the complete comparison view, with the active configuration and timestamps stored alongside each recording.
- Keep the camera-only and point-cloud modes available in the same build so they can be demonstrated under comparable conditions.
- Use synthetic or developer-only recordings until participant data handling and review requirements have been defined.

Questions this mode could help examine:

- Does the depth-based point cloud or movable viewpoint change the experience compared with a conventional mirrored camera image?
- How much do screen layout, alignment, crop, and viewing position affect usability or a future outcome measure?
- Which technical differences are large enough to justify further evaluation?

Completion: the app can run a labeled, synchronized camera-only split screen without rendering depth or point-cloud data, save a screen recording with its configuration, and switch between camera-only and point-cloud demonstrations without changing hardware.

### 3.2 Selectable point-cloud quadrants

Divide the Kinect source image into a 2×2 grid and allow each quadrant to be included in or excluded from the mirrored point cloud. Apply the selection in source-image coordinates before building the mirrored mesh so the choice remains stable when the virtual camera moves.

- Provide four clearly labeled quadrant controls and show the active selection on screen.
- Support any combination of quadrants, including none and all.
- Define boundary ownership so points on the center lines appear exactly once.
- Store the active quadrant mask with session or comparison metadata.
- Check the result from both mirror sides and from several virtual-camera positions.

This first version uses four fixed regions. A finer grid or drawn selection can be considered after the interaction and mirror geometry are verified.

Completion: each quadrant can be toggled independently, only selected source regions are copied into the mirrored mesh, no points are duplicated at quadrant boundaries, and the saved configuration can reproduce the view.

## 4. Identify questions for external review

Possible clinical questions include which user group, task, and comparison would be meaningful. Any discussion with clinicians or researchers could assess the visual experience and workflow before considering whether a supervised feasibility study is appropriate and what review it would require.

Potential engineering work includes a replaceable depth-camera interface, explicit mirror calibration, predictable recording, the comparison modes above, and simpler controls. Those priorities can be refined as technical and clinical feedback becomes available.

Completion: a clearly scoped possible next experiment with success criteria and an identified owner if it is pursued. Pain reduction, home suitability, and clinical effectiveness remain questions for evaluation.
