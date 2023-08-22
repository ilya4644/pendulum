#ifndef BARUTKIN_MAIN_WINDOW_H
#define BARUTKIN_MAIN_WINDOW_H

#include "dialog_window.h"
//#include "pendulum.h"
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


private slots:
    void startModelling();
    void onPushButtonClicked();

private:
    QLabel *confLabel{};
    QGraphicsView *view{};
    QGraphicsScene *scene{};
    QPushButton *controlButton{};
    QPushButton *confButton{};
    void setupUI(double angle, double length);
    void removeUI();
};

#endif //BARUTKIN_MAIN_WINDOW_H
