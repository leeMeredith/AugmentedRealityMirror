# Browser session JSON, version 1

Build 9 and later export `arm-rgb-study-session` records separately from `arm-rgb-setup` camera setups. There is no session import/resume or chart renderer in this milestone. Original downloaded files remain on the user's device. A session does not start or stop the camera.

## Record fields

| Field | Meaning |
| --- | --- |
| `format` | `arm-rgb-study-session` |
| `schemaVersion` | `1` |
| `sessionId` | New UUID for each session; retained across all downloads of that session |
| `participantName` | Optional user-entered name or label, otherwise an empty string |
| `limbLoss` | Zero to four `{side, limb, level}` objects; `side` is `right`/`left`, `limb` is `arm`/`leg`, and `level` is optional free text |
| `startedAt`, `endedAt` | ISO 8601 UTC timestamps; `endedAt` is `null` for an unfinished session |
| `status` | `active` for progress downloads, `completed` after a final score |
| `timeZone` | Browser's IANA timezone at session start |
| `viewSettings` | Base camera direction, all quadrant mirror choices, shared center, and preferred camera label at start |
| `endViewSettings` | Same fields at the final score, or `null` during use |
| `painEntries` | Ordered flat measurement rows, described below |

Limb-loss locations use the person's anatomical side; they never automatically toggle screen quadrants. Unselected locations are omitted. Participant details stay fixed during a session; New session allows editing them again while preserving the previous form values for convenience. In build 10 the participant-name field is shared with camera setups (setup schema 4). A camera setup cannot rename an active/completed session; loading a conflicting name keeps the session identity and shows an explanation. Both download types use the same participantName prefix, or a UTC timestamp with no name. The optional setup label is a different field.

## Pain measurement rows

Every row has `phase`, `recordedAt`, `elapsedSeconds`, `painScore`, and `viewSettings`.

| phase | elapsedSeconds | painScore |
| --- | ---: | ---: |
| `before` | 0 | 6 |
| `during` | 60 | 4 |
| `during` | 120 | 10 |
| `after` | 180 | 3 |

Scores are integers 0–10. The baseline timestamp is captured on Start session. Every during-use selection appends a row, even when it repeats the previous score. The final selection appends `after` and completes the record. Settings are copied at each measurement, so later adjustments do not change older measurements.

`recordedAt` is wall-clock UTC time; `elapsedSeconds` uses the browser's monotonic clock, rounded to hundredths of a second. Use elapsed time for within-session charts, since changing the computer clock can change wall-clock timestamps. It includes the time spent choosing the final score after End session is pressed.

## ECharts import contract

A future separate administration tool can use the `painEntries` array as its data rows: `elapsedSeconds` for the horizontal axis, `painScore` for the vertical axis, and `phase`/`recordedAt` for labels or tooltips. Score zero is a real measurement. Keep the participant name, limb-loss list and `sessionId` as context for the series. Nested `viewSettings` are optional context, not chart coordinates.

When collecting a folder of downloads, group by `sessionId` first. Repeated final downloads are the same session, not additional sessions. A final record supersedes its earlier progress snapshots; do not concatenate those arrays or duplicate the measurements. Preserve separate sessions even if participant names match or are empty. The schema is distinct from native desktop study records: a later importer must map those explicitly.

## Download and retention behavior

Named filenames start with a sanitized participant name, followed by the session start date/time. Unnamed filenames contain only the start date/time, including UTC `Z` and milliseconds, for example `2026-10-20T15-30-00-123Z.json`. Unicode letters/numbers are kept, path characters are removed, and long name prefixes are shortened to fit common filesystem limits. Browsers may append a number on repeated downloads.

End session requests the download after the final score. Save session JSON can request the same completed record again or an unfinished progress snapshot. A download request is not confirmation that a user accepted a save dialog. Check the browser's Downloads before closing or starting a new session.

Records live only in the current tab. The app does not automatically save them in localStorage, IndexedDB or a server. A navigation/reload warning is requested while a session is active; browser behavior varies. Closing, reloading or a crash can lose undownloaded measurements. No video or audio is included in these records, and generated participant JSON must not be committed to the source repository.

The separate camera window holds no session model: key presses and End session act on the controller's session. Closing/returning that window preserves records and moves the same canvas back. Keep the controller open until downloads have been checked.
