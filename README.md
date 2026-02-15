# KWin Effect: dimsoft

`dimsoft` applies configurable brightness and saturation to managed windows.

## C++ effect (Plasma 6)

The C++ implementation is in `cpp/` and uses `paintWindow(...)`.

### Prerequisites

- `kwin-dev`
- `cmake`
- `g++`
- `qt6-base-dev`
- `qt6-declarative-dev`
- `libkf6config-dev`
- `libkf6coreaddons-dev`
- `libkf6kcmutils-dev`

### Build

```bash
cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build -j"$(nproc)"
```

### Install system-wide (/usr)

```bash
sudo cmake --install cpp/build --prefix /usr
```

Plugin install path:

- `/usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/plugins/dimsoft.so`

### Configure values

You can configure the effect in **System Settings -> Window Management -> Desktop Effects -> dimsoft (settings)**.

The effect reads values from:

- `[Effect-dimsoft][General] Brightness` (0..100)
- `[Effect-dimsoft][General] Saturation` (0..100)

Example:

```bash
kwriteconfig6 --file kwinrc --group Plugins --key dimsoftEnabled true
kwriteconfig6 --file kwinrc --group "Effect-dimsoft" --group "General" --key Brightness 70
kwriteconfig6 --file kwinrc --group "Effect-dimsoft" --group "General" --key Saturation 100
qdbus6 org.kde.KWin /KWin reconfigure
```

`100` means no change
