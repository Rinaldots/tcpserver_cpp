#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QPaintEvent>
#include <vector>

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();
public slots:
    void paintEvent(QPaintEvent *e);
    void drawGrafics(std::vector<float> list);
private:
    int min_num = 5;
    int num = 5;
    std::vector<float> list_data; // Add this line
signals:
};

#endif // PLOTTER_H
