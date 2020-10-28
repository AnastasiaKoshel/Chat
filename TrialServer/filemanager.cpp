#include "filemanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "jsonType.h"

FileManager::FileManager(/*QTcpSocket* sender,*/ QTcpSocket* receiver, int size, QString login, QString name):
    //senderSocket(sender),
    receipientSocket(receiver),
    totalSizeExpected(size),
    senderLogin(login),
    fileName(name)
{

}


void FileManager::sendFileJSON()
{
    //TODO: emit signal to turn of file receive flag
    qDebug()<<"[FileManager] Send file with name" <<fileName;
    QJsonObject messageJson;
    messageJson["type"] = JSONType::FILE_MESSAGE;
    messageJson["fileName"] = fileName;
    messageJson["size"] = totalSizeExpected;
    messageJson["senderLogin"] = senderLogin;

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    receipientSocket->write(jsonData);

    qDebug()<<"[FileManager] Send file data of size = "<<data.size();
    receipientSocket->write(data);
    data.clear();
}

void FileManager::getFileData(QByteArray& curData)
{
    data+=curData;
    qDebug()<<"[FileManager] already received "<<data.size()<<" bytes, expected: "<<totalSizeExpected;
    if(data.size()>=totalSizeExpected)
    {
        sendFileJSON();
    }
}
