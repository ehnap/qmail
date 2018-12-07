#ifndef QMIMETEXT_H
#define QMIMETEXT_H

#include "qmimeobject.h"
#include "qmail_global.h"
#include <QString>

class QMAIL_EXPORT QMimeText : public QMimeObject
{
public:
    QMimeText();
    QMimeText(const QString& text);
    virtual ~QMimeText();

    QString text() const;
    void setText(const QString& text);

    virtual QString toString() const override;
protected:
    virtual QString content() const override;

    QString m_text;
};

#endif // QMIMETEXT_H
