# Bounded development path

## Boundary for milestone 1

One camera, four receiving quadrants meeting at a shared center, and one setup document. `app.js` coordinates the pieces. Each other module has one responsibility:

| File | Owns | Does not own |
| --- | --- | --- |
| `captureRGB.js` | Live stream, permission request, device enumeration, stop/disconnect handling | Canvas, participant records, DOM controls |
| `mirrorView.js` | Canvas transformations, frame sizing, animation cleanup | Camera permission, file storage |
| `mirrorRegions.js` | Region/center validation, source selection, full-frame mask geometry and horizontal reflection | DOM, camera permission, file download |
| `cameraWindow.js` | Same-origin camera-window connection, canvas transfer/return, shared actions | Capture acquisition, participant/session storage |
| `cameraWindowPage.js` | Camera-window fullscreen and standalone-page recovery | Camera acquisition, stored records |
| `fullscreenView.js` | Fullscreen and expanded layout, panel visibility, exit handling and focus | Camera lifetime, transforms, JSON storage |
| `controls.js` | Accessible page controls and status text | Camera acquisition, JSON validation |
| `studySession.js` | Optional session state, measurements, immutable snapshots and session JSON download | DOM, live camera, chart rendering |
| `sessionControls.js` | Participant form, scoring phases, keyboard guard, progress/final download actions | Camera acquisition, pixel rendering |
| `setupFile.js` | Versioned setup format, validation, export filename | Live streams, pain data |
| `app.js` | User actions and coordination | Pixel processing or format validation |

Keep these direct dependencies. Do not introduce a plugin system, universal camera interface, state framework, database, or Tauri wrapper for this milestone. Extract new modules when a concrete responsibility exists. Depth acquisition earns `captureDepth` only when work on a real depth device begins.

## Next milestones and stopping points

1. **RGB baseline — user confirmed camera orientation and setup loading in build 4.** Record browser/OS versions when doing broader acceptance. Windows and hospital validation remain explicitly unverified.
2. **Fullscreen — implemented in build 5; user acceptance pending.** Use the current screen. Show/hide settings, keep Stop and Exit accessible, fit the full frame, and handle browser rejection with an in-page expanded layout. Stop when entry, exit, and adjustments work on the user's browser. Do not change capture or setup JSON for this milestone.
3. **Shared alignment and quadrant mirroring — corrected in build 8; user acceptance pending.** One horizontal mirror choice per quadrant; two global sliders move the shared center. Each pane clips a complete normal or reflected frame; shared dividers change visibility without rescaling or translating the images. This matches the native regional renderer. Schema 3 divider values remain compatible. Independent settings scrolling keeps video visible at desktop and narrow widths. Schema 3 stores the center; schema 2 migrations discard retired vertical flips/offsets with a visible notice. Stop for the user's camera/alignment test. Split-screen comparison remains a separate milestone.
4. **Optional session tracking — implemented in build 9; user acceptance pending.** Participant name, zero to four limb-loss locations/descriptions, baseline/during/final scores and dated JSON downloads. Setup JSON remains separate. A fresh session has a new ID; completed and progress downloads share that ID. See SESSION-FORMAT.md for the flat ECharts-ready rows. Import/resume, stable participant IDs, clinician fields and native desktop adapters are separate future work. Build 11 uses two simultaneous independently scrolling control boxes with fixed headings/action rows, replacing Build 10's tabs. The detached camera window remains available in the Camera heading. A shared participant field is stored in both JSON formats; setup schema 4 adds participantName while old labels stay labels. Session format remains version 1. Stop for the user's two-box layout and session test. Browser stylesheet and module URLs are versioned together per release to avoid mixing cached code with a new layout.
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
