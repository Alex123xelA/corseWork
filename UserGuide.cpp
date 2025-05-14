#include "UserGuide.h"

UserGuide::UserGuide()
{
    // Создаем центральный виджет
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Главный горизонтальный layout
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // получение данных страниц руководства

    QFile file("GuideText.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"error";

    QString str = file.readAll();

    for (QString elem : str.split("~~~")) 
    {
        pages.append(elem.split("^^"));
    }
    qDebug() << pages;

    // Создаем левую панель с кнопками
    QVBoxLayout* leftButtonLayout = new QVBoxLayout();
    for (int i = 0; i < pages.size(); ++i) {
        QPushButton* button = new QPushButton(pages[i][0], this);
        leftButtonLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, i]() {
            this->openPage(i);
            });
    }
    leftButtonLayout->addStretch(); // Добавляем растяжку внизу

    // Создаем правую часть с текстовым полем
    QVBoxLayout* rightLayout = new QVBoxLayout();

    // Заголовок сверху по центру
    titleLabel = new QLabel("Заголовок окна", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(16);
    titleLabel->setFont(font);

    // Большое текстовое поле
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    // Нижняя панель с кнопками "назад" и "вперед"
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QPushButton* backButton = new QPushButton("Назад", this);
    QPushButton* forwardButton = new QPushButton("Вперед", this);

    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch(); // Растягиваем пространство между кнопками
    bottomLayout->addWidget(forwardButton);

    // Добавляем элементы в правый layout
    rightLayout->addWidget(titleLabel);
    rightLayout->addWidget(textEdit);
    rightLayout->addLayout(bottomLayout);

    // Добавляем левую и правую части в главный layout
    mainLayout->addLayout(leftButtonLayout, 1); // Левая часть занимает 1 часть
    mainLayout->addLayout(rightLayout, 3);     // Правая часть занимает 3 части

    //открытие первой страницы
    openPage(0);

    // Настройки окна
    setWindowTitle("Руководство по работе с программой");
    resize(800, 600);
    connect(backButton, &QPushButton::clicked, this, &UserGuide::previous);
    connect(forwardButton, &QPushButton::clicked, this, &UserGuide::next);
}

void UserGuide::openPage(int page) 
{
    currentPage = page;
    titleLabel->setText(pages[page][0]);
    textEdit->setText(pages[page][1]);
}

void UserGuide::next() 
{
    if (currentPage != pages.size() - 1)
        openPage(currentPage + 1);
}

void UserGuide::previous() 
{
    if (currentPage != 0)
        openPage(currentPage - 1);
}