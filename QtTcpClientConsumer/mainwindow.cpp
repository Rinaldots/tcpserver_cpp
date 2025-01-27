#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),  ui(new Ui::MainWindow), timer(new QTimer(this))
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(timer, &QTimer::timeout, this, &MainWindow::getData);

  getIplist();
  // Configuração inicial dos elementos da interface
  ui->txt_ip->setText(ip_address.c_str());
  ui->butao_connect->setStyleSheet("background-color: gray;");
  ui->butao_disconect->setStyleSheet("background-color: gray;");

}

void MainWindow::tcpConnect(){
    const char * ip = this->ip_address.c_str();
    socket->connectToHost(ip,this->port);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
        qDebug() << this->ip_address.c_str();
        this->is_connected = true;
    }
    else{
        qDebug() << "Can't connect";
        this->is_connected = false;
    }
}

void MainWindow::tcpDisconect(){
    socket->close();
    if(!socket->waitForConnected(3000)){
        qDebug() << "Disconnected";
        this->is_connected = false;
    }
    else{
        qDebug() << "Error";
    }
}


void MainWindow::getData(){
  QString str;
  QByteArray array;
  QStringList list;
  // Ler os dados e remove os tempos para plotar grafico.
  if(socket->state() == QAbstractSocket::ConnectedState){
    if(socket->isOpen()){
      std::string get_text = "get ";
      get_text.append(ip_target);
      get_text.append(" 5\r\n");
      QByteArray byte_text = QByteArray::fromStdString(get_text);
      socket->write(QByteArray::fromStdString(get_text));
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);
      while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        list = str.split(" ");
        if(list.size() == 2){
          str = list.at(1);
          list_data.push_back(str.toFloat());
          // Atualiza o valores de minima e maxima do grafico
          float min_val = *std::min_element(list_data.begin(), list_data.end());
          float max_val = *std::max_element(list_data.begin(), list_data.end());
          //qDebug()<< min_val;
          this->update_max_min(int(max_val),int(min_val));

          //Evita que a lista fique muito grande
          if (list_data.size() > 60) {
              list_data.erase(list_data.begin());
          }
          ui->widget->drawGrafics(this->list_data);
        }
      }
    }
  }
}
void MainWindow::getIplist(){
    QString str;
    QByteArray array;
    QStringList list;
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    if (!model) {
        model = new QStandardItemModel(this);
        ui->listView->setModel(model);
    }
    // retorna a lista de ip's usando o 'list' do servidor.
    model->clear();
    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            socket->write("list");
            socket->waitForBytesWritten(3000);
            socket->waitForReadyRead(3000);
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                qDebug() << str;
                this->iplists.push_back(str.toStdString());
                QStandardItem *item = new QStandardItem(str);
                // atualiza a lista de ip's para ser selecionado depois.
                model->appendRow(item);
                }
            }
        }
}


MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
  delete timer;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    int rowIndex = index.row();
    // seta o valor do ip_target com o ip selecionado
    if (rowIndex >= 0 && rowIndex < this->iplists.size()){
        this->ip_target = this->iplists[rowIndex];  //Seta o ip que ira ler como o ip selecionado la lista
    }
}


void MainWindow::on_butao_connect_clicked()
{
    if(this->is_connected == false){
        tcpConnect();
        ui->butao_connect->setStyleSheet("background-color: red;");
        ui->butao_disconect->setStyleSheet("background-color: gray;");
    }
}


void MainWindow::on_butao_disconect_clicked()
{
    if(this->is_connected == true){
        tcpDisconect();
        ui->butao_connect->setStyleSheet("background-color: gray;");
        ui->butao_disconect->setStyleSheet("background-color: red;");
    }

}

void MainWindow::on_slider_timing_valueChanged(int value)
{
    ui->lcd_timing->display(value);
    this->timmning = value;
}

void MainWindow::on_butao_enviar_clicked()
{
    // Se esta conectado e não lendo inicia um timer de 1000/frequencia, chamando a função
    if (this->is_connected) {
        if (!this->is_reanding) {
            this->is_reanding = true;
            timer->start(1000/this->timmning);
            qDebug() << "Timer started.";
            ui->butao_enviar->setStyleSheet("background-color: green;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }
        else{
            ui->butao_enviar->setStyleSheet("background-color: gray;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }

    }
    else{
        this->is_reanding = false;
        ui->butao_enviar->setStyleSheet("background-color: gray;");
        ui->botao_parar->setStyleSheet("background-color: gray;");
    }
}
void MainWindow::on_botao_parar_clicked()
{
    if (this->is_connected) {
        if (this->is_reanding) {
            this->is_reanding = false;
            timer->stop();
            qDebug() << "Timer started.";
            ui->butao_enviar->setStyleSheet("background-color: gray;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }
    } else{
        this->is_reanding = false;
        ui->butao_enviar->setStyleSheet("background-color: gray;");
        ui->botao_parar->setStyleSheet("background-color: gray;");
    }
}


void MainWindow::update_max_min(int max, int min){
    // Atualiza os valores de maximo e minimo na ui
    ui->lcd_max->display(max);
    ui->lcd_min->display(min);
}

void MainWindow::on_botao_parar_2_clicked()
{
    getIplist();
}

