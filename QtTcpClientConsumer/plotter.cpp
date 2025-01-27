#include "plotter.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <algorithm>

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{}

Plotter::~Plotter() {}

void Plotter::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPen pen;
    QPen pen2;
    QBrush brush;
    // azul grafico
    pen.setColor(QColor(0, 0, 255));
    pen.setWidth(2);
    painter.setPen(pen);
    // preta
    pen2.setColor(QColor(0,0,0));


    brush.setColor(QColor(255, 255, 200));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRect(0, 0, width(), height());

    if (!list_data.empty()) {
        // normalizando os dados
        float min_val = *std::min_element(list_data.begin(), list_data.end());
        float max_val = *std::max_element(list_data.begin(), list_data.end());

        // para evitar divisão por zero se todos os valores forem iguais
        if (max_val != min_val) {
            // normalizando os valores para a altura da área do gráfico
            for (auto &val : list_data) {
                val = (val - min_val) / (max_val - min_val) * height();
            }
        }

        // pega aos últimos 30 valores
        int start_index = std::max(0, int(list_data.size()) - 30);

        std::vector<float> last_30_values(list_data.begin() + start_index, list_data.end());
        painter.setPen(pen2);
        // Linha de centro
        painter.drawLine(0,height()/2,width(),height()/2);
        painter.setPen(pen);
        // Desenhando as linhas
        float step = width() / float(last_30_values.size());
        for (int i = 0; i < int(last_30_values.size()) - 1; i++) {
            painter.drawLine(step * i, height() - last_30_values[i], step * (i + 1), height() - last_30_values[i + 1]);
        }
    }
}

void Plotter::drawGrafics(std::vector<float> list){
    list_data = list;
    update(); // Trigger a repaint
}
