# ARM browser RGB prototype

A small RGB camera mirror for home use and later testing on managed computers. This lives alongside the openFrameworks RGB application in the same repository. No changes to the desktop application's source are needed to run it.

Implemented: explicit camera start/stop, camera selection, horizontal mirror toggle, independent quadrant mirroring/alignment, local setup JSON download/import, and fullscreen with show/hide controls. No account, participant name, or administrator is required. Video recording is outside the browser scope.

Build 6 adds two mirror toggles and two position sliders per quadrant, retaining the confirmed starting camera orientation. Choose a receiving quadrant by its position on screen. Left/right copies and reflects the opposite column; top/bottom copies and reflects the opposite row. Both on use the diagonal quadrant. Both off show that quadrant's normal image. Sliders move the image inside its fixed quadrant by up to 50% of its width/height in either direction. Clipped edges cannot cover neighbors; uncovered areas stay dark. **Reset quadrant** clears only that quadrant's toggles and positions.

Click **Fullscreen**, then **Show controls** to align the image alongside the settings panel. **Stop camera** and the exit button stay available in the top toolbar. **Escape** also exits. If native fullscreen is unavailable, the app uses an expanded layout inside the browser window. Leaving fullscreen restores the previous panel visibility. The overall canvas fits without stretching; individual quadrant adjustments can clip their source images.

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
    mirrorRegions.js         Quadrant defaults, validation, reflection and clipping
    fullscreenView.js        Fullscreen/fallback layout, panel visibility, exit/focus
    controls.js              DOM updates and control references
    setupFile.js             Setup schema, validation, filenames, JSON download
  tests/
    captureRGB.test.js        Permission/cancellation/disconnection behavior
    mirrorView.test.js        Mirror transforms and draw-loop cleanup
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
  "schemaVersion": 2,
  "savedAt": "2026-10-05T12:00:00.000Z",
  "name": "home chair",
  "camera": { "preferredLabel": "" },
  "view": {
    "mirrored": true,
    "regions": {
      "topLeft": { "mirrorX": true, "mirrorY": false, "offsetX": 0, "offsetY": 0 },
      "topRight": { "mirrorX": false, "mirrorY": false, "offsetX": 0, "offsetY": 0 },
      "bottomLeft": { "mirrorX": false, "mirrorY": false, "offsetX": 0, "offsetY": 0 },
      "bottomRight": { "mirrorX": false, "mirrorY": false, "offsetX": 0, "offsetY": 0 }
    }
  }
}
```

The filename begins with the optional setup name, followed by `setup` and a timestamp. With no name, it begins `ARM_setup`. The camera label is a preference, not a stable hardware ID. Browser camera IDs are not exported. An empty label means no saved camera preference.

The confirmed starting view reflects the incoming camera frame horizontally. **Reverse view** changes this base orientation before quadrant processing. `view.mirrored` retains its original meaning. Each quadrant copies from the original oriented frame, never from another processed quadrant. Positive offsets move right/down; negative offsets move left/up. Offsets are percentages of quadrant size, so resizing the view preserves relative alignment.

Schema 2 saves all four quadrants. Schema 1 files still load with their original orientation and all quadrant adjustments reset. New files require build 6 or later; earlier builds reject schema 2 rather than silently losing adjustments. Import never modifies the original file. Fullscreen, panel visibility and the currently selected editing quadrant are temporary and not saved.

This is a **setup document**, not an openFrameworks study-session document. Desktop sessions, unknown versions, invalid JSON, and oversized files are rejected without partially applying settings. A later import adapter should explicitly map supported desktop settings once the corresponding browser features exist.

## Tests

With Node.js 22 or newer, from `web`:

```sh
node --test tests/*.test.js
```

These tests require no downloaded dependencies and do not open a real camera. They do not prove hardware compatibility; follow the manual checklist too.
