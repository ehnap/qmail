#include "qemail.h"

QEmail::QEmail()
	: m_encodingType(QMimeObject::Base64)
    , m_contentData(nullptr)
{
    m_toList.clear();
}

QEmail::QEmail(const QString& address, const QString& name, const QString& subject)
	: m_encodingType(QMimeObject::Base64)
    , m_contentData(nullptr)
    , m_subject(subject)
{
    m_fromContact.name = name;
    m_fromContact.address = address;
    m_toList.clear();
}

QEmail::QEmail(const EmailContact& contact, const QString& subject)
    : m_fromContact(contact)
	, m_encodingType(QMimeObject::Base64)
    , m_contentData(nullptr)
    , m_subject(subject)
{
    m_toList.clear();
}

QEmail::~QEmail()
{
    if(m_contentData)
        delete m_contentData;
}

void QEmail::setSubject(const QString& subject)
{
    m_subject = subject;
}

QString QEmail::subject() const
{
    return m_subject;
}

void QEmail::setEncodingType(QMimeObject::EncodingType type)
{
    m_encodingType = type;
}

QMimeObject::EncodingType QEmail::encodingType() const
{
    return m_encodingType;
}

void QEmail::setfromContact(const EmailContact& contact)
{
    m_fromContact = contact;
}

EmailContact QEmail::fromContact() const
{
    return m_fromContact;
}

void QEmail::setFromName(const QString& name)
{
    m_fromContact.name = name;
}

QString QEmail::fromName() const
{
    return m_fromContact.name;
}

void QEmail::setFromAddress(const QString& address)
{
    m_fromContact.address = address;
}

QString QEmail::fromAddress() const
{
    return m_fromContact.address;
}

void QEmail::addToContact(const QString &address)
{
    EmailContact contact;
    contact.address = address;
    contact.name = "";
    m_toList.push_back(contact);
}

void QEmail::setToList(const QEmailContactList& tolist)
{
    m_toList = tolist;
}

QEmailContactList QEmail::toList() const
{
    return m_toList;
}

void QEmail::setContentData(QMimeObject *data)
{
    m_contentData = data;
}

QMimeObject *QEmail::contentData() const
{
    return m_contentData;
}

void QEmail::addCustomProperty(const QString &key, const QString &value)
{
	m_customProperty.insert(key, value);
}

void QEmail::removeCustomProperty(const QString &key)
{
    m_customProperty.remove(key);
}

QString QEmail::data() const
{
    QString srcText;

    /* =========== MIME HEADER ============ */

    /* ---------- Sender / From ----------- */
    srcText = "From:";
	if (!fromName().isEmpty())
        //srcText += fromName();
        srcText += encodingString(fromName(), m_encodingType);

    srcText += " <" + fromAddress() + ">\r\n";
    /* ---------------------------------- */


    /* ------- Recipients / To ---------- */
    srcText += "To:";
    QList<EmailContact>::const_iterator it;  int i;
    for (i = 0, it = m_toList.begin(); it != m_toList.end(); ++it, ++i)
    {
        if((*it).address.isEmpty())
            continue;

        if (i != 0) { srcText += ","; }

		if (!(*it).name.isEmpty())
            //srcText += (*it).name;
            srcText += encodingString((*it).name, m_encodingType);

        srcText += " <" + (*it).address + ">";
    }
    srcText += "\r\n";
    /* ---------------------------------- */

    /* ------- Recipients / Cc ---------- */
    srcText += "Cc:";
    for (i = 0, it = m_ccList.begin(); it != m_ccList.end(); ++it, ++i)
    {
        if((*it).address.isEmpty())
            continue;

        if (i != 0) { srcText += ","; }

        if (!(*it).name.isEmpty())
            //srcText += (*it).name;
            srcText += encodingString((*it).name, m_encodingType);

        srcText += " <" + (*it).address + ">";
    }
    srcText += "\r\n";
    /* ---------------------------------- */

    /* ------- Recipients / Bcc ---------- */
    srcText += "Bcc:";
    for (i = 0, it = m_bccList.begin(); it != m_bccList.end(); ++it, ++i)
    {
        if((*it).address.isEmpty())
            continue;

        if (i != 0) { srcText += ","; }

        if (!(*it).name.isEmpty())
            //srcText += (*it).name;
            srcText += encodingString((*it).name, m_encodingType);

        srcText += " <" + (*it).address + ">";
    }
    srcText += "\r\n";
    /* ---------------------------------- */

    /* ------------ Subject ------------- */
    srcText += "Subject:";
    srcText += encodingString(m_subject, m_encodingType);
    /* ---------------------------------- */

    srcText += "\r\n";
    srcText += "MIME-Version: 1.0\r\n";

    if(!m_customProperty.contains("X-Mailer:"))
        srcText += "X-Mailer: DreamyChi Smtp Library;\r\n";

    srcText += m_contentData->toString();
	srcText += "\r\n";
    return srcText;
}

QString QEmail::encodingString(const QString &content, QMimeObject::EncodingType type) const
{
    QString text;
    switch (type)
    {
	case QMimeObject::Base64:
        text = " =?utf-8?B?" + QByteArray().append(content).toBase64() + "?=";
        break;

    default:
        text = " " + content;
    }
    return text;
}

void QEmail::addToContact(const EmailContact& contact)
{
	m_toList.push_back(contact);
}
