/*
    SPDX-License-Identifier: MIT
*/

#include <KConfigGroup>
#include <KPluginFactory>
#include <KSharedConfig>
#include <KCModule>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QFormLayout>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVariantList>

namespace
{

constexpr int kDefaultBrightness = 70;
constexpr int kDefaultSaturation = 100;

class DimsoftConfig : public KCModule
{
    Q_OBJECT

public:
    explicit DimsoftConfig(QObject *parent, const KPluginMetaData &data)
        : DimsoftConfig(parent, data, {})
    {
    }

    explicit DimsoftConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &)
        : KCModule(parent, data)
    {
        auto *layout = new QFormLayout(widget());

        m_brightness = new QSpinBox(widget());
        m_brightness->setRange(0, 100);
        m_brightness->setSuffix(QStringLiteral("%"));
        layout->addRow(QStringLiteral("Brightness:"), m_brightness);

        m_saturation = new QSpinBox(widget());
        m_saturation->setRange(0, 100);
        m_saturation->setSuffix(QStringLiteral("%"));
        layout->addRow(QStringLiteral("Saturation:"), m_saturation);

        connect(m_brightness, &QSpinBox::valueChanged, this, [this]() {
            setNeedsSave(true);
        });
        connect(m_saturation, &QSpinBox::valueChanged, this, [this]() {
            setNeedsSave(true);
        });
    }

    void load() override
    {
        const KConfigGroup effectGroup(KSharedConfig::openConfig(QStringLiteral("kwinrc")),
                                       QStringLiteral("Effect-dimsoft"));
        const KConfigGroup generalGroup = effectGroup.group(QStringLiteral("General"));

        QSignalBlocker brightnessBlocker(m_brightness);
        QSignalBlocker saturationBlocker(m_saturation);

        m_brightness->setValue(generalGroup.readEntry(QStringLiteral("Brightness"), kDefaultBrightness));
        m_saturation->setValue(generalGroup.readEntry(QStringLiteral("Saturation"), kDefaultSaturation));
        setNeedsSave(false);
    }

    void save() override
    {
        auto config = KSharedConfig::openConfig(QStringLiteral("kwinrc"));
        KConfigGroup effectGroup(config, QStringLiteral("Effect-dimsoft"));
        KConfigGroup generalGroup = effectGroup.group(QStringLiteral("General"));

        generalGroup.writeEntry(QStringLiteral("Brightness"), m_brightness->value());
        generalGroup.writeEntry(QStringLiteral("Saturation"), m_saturation->value());
        config->sync();
        setNeedsSave(false);

        // Apply settings immediately in the running KWin instance.
        const auto message = QDBusMessage::createMethodCall(QStringLiteral("org.kde.KWin"),
                                                            QStringLiteral("/KWin"),
                                                            QStringLiteral("org.kde.KWin"),
                                                            QStringLiteral("reconfigure"));
        QDBusConnection::sessionBus().asyncCall(message);
    }

    void defaults() override
    {
        m_brightness->setValue(kDefaultBrightness);
        m_saturation->setValue(kDefaultSaturation);
        setNeedsSave(true);
    }

private:
    QSpinBox *m_brightness = nullptr;
    QSpinBox *m_saturation = nullptr;
};

} // namespace

K_PLUGIN_CLASS_WITH_JSON(DimsoftConfig, "dimsoft_config.json")

#include "dimsoftconfig.moc"
