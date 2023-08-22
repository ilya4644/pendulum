#include "main_window.h"

const double M_PI = 3.14159265358979323846;
const double g = 9.81;

class Pendulum : public QGraphicsItem
{
public:
    Pendulum(double angle, double length, double editingAngle)
            : angle((angle * M_PI) / 180), length(length), editingAngle((editingAngle * M_PI) / 180) {}

    bool direction;

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, 0, 0);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setPen(Qt::black);
        painter->drawLine(QPointF(0, 0), QPointF(length * sin(editingAngle), length * cos(editingAngle)));

        painter->setBrush(Qt::red);
        painter->drawEllipse(QPointF(0, 0), 1, 1);
        painter->drawEllipse(QPointF(length * sin(editingAngle), length * cos(editingAngle)), 5, 5);
    }

    void advance(int phase) override
    {
        if (phase) {
            if (editingAngle >= angle) {
                direction = false;
            } else if (editingAngle <= -angle) {
                direction = true;
            }
            if (direction) {
                editingAngle += 0.1;
            } else {
                editingAngle -= 0.1;
            }
//            Pendulum(angle, length);
            setPos(length * sin(editingAngle), length * cos(editingAngle));
        }
    }

private:
    double angle;
    double length;
    double editingAngle;
};


MainWindow::MainWindow(QWidget *parent)
        : QWidget(parent)
{
    MainWindow::setupUI(0.0, 0.0);
}

void MainWindow::removeUI()
{
    delete confLabel;
    confLabel = nullptr;

    delete view->scene();
    scene = nullptr;

    delete view;
    view = nullptr;

    delete controlButton;
    controlButton = nullptr;

    delete confButton;
    confButton = nullptr;

    QLayout *mainLayout = layout();
    if (mainLayout)
    {
        QLayoutItem *item;
        while ((item = mainLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
        delete mainLayout;
    }
}


void MainWindow::onPushButtonClicked()
{
    auto dialogWindow = new DialogWindow(this);
    dialogWindow->show();
}

void MainWindow::startModelling()
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "conf.bin", "rb");
    double length = 0.0;
    double angle = 0.0;
    if (err == 0) {
        fread(&length, sizeof(double), 1, file);
        fread(&angle, sizeof(double), 1, file);
    }

    if (angle > 180) {
        angle = 180;
    } else if (angle < -180) {
        angle = -180;
    }
    fclose(file);
    MainWindow::removeUI();
    MainWindow::setupUI(angle, length);
}

void MainWindow::setupUI(double angle, double length)
{
    double period = 2 * M_PI * sqrt((length / 100) / g);
    QString labelText;
    labelText = QString("Длина нити: %1 см\nУгол отклонения: %2 градусов\nПериод маятника: %3 c")
            .arg(length)
            .arg(angle)
            .arg(period);
    confLabel = new QLabel(labelText, this);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);

    double editingAngle = angle;
    auto *pendulum = new Pendulum(angle, length, editingAngle);
    scene->addItem(pendulum);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    double timerTime = ((2 * M_PI * sqrt((length / 100) / g) * 1000) / (4 * (((abs(angle) * M_PI) / 100) / 0.1)));
    if (std::isnan(timerTime)) timerTime = 0.0;
    timer->start((int)timerTime);

    controlButton = new QPushButton("Старт", this);
    connect(controlButton, &QPushButton::clicked, this, &MainWindow::startModelling);

    confButton = new QPushButton("Конфигурация", this);
    connect(confButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);

    auto *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(view, 0, 0, 2, 2);
    mainLayout->addWidget(confLabel, 2, 0, 1, 2);
    mainLayout->addWidget(controlButton, 3, 0);
    mainLayout->addWidget(confButton, 3, 1);

    setLayout(mainLayout);
    setWindowTitle("Моделирование математического маятника");
    resize(500, 400);
}



