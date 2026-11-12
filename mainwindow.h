#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QTableWidget>
#include <QLineEdit>
#include <QTimer>
#include <QMetaObject>
#include <QList>
#include <QHBoxLayout>
#include "widgets.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddSleepRecord();
    void demoDetaching();

    // Новое из 3-й лабы (QTimer и динамика):
    void updateClock();          // Слот для циклического таймера
    void showSleepReminder();    // Демонстрация singleShot
    void addQuickRecordButton(); // Динамическое создание кнопок
    void clearDynamicButtons();  // Разрыв соединений и удаление

private:
    void setupUi();

    QWidget *m_centralWidget;
    QTableWidget *m_table;
    QLineEdit *m_dateEdit;
    QLineEdit *m_sleepTimeEdit;
    QLineEdit *m_wakeTimeEdit;

    QPointer<LogButton> m_demoButton;

    // Переменные для 3-й лабы:
    QTimer *m_clockTimer;                 // Таймер обновления времени
    QWidget *m_dynamicButtonsContainer;   // Контейнер для кнопок
    QHBoxLayout *m_dynamicButtonsLayout;  // Слой для кнопок
    QList<QMetaObject::Connection> m_dynamicConnections; // Хранилище ID соединений
};

#endif // MAINWINDOW_H