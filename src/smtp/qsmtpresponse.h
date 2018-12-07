#ifndef QSMTPRESPONSE_H
#define QSMTPRESPONSE_H

#include <QString>

class QSmtpResponse
{
public:
    QSmtpResponse();
    explicit QSmtpResponse(int code, const QString& text);
    QSmtpResponse(const QSmtpResponse& response);
    ~QSmtpResponse();

    void setRetCode(int code);
    int retCode() const;

    void setRetText(const QString& text);
    QString retText() const;

private:
    int m_retCode;
    QString m_retText;
};

#endif // QSMTPRESPONSE_H
