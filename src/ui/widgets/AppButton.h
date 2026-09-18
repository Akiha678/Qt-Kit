// ============================================================
// AppButton.h —— 通用按钮组件
// ============================================================
#pragma once

#include <QPushButton>

class AppButton : public QPushButton
{
    Q_OBJECT

public:
    // 变体 = 语义类型：
    //   Primary   主操作（强调色实心）
    //   Secondary 次操作（描边/透明）
    //   Danger    危险操作（红色）
    enum class Variant { Primary, Secondary, Danger };
    Q_ENUM(Variant) // 注册进元对象系统：QMetaEnum 能拿到枚举名 → 用作 QSS 属性值

    explicit AppButton(const QString &text = QString(),
                       Variant variant = Variant::Primary,
                       QWidget *parent = nullptr);

    Variant variant() const;
    void setVariant(Variant variant); // 运行时可改（会触发 repolish 重算样式）

    // 紧凑模式：用于表格行、工具栏等空间小的场景
    // （更矮、字号更小；默认普通尺寸）
    void setCompact(bool compact = true);

private:
    void applyVariant(Variant variant);
    void applyCompact(bool compact);

    bool m_compact = false;
};