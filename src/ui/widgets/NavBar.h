// ============================================================
// NavBar.h —— 侧边导航栏组件
// ============================================================
#pragma once

#include <QWidget>

class QButtonGroup;
class QIcon;
class QVBoxLayout;

class NavBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavBar(QWidget *parent = nullptr);

    // 追加一个导航项
    int addItem(const QIcon &icon, const QString &toolTip);

    // 程序化选中某项
    void setCurrentIndex(int index);

signals:
    // 当前选中页面
    void currentChanged(int index);

private:
    QButtonGroup *m_group = nullptr;
    QVBoxLayout *m_layout = nullptr;
};