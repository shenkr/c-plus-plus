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

    QMessageBox messageAuthor;
    messageAuthor.setIcon(QMessageBox::Information);
    messageAuthor.setWindowTitle("Інформація");
    messageAuthor.setText("Студент групи 473\nПаутов А.А.\nВаріант-10");
    messageAuthor.exec();
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
    ui->tableWidget->clearContents();
    ui->graph->clearGraphs();
    ui->tableWidget->setVisible(false);
}

void MainWindow::on_buttonCalc_clicked()
{
    ui->tableWidget->clearContents();
    ui->graph->clearGraphs();

    double left = ui->left->text().toDouble();
    double right = ui->right->text().toDouble();
    double step = ui->step->text().toDouble();

    if(left > right)
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Помилка");
        message.setText("Ліва межа не може бути більше за праву");
        message.exec();
        return;
    }

    if(step <= 0)
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Помилка");
        message.setText("Крок не може бути менше або дорівнювати нулю");
        message.exec();
        return;
    }

    QVector<double> xVect, yVect;
    int row = 0;
    for(double i = left; i < right + step; i += step)
    {
        double func = f(i);
        QString x;
        xVect.push_back(i);
        yVect.push_back(func);
        x.setNum(i, 'g', 3);
        QString f_x;
        f_x.setNum(func, 'g', 3);
        ui->tableWidget->setRowCount(row+1);
        QTableWidgetItem *itemX = new QTableWidgetItem;
        itemX->setText(x);
        QTableWidgetItem *itemFunc = new QTableWidgetItem;
        itemFunc->setText(f_x);
        ui->tableWidget->setItem(row, 0, itemX);
        ui->tableWidget->setItem(row, 1, itemFunc);
        row++;
    }

    paintGraph(ui->graph, xVect, yVect);
}

double MainWindow::f(double x)
{
    return 2 * sin(fabs(2 * x)) * cos(2 * x);
}

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    // Создание меню
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionTab);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionClear);

    QPoint globalPos;

    globalPos = ui->tableWidget->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionTab_triggered()
{
    on_buttonCalc_clicked();
}

void MainWindow::on_pushButtonCheck_clicked()
{
    double left = ui->left->text().toDouble();
    double right = ui->right->text().toDouble();
    double step = ui->step->text().toDouble();

    if(left > right)
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Помилка");
        message.setText("Ліва межа не може бути більше за праву");
        message.exec();
        return;
    }

    if(step <= 0)
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Помилка");
        message.setText("Крок не може бути менше або дорівнювати нулю");
        message.exec();
        return;
    }
}

// Збереження результатів у файл
void MainWindow::on_pushButtonSave_clicked()
{
    // Перевірка на наявність результатів
    if(sumTask2.isEmpty())
    {
        QMessageBox messageError;
        messageError.setIcon(QMessageBox::Critical);
        messageError.setWindowTitle("Помилка");
        messageError.setText("Відсутні результати обчислень!");
        messageError.exec();
        return;
    }
    // Вибір файлу для збереження результатів
    filename = QFileDialog::getSaveFileName(0, "Зберегти", "", "*.txt");
    // Перевірка вибору файлу
    if(filename.isEmpty())
    {
        // Вивести повідомлення, якщо файл не вибраний
        QMessageBox messageError;
        messageError.setIcon(QMessageBox::Critical);
        messageError.setWindowTitle("Помилка");
        messageError.setText("Виберіть файл для збереження!");
        messageError.exec();
        return;
    }
    else
    {
        // Показати який файл вибраний для збереження
        statusBar()->showMessage("Файл для збереження результатів: " + filename, 1000);

        // Створення файлу
        QFile file(filename);
        file.open(QIODevice::Text | QIODevice::WriteOnly);
        // Запис у файл
        QTextStream data(&file);
        data << sumTask2;
        file.close();

        statusBar()->showMessage("Результати збереженні в файл", 1000);
    }
}

void MainWindow::on_pushButtonSum_clicked()
{
    double left = ui->left->text().toDouble();
    double right = ui->right->text().toDouble();
    double step = ui->step->text().toDouble();

    int count = 0;
    // Сума кубів додатніх значень
    for(double i = left; i < right + step; i += step)
    {
        double func = f(i);
        // Перевірка значення
        if(func < -3 || func > 0.4)
            count++;
    }

    sumTask2 = QString("Количество: %1").arg(count);

    // Вивід результату
    statusBar()->showMessage(sumTask2, 1000);
}

void MainWindow::on_actionClear_triggered()
{
    on_buttonClear_clicked();
}
