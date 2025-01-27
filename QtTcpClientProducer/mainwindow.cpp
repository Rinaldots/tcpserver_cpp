#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), socket(new QTcpSocket(this)), timer(new QTimer(this)) {

    // Conectar o sinal timeout do timer ao slot putData
    connect(timer, &QTimer::timeout, this, &MainWindow::putData);

    // Configuração da interface do usuário
    ui->setupUi(this);

    // Configuração inicial dos elementos da interface
    ui->txt_ip->setText(ip_address.c_str());
    ui->butao_connect->setStyleSheet("background-color: gray;");
    ui->butao_disconect->setStyleSheet("background-color: gray;");
}

MainWindow::~MainWindow() {
    // Deletar objetos alocados dinamicamente
    delete ui;
    delete socket;
    delete timer;
}

void MainWindow::tcpConnect(){
    const char * ip = this->ip_address.c_str();
    socket->connectToHost(ip,this->port);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
        qDebug() << this->ip_address.c_str();
        this->is_connected = true;
        ui->txt_status->setText("Conectado");
    }
    else{
        qDebug() << "Can't connect";
        mensagens("Can't connect");
        this->is_connected = false;
    }
}

void MainWindow::tcpDisconect(){
    socket->close();
    if(!socket->waitForConnected(3000)){
        qDebug() << "Disconnected";
        this->is_connected = false;
        ui->txt_status->setText("Disconectado");
    }
    else{
        qDebug() << "Error";
        mensagens("Error");
    }
}

void MainWindow::putData() {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        // Calcular a diferença e garantir que max_value seja maior que min_value
        float delta = static_cast<float>(this->max_value - this->min_value);
        if (delta <= 0.0f) {
            qWarning() << "Invalid range: max_value should be greater than min_value.";
            return;
        }

        // Obter o tempo atual em milissegundos
        qint64 msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();

        // Gerar um número float aleatório dentro do intervalo
        float randomValue = this->min_value + static_cast<float>(rand()) / RAND_MAX * delta;
        QString str = QString("set %1 %2")
                          .arg(msecdate)
                          .arg(randomValue, 0, 'f', 6); // 6 casas decimais de precisão

        qDebug() << "Sending data:" << str;
        ui->mensagens->append(str);
        qint64 bytesWritten = socket->write(str.toUtf8());
        qDebug() << bytesWritten << "bytes written";

    } else {
        qWarning() << "Socket is not connected.";
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


void MainWindow::on_slider_max_valueChanged(int valor)
{
    if(valor >= this->min_value){
        ui->lcd_max->display(valor);
        this->max_value = valor;
    }else{
        ui->lcd_max->display(this->min_value);
        this->max_value = min_value;
        ui->slider_max->setValue(min_value);
    }
}


void MainWindow::on_slider_min_valueChanged(int valor)
{
    if(valor <= this->max_value){
        ui->lcd_min->display(valor);
        this->min_value = valor;
    }else{
        ui->lcd_min->display(this->max_value);
        this->min_value = max_value;
        ui->slider_min->setValue(max_value);
    }

}


void MainWindow::on_slider_timing_valueChanged(int value)
{
    ui->lcd_timing->display(value);
    this->timmning = value;
}

void MainWindow::mensagens(std::string string){
    const QString text = QString::fromStdString(string);
    ui->mensagens->append(text);
}


void MainWindow::on_butao_enviar_clicked()
{
    if (this->is_connected) {
        if (!this->is_sending) {
            this->is_sending = true;
            timer->start(1000/this->timmning); // Executa a cada 1 segundo
            qDebug() << "Timer started.";
            ui->butao_enviar->setStyleSheet("background-color: green;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }
        else{
            ui->butao_enviar->setStyleSheet("background-color: gray;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }
    }
}


void MainWindow::on_txt_ip_textChanged(const QString &arg1)
{
    this->ip_address = arg1.toStdString();
}


void MainWindow::on_botao_parar_clicked()
{
    if (this->is_connected) {
        if (this->is_sending) {
            this->is_sending = false;
            timer->stop(); // Executa a cada 1 segundo
            qDebug() << "Timer started.";
            ui->butao_enviar->setStyleSheet("background-color: gray;");
            ui->botao_parar->setStyleSheet("background-color: gray;");
        }
    }
}

