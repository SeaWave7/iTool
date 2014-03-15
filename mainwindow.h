#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "packet.h"
#include "networkthread.h"
#include "packetprocessor.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QDebug>
#include <QtCore>

namespace Ui {
  class MainWindow;
}

 class MainWindow : public QMainWindow
{
  Q_OBJECT


public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  Ui::MainWindow *ui;

public slots:

protected:
  void closeEvent(QCloseEvent *event);

private:
  // Network Thread
  NetworkThread *nThread;
};

#endif // MAINWINDOW_H
