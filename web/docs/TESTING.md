# Browser acceptance checklist

Automated unit tests use fake streams and canvas contexts. Headless checks can verify page behavior with synthetic camera frames. Neither substitutes for a person checking a real camera.

## Automated unit checks

```sh
node --test tests/*.test.js
```

These cover invalid setup imports, Unicode/unsafe filenames, stopping while permission is pending, failed capture, disconnects, and mirror transform cleanup.

## Optional fullscreen browser checks

The integration test uses headless Chromium and a synthetic camera. Playwright is an optional developer tool, not an application dependency. With the development server running on port 8001:

```sh
npm install --no-save --package-lock=false playwright
npx playwright install chromium
node tests/fullscreen.browser.mjs
node tests/regions.browser.mjs
```

Set `ARM_TEST_URL` to test a different local address. Existing test environments can supply `ARM_PLAYWRIGHT_MODULE` (a module path) and `ARM_CHROME_EXECUTABLE` instead of installing another copy. `ARM_SCREENSHOT_DIR` optionally saves review images outside source. These checks exercise native fullscreen, Escape, browser-driven exit, rejected/unavailable fullscreen, control visibility, Stop, focus restoration, and narrow-screen layout.

## Manual check on each available computer

Record the operating system, browser/version, camera, date, and result. Test desktop Mac and Windows first. Tablet, phone, and managed hospital environments remain separate targets.

1. Open the local development URL or deployed HTTPS link. Record the build label in the footer. Confirm no camera permission prompt appears until Start camera is pressed.
2. Start and allow the camera. Confirm a live view and the browser's camera-use indicator.
3. On a fresh page, raise one hand and compare the initial view with Reverse view pressed once, then twice. Build 5 retains the confirmed build 4 starting direction. Check horizontal reversal with asymmetric text or an object. Confirm the full image fits without stretching or cropping.
4. Select a different camera if available and press Use selected camera. Confirm the old camera is released and the new view updates.
5. Press Stop. Confirm the preview clears and the stream is released (the browser's indicator may take a moment to update).
6. Start again and leave it running for several minutes. Confirm movement stays live.
7. Enter an invented setup name, press Reverse view once, and save. Confirm the download begins with that name and contains only setup fields.
8. Reload the page. Confirm the camera is off. Start it and check that the default direction returned. Load the saved file; confirm the name and saved direction restore. Also confirm older setups with `view.mirrored: true` and `false` preserve their respective directions.
9. Try invalid JSON, a desktop study-session JSON, and a future-version setup. Confirm an explanation and no change to the current setup.
10. Deny camera access. Confirm a readable recovery message. Restore permission and retry.
11. Press Stop while the permission request is pending, then allow the request. Confirm the late stream is released and the preview stays off.
12. Unplug an external camera while viewing. Confirm the page explains that it stopped; reconnect and start again.
13. Navigate with Tab, Enter, and Space; verify visible focus and access to every control. Zoom to 200% and try a narrow window without horizontal page scrolling.
14. Navigate to the user guide while streaming. Confirm the old page releases its camera. Return: the camera must require an explicit Start again.

## Fullscreen (build 5)

1. Start the camera and enter Fullscreen on the current screen. Confirm the settings panel hides, the full image fits, and movement stays live with the same direction.
2. Show controls, reverse the view, then hide controls. Confirm the toolbar always provides Show controls, Exit, and Stop camera. On a narrow window, the controls may cover part of the view and should scroll independently.
3. Exit using the button and repeat using Escape. Confirm normal layout and previous panel visibility return, with the camera still running. Repeat with controls hidden before entering fullscreen.
4. Hide controls and stop the camera from the top toolbar. Confirm the image clears and capture ends. Show controls to start again.
5. Load and save settings while fullscreen. Confirm settings are preserved even if the browser exits fullscreen for a file dialog. Loading a setup must never automatically enter fullscreen.
6. Test a browser or policy that denies fullscreen. Confirm the expanded in-page view works, explains the limitation, and exits with Escape or Exit expanded view.
7. Test keyboard navigation and 200% zoom, including a narrow or short window. Controls and exits must remain reachable. A second monitor is optional, not a requirement for this milestone.

## Quadrants (build 6)

1. Start with the confirmed camera orientation. Choose a receiving quadrant by its screen position. Check each toggle separately, then both together; confirm sources come from the opposite column, row or diagonal respectively.
2. Move both sliders in both directions. Confirm only the selected quadrant changes, dividing lines stay fixed and uncovered areas become dark.
3. Configure two different quadrants, switch between them, and reset one. Confirm the other keeps its settings.
4. Save a named setup, reload the page, then load it. Confirm all four quadrants restore and capture still requires Start. Import a schema 1 setup and confirm it clears quadrant adjustments while retaining its original mirror direction.
5. Repeat adjustments with Fullscreen and Show controls. Check live movement, slider keyboard access, stop and exit. Confirm posture alignment with a real person; automated pixels do not establish that.

## Record actual results

Only record tests that were actually performed. Suggested columns:

| Date | OS | Browser/version | Camera or synthetic source | Checks | Result/limitations |
| --- | --- | --- | --- | --- | --- |
| 2026-10-05 | macOS (development machine) | Node 24.12.0 | Fake media streams / canvas context | 12 unit checks | Passed; no hardware exercised |
| 2026-10-05 | macOS (development machine) | Headless Chrome 154.0.8037.97 | Browser-generated synthetic video | 10 page checks: explicit start, video-only capture, named JSON download, import without camera start, invalid import preservation, stop/release, pixel-level horizontal reversal, narrow layout/guide, denied permission, script errors/external requests | Passed; not a physical camera or Windows test |

Hospital restrictions, physical Windows cameras, and mobile devices are not assumed to work because the local Mac or a synthetic headless test passes.

Build 5 verification on 2026-10-05: all 13 Node 24.12.0 unit checks passed. All 9 fullscreen browser checks passed in headless Chrome 154.0.8037.98 with synthetic video, including native and fallback exits, unchanged orientation, capture release, and narrow-screen controls. Desktop and narrow screenshots were inspected. Physical-camera fullscreen acceptance remains pending with the user.

Build 6 verification on 2026-10-09: all 14 Node unit checks passed. Headless Chrome 155.0.8059.39 passed 96 asymmetric-image pixel cases covering every quadrant, both base orientations, both toggles and positive/negative offsets; it also passed independent controls/reset, named schema 2 downloads/imports, invalid import preservation, schema 1 migration, keyboard sliders, fullscreen editing, narrow layout and capture release. All 9 existing fullscreen checks passed. Fullscreen and narrow screenshots were inspected. Only synthetic camera data was used; physical-camera alignment remains for user acceptance.
