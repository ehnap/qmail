#include "qmimetext.h"

QMimeText::QMimeText()
	: QMimeObject()
{
	m_type = "text/plain";
	m_charset = "utf-8";
    m_encodingType = _8Bit;
}

QMimeText::QMimeText(const QString& text)
	: QMimeObject()
    , m_text(text)
{
	m_type = "text/plain";
	m_charset = "utf-8";
    m_encodingType = _8Bit;
}

QMimeText::~QMimeText()
{
}

QString QMimeText::text() const
{
    return m_text;
}

void QMimeText::setText(const QString& text)
{
    m_text = text;
}

QString QMimeText::toString() const
{
    QString mimeText;

    /* === Header Prepare === */

    /* Content-Type */
    mimeText.append("Content-Type: ").append(m_type).append("; ");

    if (!m_charset.isEmpty())
        mimeText.append("charset=").append(m_charset).append("\r\n");

    /* Message-Id */
    if (!m_id.isEmpty())
        mimeText.append("Message-ID: <").append(m_id).append(">\r\n");
    /* ---------- */

    /* Addition header lines */
    if (!m_customHeader.isEmpty())
        mimeText.append(m_customHeader).append("\r\n");
    /* ------------------------- */

    /* === End of Header Prepare === */

    mimeText.append("\r\n");

    /* ------------------------ */

    /* === Content === */
    mimeText.append("\r\n");
    mimeText.append(content());
    mimeText.append("\r\n");
    /* === End of Content === */

    return mimeText;
}

QString QMimeText::content() const
{
    return m_text;
}
