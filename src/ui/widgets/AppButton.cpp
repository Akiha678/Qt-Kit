// ============================================================
// AppButton.cpp —— 通用按钮组件实现
// ============================================================

#include "AppButton.h"

#include <QApplication>
#include <QMetaEnum>
#include <QStyle>

AppButton::AppButton(const QString &text, Variant variant, QWidget *parent)
    : QPushButton(text, parent)
{
    applyVariant(variant);
}

AppButton::Variant AppButton::variant() const
{
    // 从动态属性反查当前变体；找不到时回退 Primary
    const QString key = property("variant").toString();
    const QMetaEnum me = QMetaEnum::fromType<Variant>();
    bool ok = false;
    const int value = me.keysToValue(key.toUtf8().constData(), &ok);
    return ok ? static_cast<Variant>(value) : Variant::Primary;
}

void AppButton::setVariant(Variant variant)
{
    applyVariant(variant);
}

void AppButton::applyVariant(Variant variant)
{
    const QMetaEnum me = QMetaEnum::fromType<Variant>();
    const QString key = QString::fromLatin1(me.valueToKey(static_cast<int>(variant)));

    if (property("variant") == key)
        return; // 没变化就跳过，避免无谓重绘

    setProperty("variant", key);

    style()->unpolish(this);
    style()->polish(this);
    update();
}

void AppButton::setCompact(bool compact)
{
    if (m_compact == compact)
        return;
    m_compact = compact;
    applyCompact(compact);
}

void AppButton::applyCompact(bool compact)
{
    // 高度：紧凑 28px，普通恢复自动
    setMinimumHeight(compact ? 28 : 0);
    setMaximumHeight(compact ? 28 : QWIDGETSIZE_MAX);

    // 字号：紧凑 9pt，普通恢复应用默认字号
    QFont f = font();
    f.setPointSize(compact ? 9 : QApplication::font().pointSize());
    setFont(f);
}