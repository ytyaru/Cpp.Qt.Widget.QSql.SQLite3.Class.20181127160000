#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memodb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MemoDb memoDb;
    memoDb.Open(QApplication::applicationDirPath());
    QStringList record;
    record.append("ABCDEFG");
    record.append("2000-01-01 00:00:00.000");
    memoDb.Write(record);
    memoDb.Close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
