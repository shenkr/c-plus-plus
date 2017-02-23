#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QFileDialog>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintGraph(QCustomPlot *customPlot, QVector<double> &x, QVector<double> &y)
{
    QVector<double> extrX(2), extrY(2);
    extrY[0] = y[0], extrY[1] = y[0];
    for(int i = 0; i < y.size(); i++)
    {
        if(y[i] < extrY[0]) { extrY[0] = y[i]; extrX[0] = x[i]; }
        if(y[i] > extrY[1]) { extrY[1] = y[i]; extrX[1] = x[i]; }
    }

    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->graph(0)->setData(x, y);
    customPlot->graph(1)->setData(extrX, extrY);

    customPlot->graph(0)->rescaleAxes();

    customPlot->graph(1)->rescaleAxes(true);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::on_buttonClear_clicked()
{
    ui->left->clear();
    ui->right->clear();
    ui->step->clear();
    ui->textEdit->clear();
    ui->tableWidget->clearContents();
    ui->graph->clearGraphs();
    ui->radioButton_text->setChecked(true);
    ui->radioButton_table->setChecked(false);
    ui->tableWidget->setVisible(false);
    ui->textEdit->setVisible(true);
    ui->checkBox_saveInto->setChecked(false);
}

void MainWindow::on_checkBox_saveInto_clicked()
{
    if(ui->checkBox_saveInto->isChecked())
    {
        filename = QFileDialog::getSaveFileName(0, "Зберегти", "", "*.txt");
        if(filename.isEmpty())
            ui->checkBox_saveInto->setChecked(false);
        else
            statusBar()->showMessage("Файл для збереження результатів: " + filename, 2000);
    }
}

void MainWindow::on_buttonCalc_clicked()
{
    ui->textEdit->clear();
    ui->tableWidget->clearContents();
    ui->graph->clearGraphs();

    double left = ui->left->text().toDouble();
    double right = ui->right->text().toDouble();
    double step = ui->step->text().toDouble();

    if(left > right)
    {
        QMessageBox msg;
        msg.setText("Помилка!");
        msg.setInformativeText("Ліва межа більше правої!");
        msg.exec();

        return;
    }
    if(step <= 0)
    {
        QMessageBox msg;
        msg.setText("Помилка!");
        msg.setInformativeText("Крок не може дорівнювати нулю!");
        msg.exec();

        return;
    }


        QString res;
        QString res_file;
        res.push_back(QString("\tx\t|\tf(x)\n"));
        res_file.push_back(QString("\t%1%2\t%3\n").arg('x', 4).arg('|', 4).arg("f(x)", 4));
        QVector<double> xVect, yVect;
        int row = 0;
        double sum = 0;
        for(double i = left; i < right + step; i += step)
        {
            double func = f(i);

            if(func >= 0) sum += func*func;

            QString x;
            xVect.push_back(i);
            yVect.push_back(func);
            x.setNum(i, 'g', 3);
            QString f_x;
            f_x.setNum(func, 'g', 3);
            res.push_back(QString("\t%1\t|\t%2\n").arg(i, 4).arg(func, 4));
            res_file.push_back(QString("\t%1%2\t%3\n").arg(i, 4).arg('|', 4).arg(func, 4));
            ui->tableWidget->setRowCount(row+1);
            QTableWidgetItem *itemX = new QTableWidgetItem;
            itemX->setText(x);
            QTableWidgetItem *itemFunc = new QTableWidgetItem;
            itemFunc->setText(f_x);
            ui->tableWidget->setItem(row, 0, itemX);
            ui->tableWidget->setItem(row, 1, itemFunc);
            row++;
        }

        res.push_back(QString("Сума квадратiв: ") + QString::number(sum));
        res_file.push_back(QString("Сума квадратiв додатнiх значень: ").arg(sum, 4));

        QMessageBox msgBox;
        msgBox.setText("Зберегти.");
        msgBox.setInformativeText("Зберегти файл?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Save:

            filename = QFileDialog::getSaveFileName(0, "Зберегти", "", "*.txt");
            if(filename.isEmpty()) ui->checkBox_saveInto->setChecked(false);

            else statusBar()->showMessage("Файл для збереження результатів: " + filename, 2000);

            break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              break;
          default:
              // should never be reached
              break;
        }

        QMessageBox msg;
        msg.setText("Помилка!");
        msg.setInformativeText(QString::number(sum));
        msg.exec();

        res.push_back(QString("Сума квадратiв додатнiх значень: ").arg(sum, 4));
        res_file.push_back(QString("Сума квадратiв додатнiх значень: ").arg(sum, 4));

        if(!filename.isEmpty() && ui->checkBox_saveInto->isChecked())
        {
            QFile file(filename);
            file.open(QIODevice::Text | QIODevice::WriteOnly);
            QTextStream data(&file);
            data << res_file;
            file.close();
        }
        ui->textEdit->setText(res);
        paintGraph(ui->graph, xVect, yVect);
}

void MainWindow::on_radioButton_table_clicked()
{
    if(ui->radioButton_table->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_radioButton_text_clicked()
{
    if(ui->radioButton_text->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

double MainWindow::f(double x)
{
    return sin(abs(3*x))+cos(x);
}
