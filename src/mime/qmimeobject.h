#ifndef QMIMEOBJECT_H
#define QMIMEOBJECT_H

#include <QString>

class QMimeObject
{
public:
    enum EncodingType
    {
        _7Bit,
        _8Bit,
        Base64,
        //todo QuotedPrintable
    };

    QMimeObject();
    virtual ~QMimeObject();

    void setCustomHeader(const QString& customheader);
    QString customHeader() const;

    void setContentId(const QString& id);
    QString contentId() const;

    void setContentName(const QString& name);
    QString contentName() const;

    void setContentType(const QString& contenttype);
    QString contentType() const;

    void setCharset(const QString& cst);
    QString charset() const;

    void setEncodingType(EncodingType type);
    EncodingType encodingType() const;

    virtual QString toString() const;

protected:
    virtual QString content() const = 0;
    virtual QString encodeHelper(const QByteArray& contentdata, EncodingType type) const;
	QString encodingTypeToStr(EncodingType type) const;

    QString m_customHeader;
    QString m_id;
    QString m_name;
    QString m_type;
    QString m_charset;
    QString m_boundary;
    EncodingType m_encodingType;

private:
	QString formatBase64(const QString& contentdata) const;
};

#endif // QMIMEOBJECT_H
