#include "client.h"
#include "ui_client.h"
#include <QTcpSocket>
#include <QCoro/QCoroNetwork> // Required to await QTcpSocket
#include <QMessageBox>        // Required for UI feedback
#include <QDebug>

client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}

QCoro::Task<void> client::sendNetworkMessage(QString host, quint16 port, QString message)
{
    // TODO: Create a QTcpSocket locally. (It will be destroyed automatically when this function ends).
    QTcpSocket socket;
    // TODO: Print a qDebug() message stating you are attempting to connect to the host and port.
    qDebug() << "Attempting to connect to " << host << ":" << port;
    // TODO: Initiate the connection to the host using the socket.
    socket.connectToHost(host, port);
    // TODO: Asynchronously wait for the connection to establish using co_await and qCoro.
    // Save the result into a boolean variable (e.g., 'connected').
    bool connected = co_await qCoro(socket).waitForConnected();
    // TODO: Write an if-statement. If NOT connected:
    // 1. Show a QMessageBox::critical stating the connection failed, using socket.errorString().
    // 2. co_return; to exit early.
    if (!connected){
        QMessageBox::critical(nullptr, "Connection Failed", "Could not connect to server: " + socket.errorString());
        co_return;
    }
     // TODO: Print a qDebug() message confirming connection.
    qDebug() << "Conneted Successfully to " << host << ":" << port;

    // TODO: Write the 'message' variable to the socket (remember to convert it to Utf8).
    socket.write(message.toUtf8());
    // TODO: Tell Qt to gracefully disconnect from the host.
    socket.disconnectFromHost();
    // TODO: Asynchronously wait for Qt to finish flushing the buffer and disconnecting using co_await and qCoro.
    co_await qCoro(socket).waitForDisconnected();
    // TODO: Show a QMessageBox::information stating the message was sent successfully.
    QMessageBox::information(nullptr, "Success", "Message sent successfully!");
}
