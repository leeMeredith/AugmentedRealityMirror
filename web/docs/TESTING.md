# Browser acceptance checklist

Automated unit tests use fake streams and canvas contexts. Headless checks can verify page behavior with synthetic camera frames. Neither substitutes for a person checking a real camera.

## Automated unit checks

```sh
node --test tests/*.test.js
```

These cover invalid setup imports, Unicode/unsafe filenames, stopping while permission is pending, failed capture, disconnects, and mirror transform cleanup.

## Manual check on each available computer

Record the operating system, browser/version, camera, date, and result. Test desktop Mac and Windows first. Tablet, phone, and managed hospital environments remain separate targets.

1. Open the local development URL or deployed HTTPS link. Record the build label in the footer. Confirm no camera permission prompt appears until Start camera is pressed.
2. Start and allow the camera. Confirm a live view and the browser's camera-use indicator.
3. On a fresh page, raise one hand and compare the initial view with Reverse view pressed once, then twice. Build 4 should start in the opposite direction from build 3. Check horizontal reversal with asymmetric text or an object. Confirm the full image fits without stretching or cropping.
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

## Record actual results

Only record tests that were actually performed. Suggested columns:

| Date | OS | Browser/version | Camera or synthetic source | Checks | Result/limitations |
| --- | --- | --- | --- | --- | --- |
| 2026-10-05 | macOS (development machine) | Node 24.12.0 | Fake media streams / canvas context | 12 unit checks | Passed; no hardware exercised |
| 2026-10-05 | macOS (development machine) | Headless Chrome 154.0.8037.97 | Browser-generated synthetic video | 10 page checks: explicit start, video-only capture, named JSON download, import without camera start, invalid import preservation, stop/release, pixel-level horizontal reversal, narrow layout/guide, denied permission, script errors/external requests | Passed; not a physical camera or Windows test |

Hospital restrictions, physical Windows cameras, and mobile devices are not assumed to work because the local Mac or a synthetic headless test passes.
