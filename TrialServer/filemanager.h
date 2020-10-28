#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QThread>
#include <QTcpSocket>


class FileManager: public QThread
{
    Q_OBJECT

public:
    FileManager(/*QTcpSocket* sender,*/ QTcpSocket* receiver, int size, QString login, QString name);
    QByteArray data;
    void sendFileJSON();
    void getFileData(QByteArray& curData);

private slots:
    //void onReadyRead();

signals:
    void finishFileReceive(QByteArray&);


private:
    QTcpSocket* senderSocket;
    QTcpSocket* receipientSocket;
    qintptr     senderSocketDesriptor;      ///< Incoming from server socket descriptor.
    qintptr     receiverSocketDesriptor;      ///< Incoming from server socket descriptor.
    int         totalSizeExpected;             ///< Max size of block socket able to recieved.

    QString fileName;
    QString senderLogin;
};

#endif // FILEMANAGER_H
