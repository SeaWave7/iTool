#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  qDebug() << "Create NetworkThread";
  nThread = new NetworkThread();
  nThread->start(QThread::NormalPriority);
  qDebug() << "NetworkThread created!";

}

MainWindow::~MainWindow()
{
  if(nThread) delete nThread;
  delete ui;  
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}
