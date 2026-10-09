# Bounded development path

## Boundary for milestone 1

One camera, four receiving quadrants meeting at a shared center, and one setup document. `app.js` coordinates the pieces. Each other module has one responsibility:

| File | Owns | Does not own |
| --- | --- | --- |
| `captureRGB.js` | Live stream, permission request, device enumeration, stop/disconnect handling | Canvas, participant records, DOM controls |
| `mirrorView.js` | Canvas transformations, frame sizing, animation cleanup | Camera permission, file storage |
| `mirrorRegions.js` | Region/center validation, source selection, upright reflection and proportional fill | DOM, camera permission, file download |
| `fullscreenView.js` | Fullscreen and expanded layout, panel visibility, exit handling and focus | Camera lifetime, transforms, JSON storage |
| `controls.js` | Accessible page controls and status text | Camera acquisition, JSON validation |
| `setupFile.js` | Versioned setup format, validation, export filename | Live streams, pain data |
| `app.js` | User actions and coordination | Pixel processing or format validation |

Keep these direct dependencies. Do not introduce a plugin system, universal camera interface, state framework, database, or Tauri wrapper for this milestone. Extract new modules when a concrete responsibility exists. Depth acquisition earns `captureDepth` only when work on a real depth device begins.

## Next milestones and stopping points

1. **RGB baseline — user confirmed camera orientation and setup loading in build 4.** Record browser/OS versions when doing broader acceptance. Windows and hospital validation remain explicitly unverified.
2. **Fullscreen — implemented in build 5; user acceptance pending.** Use the current screen. Show/hide settings, keep Stop and Exit accessible, fit the full frame, and handle browser rejection with an in-page expanded layout. Stop when entry, exit, and adjustments work on the user's browser. Do not change capture or setup JSON for this milestone.
3. **Shared alignment and quadrant mirroring — implemented in build 7; user acceptance pending.** One horizontal mirror choice per quadrant; two global sliders move the shared center. Source crops fill unequal panes proportionally without gaps. Independent settings scrolling keeps video visible at desktop and narrow widths. Schema 3 stores the center; schema 2 migrations discard retired vertical flips/offsets with a visible notice. Stop for the user's camera/alignment test. Split-screen comparison remains a separate milestone.
4. **Optional session tracking.** Add `studySession` and a dedicated session panel when pain entries are implemented. Camera use remains available without entering identity or administrator details. Keep setups separate from timestamped sessions. Match the existing study JSON fields where their meaning is shared; write and test explicit mappings for desktop records. Never silently drop unsupported alignment or clinical fields during import. Stop when continuing a participant's history creates a new session without overwriting older entries.
5. **Shareable browser release.** Choose HTTPS hosting, test a public link in current desktop browsers, and supply a download-free quick start. Include keyboard navigation and touch-sized controls. Add tablet/phone validation as separate recorded results. Do not claim hospital compatibility based on a home-computer test.
6. **Administrative charts.** Create a separate folder/file import tool for ECharts once the session format is working. Participant camera use should not depend on chart libraries.

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
