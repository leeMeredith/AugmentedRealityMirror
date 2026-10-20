# ARM browser RGB prototype

A small RGB camera mirror for home use and later testing on managed computers. This lives alongside the openFrameworks RGB application in the same repository. No changes to the desktop application's source are needed to run it.

Implemented: explicit camera start/stop, camera selection, horizontal mirror toggle, independent quadrant mirroring/alignment, optional participant/pain sessions, local setup JSON download/import, and fullscreen with show/hide controls. No account, participant name, or administrator is required. Video recording is outside the browser scope.

Build 8 uses **shared reveal dividers** over full-frame camera images. Horizontal moves the vertical boundary; vertical moves the horizontal boundary. All four panes remain joined. Each quadrant keeps one independent **Left/right mirror** toggle; top/bottom flipping has been removed. Both sliders range from 10% to 90% of the full image so no pane collapses. **Reset alignment** returns both to 50% without changing mirror choices.

Each quadrant masks either the full base camera image or its full horizontal reflection. Moving a divider changes only what is revealed: the underlying images stay at a constant scale and position. The reflection axis stays at the middle of the full frame; the divider is not a moving reflection axis. There is no pane-specific zoom, translation or vertical flip. This matches the existing openFrameworks regional rendering. The full composed view may still have outer borders to preserve its proportions; there are no empty gaps between panes.

The controls scroll independently while the camera remains visible, in normal and fullscreen views. On narrow screens the camera sits above the scrolling controls rather than behind them. **Fullscreen** initially hides settings; **Show controls**, **Stop camera**, and Exit stay available. Escape exits, and an in-page expanded view is used if native fullscreen is unavailable.

## Open the hosted app

