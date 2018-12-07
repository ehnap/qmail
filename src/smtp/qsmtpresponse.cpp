#include "qsmtpresponse.h"

QSmtpResponse::QSmtpResponse()
    : m_retCode(0)
    , m_retText("success!")
{
}

QSmtpResponse::QSmtpResponse(int code, const QString &text)
    : m_retCode(code)
    , m_retText(text)
{
}

QSmtpResponse::QSmtpResponse(const QSmtpResponse &response)
{
    m_retCode = response.m_retCode;
    m_retText = response.m_retText;
}

QSmtpResponse::~QSmtpResponse()
{
}

void QSmtpResponse::setRetCode(int code)
{
    m_retCode = code;
}

int QSmtpResponse::retCode() const
{
    return m_retCode;
}

void QSmtpResponse::setRetText(const QString &text)
{
    m_retText = text;
}

QString QSmtpResponse::retText() const
{
    return m_retText;
}
