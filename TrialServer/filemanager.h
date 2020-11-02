#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QThread>
#include <QTcpSocket>


class FileManager: public QThread
{
    Q_OBJECT

public:
    FileManager();

    void sendFileJSON();
    void getFileData(QByteArray& curData);

    QTcpSocket* getReceipientSocket() { return receipientSocket; }
    void setReceipientSocket(QTcpSocket* socket) {receipientSocket = socket;}

    int getSizeExpected() {return totalSizeExpected;}
    void setSizeExpected(int size){totalSizeExpected = size;}

    QString getFileName(){return fileName;}
    void setFileName(QString name){fileName = name;}

    QString getSenderLogin(){return senderLogin;}
    void setSenderLogin(QString login){senderLogin= login;}


private slots:
    //void onReadyRead();

signals:
    void finishFileReceive(QByteArray&);


private:
    QTcpSocket* receipientSocket;
    int         totalSizeExpected;
    QString fileName;
    QString senderLogin;
    QByteArray data;
};

#endif // FILEMANAGER_H
