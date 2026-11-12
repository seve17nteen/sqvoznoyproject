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
#include <QDateTime>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();

    // --- ИНИЦИАЛИЗАЦИЯ QTimer (3 лаба) ---
    m_clockTimer = new QTimer(this);
    // Лямбда-слот с контекстом this! Обновляет время каждую секунду.
    connect(m_clockTimer, &QTimer::timeout, this, [this]() {
        updateClock();
    });
    m_clockTimer->start(1000); // 1000 мс = 1 секунда
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
    if (m_clockTimer->isActive()) {
        m_clockTimer->stop();
    }
}

void MainWindow::setupUi()
{
    setWindowTitle("Трекер сна - Сквозной проект");
    resize(800, 600);

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

    // --- 3. Динамические кнопки (3 лаба) ---
    mainLayout->addWidget(new QLabel("Быстрые записи (динамические кнопки):", m_centralWidget));
    m_dynamicButtonsContainer = new QWidget(m_centralWidget);
    m_dynamicButtonsLayout = new QHBoxLayout(m_dynamicButtonsContainer);
    m_dynamicButtonsLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_dynamicButtonsContainer);

    // Кнопки управления динамикой
    QHBoxLayout *dynamicControls = new QHBoxLayout();
    LogButton *btnReminder = new LogButton("Напомнить о сне (singleShot)", m_centralWidget);
    connect(btnReminder, &QPushButton::clicked, this, &MainWindow::showSleepReminder);

    LogButton *btnAddQuick = new LogButton("+ Быстрая запись", m_centralWidget);
    connect(btnAddQuick, &QPushButton::clicked, this, &MainWindow::addQuickRecordButton);

    LogButton *btnClear = new LogButton("Очистить быстрые записи", m_centralWidget);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearDynamicButtons);

    dynamicControls->addWidget(btnReminder);
    dynamicControls->addWidget(btnAddQuick);
    dynamicControls->addWidget(btnClear);
    mainLayout->addLayout(dynamicControls);

    // --- 4. Статус-бар и 2-я лаба ---
    statusBar()->showMessage("Готово к работе");
    m_demoButton = new LogButton("Демо setParent(nullptr)", this);
    statusBar()->addPermanentWidget(m_demoButton.data());
    connect(m_demoButton.data(), &QPushButton::clicked, this, &MainWindow::demoDetaching);
}

// --- Слот таймера (3 лаба) ---
void MainWindow::updateClock()
{
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    statusBar()->showMessage("Текущее время: " + timeStr);
}

// --- Напоминание через singleShot (3 лаба) ---
void MainWindow::showSleepReminder()
{
    statusBar()->showMessage("Таймер напоминания установлен на 5 секунд...", 5000);
    qDebug() << "[TIMER] Установлен singleShot на 5000 мс";

    // Одноразовый таймер
    QTimer::singleShot(5000, this, [this]() {
        QMessageBox::information(this, "Напоминание", "Пора спать! Не забудьте внести запись в дневник.");
        qDebug() << "[TIMER] singleShot сработал - показано напоминание";
    });
}

// --- Динамическое создание (3 лаба) ---
void MainWindow::addQuickRecordButton()
{
    // Создаем кнопку динамически
    int hours = QRandomGenerator::global()->bounded(4, 12); // Случайное количество часов
    LogButton *btn = new LogButton(QString("Спал %1 ч.").arg(hours), m_dynamicButtonsContainer);
    m_dynamicButtonsLayout->addWidget(btn);

    // Сохраняем QMetaObject::Connection!
    QMetaObject::Connection conn = connect(btn, &QPushButton::clicked, this, [this, hours, btn]() {
        QString date = QDateTime::currentDateTime().toString("dd.MM.yyyy");
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(date));
        m_table->setItem(row, 1, new QTableWidgetItem(QString("%1:00").arg(24 - hours)));
        m_table->setItem(row, 2, new QTableWidgetItem("07:00"));
        statusBar()->showMessage(QString("Добавлена быстрая запись: %1 часов").arg(hours), 3000);
    });

    m_dynamicConnections.append(conn); // Запоминаем связь
    qDebug() << "[DYNAMIC] Создано соединение для кнопки. Всего соединений:" << m_dynamicConnections.size();
}

// --- Разрыв соединений (3 лаба) ---
void MainWindow::clearDynamicButtons()
{
    // 1. Явно рвем все соединения
    for (QMetaObject::Connection &c : m_dynamicConnections) {
        if (c) disconnect(c);
    }
    m_dynamicConnections.clear();

    // 2. Удаляем кнопки из Layout и вызываем deleteLater
    QLayoutItem *item;
    while ((item = m_dynamicButtonsLayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater(); // Безопасное удаление
        }
        delete item;
    }
    statusBar()->showMessage("Динамические соединения разорваны, кнопки удалены.", 3000);
    qDebug() << "[DYNAMIC] Все динамические кнопки удалены.";
}

// --- Старые методы из 1 и 2 лабы ---
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