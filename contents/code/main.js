"use strict";

function clamp(value, min, max) {
    return Math.min(Math.max(value, min), max);
}

var dimsoftEffect = {
    loadConfig: function () {
        dimsoftEffect.duration = animationTime(0);
        dimsoftEffect.brightness = clamp(effect.readConfig("Brightness", 67) / 100.0, 0.0, 1.0);
        dimsoftEffect.saturation = clamp(effect.readConfig("Saturation", 67) / 100.0, 0.0, 1.0);
    },
    startAnimation: function (window) {
        if (!window || !window.managed || window.deleted || window.minimized || !window.visible) {
            dimsoftEffect.cancelAnimationInstant(window);
            return;
        }

        if (window.dimsoftAnimation) {
            cancel(window.dimsoftAnimation);
        }

        window.dimsoftAnimation = set({
            window: window,
            curve: QEasingCurve.InOutSine,
            duration: dimsoftEffect.duration,
            keepAlive: true,
            animations: [
                {
                    type: Effect.Saturation,
                    to: dimsoftEffect.saturation
                },
                {
                    type: Effect.Brightness,
                    to: dimsoftEffect.brightness
                }
            ]
        });
    },
    startAnimationInstant: function (window) {
        dimsoftEffect.startAnimation(window);

        if (window.dimsoftAnimation) {
            complete(window.dimsoftAnimation);
        }
    },
    cancelAnimationInstant: function (window) {
        if (!window || !window.dimsoftAnimation) {
            return;
        }

        cancel(window.dimsoftAnimation);
        delete window.dimsoftAnimation;
    },
    applyToAllWindowsInstant: function () {
        var windows = effects.stackingOrder;
        for (var i = 0; i < windows.length; ++i) {
            dimsoftEffect.cancelAnimationInstant(windows[i]);
            dimsoftEffect.startAnimationInstant(windows[i]);
        }
    },
    slotDesktopChanged: function () {
        dimsoftEffect.applyToAllWindowsInstant();
    },
    slotWindowAdded: function (window) {
        window.windowDesktopsChanged.connect(() => {
            // dimsoftEffect.cancelAnimationInstant(window);
            dimsoftEffect.startAnimationInstant(window);
        });
        window.minimizedChanged.connect(() => {
            // if (window.minimized) {
            //     dimsoftEffect.cancelAnimationInstant(window);
            //     return;
            // }
            dimsoftEffect.startAnimationInstant(window);
        });

        dimsoftEffect.startAnimationInstant(window);
    },
    slotConfigChanged: function () {
        dimsoftEffect.loadConfig();
        dimsoftEffect.applyToAllWindowsInstant();
    },
    init: function () {
        dimsoftEffect.loadConfig();

        effect.configChanged.connect(dimsoftEffect.slotConfigChanged);
        effects.windowAdded.connect(dimsoftEffect.slotWindowAdded);
        effects.windowClosed.connect(dimsoftEffect.cancelAnimationInstant);
        effects.desktopChanged.connect(dimsoftEffect.slotDesktopChanged);

        for (const window of effects.stackingOrder) {
            dimsoftEffect.slotWindowAdded(window);
        }
        dimsoftEffect.applyToAllWindowsInstant();
    }
};

dimsoftEffect.init();
