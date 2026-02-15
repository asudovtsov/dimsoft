"use strict";

function clamp(value, min, max) {
    return Math.min(Math.max(value, min), max);
}

var globalDimEffect = {
    loadConfig: function () {
        globalDimEffect.duration = animationTime(250);
        globalDimEffect.brightness = clamp(effect.readConfig("Brightness", 67) / 100.0, 0.0, 1.0);
        globalDimEffect.saturation = clamp(effect.readConfig("Saturation", 67) / 100.0, 0.0, 1.0);
    },
    startAnimation: function (window) {
        if (!window || !window.managed) {
            return;
        }

        if (window.globalDimAnimation) {
            cancel(window.globalDimAnimation);
        }

        window.globalDimAnimation = set({
            window: window,
            curve: QEasingCurve.InOutSine,
            duration: globalDimEffect.duration,
            keepAlive: false,
            animations: [
                {
                    type: Effect.Saturation,
                    to: globalDimEffect.saturation
                },
                {
                    type: Effect.Brightness,
                    to: globalDimEffect.brightness
                }
            ]
        });
    },
    startAnimationInstant: function (window) {
        globalDimEffect.startAnimation(window);

        if (window.globalDimAnimation) {
            complete(window.globalDimAnimation);
        }
    },
    cancelAnimationInstant: function (window) {
        if (!window || !window.globalDimAnimation) {
            return;
        }

        cancel(window.globalDimAnimation);
        delete window.globalDimAnimation;
    },
    applyToAllWindowsInstant: function () {
        var windows = effects.stackingOrder;
        for (var i = 0; i < windows.length; ++i) {
            globalDimEffect.startAnimationInstant(windows[i]);
        }
    },
    slotWindowAdded: function (window) {
        window.windowDesktopsChanged.connect(() => {
            globalDimEffect.startAnimationInstant(window);
        });
        window.minimizedChanged.connect(() => {
            if (window.minimized) {
                globalDimEffect.cancelAnimationInstant(window);
                return;
            }
            globalDimEffect.startAnimationInstant(window);
        });

        globalDimEffect.startAnimationInstant(window);
    },
    slotConfigChanged: function () {
        globalDimEffect.loadConfig();
        globalDimEffect.applyToAllWindowsInstant();
    },
    init: function () {
        globalDimEffect.loadConfig();

        effect.configChanged.connect(globalDimEffect.slotConfigChanged);
        effects.windowAdded.connect(globalDimEffect.slotWindowAdded);

        for (const window of effects.stackingOrder) {
            globalDimEffect.slotWindowAdded(window);
        }
        globalDimEffect.applyToAllWindowsInstant();
    }
};

globalDimEffect.init();
