/*
    SPDX-License-Identifier: MIT
*/

#pragma once

#include "effect/effect.h"

namespace KWin {

class DimsoftEffect : public Effect {
 public:
  DimsoftEffect();

  void reconfigure(ReconfigureFlags flags) override;
  void paintWindow(const RenderTarget &renderTarget,
                   const RenderViewport &viewport, EffectWindow *window,
                   int mask, QRegion region, WindowPaintData &data) override;
  bool isActive() const override;

 private:
  void loadConfig();
  bool shouldAffectWindow(const EffectWindow *window) const;

  qreal m_brightness = 0.7;
  qreal m_saturation = 1.0;
};

}  // namespace KWin
