# このソフトウェアについて

　Qt5学習。QtでSQLite3を使ってみる。Class化した。

# 解決した問題

　`QSqlDatabasePrivate::addDatabase: duplicate connection name 'MemoDb', old connection removed.`のような表示が出る。

## 解決方法

* `QSqlDatabase::addDatabase`は1回のみ実行すべき
* 第2引数にデータベース名を渡すべき

### 問題コード抜粋

mainwindow.cpp
```cpp
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
```

　`MemoDb memoDb;`と`memoDb.Open(...);`で2回`Open()`が読み出されている。ここで`QSqlDatabase::addDatabase`が2回実行されてしまっていた。

memodb.cpp
```cpp
void MemoDb::Open(QString path) {
    QSqlDatabase::removeDatabase("MemoDb");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "MemoDb");
    db.setDatabaseName(this->dbPath);
}
void MemoDb::CreateTable() {
    QSqlDatabase db = QSqlDatabase::database("MemoDb");
    QSqlQuery query(db);
    query.exec("create table Memo(id INTEGER PRIMARY KEY AUTOINCREMENT, Memo text, DateTime text)");
}
void MemoDb::Close() {
    QSqlDatabase::removeDatabase("MemoDb");
}
```

　`QSqlDatabase::addDatabase`の前に`QSqlDatabase::removeDatabase`がないと以下の表示が出る。

```
QSqlDatabasePrivate::addDatabase: duplicate connection name 'MemoDb', old connection removed.
```

　`Close()`でも`removeDatabase`しているのに、なぜか`addDatabase`の直前でないと上記が出る。

### 解決コード抜粋

```cpp
void MemoDb::Open(QString path)
{
    if (!QSqlDatabase::contains("MemoDb")) {
        this->dbPath = QDir(path).filePath("memo.sqlite3");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "MemoDb");
        db.setDatabaseName(this->dbPath);
        this->CreateTable();
    }
}
```

　`QSqlDatabase::contains("MemoDb")`で既存判定をする。既存なら`addDatabase`しない。

# 参考

* http://tomokiit.hatenablog.jp/entry/Qt_SQLite
* https://blogs.yahoo.co.jp/hmfjm910/10815002.html
* https://forum.qt.io/topic/18700/sqlite-connection-qsqldatabaseprivate-adddatabase-duplicate-connection-name-qt_sql_default_connection-old-connection-removed/7

## Qt要素

* http://doc.qt.io/qt-5/qsql.html
    * http://doc.qt.io/qt-5/qsqldatabase.html
    * http://doc.qt.io/qt-5/qsqlquery.html

# 開発環境

* [Raspberry Pi](https://ja.wikipedia.org/wiki/Raspberry_Pi) 3 Model B+
    * [Raspbian](https://www.raspberrypi.org/downloads/raspbian/) GNU/Linux 9.0 (stretch) 2018-06-27
        * Qt 5.7.1

## 環境構築

* [Raspbian stretch で Qt5.7 インストールしたもの一覧](http://ytyaru.hatenablog.com/entry/2019/12/17/000000)

# ライセンス

　このソフトウェアはCC0ライセンスである。

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)

## 利用ライブラリ

ライブラリ|ライセンス|ソースコード
----------|----------|------------
[Qt](http://doc.qt.io/)|[LGPL](http://doc.qt.io/qt-5/licensing.html)|[GitHub](https://github.com/qt)

* [参考1](https://www3.sra.co.jp/qt/licence/index.html)
* [参考2](http://kou-lowenergy.hatenablog.com/entry/2017/02/17/154720)
* [参考3](https://qiita.com/ynuma/items/e8749233677821a81fcc)
