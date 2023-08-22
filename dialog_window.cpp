#include "dialog_window.h"


DialogWindow::DialogWindow(QWidget *parent) : QDialog(parent)
{
    setupUI();
}

void DialogWindow::setupUI()
{
    lengthLabel = new QLabel("Введите длину нити в сантиметрах:", this);

    lengthEdit = new QLineEdit(this);
    connect(lengthEdit, &QLineEdit::returnPressed, this, &DialogWindow::saveConf);

    angleLabel = new QLabel("Введите угол отклонения в градусах. (Не более 180):", this);

    angleEdit = new QLineEdit(this);
    connect(angleEdit, &QLineEdit::returnPressed, this, &DialogWindow::saveConf);

    saveButton = new QPushButton("Сохранить конфигурацию", this);
    connect(saveButton, &QPushButton::clicked, this, &DialogWindow::saveConf);

    auto *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(lengthLabel, 0, 0, 1, 2);
    mainLayout->addWidget(lengthEdit, 1, 0, 1, 2);
    mainLayout->addWidget(angleLabel, 2, 0, 1, 2);
    mainLayout->addWidget(angleEdit, 3, 0, 1, 2);
    mainLayout->addWidget(saveButton, 4, 0, 1, 2);

    setLayout(mainLayout);
    setWindowTitle("Конфигуратор");
    setFixedSize(400, 300);
}

void DialogWindow::saveConf()
{
    auto length = lengthEdit->text().toDouble();
    auto angle = angleEdit->text().toDouble();
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "conf.bin", "wb");
    if (err == 0) {
        fwrite(&length, sizeof(double), 1, file);
        fwrite(&angle, sizeof(double), 1, file);
    }
    fclose(file);
    DialogWindow::close();
}

