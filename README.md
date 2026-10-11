# Augmented Reality Mirror (ARM)

Augmented Reality Mirror is a project by Lee Meredith exploring digital mirror images and visual feedback for people with limb loss and phantom limb pain. It began as a Kinect and openFrameworks prototype and now also includes a browser-based RGB camera version.

[Watch the original demonstration](https://vimeo.com/49252026) · [Browser application and instructions](web/README.md) · [Original project README](docs/history/README.original.md)

## Original work

The original ARM used a Kinect to capture an intact limb and create a digitally mirrored limb image. Its point-cloud view let the viewer change perspective around the captured scene, exploring possibilities beyond a fixed physical mirror.

The historical work was built with openFrameworks and ofxKinect. The original README described the intended application to phantom limb pain, linked the demonstration above, and acknowledged the people who helped with the project. That README is preserved verbatim in [the historical archive](docs/history/README.original.md), including its original wording and installation instructions. Those instructions describe the older project, not the current build.

The project is an experimental prototype; its intended use and historical demonstration do not establish clinical effectiveness. A movable viewpoint cannot show surfaces the depth camera did not capture.

## Recognition

Original acknowledgments:

**Kareemah Batts, Desmond Heeley, Joe Labrie, Michael Lichter, Jennifer Eftychiou PT DPT, and Jeffrey Heckman DO.**

These acknowledgments preserve the project's past contributions and do not imply current participation or endorsement.

## Continuing the project

The current work starts with an ordinary RGB camera so the viewing controls and saved setups can be used without a depth camera. Depth-camera and point-cloud work remain part of the project's history and possible future development.

There are two applications in the current checkout:

| Location | Application |
| --- | --- |
| [`web/`](web/) | Browser version: camera start/stop, full-image mirror masks, shared dividers, fullscreen, independently scrolling controls, and JSON setup import/export. No video recording or pain-entry interface in this version. |
| [`src/`](src/) and `AugmentedRealityMirrorRGB.xcodeproj` | Native openFrameworks RGB rebuild: regional mirroring, split-screen comparison, JSON study-session controls and recording code. |

The browser version is **Build 8**. Each selected region reveals a normal or horizontally reflected full camera image. Moving the dividers changes which image is visible without enlarging or shifting the underlying images. Saved setups preserve the camera direction, quadrant choices and shared divider positions.

### Try the browser version locally

From the repository's `web` folder:

```sh
python3 serve.py
```

Then open `http://localhost:8000`, press **Start camera**, and allow camera access. The [browser README](web/README.md) has the full instructions. A hosted browser link is the next publishing step; GitHub Pages has not yet been enabled.

For the native RGB project, use openFrameworks **0.12.1** with **ofxGui**, as listed in [`addons.make`](addons.make). Its source and Xcode project remain alongside the browser version.

## Documentation and history

- [Browser user guide](web/guide.html)
- [Bounded development path](web/docs/DEVELOPMENT.md)
- [Testing and verification record](web/docs/TESTING.md)
- [Browser data handling and deployment notes](web/docs/IT-NOTES.md)
- [Original README, preserved verbatim](docs/history/README.original.md)
- [Kinect restoration README and documentation at their historical revision](https://github.com/leeMeredith/AugmentedRealityMirror/blob/ccebf7d37c50c59f1b4f40012154619c3747d2d0/README.md)

The Git history retains the earlier Kinect work. The historical snapshot above includes its build notes, project overview, roadmap and neutral possible-collaborator information. Those documents describe that revision, not the current RGB source tree.

## Data and licensing

The browser processes camera images locally and saves setups only when requested. Use invented participant information when developing or demonstrating the native study-session features, and keep personal records out of the repository.

A project license has not yet been selected. Dependency and media attribution requirements still apply.
