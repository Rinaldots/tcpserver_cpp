#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
  void tcpConnect();
  void tcpDisconect();
public slots:
  void getData();
  void getIplist();

  private slots:

  void on_butao_connect_clicked();

  void on_butao_disconect_clicked();

  void on_listView_clicked(const QModelIndex &index);

  void on_slider_timing_valueChanged(int value);

  void on_butao_enviar_clicked();

  void on_botao_parar_clicked();

  void on_botao_parar_2_clicked();

  void update_max_min(int max, int min);

  private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QTimer *timer;
  bool is_connected = false;
  bool is_reanding = false;
  std::vector<std::string> iplists;
  std::vector<float> list_data;
  std::string ip_address = "127.0.0.1";
  std::string ip_target = "0.0.0.0";
  int port = 1234;
  int timmning = 1;
};

#endif // MAINWINDOW_H
