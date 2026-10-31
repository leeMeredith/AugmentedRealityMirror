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
node tests/session.browser.mjs
node tests/windows.browser.mjs
node tests/panels.browser.mjs
```

Set `ARM_TEST_URL` to test a different local address. Existing test environments can supply `ARM_PLAYWRIGHT_MODULE` (a module path) and `ARM_CHROME_EXECUTABLE` instead of installing another copy. `ARM_SCREENSHOT_DIR` optionally saves review images outside source. These checks exercise native fullscreen, Escape, browser-driven exit, rejected/unavailable fullscreen, control visibility, Stop, focus restoration, and narrow-screen layout.

## Manual check on each available computer

Record the operating system, browser/version, camera, date, and result. Test desktop Mac and Windows first. Tablet, phone, and managed hospital environments remain separate targets.

1. Open the local development URL or deployed HTTPS link. Record the build label in the footer. Confirm no camera permission prompt appears until Start camera is pressed.
2. Start and allow the camera. Confirm a live view and the browser's camera-use indicator.
3. On a fresh page, raise one hand and compare the initial view with Reverse view pressed once, then twice. Build 8 retains the confirmed build 4 starting direction. Check horizontal reversal with asymmetric text or an object. Confirm the full image fits without stretching or cropping.
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
2. Show controls, reverse the view, then hide controls. Confirm the toolbar always provides Show controls, Exit, and Stop camera. The controls must remain on the right and scroll independently without moving or covering the video; Hide controls expands the image.
3. Exit using the button and repeat using Escape. Confirm normal layout and previous panel visibility return, with the camera still running. Repeat with controls hidden before entering fullscreen.
4. Hide controls and stop the camera from the top toolbar. Confirm the image clears and capture ends. Show controls to start again.
5. Load and save settings while fullscreen. Confirm settings are preserved even if the browser exits fullscreen for a file dialog. Loading a setup must never automatically enter fullscreen.
6. Test a browser or policy that denies fullscreen. Confirm the expanded in-page view works, explains the limitation, and exits with Escape or Exit expanded view.
7. Test keyboard navigation and 200% zoom, including a narrow or short window. Controls and exits must remain reachable. A second monitor is optional, not a requirement for this milestone.

## Shared dividers and scrolling (build 8)

1. Check each quadrant's left/right mirror independently. People stay upright; there is no top/bottom control.
2. Move each shared slider toward both ends. All pane boundaries move together and remain joined without gaps. Use an asymmetric grid or stationary object: its size and position must remain unchanged within either underlying image as the boundaries move. Unequal panes must never zoom their images. With all mirrors off, the original image remains unchanged.
3. Change the selected quadrant and confirm the shared slider positions remain unchanged. Reset alignment; both return to 50% and mirror choices remain intact.
4. Scroll from Camera through Setup while watching the video. Confirm it stays visible and stationary. Repeat in fullscreen, a narrow portrait window and a short landscape window. Controls must never overlap the video.
5. Save a named setup, reload and import it. Confirm shared center and all mirror choices return without automatically starting capture. Import schema 2: camera direction and left/right choices stay, old vertical flips/offsets are removed, alignment centers and a message explains this. Invalid imports must preserve the current setup.
6. Check keyboard slider controls, fullscreen exits and Stop. Complete a real-camera alignment check before accepting this milestone.

## Record actual results

Only record tests that were actually performed. Suggested columns:

| Date | OS | Browser/version | Camera or synthetic source | Checks | Result/limitations |
| --- | --- | --- | --- | --- | --- |
| 2026-10-05 | macOS (development machine) | Node 24.12.0 | Fake media streams / canvas context | 12 unit checks | Passed; no hardware exercised |
| 2026-10-05 | macOS (development machine) | Headless Chrome 154.0.8037.97 | Browser-generated synthetic video | 10 page checks: explicit start, video-only capture, named JSON download, import without camera start, invalid import preservation, stop/release, pixel-level horizontal reversal, narrow layout/guide, denied permission, script errors/external requests | Passed; not a physical camera or Windows test |

Hospital restrictions, physical Windows cameras, and mobile devices are not assumed to work because the local Mac or a synthetic headless test passes.

Build 5 verification on 2026-10-05: all 13 Node 24.12.0 unit checks passed. All 9 fullscreen browser checks passed in headless Chrome 154.0.8037.98 with synthetic video, including native and fallback exits, unchanged orientation, capture release, and narrow-screen controls. Desktop and narrow screenshots were inspected. Physical-camera fullscreen acceptance remains pending with the user.

Build 6 verification on 2026-10-09: all 14 Node unit checks passed. Headless Chrome 155.0.8059.39 passed 96 asymmetric-image pixel cases covering every quadrant, both base orientations, both toggles and positive/negative offsets; it also passed independent controls/reset, named schema 2 downloads/imports, invalid import preservation, schema 1 migration, keyboard sliders, fullscreen editing, narrow layout and capture release. All 9 existing fullscreen checks passed. Fullscreen and narrow screenshots were inspected. Only synthetic camera data was used; physical-camera alignment remains for user acceptance.

Build 7 verification on 2026-10-09: all 16 unit checks passed. Headless Chrome 155.0.8059.39 passed 320 synthetic pixel cases for gap-free coverage, upright images and independent mirroring at centered/extreme boundaries with odd/even dimensions. Shared controls, global reset, schema 3 persistence, explicit schema 2 migration, invalid imports, stationary visible video while settings scroll, narrow/short layouts, keyboard sliders and capture stop passed. All 9 fullscreen checks passed. Desktop and narrow screenshots were inspected. Real-camera alignment remains pending.

Build 8 corrects a limitation of the earlier tests: checking coverage and upright images did not detect pane-specific enlargement. The pixel regression now checks exact source coordinates in every quadrant for both camera orientations and all 16 mirror selections across 49 divider positions, with odd/even frames (3,136 cases).

Build 8 verification on 2026-10-11: all 16 unit checks passed. Headless Chrome 155.0.8059.39 passed all 3,136 exact-coordinate pixel cases and the shared-control, JSON migration, layout/scrolling, keyboard and capture-stop integration checks. All 9 fullscreen checks passed. The desktop screenshot was inspected. Tests used synthetic video only; live-camera acceptance remains pending.

Hosted Build 8 verification on 2026-10-11: the HTTPS GitHub Pages application at `https://leemeredith.github.io/AugmentedRealityMirror/web/` passed all 3,136 exact-coordinate pixel cases and shared controls, JSON download/import, legacy conversion, independent scrolling, keyboard and capture-stop checks in headless Chrome 155.0.8059.39. All 9 fullscreen checks also passed. Capture used browser-generated synthetic video, not a physical camera. The project homepage and app JavaScript returned HTTP 200, and the deployed JavaScript identified itself as Browser build 8. Physical-camera and managed-hospital checks remain separate acceptance steps.

