/*
    SPDX-License-Identifier: MIT
*/

#include "dimsofteffect.h"

#include <KConfigGroup>
#include <QtGlobal>

#include "effect/effecthandler.h"
#include "effect/effectwindow.h"

namespace KWin {

namespace {

constexpr qreal kDefaultBrightness = 0.7;
constexpr qreal kDefaultSaturation = 1.;
constexpr qreal kNoChange = 1.0;

qreal toUnitRange(int percent, qreal fallback) {
  const qreal raw = (percent >= 0 ? percent : qRound(fallback * 100.0)) / 100.0;
  return qBound<qreal>(0.0, raw, 1.0);
}

}  // namespace

DimsoftEffect::DimsoftEffect() { loadConfig(); }

void DimsoftEffect::reconfigure(ReconfigureFlags flags) {
  Effect::reconfigure(flags);
  loadConfig();
  effects->addRepaintFull();
}

void DimsoftEffect::paintWindow(const RenderTarget &renderTarget,
                                const RenderViewport &viewport,
                                EffectWindow *window, int mask, QRegion region,
                                WindowPaintData &data) {
  data.multiplyBrightness(m_brightness);
  data.multiplySaturation(m_saturation);

  effects->paintWindow(renderTarget, viewport, window, mask, region, data);
}

bool DimsoftEffect::isActive() const {
  return !qFuzzyCompare(m_brightness, kNoChange) ||
         !qFuzzyCompare(m_saturation, kNoChange);
}

void DimsoftEffect::loadConfig() {
  const KConfigGroup effectGroup(effects->config(),
                                 QStringLiteral("Effect-dimsoft"));
  const KConfigGroup generalGroup =
      effectGroup.group(QStringLiteral("General"));

  m_brightness =
      toUnitRange(generalGroup.readEntry(QStringLiteral("Brightness"),
                                         qRound(kDefaultBrightness * 100.0)),
                  kDefaultBrightness);
  m_saturation =
      toUnitRange(generalGroup.readEntry(QStringLiteral("Saturation"),
                                         qRound(kDefaultSaturation * 100.0)),
                  kDefaultSaturation);
}

}  // namespace KWin
