#include "qsmtp.h"

#include <QSslSocket>

static const int TIMEOUT_CONNECT				= -1;
static const int TIMEOUT_SSLCONNECT				= -2;
static const int TIMEOUT_FROMHOST				= -3;
static const int TIMEOUT_TOHOST					= -4;
static const int SUCCESS_AUTHPLAIN				= 235;
static const int SUCCESS_AUTHLOGIN				= 334;
static const int SUCCESS_AUTHLOGIN_USERNAME		= SUCCESS_AUTHLOGIN;
static const int SUCCESS_AUTHLOGIN_PASSWORD		= SUCCESS_AUTHPLAIN;
static const int SUCCESS_SENDDATA				= 354;
static const int SUCCESS_ENDDATA				= 250;
static const int SUCCESS_EHLO					= SUCCESS_ENDDATA;
static const int SUCCESS_CONNECT				= 220;
static const int SUCCESS_STARTTLS				= SUCCESS_CONNECT;
static const int SUCCESS_MAILFROM				= SUCCESS_ENDDATA;
static const int SUCCESS_RCPTTO					= SUCCESS_ENDDATA;

QSmtp::QSmtp(QObject *parent)
    : QObject(parent)
{
    m_sslSocket = new QSslSocket(this);
}

QSmtp::~QSmtp()
{
}

bool QSmtp::connectToServer(const QString &host, int port, QSmtp::ConnectionType type, int timeout, bool useSTARTTLS)
{
    QSmtpResponse result;
    m_timeout = timeout;
    switch(type)
    {
    case SSLConnection:
        m_sslSocket->connectToHostEncrypted(host, port);
        break;
    case DefaultConnection:
    default:
        m_sslSocket->connectToHost(host, port);
        break;
    }

    if (!m_sslSocket->waitForConnected(m_timeout))
    {
        emit errorOccured(TIMEOUT_CONNECT, "Timeout:Connect to host!");
        return false;
    }

    // Wait for the server's response
    result = getResponse();

    // If the response code is not 220 (Service ready)
    // means that is something wrong with the server
    if (result.retCode() != SUCCESS_CONNECT)
    {
        emit errorOccured(result.retCode(), result.retText());
        return false;
    }

    // Send a EHLO/HELO message to the server
    // The client's first command must be EHLO/HELO
    if(!sendMessage("EHLO " + host))
        return false;

    // Wait for the server's response
    result = getResponse();

    // The response code needs to be 250.
    if (result.retCode() != SUCCESS_EHLO) {
        emit errorOccured(result.retCode(), result.retText());
        return false;
    }

    if (useSTARTTLS) {
        // send a request to start TLS handshake
        if(!sendMessage("STARTTLS"))
            return false;

        // Wait for the server's response
        result = getResponse();

        // The response code needs to be 220.
        if (result.retCode() != SUCCESS_STARTTLS) {
            emit errorOccured(result.retCode(), result.retText());
            return false;
        };

        m_sslSocket->startClientEncryption();

        if (!m_sslSocket->waitForEncrypted(m_timeout)) {
            qDebug() << m_sslSocket->errorString();
            emit errorOccured(TIMEOUT_SSLCONNECT, "Timeout:Ssl connection encrypt!");
            return false;
        }

        // Send ELHO one more time
        if(!sendMessage("EHLO " + host))
            return false;

        // Wait for the server's response
        result = getResponse();

        // The response code needs to be 250.
        if (result.retCode() != SUCCESS_EHLO) {
            emit errorOccured(result.retCode(), result.retText());
            return false;
        }
    }

    return true;
}

