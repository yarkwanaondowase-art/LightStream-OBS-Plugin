# LightStream Graphics — Native OBS Plugin

LightStream is a lightweight native OBS Studio graphics source aimed at low-end Windows PCs.

## V1 included

- Native OBS source: `LightStream Graphics`
- Lower third: name + title
- PNG/JPG logo field
- Fade, slide, wipe, and scale animation modes
- Adjustable animation duration
- Scrolling ticker with adjustable speed
- Clock
- Countdown display field
- Show/hide controls
- Color controls
- 1920x1080 transparent video source
- GitHub Actions Windows build entry point

## Important build note

This repository is source code. A final Windows DLL must be compiled against the exact OBS Studio version/API you intend to run. The GitHub workflow is intentionally separated from the source so the OBS SDK/dependency revision can be pinned and validated before release.

The first runtime target is Windows OBS. The plugin uses OBS-native C++/scene rendering and does not require Electron or Chromium.

## OBS usage after a successful build

1. Install the generated plugin package into the OBS plugin directory.
2. Restart OBS.
3. Add **Sources → LightStream Graphics**.
4. Enter the lower-third name/title and ticker.
5. Enable the graphics you want.

## Development roadmap

The V1 architecture is ready for adding templates, saved presets, keyboard shortcuts, richer text layout, and additional transitions without changing the OBS source model.
