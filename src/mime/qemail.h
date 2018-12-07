#ifndef QEMAIL_H
#define QEMAIL_H

#include "qmail_global.h"
#include "qmimeobject.h"

#include <QString>
#include <QList>
#include <QHash>

struct QMAIL_EXPORT EmailContact
{
    QString name;
    QString address;
};

typedef QList<EmailContact> QEmailContactList;
typedef QHash<QString, QString> QEmailCustomProperty;

class QMAIL_EXPORT QEmail
{
public:
    QEmail();
    QEmail(const QString& fromAddress, const QString& fromName = "", const QString& subject = "");
    QEmail(const EmailContact& contact, const QString& subject = "");

    virtual ~QEmail();

    void setSubject(const QString& subject);
    QString subject() const;

	void setEncodingType(QMimeObject::EncodingType type);
    QMimeObject::EncodingType encodingType() const;

    void setfromContact(const EmailContact& contact);
    EmailContact fromContact() const;

    void setFromName(const QString& name);
    QString fromName() const;

    void setFromAddress(const QString& address);
    QString fromAddress() const;

    void addToContact(const QString& address);
	void addToContact(const EmailContact& contact);
	void setToList(const QEmailContactList& tolist);
    QEmailContactList toList() const;

    void setContentData(QMimeObject* data);
    QMimeObject* contentData() const;

	void QEmail::addCustomProperty(const QString &key, const QString &value);
	void QEmail::removeCustomProperty(const QString &key);

    virtual QString data() const;

private:
	QString encodingString(const QString& content, QMimeObject::EncodingType type) const;

    QString m_subject;
    EmailContact m_fromContact;
    QEmailContactList m_toList;
    QEmailContactList m_ccList;
    QEmailContactList m_bccList;
    QEmailCustomProperty m_customProperty;
    QMimeObject::EncodingType m_encodingType;
    QMimeObject* m_contentData;
};

#endif // QEMAIL_H
