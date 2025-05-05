#include "TaskApp.h"


UsersEditor::UsersEditor() 
{
    // Главный вертикальный layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 1. Горизонтальный слайдер наверху
    topSlider = new QSlider(Qt::Horizontal, this);
    mainLayout->addWidget(topSlider);
    topSlider->setRange(1, 2);
    topSlider->setValue(1);

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
    WorkersSlider = new QSlider(Qt::Vertical, this);
    centerLayout->addWidget(WorkersSlider);
    WorkersSlider->setRange(1, 10);
    WorkersSlider->setValue(1);
    WorkersSlider->setInvertedAppearance(true);

    bottomLayout->addLayout(centerLayout, 1); // Растягиваем центральную часть

    // 4. Правая часть с таблицей и вертикальным слайдером
    QHBoxLayout* rightLayout = new QHBoxLayout();

    // 5. Статусная строка внизу
    statusLabel = new QLabel(this);
    updateStatusLabel(); // Устанавливаем начальное значение
    leftButtonsLayout->addWidget(statusLabel);

    // Таблица справа
    ManagersTable = new QTableWidget(5, 2, this);
    rightLayout->addWidget(ManagersTable);

    // Вертикальный слайдер справа
    ManagersSlider = new QSlider(Qt::Vertical, this);
    rightLayout->addWidget(ManagersSlider);
    ManagersSlider->setRange(1, 10);
    ManagersSlider->setValue(1);
    ManagersSlider->setInvertedAppearance(true);

    bottomLayout->addLayout(rightLayout);

    // Добавляем нижнюю часть в главный layout
    mainLayout->addLayout(bottomLayout, 1);
    connect(topSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);
    connect(WorkersSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);
    connect(ManagersSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);

}
void UsersEditor::updateStatusLabel() 
{
    if (topSlider->value()==1)
        statusLabel->setText(QString("Текущий сотрудник - рабочий : %1").arg(WorkersSlider->value()));
    else
        statusLabel->setText(QString("Текущий сотрудник - руководитель : %1").arg(ManagersSlider->value()));
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