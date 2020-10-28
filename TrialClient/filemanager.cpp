#include "filemanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "jsonType.h"
#include <QFile>
#include <QDataStream>
#include <QImage>
#include <QBuffer>
#include <QFileInfo>

FileManager::FileManager(QTcpSocket* sender, int size, QString login, QString name):
    senderSocket(sender),
    totalSizeExpected(size),
    senderLogin(login),
    fileName(name)
{

}

void FileManager::receiveFileData(QByteArray& curData)
{
    qDebug()<<"[FileManager] receving file Data of size = "<< curData.size();
    data+=curData;
    qDebug()<<"[FileManager] cur size = "<< data.size()<<" expected "<<totalSizeExpected;
    if(data.size()>=totalSizeExpected)
    {
        saveFile(fileName, data);
        data.clear();
    }
}
void FileManager::saveFile(const QString& fileName,  const QByteArray& fileContent)
{

    const QString filePath = pathToSaveFiles+fileName;

    QFileInfo fi(filePath);
    QString ext = fi.completeSuffix();
    if(ext == "PNG")
    {
        QImage image;
        image.loadFromData(fileContent);
        image.save(filePath, "PNG");
    }
    else
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
            return;
        QDataStream out(&file);
        out<<fileContent;
        qDebug()<<"[MessageParser] Saved File with name "<<filePath;
        file.close();
    }

    emit fileSavedSignal(filePath);
}
