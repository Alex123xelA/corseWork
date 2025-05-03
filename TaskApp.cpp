#include "TaskApp.h"
#include <QDebug>

TaskApp::TaskApp(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Создаем центральный виджет
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // Создаем основной вертикальный layout
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Добавляем растягивающий элемент сверху
    layout->addStretch();

    // Поле для имени
    name = new QLineEdit(centralWidget);
    name->setPlaceholderText("Имя пользователя");  // Добавляем подсказку
    layout->addWidget(name);

    // Поле для пароля
    password = new QLineEdit(centralWidget);
    password->setPlaceholderText("Пароль");       // Добавляем подсказку
    password->setEchoMode(QLineEdit::Password);   // Скрываем ввод пароля
    layout->addWidget(password);

    // Горизонтальный layout для кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    enterButton = new QPushButton("Войти", centralWidget);  // Изменено на "Войти"
    buttonLayout->addWidget(enterButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    // Добавляем растягивающий элемент снизу
    layout->addStretch();

    // Устанавливаем отступы
    layout->setContentsMargins(50, 20, 50, 20);

    // Устанавливаем интервал между виджетами
    layout->setSpacing(15);

    // Подключаем сигнал нажатия кнопки к слоту
    connect(enterButton, &QPushButton::clicked, this, &TaskApp::enterButtonClicked);
}

void TaskApp::enterButtonClicked() {
    strName = name->text();        // Получения текста из поля name
    strPassword = password->text(); // Получение текста из поля password

    passwordsAndNames fileCheck;

    int status = fileCheck.isCorrect(strName, strPassword);
    if (status == 0) {
        viewWindow* window = new viewWindow();  // Создаем окно в куче
        window->createEditorWindow();           // Инициализируем окно
        window->show();                        // Показываем окно
    }
    if (status == 1) {
        // Обработка другого статуса
    }
}

TaskApp::~TaskApp()
{
}