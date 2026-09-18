// ============================================================
// AppButton.cpp —— 通用按钮组件实现
// ============================================================

#include "AppButton.h"

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