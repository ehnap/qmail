#include "qmimeobject.h"

QMimeObject::QMimeObject()
    : m_encodingType(Base64)
{
}

QMimeObject::~QMimeObject()
{
}

void QMimeObject::setCustomHeader(const QString& customheader)
{
    m_customHeader = customheader;
}

QString QMimeObject::customHeader() const
{
    return m_customHeader;
}

void QMimeObject::setContentId(const QString &id)
{
    m_id = id;
}

QString QMimeObject::contentId() const
{
    return m_id;
}

void QMimeObject::setContentName(const QString &name)
{
    m_name = name;
}

QString QMimeObject::contentName() const
{
    return m_name;
}

void QMimeObject::setContentType(const QString &contenttype)
{
    m_type = contenttype;
}

QString QMimeObject::contentType() const
{
    return m_type;
}

void QMimeObject::setCharset(const QString &cst)
{
    m_charset = cst;
}

QString QMimeObject::charset() const
{
    return m_charset;
}

void QMimeObject::setEncodingType(QMimeObject::EncodingType type)
{
    m_encodingType = type;
}

QMimeObject::EncodingType QMimeObject::encodingType() const
{
    return m_encodingType;
}

QString QMimeObject::toString() const
{
    QString contentText;

    /* === Header Prepare === */

    /* Content-Type */
    //mimeText.append("Content-Type: ").append("multipart/alternative").append(";\r\n");
    contentText.append("Content-Type: ").append(m_type).append("; ");

    if (!m_charset.isEmpty())
        contentText.append("charset=").append(m_charset).append("\r\n");

	//m_boundary = "----=_001_NextPart676443054286_=----";
    contentText.append("boundary=\"").append(m_boundary).append("\"\r\n");

    /* Content-Transfer-Encoding */
    contentText.append("Content-Transfer-Encoding: ");
	contentText.append(encodingTypeToStr(m_encodingType));

    /* Message-Id */
    if (!m_id.isEmpty())
        contentText.append("Message-ID: <").append(m_id).append(">\r\n");
    /* ---------- */

    /* Addition header lines */
    if (!m_customHeader.isEmpty())
        contentText.append(m_customHeader).append("\r\n");

    /* ------------------------- */

    /* === End of Header Prepare === */
	contentText.append("\r\n");

    if (!m_name.isEmpty())
        contentText.append("name=\"").append(m_name).append("\"").append("\r\n");

    /* ------------------------ */

    /* === Content === */
	contentText.append("\r\n");
    contentText.append(content());
    contentText.append("\r\n");
	contentText.append(m_boundary).append("--\r\n");
    /* === End of Content === */

    return contentText;
}

QString QMimeObject::encodeHelper(const QByteArray& contentdata, QMimeObject::EncodingType type) const
{
    switch (type)
    {
    case _7Bit:
        return QString(contentdata).toLatin1();
    case _8Bit:
        return contentdata;
    case Base64:
       return formatBase64(contentdata.toBase64());
//    case QuotedPrintable:
//        mimeString.append(formatter.format(QuotedPrintable::encode(content), true));
//        break;
    }
    return contentdata;
}

QString QMimeObject::encodingTypeToStr(EncodingType type) const
{
	QString resultStr;
	switch (type)
	{
	case _7Bit:
		resultStr.append("7bit\r\n");
		break;
	case _8Bit:
		resultStr.append("8bit\r\n");
		break;
	case Base64:
		resultStr.append("base64\r\n");
		break;
		//    case QuotedPrintable:
		//        mimeString.append("quoted-printable\r\n");
		//        break;
	}
	return resultStr;
}

QString QMimeObject::formatBase64(const QString& contentdata) const
{
	int i = 0;
	int j = 0;
	QString formatdata;
	while (i < contentdata.size())
	{
		formatdata.append(contentdata[i]);
		if (j == 75)
		{
			formatdata.append("\n");
			j = -1;
		}
		++i;
		++j;
	}
	return formatdata;
}
