#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <vector>


namespace Ui {
class MainWindow;
}
/**
 * @brief A classe MainWindow é a classe que gera a janela principal
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT
    int inferior,superior,tempo;
    std::vector <int>timers;
    QString ip_addr;
public:
  explicit MainWindow(QWidget *parent = 0);
  void timerEvent(QTimerEvent *event);
  ~MainWindow();
  void enviarInfo();

  int frand();
public slots:
  void conectar();

  void desconectar();

  void atualizarIP(QString _IP);

  void limiteInferior(int _min);

  void limiteSuperior(int _max);

  void ligaTimer();

  void desligaTimer();

  void setTimer(int _t);

private slots:

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
};

#endif // MAINWINDOW_H
