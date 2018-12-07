#ifndef QSMTP_H
#define QSMTP_H

#include "qmail_global.h"
#include "qsmtpresponse.h"
#include "qemail.h"
#include <QObject>

class QSslSocket;

class QMAIL_EXPORT QSmtp : public QObject
{
    Q_OBJECT

public:
    enum ConnectionType
    {
        DefaultConnection,
        SSLConnection,
    };

	enum AuthMethod
	{
		AuthLogin,
		AuthPlain,
	};

    explicit QSmtp(QObject *parent = 0);
    virtual ~QSmtp();

    bool connectToServer(const QString& host,
                       int port = 25,
                       ConnectionType type = DefaultConnection,
                       int timeout = 3000,
                       bool useSTARTTLS = true);
    bool login(const QString& username, const QString& password, AuthMethod authMethod = AuthPlain);
    bool sendEmail(const QEmail& email);
    bool quit();

protected:
    QSmtpResponse getResponse();
    bool sendMessage(const QString &text);

signals:
    void errorOccured(int, QString);

private slots:
    void onErrorOccured(int, QString);

private:
    QSslSocket* m_sslSocket;
    ConnectionType m_type;
    int m_timeout;
};

#endif // QSMTP_H