## Optional sessions (build 9)

1. Confirm the camera still works without participant information or a session. Select a baseline and start a named session with multiple limb-loss locations. Participant details should lock for that record.
2. Press 1–9 and 0 while watching the view: each press records once, with key 0 meaning 10. No pain (0) records zero. Holding a key, typing a name, using a slider or entering text must not add pain measurements.
3. Save progress, then continue scoring. The progress file should be active with no end timestamp. End session, then select the final score: one after entry and one download request. Repeat download must preserve the session ID and entries.
4. Test a blank name, bilateral locations, Continue session after requesting an end, and New session. New records get new IDs; unnamed filenames begin with the UTC date/time. Stopping the camera must not silently end the session.
5. Test hidden controls/fullscreen scoring and final-score entry using the toolbar. On a narrow screen, the panel still scrolls while the view remains visible.
6. Check JSON with invented data only: before/during/after phases, integer score 0–10, full UTC timestamps, numeric elapsedSeconds, start/end/per-entry mirror snapshots and separate session format. Setup import should reject a session file without changing the view.
7. Request navigation/reload during an active record: the browser should offer its leave warning. A browser crash or ignored warning can still lose undownloaded records. Confirm downloads before closing; no automatic storage is promised.

Build 9 staging verification on 2026-10-20: all 22 Node unit checks passed. Chrome 155.0.8059.85 with synthetic video and invented participants passed the full session flow, key mapping/guards, progress and final downloads, multiple limb locations, anonymous filenames, zero pain, new IDs, end cancellation, camera/session independence and setup/session separation. All 3,136 rendering pixel cases and all 9 fullscreen checks passed. The narrow session screenshot was inspected. Real-camera session acceptance remains pending.

Release check: publish only application sources/docs/tests, never generated participant records. If the hosted page has a script error or loses/mislabels scores, revert the Build 9 commit to restore the previous static application, retaining already downloaded session JSON separately. Verify the Pages deployment and a synthetic baseline/during/final download after publishing.

## Separate camera window / shared name (build 10)

1. Enter an invented participant name in Create session. Save a camera setup and complete a session. Both JSON records must include the same participantName, with that prefix in both filenames. With no name, both files must have empty participantName and date/time filenames.
2. Load a schema 4 setup before starting: the name should fill in. Try a different name while a session is active/completed: the session name must stay and the setup status explain it. New session unlocks the field. Earlier setup labels must not become participant names automatically.
3. Open camera window. Verify the original stream remains live and capture is not requested twice. With the main window in the background, confirm the camera view still updates and pain keys record once. Test popup fullscreen/Escape and final score/download from the popup.
4. Return the view, close the camera window during an active session, and reopen it. Confirm the same capture and record remain. Closing the controller should close the child. A camera page opened directly must explain how to open it from the controller and never acquire a stream.
5. Test blocked pop-ups, small screens, session-dialog keyboard navigation and opening the camera window from fullscreen. The fallback embedded view should remain available and the session controls must not disappear.

