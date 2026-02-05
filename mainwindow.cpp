
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " " + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

    loadConfig("targetParams.json");
    if(!cfgok) QMessageBox::critical(this,"Error","Не удалось загрузить targetParams.json");
    initPlot();

    updaterT.setInterval(500);
    connect(&updaterT,&QTimer::timeout,this,&MainWindow::updateColorMap);
    if(cfgok) updaterT.start();


    QString filePath = "dataTest2.txt";
    QFile f(filePath);
    f.open(QIODevice::Text | QIODevice::ReadOnly);
    QTextStream str(&f);

    while(!str.atEnd()){
        QString line = str.readLine();
        QStringList list = line.split("\t");
        QList<double> row;
        for(int i = 0; i < list.count(); i++){
            row.append(list[i].toDouble());
        }
        data.append(row);
    }


    tim.setInterval(100);

    connect(&tim,&QTimer::timeout,this,&MainWindow::goNext);
    tim.start();
}
void MainWindow::goNext()
{
    static int counter = 0;
    if(data.isEmpty()) return;
    for(int i = 0; i < 8;i++){
        colorMap->setTemperatureAt(i,data.at(counter).at(i));
    }
    updateColorMap();
    counter++;
    if(counter == data.count()){
        counter = 0;
    }
    ui->label->setText(QString::number(counter) + "/"+QString::number(data.count()));
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::serverConnected()
{
    ui->label->setText("connected");
}

void MainWindow::serverDisconnected()
{
    ui->label->setText("disconnected");
}

void MainWindow::updateColorMap()
{
    colorMap->calculateGausses(ui->accuracyEdit->value());
    colorMap->plotGauss();
    colorMap->rescaleReplot();
}

void MainWindow::loadConfig(QString filePath)
{
    if(!QFileInfo(filePath).isFile()){qDebug() << "not a file"; return;}
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly)){qDebug() << "could not open"; return;}
    QString all = f.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(all.toUtf8());
    if(doc.isNull()){qDebug() << "could not open"; return;}
    diameter = doc["Diameter"].toDouble();
    fontsize = doc["LabelSize"].toInt();
    QJsonArray ar = doc["HotSpots"].toArray();
    for(int i = 0; i < ar.size(); i++){
        double x = ar.at(i)["x"].toDouble();
        double y = ar.at(i)["y"].toDouble();
        hs.append(HotSpot{x,y,25.0});
    }
    hs.append(HotSpot{100.0,100.0,25.0});
    hs.append(HotSpot{-100.0,100.0,25.0});
    hs.append(HotSpot{100.0,-100.0,25.0});
    hs.append(HotSpot{-100.0,-100.0,25.0});

    cfgok = true;
}


void MainWindow::initPlot()
{
    colorMap = new HotSpotMap(ui->plot,Gauss2d(2,0.1,0.1,20.0,20.0,25),Gauss2d(0,-30,-30,0.1,0.1,20),Gauss2d(500,30,30,100,100,500),fontsize);

    for(int i = 0; i < hs.count(); i++){
        colorMap->addHotSpot(hs.at(i));
    }
    colorMap->setSize(100,100);
    double pad = diameter*0.25;
    colorMap->setRanges(QCPRange(-diameter-pad,diameter+pad),QCPRange(-diameter-pad,diameter+pad));
    colorMap->rescaleReplot();

    QCPColorScale * scale = new QCPColorScale(ui->plot);
    scale->setMargins(QMargins(10,0,0,0));
    ui->plot->plotLayout()->addElement(0,1,scale);
    scale->setType(QCPAxis::atRight);
    colorMap->setColorScale(scale);
    QCPColorGradient g(QCPColorGradient::gpHot);
    g.setLevelCount(30);
    colorMap->setGradient(g);
    scale->setDataRange(QCPRange(20,100));
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}




void MainWindow::on_rescaleButton_clicked()
{
    ui->plot->rescaleAxes();
    ui->plot->replot();
}



void MainWindow::on_rescaleButton_3_clicked()
{
    colorMap->currentGauss = Gauss2d(2,0.1,0.1,20.0,20.0,25);
    ui->plot->replot();
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    colorMap->historyLength = arg1;
}


void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    tim.setInterval(arg1);
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    colorMap->autoScaleData = checked;
}

void MainWindow::on_angleShiftBox_valueChanged(double arg1)
{
    colorMap->setAngle(arg1);
    ui->plot->replot();
}

