# Bounded development path

## Boundary for milestone 1

One camera, one full-frame mirror, and one setup document. `app.js` coordinates the pieces. Each other module has one responsibility:

| File | Owns | Does not own |
| --- | --- | --- |
| `captureRGB.js` | Live stream, permission request, device enumeration, stop/disconnect handling | Canvas, participant records, DOM controls |
| `mirrorView.js` | Canvas transformations, frame sizing, animation cleanup | Camera permission, file storage |
| `controls.js` | Accessible page controls and status text | Camera acquisition, JSON validation |
| `setupFile.js` | Versioned setup format, validation, export filename | Live streams, pain data |
| `app.js` | User actions and coordination | Pixel processing or format validation |

Keep these direct dependencies. Do not introduce a plugin system, universal camera interface, state framework, database, or Tauri wrapper for this milestone. Extract new modules when a concrete responsibility exists. Depth acquisition earns `captureDepth` only when work on a real depth device begins.

## Next milestones and stopping points

1. **RGB baseline — implemented here; physical-camera acceptance pending.** Verify Start, Stop, selection, mirrored orientation, and JSON save/load on available computers. Record browser/OS versions. Hospital validation can happen later and must remain explicitly unverified until it does.
2. **Regional mirroring and alignment.** Add quadrants and the existing desktop app's horizontal/vertical adjustments, with reset controls. Introduce a `mirrorRegions` module and explicit view settings at that point. Compare using an asymmetric test image and then a live camera. Stop when one named setup restores the same complete view. Add split-screen comparison after regional behavior is accepted.
3. **Optional session tracking.** Add `studySession` and a dedicated session panel when pain entries are implemented. Camera use remains available without entering identity or administrator details. Keep setups separate from timestamped sessions. Match the existing study JSON fields where their meaning is shared; write and test explicit mappings for desktop records. Never silently drop unsupported alignment or clinical fields during import. Stop when continuing a participant's history creates a new session without overwriting older entries.
4. **Shareable browser release.** Choose HTTPS hosting, test a public link in current desktop browsers, and supply a download-free quick start. Include keyboard navigation and touch-sized controls. Add tablet/phone validation as separate recorded results. Do not claim hospital compatibility based on a home-computer test.
5. **Administrative charts.** Create a separate folder/file import tool for ECharts once the session format is working. Participant camera use should not depend on chart libraries.

Offline installation/PWA support is a later, separate milestone if users need it. Add versioned caching and update behavior only then. Future depth work remains in the native project until actual hardware requirements justify integration.

## Clinical versus home use

The same camera interface serves both. Home users do not need study enrollment or an administrator. Session tracking is optional. Documentation explains operation rather than prescribing a treatment schedule or claiming effectiveness. Camera position and posture always need a visual check; saved software settings cannot reposition the physical camera.

## Maintenance rules

- Keep source split by the responsibilities above. Avoid adding unrelated behavior to `app.js`.
- Extend and validate the JSON version whenever a saved field changes meaning. Keep original imported files untouched.
- Add tests for meaningful behavior: transforms, invalid imports, stream lifecycle, and later session continuity.
- Update the user guide and the testing record with each accepted milestone.
- Keep data and recordings out of the repository. This browser milestone does not add video recording.
- Commit under the repository owner's configured Git identity, without assistant co-author attribution.
