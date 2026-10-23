#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QTableWidget>
#include <QLineEdit>
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

private:
    void setupUi();

    QWidget *m_centralWidget;
    QTableWidget *m_table;
    QLineEdit *m_dateEdit;
    QLineEdit *m_sleepTimeEdit;
    QLineEdit *m_wakeTimeEdit;

    QPointer<LogButton> m_demoButton;
};

#endif // MAINWINDOW_H