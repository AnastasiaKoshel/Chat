#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QThread>
#include <QTcpSocket>


class FileManager: public QThread
{
    Q_OBJECT

public:
    FileManager();
    void start();
    QByteArray data;
    void saveFile(const QString& fileName, const QByteArray& fileContent);
    const QString pathToSaveFiles = "C:/Users/Anastasiia_Koshel/Documents/Chat/";

    void receiveFileData(QByteArray& curData);

    //TIDI:write setters
    QTcpSocket* senderSocket;
    qintptr     senderSocketDesriptor;      ///< Incoming from server socket descriptor.
    int         totalSizeExpected;             ///< Max size of block socket able to recieved.

    QString fileName;
    QString senderLogin;

signals:
    void fileReceivedSignal(QByteArray& fileData, QString fileName, QString senderLogin);
    void fileSavedSignal(QString filePath);


private:
    /*QTcpSocket* senderSocket;
    qintptr     senderSocketDesriptor;      ///< Incoming from server socket descriptor.
    int         totalSizeExpected;             ///< Max size of block socket able to recieved.

    QString fileName;
    QString senderLogin;*/
};

#endif // FILEMANAGER_H
