#ifndef MEMODB_H
#define MEMODB_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QStringList>
#include <QDebug>
#include <QDir>

class MemoDb
{
public:
    MemoDb();
    ~MemoDb();
    void Open(QString path="./");
    void Close();
    void Write(QStringList record);
private:
    QSqlDatabase db;
    QString dbPath;
    QSqlDatabase OpenDb(QString path="./");
    bool IsExsistTable();
    void CreateTable();
};

#endif // MEMODB_H