bool QSmtp::login(const QString& username, const QString& password, AuthMethod authMethod/* = AuthPlain*/)
{
    QSmtpResponse result;
	switch (authMethod)
	{
	case QSmtp::AuthLogin:
	{
		// Sending command: AUTH LOGIN
		if (!sendMessage("AUTH LOGIN"))
			return false;

		// Wait for 334 response code
		result = getResponse();
		if (result.retCode() != SUCCESS_AUTHLOGIN)
		{
			emit errorOccured(result.retCode(), result.retText());
			return false;
		}

		// Send the username in base64
		if (!sendMessage(QByteArray().append(username).toBase64()))
			return false;

		// Wait for 334
		result = getResponse();
		if (result.retCode() != SUCCESS_AUTHLOGIN_USERNAME)
		{
			emit errorOccured(result.retCode(), result.retText());
			return false;
		}

		// Send the password in base64
		if (!sendMessage(QByteArray().append(password).toBase64()))
			return false;

		// Wait for the server's responce
		result = getResponse();

		// If the response is not 235 then the authentication was faild
		if (result.retCode() != SUCCESS_AUTHLOGIN_PASSWORD)
		{
			emit errorOccured(result.retCode(), result.retText());
			return false;
		}
		break;
	}
	case QSmtp::AuthPlain:
	{
		// Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
		if (!sendMessage("AUTH PLAIN " + QByteArray().append((char)0).append(username).append((char)0).append(password).toBase64()))
			return false;

		// Wait for the server's response
		result = getResponse();

		// If the response is not 235 then the authentication was faild
		if (result.retCode() != SUCCESS_AUTHPLAIN)
		{
			emit errorOccured(result.retCode(), result.retText());
			return false;
		}
		break;
	}
	default:
		return false;
		break;
	}

    return true;
}

bool QSmtp::sendEmail(const QEmail& email)
{
    QSmtpResponse result;
    // Send the MAIL command with the sender
    if(!sendMessage("MAIL FROM: <" + email.fromAddress() + ">"))
        return false;

    result = getResponse();

    if (result.retCode() != SUCCESS_MAILFROM)
    {
        emit errorOccured(result.retCode(), result.retText());
        return false;
    }

    // Send RCPT command for each recipient
    QEmailContactList::const_iterator it, itEnd;
    // To (primary recipients)
    QEmailContactList contactToList = email.toList();
    for (it = contactToList.begin(), itEnd = contactToList.end();
         it != itEnd; ++it)
    {
        if(!sendMessage("RCPT TO: <" + (*it).address + ">"))
            return false;

        result = getResponse();

        if (result.retCode() != SUCCESS_RCPTTO)
        {
            emit errorOccured(result.retCode(), result.retText());
            return false;
        }
    }

    // Send DATA command
    if(!sendMessage("DATA"))
        return false;

    result = getResponse();

    if (result.retCode() != SUCCESS_SENDDATA)
    {
        emit errorOccured(result.retCode(), result.retText());
        return false;
    }

    if(!sendMessage(email.data()))
        return false;

    // Send \r\n.\r\n to end the mail data
    if(!sendMessage(".\r\n"))
        return false;

    result = getResponse();

    if (result.retCode() != SUCCESS_ENDDATA)
    {
        emit errorOccured(result.retCode(), result.retText());
        return false;
    }

    return true;
}

bool QSmtp::quit()
{
    return sendMessage("QUIT");
}

QSmtpResponse QSmtp::getResponse()
{
    int ret = 0;
    QString text("success!");
    do {
        if (!m_sslSocket->waitForReadyRead(m_timeout))
            return QSmtpResponse(TIMEOUT_FROMHOST, "Timeout:Get response from host!");

        while (m_sslSocket->canReadLine()) {
            // Save the server's response
            QString responseText = m_sslSocket->readLine();

            // Extract the respose code from the server's responce (first 3 digits)
            ret = responseText.left(3).toInt();
            text = responseText.mid(4);
			if (responseText[3] == ' ') { return QSmtpResponse(ret, text); }
        }
    } while (true);

    return QSmtpResponse(ret, text);
}

bool QSmtp::sendMessage(const QString &text)
{
    m_sslSocket->write(text.toUtf8() + "\r\n");
    if (! m_sslSocket->waitForBytesWritten(m_timeout))
    {
      emit errorOccured(TIMEOUT_TOHOST, "Timeout:Send data to host!");
      return false;
    }

    return true;
}

void QSmtp::onErrorOccured(int code, QString text)
{
    qDebug() << code << "\n" << text;
}
