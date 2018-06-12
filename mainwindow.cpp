#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(ui->Start,
          SIGNAL(clicked(bool)),
          this,
          SLOT(ligaTimer()));
  connect(ui->Stop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(desligaTimer()));
  connect(ui->sliderTiming,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(setTimer(int)));
  //
  // Maximo
  connect(ui->SliderMax,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(limiteSuperior(int)));
  //
  // Minimo
  connect(ui->SliderMin,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(limiteInferior(int)));
  connect(ui->Connect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(conectar()));
  connect(ui->Disconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(desconectar()));
  connect(ui->serverIP,
          SIGNAL(textChanged(QString)),
          this,
          SLOT(atualizarIP(QString)));
  ip_addr="127.0.0.1";
  tempo=1;
  superior=10;
  inferior=1;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    enviarInfo();
}

void MainWindow::conectar(){
  socket->connectToHost(ip_addr,1234);
  if(socket->waitForConnected(3000)){
      QStringList list("Connected");
      ui->listWidget->addItems(list);
    qDebug() << "Connected";
      ui->labelConnect->setText("Connected to "+ip_addr);
  }
  else{
      QStringList list("Connection error");
      ui->listWidget->addItems(list);
    qDebug() <<"Connection error";
    ui->labelConnect->setText("Connection error");
  }
}

void MainWindow::desconectar()
{
    socket->disconnectFromHost();
    if(socket->waitForConnected(3000)){
        QStringList list("Connected");
        ui->listWidget->addItems(list);
      qDebug() << "Connected";
       ui->labelConnect->setText("Connected to "+ip_addr);
    }
    else{
        QStringList list("Disconnected");
        ui->listWidget->addItems(list);
      qDebug() << "Disconnected";
       ui->labelConnect->setText("Disconnected");
    }
}

void MainWindow::atualizarIP(QString _IP)
{
    ip_addr=QString (_IP);
}

void MainWindow::enviarInfo(){
  QDateTime datetime;
  QString str;
  qint64 msecdate;

  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    str = "set "+ QString::number(msecdate) + " " + QString::number((float)inferior+((float)qrand()/(float)RAND_MAX)*(float)(superior-inferior))+"\r\n";
    QStringList list(str);
    ui->listWidget->addItems(list);
      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
      }
  }
}

void MainWindow::limiteInferior(int _min)
{

    if (superior <= _min)
        ui->SliderMax->setValue(_min);
    inferior=_min;
}

void MainWindow::limiteSuperior(int _max)
{
    if (inferior >= _max)
        ui->SliderMin->setValue(_max);
    superior=_max;
}

void MainWindow::ligaTimer()
{
    if(timers.size()==0){
       timers.push_back(startTimer(1000*tempo));
    }
    else{
        killTimer(timers[0]);
        timers[0]=startTimer(1000*tempo);
    }
}

void MainWindow::desligaTimer()
{
    killTimer(timers[0]);
    timers.clear();
}

void MainWindow::setTimer(int _t)
{
    tempo=_t;
    if(timers.size()){
        ligaTimer();
    }
}

MainWindow::~MainWindow(){
  delete socket;
  delete ui;
}
