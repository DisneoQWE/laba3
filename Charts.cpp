#include "Charts.h"
#include <QBarSeries>
#include <QPieSeries>
#include <QBarSet>
#include <QGraphicsLayout>
#include <QChartView>
#include <QLayout>

using namespace QtCharts;


Charts::~Charts()
{
    delete chart_model;
    delete chart_view;
}

Charts::Charts(QLayout *l)
{
    chart_model = new QChart();
    chart_view = new QChartView();
    chart_view->setRenderHint(QPainter::Antialiasing); // сглаживание
    // отображаем легенду справа
    chart_model->legend()->setAlignment(Qt::AlignRight);
    chart_view->setChart(chart_model);
    addWidgetToLayout(l);
}

void Charts::setChart(const std::unique_ptr<QList<Data> > &data) const
{
    // подсчитываем общий размер папки
    qint64 total_size = 0;
    for (auto& x : *data) {
        total_size += x._size.toLongLong();
    }
    chart_model->setTitle("");
    // в случае когда папка пуста выводим надпись
    if (total_size == 0 || data->isEmpty()) {
        chart_model->setTitle("Folder is empty");
        chart_model->removeAllSeries();
        return;
    }
    // элементы идущие после 6 относим к маленьким
    // подсчитываем их общий размер и размещаем их в категорию others
    if (data->size() > 6) {
        qint64 others_size = 0;
        auto it = data->begin() + 5;
        while (it != data->end()) {
            others_size += it->_size.toLongLong();
            it = data->erase(it);
        }

        double percent = double(others_size * 100) / total_size;
        data->push_back(Data("Others", QString::number(others_size), QString::number(percent, 'f', 2).append(" %"), (qreal)others_size / total_size));
    }
    // набор данных устанавливаются в диаграмму
    setDataToChart(data);
}

void Charts::addWidgetToLayout(QLayout *l)
{
    l->addWidget(chart_view);
}

void Charts::UpdateDisplay(const std::unique_ptr<QList<Data> > &data) const
{
    setChart(data);
}


void Charts::removeSeriesFromChart(QChart *c) const
{
    c->removeAllSeries();
}

void Charts::addSeriesToChart(QAbstractSeries *series) const
{
    chart_model->addSeries(series);
}



/**
 * @brief Заполнение вертикальной диаграммы данными
 * @param data - данные для заполнения
 */

void Charts::setDataToChart(const std::unique_ptr<QList<Data> > &data) const
{
    removeSeriesFromChart(chart_model);
    QAbstractSeries* series = addDataToSeries(data);
    addSeriesToChart(series);
}


BarChart::BarChart(QLayout *l) : Charts(l) {}


QAbstractSeries *BarChart::addDataToSeries(const std::unique_ptr<QList<Data> > &data) const
{
    QBarSeries* series = new QBarSeries();
    series->setBarWidth(1);
    for (auto& item : *data) {
        QBarSet* set = new QBarSet(item._name + " (" + item._percent.toHtmlEscaped() + ")");
        set->append(item._ratio);
        series->append(set);
    }
    return series;
}


PieChart::PieChart(QLayout *l) : Charts(l) {}


QAbstractSeries *PieChart::addDataToSeries(const std::unique_ptr<QList<Data> > &data) const
{
    QPieSeries* series = new QPieSeries();
    series->setPieSize(1);
    for (auto& item : *data) {
        series->append(item._name + " (" + item._percent.toHtmlEscaped() + ")", item._ratio);
    }
    return series;
}


