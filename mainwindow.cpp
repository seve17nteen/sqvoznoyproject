#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    setWindowTitle("Трекер сна - Сквозной проект");
    resize(700, 500);

    // --- 1. Меню и Панель инструментов ---
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    QAction *exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *helpMenu = menuBar()->addMenu("Справка");
    helpMenu->addAction("О программе");

    QToolBar *toolBar = addToolBar("Главная панель");
    QAction *addAction = toolBar->addAction("Добавить запись");
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddSleepRecord);

    // --- 2. Центральный виджет ---
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);

    // Заголовок
    LogLabel *titleLabel = new LogLabel("Дневник сна", m_centralWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Форма ввода
    QHBoxLayout *formLayout = new QHBoxLayout();

    m_dateEdit = new QLineEdit(m_centralWidget);
    m_dateEdit->setPlaceholderText("Дата (ДД.ММ.ГГГГ)");

    m_sleepTimeEdit = new QLineEdit(m_centralWidget);
    m_sleepTimeEdit->setPlaceholderText("Время засыпания");

    m_wakeTimeEdit = new QLineEdit(m_centralWidget);
    m_wakeTimeEdit->setPlaceholderText("Время пробуждения");

    LogButton *btnAdd = new LogButton("Добавить", m_centralWidget);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddSleepRecord);

    formLayout->addWidget(m_dateEdit);
    formLayout->addWidget(m_sleepTimeEdit);
    formLayout->addWidget(m_wakeTimeEdit);
    formLayout->addWidget(btnAdd);
    mainLayout->addLayout(formLayout);

    // Таблица
    m_table = new QTableWidget(m_centralWidget);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"Дата", "Засыпание", "Пробуждение"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    // --- 3. Статус-бар и Демонстрация 2-й лабы ---
    statusBar()->showMessage("Готово к работе");

    m_demoButton = new LogButton("Демо setParent(nullptr)", this);
    statusBar()->addPermanentWidget(m_demoButton.data());
    connect(m_demoButton.data(), &QPushButton::clicked, this, &MainWindow::demoDetaching);
}

void MainWindow::onAddSleepRecord()
{
    QString date = m_dateEdit->text().trimmed();
    QString sleepTime = m_sleepTimeEdit->text().trimmed();
    QString wakeTime = m_wakeTimeEdit->text().trimmed();

    if (date.isEmpty() || sleepTime.isEmpty() || wakeTime.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, new QTableWidgetItem(date));
    m_table->setItem(row, 1, new QTableWidgetItem(sleepTime));
    m_table->setItem(row, 2, new QTableWidgetItem(wakeTime));

    m_dateEdit->clear();
    m_sleepTimeEdit->clear();
    m_wakeTimeEdit->clear();

    statusBar()->showMessage("Запись успешно добавлена", 3000);
}

// --- ДЕМОНСТРАЦИЯ ДЕРЕВА ОБЪЕКТОВ (2 лаба) ---
void MainWindow::demoDetaching()
{
    qDebug() << "[DEMO] Начинаем тест setParent(nullptr)";

    QWidget *tempOwner = new QWidget(this);
    tempOwner->setObjectName("TempOwner");

    LogLabel *child1 = new LogLabel("Ребенок TempOwner 1", tempOwner);
    LogLabel *child2 = new LogLabel("Отвязанный ребенок", tempOwner);

    qDebug() << "[DEMO] Количество детей tempOwner до detach:" << tempOwner->children().size();

    child2->setParent(nullptr);

    qDebug() << "[DEMO] Количество детей tempOwner после detach:" << tempOwner->children().size();

    delete tempOwner;

    qDebug() << "[DEMO] Удаляем отвязанный child2 вручную";
    delete child2;

    statusBar()->showMessage("Демо setParent(nullptr) выполнено! Смотри Application Output.", 5000);
}