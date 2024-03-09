#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "credentialwidget.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLineEdit>
#include <QJsonParseError>

#include <openssl/evp.h>

// password is "vikter"

/*TODO
 * 1) создать незащищённффй файл с учетными записями
 * 2) считать его в приложении Qt
 */


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit, &QLineEdit::textEdited, this, &MainWindow::filterListWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Функция считывает учётные записи из файла json в структуру данных Qlist */
bool MainWindow::readJSON(const QByteArray & aes256_key)
{
    QFile jsonFile("C:/Users/viktortenaev/Desktop/kriptaALL/lab1_Tenyaev/lab1_ten/json/cridentials_enc.json"); //выводится не тот файл. Выводится ориг вместо зашифрованного
    // Проверка существования и доступности файла
    if (!jsonFile.exists()) {
        qDebug() << "File not found: cridentials_enc.json";
        return false;
    }

    if (!jsonFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open file: cridentials_enc.json";
        return false;
    }

    // jsonFile.readAll() -> QByteArray(hex+encrypted)
    // QByteArray(hex+encrypted) -> QByteArray::fromHex -> QByteArray(encrypted)
    // QByteArray(encrypted) -> расшифровка -> QByteArray(decrypted)
    // QByteArray(decrypted) -> парсинг JSON

    QByteArray hexEncryptedBytes = jsonFile.readAll();
    qDebug() << "*** hexEncryptedBytes orig" << hexEncryptedBytes;
    QByteArray encryptedBytes = QByteArray::fromHex(hexEncryptedBytes);
    qDebug() << "*** hexEncryptedBytes" << encryptedBytes;
    QByteArray decryptedBytes;
    int ret_code = decryptFile(aes256_key, encryptedBytes, decryptedBytes);
    qDebug() << "*** decryptFile(), decryptedBytes = " << decryptedBytes << "retCODE" << ret_code;

    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(decryptedBytes, &jsonErr);
    if(jsonErr.error != QJsonParseError::NoError)
        return false;
    qDebug() << "*** jsonDoc = " << jsonDoc;
    QJsonObject rootObject = jsonDoc.object();
    qDebug() << "*** rootObject = " << rootObject;

    m_jsonarray = rootObject["cridentials"].toArray();

    if (ret_code==0) {
        return true;
    }

    jsonFile.close();
}


void MainWindow::filterListWidget(const QString &searchString)
{

        ui->listWidget->clear();
        for (int i = 0; i < m_jsonarray.size(); i++){


            if(m_jsonarray[i].toObject()["site"]
                    .toString().toLower()
                    .contains(searchString.toLower())
                || searchString.isEmpty())
            {
                QListWidgetItem * newItem = new QListWidgetItem();
                credentialwidget * itemWidget =
                    new credentialwidget(m_jsonarray[i].toObject()["site"].toString(),ui->listWidget);

                newItem->setSizeHint(itemWidget->sizeHint());
                ui->listWidget->addItem(newItem);
                ui->listWidget->setItemWidget(newItem, itemWidget);
            }
        }

}

int MainWindow::decryptFile(
    const QByteArray & aes256_key,
    const QByteArray & encryptedBytes,
    QByteArray & decryptedBytes
    ) {
    qDebug() << "*** aes256_key " << aes256_key;

    unsigned char key[32] = {0};
    memcpy(key, aes256_key.data(), 32);
    qDebug() << "*** key " << key;

    QByteArray iv_hex("de1358eb7cd471c58dc76ea9a5977983");
    QByteArray iv_ba = QByteArray::fromHex(iv_hex);
    unsigned char iv[16] = {0};
    memcpy(iv, iv_ba.data(), 16);
    qDebug() << "*** iv " << iv;

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        qDebug() << "*** EVP_DecryptInit_ex2() ERROR ";
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    qDebug() << "*** EVP_DecryptInit_ex2() OK ";
#define BUF_LEN 256
    unsigned char encrypted_buf[BUF_LEN] = {0}, decrypted_buf[BUF_LEN] = {0};
    int encr_len, decr_len;

    QDataStream encrypted_stream(encryptedBytes);
    decryptedBytes.clear();

    QBuffer decrypted_buffer(&decryptedBytes);
    decrypted_buffer.open(QIODevice::WriteOnly);

    encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    while(encr_len > 0)
    {
        if (!EVP_DecryptUpdate(ctx,
                               decrypted_buf, &decr_len,
                               encrypted_buf, encr_len)) {
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        qDebug() << "*** EVP_DecryptUpdate():" << QByteArray(reinterpret_cast<char*>(decrypted_buf), decr_len);
        decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), decr_len);
        encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    }

    int tmplen;
    if (!EVP_DecryptFinal_ex(ctx, decrypted_buf, &tmplen)){
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    qDebug() << "*** EVP_DecryptFinal_ex():" << QByteArray(reinterpret_cast<char*>(decrypted_buf), tmplen);
    decrypted_buffer.write(reinterpret_cast<char*>(decrypted_buf), tmplen);
    decrypted_buffer.close();
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}


void MainWindow::on_editPin_returnPressed()
{
    // получить ключ из пин-кода
    QByteArray hash = QCryptographicHash::hash(
        ui->editPin->text().toUtf8(),
        QCryptographicHash::Sha256);
    qDebug() << "*** Sha256 = " << hash.toHex();

    // расшифровать файл и проверить верность пин-кода
    // если верный - сменить панель и отрисовать список
    // если неверный - предупреждение
    if(readJSON(hash)) {
        ui->stackedWidget->setCurrentIndex(0);
        filterListWidget("");
    } else {
        ui->lblLogin->setText("Неверный пин");
        ui->lblLogin->setStyleSheet("color:red;");
    }


    ui->editPin->setText(QString().fill('*', ui->editPin->text().size()));
    ui->editPin->clear();
    hash.setRawData(
        const_cast<char*>(QByteArray().fill('*', 32).data() ),
        32);
    hash.clear();
    // удалить ключ и пин код
}

