# Augmented Reality Mirror — project overview

Augmented Reality Mirror (ARM) is a Kinect-based software prototype created by Lee Meredith in Tennessee. It explores visual feedback for people with unilateral limb loss by showing captured and mirrored depth points on a monitor. A virtual camera changes the viewpoint around the captured point cloud.

The project was motivated by questions about phantom limb pain rehabilitation. The repository does not establish pain relief, clinical effectiveness, or suitability for home use. A single Kinect cannot show surfaces hidden from the sensor, and the current interface and mirror alignment still need evaluation.

The source has been updated for openFrameworks 0.12.1. A development run before the JSON storage change showed live Kinect capture, mirror-side switching, and viewpoint movement. The JSON build compiles and its storage tests pass; a repeat live run and recording/playback checks remain open. The [original demonstration](https://vimeo.com/49252026) is historical.

Relevant perspectives for future assessment are outlined in [possible collaborators](POSSIBLE_COLLABORATORS.md). That page describes areas of expertise, not confirmed partners or an outreach plan. Any evaluation involving participants would need to be defined with appropriate clinical and institutional input.
