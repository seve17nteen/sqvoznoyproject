#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDebug>
#include <QLabel>
#include <QPushButton>

class LogLabel : public QLabel {
public:
    explicit LogLabel(const QString &text, QWidget *parent = nullptr)
        : QLabel(text, parent) {
        qDebug() << "[CREATE] LogLabel" << text;
    }
    ~LogLabel() override { qDebug() << "[DESTROY] LogLabel"; }
};

class LogButton : public QPushButton {
public:
    explicit LogButton(const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent) {
        qDebug() << "[CREATE] LogButton" << text;
    }
    ~LogButton() override { qDebug() << "[DESTROY] LogButton"; }
};

#endif // WIDGETS_H