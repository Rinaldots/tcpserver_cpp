#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

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
  void mensagens(std::string string);
public slots:
  void putData();
private slots:
    void on_butao_connect_clicked();

    void on_butao_disconect_clicked();

    void on_slider_max_valueChanged(int value);

    void on_slider_min_valueChanged(int value);

    void on_slider_timing_valueChanged(int value);

    void on_butao_enviar_clicked();

    void on_txt_ip_textChanged(const QString &arg1);

    void on_botao_parar_clicked();

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QTimer *timer;
  bool is_connected = false;
  bool is_sending = false;
  std::string ip_address = "127.0.0.1";
  int port = 1234;
  int max_value = 0;
  int min_value = 0;
  int timmning = 1;
};

#endif // MAINWINDOW_H