[**Open ARM camera mirror**](https://leemeredith.github.io/AugmentedRealityMirror/web/) · [Online user guide](https://leemeredith.github.io/AugmentedRealityMirror/web/guide.html)

Open the link, press **Start camera**, and allow camera access. No installation or GitHub account is required. The live app is Build 10. Camera frames are processed on the visitor's device.

## Separate camera window and shared participant name

Build 10 adds View settings / Session tabs and an Open camera window button. The same canvas moves to a same-origin camera page, sharing the original stream rather than opening a second camera. Its animation scheduler follows the destination window so the live view can update with the controller in the background. Return/close restores the canvas to the controller without restarting capture. The camera window supports fullscreen, Stop, End session, status and number-key scoring; the main window owns records and requests downloads. Keep the main window open.

Both panels share one participant-name field, fixed during an active or completed session. Setup and session JSON contain `participantName`; both filenames begin with that name when supplied, otherwise with a UTC timestamp. Loading a camera setup fills the name only before a session starts. A conflicting name is kept out of active/completed records with an explanation. New session unlocks the field. The old setup `name` remains an optional view label and never becomes a participant identity automatically.

If pop-ups are blocked, the embedded view remains usable. A browser may choose a tab instead of a window. A second monitor is optional.

## Optional participant and pain sessions

Build 9 adds an optional session panel. Enter a name/label if wanted, select anatomical limb-loss locations (multiple locations and bilateral loss are supported), choose a baseline score, and press Start session. During use, one score selection appends one timestamped measurement. End session asks for a final score and requests a JSON download. Key 0 means score 10; use the No pain (0) button for a zero score. Number keys do not record while typing or adjusting form fields.

Session files begin with the optional name plus the UTC start date/time, or just the date/time when unnamed. Save session JSON can also download unfinished progress or repeat a completed download. New session retains form details for review but creates a new record. Records live only in the current tab until downloaded; check Downloads before closing or starting another session. Camera Start/Stop is independent of session Start/End.

The [session format](docs/SESSION-FORMAT.md) documents the flat timestamp/elapsed-time/score rows for a future separate ECharts administration tool. ECharts is not a dependency of the camera app. Session import/resume and desktop-session conversion remain later work.

## Run locally for development

From this `web` directory, with Python 3 installed:

```sh
python3 serve.py
```

Open `http://localhost:8000` yourself. Stop the server with Ctrl+C. Localhost is allowed for camera development; do not open `index.html` by double-clicking it. The application uses JavaScript modules and expects an HTTP server.

The development server always serves this `web` folder and disables caching for HTML, CSS, and all JavaScript modules. The footer shows a build label set by the running JavaScript. If an already-open tab seems stale, reload it; if needed, run `python3 serve.py --port 8001` and use the new printed address to test with a fresh browser origin. Stop the camera in the old tab first. A new port may ask for camera permission again. Loading an older setup can still restore its old orientation, so check a fresh start before loading a file.

No npm installation or build step is needed to run the app. The `package.json` only enables ES modules and the Node test command.

GitHub Pages now serves the app over HTTPS at the link above, using the repository root on `master`. The root address shows project information; `/web/` opens the app. Ordinary users do not run Python or install development tools. Opening a developer machine's HTTP LAN address is not an equivalent camera test: use HTTPS for remote access.

## Guides

- [User guide](guide.html): camera placement, controls, setup files, and troubleshooting. Also linked inside the app.
- [Development path](docs/DEVELOPMENT.md): file boundaries, next milestones, and stopping points.
- [Testing](docs/TESTING.md): automated checks and device/browser checklist.
- [IT notes](docs/IT-NOTES.md): permissions, local processing, and deployment requirements.

## File structure

```text
web/
  index.html                 Main interface
  camera.html                Camera-only window, opened from the controller
  guide.html                 User instructions available without a camera
  styles.css                 Shared, responsive styling
  serve.py                   Local server with explicit root and caching disabled
  src/
    app.js                   Connects controls, capture, view, and setup files
    captureRGB.js            Camera acquisition, ownership, release, errors
    mirrorView.js            Canvas mirror rendering and draw-loop lifetime
    mirrorRegions.js         Shared masks, quadrant validation and full-frame reflection
    fullscreenView.js        Fullscreen/fallback layout, panel visibility, exit/focus
    cameraWindow.js          Canvas/window lifecycle and shared camera/session actions
    cameraWindowPage.js      Camera-window fullscreen and connection recovery
    controlTabs.js           View/Session panel selection and keyboard tab navigation
    controls.js              DOM updates and control references
    setupFile.js             Setup schema, validation, filenames, JSON download
    studySession.js          Session state, measurements, snapshots and dated JSON files
    sessionControls.js       Participant form, scoring phases and keyboard actions
  tests/
    captureRGB.test.js        Permission/cancellation/disconnection behavior
    mirrorView.test.js        Mirror transforms and draw-loop cleanup
    mirrorRegions.test.js     Gap-free mask geometry
    setupFile.test.js         Round trips, invalid files, filename safety
    studySession.test.js      Session lifecycle, immutable records, timestamps, filenames
    session.browser.mjs       Participant/scoring/download flow with synthetic data
    regions.browser.mjs      Synthetic pixel and quadrant-control integration checks
    fullscreen.browser.mjs   Native/fallback fullscreen integration checks
  docs/
    DEVELOPMENT.md
    TESTING.md
    IT-NOTES.md
```

There are no runtime libraries, remote fonts, CDNs, analytics, or application backend. Camera frames remain in the video/canvas pipeline; they are not converted to JSON. Setup JSON is downloaded explicitly, not silently saved in browser storage.

## Setup format

```json
{
  "format": "arm-rgb-setup",
  "schemaVersion": 4,
  "savedAt": "2026-10-09T12:00:00.000Z",
  "name": "home chair",
  "participantName": "Example Person",
  "camera": {
    "preferredLabel": ""
  },
  "view": {
    "mirrored": true,
    "center": {
      "x": 50,
      "y": 50
    },
    "regions": {
      "topLeft": {
        "mirrorX": true
      },
      "topRight": {
        "mirrorX": false
      },
      "bottomLeft": {
        "mirrorX": false
      },
      "bottomRight": {
        "mirrorX": false
      }
    }
  }
}
```

The filename begins with `participantName` when provided, followed by UTC date/time and `_setup.json`. An unnamed file begins with the date/time. The `name` field remains a descriptive setup label; it does not name the participant. The camera label is a preference, not a stable hardware ID. Browser camera IDs are not exported. An empty label means no saved camera preference.

The confirmed starting view reflects the incoming camera frame horizontally. **Reverse view** changes this base orientation before quadrant processing. `view.mirrored` retains its original meaning. Each quadrant reads the original frame, never another processed quadrant. `view.center.x/y` are percentages measured from the left/top of the full image. Their rounded pixel boundaries are shared by neighboring masks to prevent seams. Schema 4 adds participantName without changing divider or mirror meanings. Version 3 setups still load with their view settings and a blank participant name.

Schema 4 stores the shared center, four independent `mirrorX` choices and participantName. Schema 1 imports retain camera direction with neutral quadrants and centered alignment. Schema 2 imports retain camera direction and left/right choices, explicitly discard retired vertical flips and per-pane offsets, and center the shared alignment. A visible import notice explains the conversion. Original files are never modified. Builds before 10 reject schema 4. Fullscreen, panel visibility and the selected editing quadrant are not saved.

This is a **setup document**, not an openFrameworks study-session document. Desktop sessions, unknown versions, invalid JSON, and oversized files are rejected without partially applying settings. A later import adapter should explicitly map supported desktop settings once the corresponding browser features exist.

## Tests

With Node.js 22 or newer, from `web`:

```sh
node --test tests/*.test.js
```

These tests require no downloaded dependencies and do not open a real camera. They do not prove hardware compatibility; follow the manual checklist too.
