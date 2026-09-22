# Augmented Reality Mirror — collaboration brief

- **Developer:** Lee Meredith, Tennessee
- **Stage:** Restoring a prototype for clinical and engineering review
- **Project:** [AugmentedRealityMirror on GitHub](https://github.com/leeMeredith/AugmentedRealityMirror)
- **Historical demonstration:** [Mirror Box Example](https://vimeo.com/49252026)

## The idea

Augmented Reality Mirror (ARM) explores digital mirror feedback for people with unilateral limb loss. A Kinect captures color and depth data from the scene, including the intact limb. The application displays a mirrored point cloud alongside the captured view, with an adjustable virtual viewpoint.

The original motivation was to explore whether this visual experience could be useful in phantom limb pain rehabilitation. Pain relief is an evaluation question; this repository does not establish clinical effectiveness.

## What exists

The project contains C++ / openFrameworks source, interface assets, and a link to a historical demonstration. The implementation includes mirrored depth rendering, left/right input selection, virtual-camera controls, and JSON-based camera/session records. A recent Xcode run showed live Kinect capture, side switching, and viewpoint movement. The JSON version builds, but its live demonstration and recording/playback still need to be rechecked before it is shared as a current demo.

The viewpoint moves around captured surfaces; a single sensor does not supply a complete view of occluded surfaces. The original consumer-hardware approach motivates an accessibility goal, but present-day cost, setup burden, and suitability for home use have not been established.

## Collaboration being sought

An initial 20-minute discussion with a limb-loss rehabilitation clinician or rehabilitation engineer would help answer:

1. Is there a specific clinical or research use worth exploring with this type of visual feedback?
2. Which aspects of alignment, control, comfort, and setup should guide a restored demonstration?
3. What would a useful next evaluation look like, and who would be the appropriate clinical and technical collaborators?

Lee would bring the existing code and project history to that discussion. A practical first milestone is a reproducible demonstration with synthetic session data, followed by a jointly defined evaluation plan if there is a good fit.

**Immediate request:** Feedback on the concept and an introduction to the appropriate Vanderbilt team. A patient pilot is a later possibility to define with that team.
