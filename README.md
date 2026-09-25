# Augmented Reality Mirror (ARM)

A Kinect-based digital mirror prototype by Lee Meredith, created to explore visual feedback for people with unilateral limb loss and phantom limb pain.

ARM uses a Kinect depth camera to capture a colored point cloud, mirror one side of the captured scene, and display both views on a monitor. A virtual camera lets the viewer change the perspective on that point cloud.

**Status:** ARM builds with openFrameworks 0.12.1 on Apple silicon. On September 24, 2026, the JSON build opened directly into the Kinect screen and displayed live color and depth data after the sensor's startup retry. The repaired `c` and `o` controls closed and reopened the Kinect successfully. Audio playback is parked and disabled by default. Longer stability, mirror alignment, recording controls, and session playback still need hardware testing. Clinical effectiveness has not been established by evidence included in this repository.

[Watch the original demonstration](https://vimeo.com/49252026) · [Read the project overview](docs/PROJECT_OVERVIEW.md) · [Restore a development build](docs/BUILD.md)

## Why this project exists

The project explores whether a digitally mirrored view of an intact limb could be a useful tool for clinician-guided rehabilitation research. Its distinguishing idea is a movable viewpoint around the captured depth data, rather than a fixed view in a physical mirror.

The virtual camera changes the view of what the sensor captured. It does not reconstruct surfaces hidden from the Kinect, and the prototype does not establish unrestricted movement, complete 360-degree capture, or pain relief. Accessible home use is a future research goal.

## What is in the source

- Kinect color/depth acquisition and a mirrored point-cloud display, with selection of either half of the sensor image.
- Two virtual cameras, orbit controls, and camera-position playback code.
- Kinect, form, camera, and session interface modes.
- JSON-based flythrough and session records, including camera coordinates, participant fields, and pain-score entries.
- Audio files and playback code retained for possible later work; playback is disabled by default.

These are features visible in the implementation. Live capture was observed with Kinect hardware after the JSON change; mirror-side switching and viewpoint movement were observed before it and still need a repeat check on the current build. Recording/playback refers to camera and session state, not a verified depth-video recording system.

## Getting started

For a quick introduction, watch the historical demo and read the [project overview](docs/PROJECT_OVERVIEW.md). Developers should start with the [build and restoration notes](docs/BUILD.md). The repository contains the generated macOS Xcode project and runtime assets; openFrameworks 0.12.1 remains an external dependency.

The source depends on openFrameworks 0.12.1 and its bundled `ofxKinect` and `ofxOpenCv` add-ons. JSON storage uses the JSON library already included with openFrameworks. The add-ons are listed in [addons.make](addons.make). The [build notes](docs/BUILD.md) describe how to convert older local XML records without overwriting them.

## Project layout

| Path | Purpose |
| --- | --- |
| `src/main.cpp` | Application entry point and window setup |
| `src/testApp.*` | Kinect capture, point-cloud mirroring, cameras, and UI coordination |
| `src/camera/` | Camera playback |
| `src/cognition/` | Smoothing and interpolation helpers |
| `src/gui/` | Interface controls, JSON records, and session state |
| `src/audio/`, `src/timers/` | Audio playback and timing |
| `bin/data/` | Fonts, interface images, audio, and message presets |
| `docs/` | Build notes, restoration priorities, and project context |

## Continuing development

The next milestone is a Kinect-connected demonstration of capture, mirroring, camera controls, and session playback. Planned comparison work includes a camera-only mirrored split screen with screen recording and a point-cloud mode with independently selectable quadrants. See the [restoration roadmap](docs/ROADMAP.md) for the research questions and concrete completion criteria, and [CONTRIBUTING.md](CONTRIBUTING.md) for how to report a build or propose a change.

The [possible collaborators](docs/POSSIBLE_COLLABORATORS.md) page describes areas of expertise that could help assess ARM. It does not name or imply any committed partners.

## Data and research use

Session code can write participant information, age, amputation information, and pain scores to local JSON. Use invented data for development. Generated records under `bin/data/record/address/` are ignored by Git; that exclusion is housekeeping, not a privacy or security control.

Any proposed evaluation involving participants would require appropriate decisions about consent, data handling, and research review. This repository supplies a prototype, not a treatment protocol.

## License and acknowledgments

No project license file was present in the reviewed repository. A license choice and attribution review remain open items before describing ARM as a licensed open-source release. Dependencies and bundled media need their own attribution review.

Original acknowledgments, preserved from the historical README: Kareemah Batts, Desmond Heeley, Joe Labrie, Michael Lichter, Jennifer Eftychiou PT DPT, and Jeffrey Heckman DO. These acknowledgments do not establish a study, clinical endorsement, or current affiliation.
