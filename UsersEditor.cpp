#include "TaskApp.h"


UsersEditor::UsersEditor() 
{
    // Главный вертикальный layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 1. Горизонтальный слайдер наверху
    QSlider* topSlider = new QSlider(Qt::Horizontal, this);
    mainLayout->addWidget(topSlider);

    // Горизонтальный layout для нижней части
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    // 2. Три кнопки слева
    QVBoxLayout* leftButtonsLayout = new QVBoxLayout();

    QPushButton* button1 = new QPushButton("Добавить", this);
    QPushButton* button2 = new QPushButton("Удалить", this);
    QPushButton* button3 = new QPushButton("Сохранить", this);
    leftButtonsLayout->addWidget(button1);
    leftButtonsLayout->addWidget(button2);
    leftButtonsLayout->addWidget(button3);
    bottomLayout->addLayout(leftButtonsLayout);

    // 3. Центральная часть с таблицей и вертикальным слайдером
    QHBoxLayout* centerLayout = new QHBoxLayout();

    // Таблица в центре
    WorcersTable = new QTableWidget(5, 3, this);
    centerLayout->addWidget(WorcersTable);

    // Вертикальный слайдер в центре
    QSlider* centerSlider = new QSlider(Qt::Vertical, this);
    centerLayout->addWidget(centerSlider);

    bottomLayout->addLayout(centerLayout, 1); // Растягиваем центральную часть

    // 4. Правая часть с таблицей и вертикальным слайдером
    QHBoxLayout* rightLayout = new QHBoxLayout();

    // Таблица справа
    ManagersTable = new QTableWidget(5, 2, this);
    rightLayout->addWidget(ManagersTable);

    // Вертикальный слайдер справа
    QSlider* rightSlider = new QSlider(Qt::Vertical, this);
    rightLayout->addWidget(rightSlider);

    bottomLayout->addLayout(rightLayout);

    // Добавляем нижнюю часть в главный layout
    mainLayout->addLayout(bottomLayout, 1);
}
void UsersEditor::add() 
{

}
void UsersEditor::remote() 
{

}
void UsersEditor::edit() 
{
	
}