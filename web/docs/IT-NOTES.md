# IT and deployment notes

ARM's first browser milestone is a static HTML/CSS/JavaScript application. It requests video from a camera after an explicit Start action. It never requests a microphone. No account, browser extension, installer, or application server is required for a hosted user.

## Published site

- Project information: https://leemeredith.github.io/AugmentedRealityMirror/
- Browser application: https://leemeredith.github.io/AugmentedRealityMirror/web/
- User guide: https://leemeredith.github.io/AugmentedRealityMirror/web/guide.html

GitHub Pages publishes from the root of `master`. Keep the `/web/` path in links intended to open the camera application. Pushes to `master` trigger the Pages build; check the repository's Actions results and the live build label before announcing an update.

## Network and data

- Host all files under `web` at one HTTPS origin, preserving relative paths and JavaScript MIME types. Only the static application and guide are fetched.
- The application contains no analytics, remote fonts, CDNs, upload endpoint, WebSocket connection, or application API. Its Content Security Policy sets `connect-src 'none'`.
- Camera frames are rendered locally to a canvas. This milestone does not record video.
- A setup JSON download includes an optional user-entered label, a camera label preference, mirror choice, four horizontal mirror choices and shared center settings, and timestamp. Imported files are processed locally, with a 64 KiB size limit and format validation.
- Session JSON downloads can include an optional participant name, limb-loss locations/descriptions, timestamps, pain scores and mirror settings. Records stay in memory until explicitly downloaded. A filename can contain the name entered by the user. There is no upload or remote participant database. See [session format](SESSION-FORMAT.md).
- A user-opened, same-origin camera window shares the controller's existing canvas/stream. The controller retains session records and handles downloads. Camera-window pop-ups can be blocked by browser policy; the embedded layout remains available. No identity or records are sent to another origin.
- There is no automatic localStorage/IndexedDB storage or service worker. Setup persistence requires an explicit downloaded file. This milestone does not promise offline loading.
- The host may still log standard page requests, including IP addresses. Browser extensions, managed-device software, and cloud-sync folders are outside this app's control.

## Permissions and restrictions

Camera access needs a secure browser context and user permission. Use HTTPS for deployed access; localhost is suitable for developer testing. Site allowlisting, operating-system privacy settings, or enterprise camera policies can still block access. Downloads/imports may also be restricted on managed devices. This app does not bypass those restrictions.

Prefer a top-level page rather than an embedded iframe, which can add camera permission restrictions. A host can add these response headers after checking its configuration:

```text
Permissions-Policy: camera=(self), microphone=()
Referrer-Policy: no-referrer
X-Content-Type-Options: nosniff
Content-Security-Policy: frame-ancestors 'none'
```

The last header supplements the page's existing meta CSP; do not replace its other restrictions. `frame-ancestors` requires an HTTP response header.

No clinical effectiveness, regulatory status, or institutional approval is implied by these implementation details. Validate actual managed-computer access when an institution is available.