Use only invented participant data in automated checks. Real-camera and multiple-monitor acceptance remain user checks.

Build 10 staging verification on 2026-10-20: all 24 Node unit checks passed. Chrome 155.0.8059.85 with synthetic video/invented participants passed the new two-window suite, including one-stream/canvas identity, visible rendering with the controller in the background, camera-window scoring and final download, fullscreen/Escape, active-session close/reopen, return, parent-close cleanup, blocked pop-ups and standalone recovery. Shared-name downloads, schema 4 setup imports, legacy label handling and current-session name protection passed. The existing session suite, 3,136 exact-coordinate rendering cases and all 9 fullscreen checks passed with the new tabs. Camera-window and narrow controller screenshots were inspected. Real-camera and physical window/monitor arrangement remain user acceptance checks.

## Two visible control boxes (build 11)

1. Confirm the name stays above both Camera & view and Session & pain, with a gap between boxes. Both should remain visible without switching tabs. On desktop, Session gets more height. On small screens both sit below the fixed video.
2. Scroll the camera box to its setup files. The Session content, both headings, shared name and live image must stay in place. Scroll Session independently; End remains in its heading and Start/Save/New remain in the fixed bottom row as appropriate.
3. Confirm scoring buttons appear first. Optional limb details can be expanded below them before starting; records and key mappings must still match Build 10. Download a named setup/session pair and check the matching names.
4. Check fullscreen, detached/returned camera, blocked pop-ups and narrow/short windows. Verify each box has a usable content scroller and actions remain inside the Session box. Camera/window placement and records must survive layout changes.
5. Compare browsers using `index.html?v=11` after saving active records. The footer must say Browser build 11. HTML/style/module addresses use release queries so an updated page does not load an old unversioned dependency.


Build 11 staging verification on 2026-10-27: all 24 Node unit checks passed. Chrome 156.0.8078.48 with synthetic video and invented participants passed the new independent-panel checks at desktop, fullscreen, 390×600 and 780×400 sizes. Camera/Session content scrolls independently, headings and video stay fixed, Session has more desktop height, and actions stay inside its box. The versioned CSS/module requests were checked. Existing session/download, detached-window, all 9 fullscreen checks and all 3,136 pixel cases passed. Desktop, active-session and narrow screenshots were inspected. Real-camera/Safari acceptance remains for the user.


## Focused session dialog and compact camera toolbar (build 12)

1. Camera & view is the only scrolling sidebar, on the right. Create session in the toolbar opens a modal with participant name, all four limb-loss sides and descriptions expanded, and baseline pain. On small screens the modal fills the screen. Its header and actions stay fixed as the contents scroll.
2. Close, Escape or backdrop click must preserve form inputs and return focus to Create session / Current session. Tab must stay within the modal or the browser's own interface. Opening the session over camera fullscreen must work; Escape closes the session before exiting expanded view.
3. Start session closes the modal. Current session reopens it with locked identity/limb summary. New session unlocks and retains those fields for review. Both JSON files and their filenames must still share the participant name.
4. Record pain opens a compact score tray beside the camera, with 0–10 buttons. Each choice records once and closes the tray. Key 0 still means 10; No pain (0) records zero. End session in the main toolbar opens the final-score step. Closing that dialog must not discard the session or its end request.
5. The separate camera window uses the same capture/session. Its compact toolbar includes Hide/Show controls, Record pain, Stop, End session, Fullscreen and Return. Hide leaves Show controls reachable; numeric scoring continues. End accepts a final score using that window's tray or keyboard. No inactive-session instruction or repeating main-window scoring message appears over the camera.
6. Save active records before comparing browsers with `index.html?v=12`. The footer must show Browser build 12; all stylesheet and module queries must be v=12. Check a real camera in Safari separately.

Build 12 staging verification on 2026-10-31: all 24 Node checks passed. Headless Chrome 156.0.8078.74 with synthetic video/invented participants passed session-dialog focus, dismissal, retained forms, independent camera scrolling, expanded limb choices, quick scoring, compact toolbars and fullscreen-modal checks at 1440×1000, 390×600 and 780×400. Session/download and two-window suites passed, along with all 9 fullscreen checks and all 3,136 exact-coordinate pixel cases. Desktop and narrow screenshots were inspected. No physical camera or Safari was exercised. The static release can be reverted to the preceding commit if the hosted session or camera flow fails; keep downloaded participant records separately.
