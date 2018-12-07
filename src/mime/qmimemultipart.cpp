#include "qmimemultipart.h"
#include "qmimeattach.h"
#include <QUuid>

QMimeMultipart::QMimeMultipart()
    : QMimeObject()
{
	m_type = "multipart/mixed";
	m_charset = "utf-8";
	m_encodingType = QMimeObject::_8Bit;
	m_boundary = "----=_001_NextPart425466543273_=----";
}

QMimeMultipart::~QMimeMultipart()
{

}

void QMimeMultipart::setText(const QString & content)
{
	m_text = content;
}

QString QMimeMultipart::toString() const
{
    QString mimeContent;

    /* === Header Prepare === */

    /* Content-Type */
    mimeContent.append("Content-Type: ").append(m_type).append(";\r\n");
	mimeContent.append("\tboundary=\"").append(m_boundary).append("\"\r\n");

    /* Message-Id */
    if (!m_id.isEmpty())
        mimeContent.append("Message-ID: <").append(m_id).append(">\r\n");
    /* ---------- */

    /* Addition header lines */
    if (!m_customHeader.isEmpty())
        mimeContent.append(m_customHeader).append("\r\n");
    /* ------------------------- */

    /* === End of Header Prepare === */
	mimeContent.append("\r\n");
    /* ------------------------ */
    mimeContent.append("\r\n");
    mimeContent.append("This is a multi-part message in MIME format.\r\n\r\n");

    /* === Content === */
	mimeContent.append("--").append(m_boundary).append("\r\n");
    mimeContent.append(content());
    /* === End of Content === */
    QString boundaryEnd = "--" + m_boundary + "--\r\n";
    mimeContent.append(boundaryEnd);

    return mimeContent;
}

void QMimeMultipart::addAttach(QMimeAttach* attach)
{
	m_parts.push_back(attach);
}

QString QMimeMultipart::content() const
{
    QString multiContent;
	multiContent.append("Content-Type: ").append("multipart/alternative").append(";\r\n");
	QString boundary = "----=_002_NextPart021751710155_=----";
	multiContent.append("\tboundary=\"").append(boundary).append("\"\r\n\r\n\r\n");

	multiContent.append("--").append(boundary).append("\r\n");
	multiContent.append("Content-Type: ").append("text/plain").append(";\r\n");
	multiContent.append("\tcharset=\"").append(m_charset).append("\"\r\n");
	multiContent.append("Content-Transfer-Encoding: ");
	multiContent.append(encodingTypeToStr(m_encodingType));
	multiContent.append("\r\n");
	multiContent.append(encodeHelper(m_text.toUtf8(), m_encodingType));
	multiContent.append("\r\n");
	multiContent.append("--").append(boundary).append("--\r\n\r\n");

    for (auto it = m_parts.begin(); it != m_parts.end(); ++it)
    {
		multiContent.append("--").append(m_boundary).append("\r\n");
        multiContent.append((*it)->toString());
        multiContent.append("\r\n");    
    }

    return multiContent;
}
