#ifndef BARUTKIN_DIALOG_WINDOW_H
#define BARUTKIN_DIALOG_WINDOW_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QObject>

class DialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWindow(QWidget *parent = nullptr);

private:
    QLabel *angleLabel{};
    QLabel *lengthLabel{};
    QLineEdit *angleEdit{};
    QLineEdit *lengthEdit{};
    QPushButton *saveButton{};

    void setupUI();
    void saveConf();
};

#endif //BARUTKIN_DIALOG_WINDOW_H
