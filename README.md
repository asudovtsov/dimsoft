# KWin Effect: dimsoft

`dimsoft` is a KWin Wayland effect that applies configurable brightness and saturation to all windows.

## What it does

- Applies `Brightness` and `Saturation` to all managed windows in the current session.
- Applies the same values to newly opened windows automatically.
- Runs only in a Wayland Plasma session (`kwin_wayland`).

## Effect settings

- `Brightness` (0..100)
- `Saturation` (0..100)

`100` means no change. Lower values dim/desaturate stronger.

## Install

1. Install the effect package:
   - `kpackagetool6 --type KWin/Effect --install /home/asudovtsov/Projects/self/dimsoft`
2. Open **System Settings -> Window Management -> Desktop Effects**.
3. Enable **dimsoft**.
4. Open effect settings and tune `Brightness` / `Saturation`.

## Update after edits

- `kpackagetool6 --type KWin/Effect --upgrade /home/asudovtsov/Projects/self/dimsoft`

## Remove

- `kpackagetool6 --type KWin/Effect --remove dimsoft`
