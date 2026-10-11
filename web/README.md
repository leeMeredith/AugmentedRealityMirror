# ARM browser RGB prototype

A small RGB camera mirror for home use and later testing on managed computers. This lives alongside the openFrameworks RGB application in the same repository. No changes to the desktop application's source are needed to run it.

Implemented: explicit camera start/stop, camera selection, horizontal mirror toggle, independent quadrant mirroring/alignment, local setup JSON download/import, and fullscreen with show/hide controls. No account, participant name, or administrator is required. Video recording is outside the browser scope.

Build 8 uses **shared reveal dividers** over full-frame camera images. Horizontal moves the vertical boundary; vertical moves the horizontal boundary. All four panes remain joined. Each quadrant keeps one independent **Left/right mirror** toggle; top/bottom flipping has been removed. Both sliders range from 10% to 90% of the full image so no pane collapses. **Reset alignment** returns both to 50% without changing mirror choices.

Each quadrant masks either the full base camera image or its full horizontal reflection. Moving a divider changes only what is revealed: the underlying images stay at a constant scale and position. The reflection axis stays at the middle of the full frame; the divider is not a moving reflection axis. There is no pane-specific zoom, translation or vertical flip. This matches the existing openFrameworks regional rendering. The full composed view may still have outer borders to preserve its proportions; there are no empty gaps between panes.

The controls scroll independently while the camera remains visible, in normal and fullscreen views. On narrow screens the camera sits above the scrolling controls rather than behind them. **Fullscreen** initially hides settings; **Show controls**, **Stop camera**, and Exit stay available. Escape exits, and an in-page expanded view is used if native fullscreen is unavailable.

## Run locally

From this `web` directory, with Python 3 installed:

```sh
python3 serve.py
```

Open `http://localhost:8000` yourself. Stop the server with Ctrl+C. Localhost is allowed for camera development; do not open `index.html` by double-clicking it. The application uses JavaScript modules and expects an HTTP server.

The development server always serves this `web` folder and disables caching for HTML, CSS, and all JavaScript modules. The footer shows a build label set by the running JavaScript. If an already-open tab seems stale, reload it; if needed, run `python3 serve.py --port 8001` and use the new printed address to test with a fresh browser origin. Stop the camera in the old tab first. A new port may ask for camera permission again. Loading an older setup can still restore its old orientation, so check a fresh start before loading a file.

No npm installation or build step is needed to run the app. The `package.json` only enables ES modules and the Node test command.

For ordinary users, a maintainer would host the static `web` folder over HTTPS and share a link. Users would not run Python or install development tools. Hosting has not been configured or published by this milestone. Opening a developer machine's HTTP LAN address is not an equivalent camera test: use HTTPS for remote access.

## Guides

- [User guide](guide.html): camera placement, controls, setup files, and troubleshooting. Also linked inside the app.
- [Development path](docs/DEVELOPMENT.md): file boundaries, next milestones, and stopping points.
- [Testing](docs/TESTING.md): automated checks and device/browser checklist.
- [IT notes](docs/IT-NOTES.md): permissions, local processing, and deployment requirements.

## File structure

```text
web/
  index.html                 Main interface
  guide.html                 User instructions available without a camera
  styles.css                 Shared, responsive styling
  serve.py                   Local server with explicit root and caching disabled
  src/
    app.js                   Connects controls, capture, view, and setup files
    captureRGB.js            Camera acquisition, ownership, release, errors
    mirrorView.js            Canvas mirror rendering and draw-loop lifetime
    mirrorRegions.js         Shared masks, quadrant validation and full-frame reflection
    fullscreenView.js        Fullscreen/fallback layout, panel visibility, exit/focus
    controls.js              DOM updates and control references
    setupFile.js             Setup schema, validation, filenames, JSON download
  tests/
    captureRGB.test.js        Permission/cancellation/disconnection behavior
    mirrorView.test.js        Mirror transforms and draw-loop cleanup
    mirrorRegions.test.js     Gap-free mask geometry
    setupFile.test.js         Round trips, invalid files, filename safety
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
  "schemaVersion": 3,
  "savedAt": "2026-10-09T12:00:00.000Z",
  "name": "home chair",
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

The filename begins with the optional setup name, followed by `setup` and a timestamp. With no name, it begins `ARM_setup`. The camera label is a preference, not a stable hardware ID. Browser camera IDs are not exported. An empty label means no saved camera preference.

The confirmed starting view reflects the incoming camera frame horizontally. **Reverse view** changes this base orientation before quadrant processing. `view.mirrored` retains its original meaning. Each quadrant reads the original frame, never another processed quadrant. `view.center.x/y` are percentages measured from the left/top of the full image. Their rounded pixel boundaries are shared by neighboring masks to prevent seams. Schema 3 fields are unchanged: saved divider positions and left/right choices still load, now using fixed-scale rendering instead of build 7's pane-fitting behavior.

Schema 3 stores the shared center and four independent `mirrorX` choices. Schema 1 imports retain camera direction with neutral quadrants and centered alignment. Schema 2 imports retain camera direction and left/right choices, explicitly discard retired vertical flips and per-pane offsets, and center the shared alignment. A visible import notice explains the conversion. Original files are never modified. Older app builds reject schema 3. Fullscreen, panel visibility and the selected editing quadrant are not saved.

This is a **setup document**, not an openFrameworks study-session document. Desktop sessions, unknown versions, invalid JSON, and oversized files are rejected without partially applying settings. A later import adapter should explicitly map supported desktop settings once the corresponding browser features exist.

## Tests

With Node.js 22 or newer, from `web`:

```sh
node --test tests/*.test.js
```

These tests require no downloaded dependencies and do not open a real camera. They do not prove hardware compatibility; follow the manual checklist too.
