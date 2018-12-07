#ifndef QMIMEMULTIPART_H
#define QMIMEMULTIPART_H

#include "qmimeobject.h"
#include "qmail_global.h"
#include <QList>

class QMimeAttach;
class QMAIL_EXPORT QMimeMultipart : public QMimeObject
{
public:
    QMimeMultipart();
    virtual ~QMimeMultipart();

	void setText(const QString& content);
    virtual QString toString() const override;

	void addAttach(QMimeAttach* attach);

protected:
    virtual QString content() const override;

    QList<QMimeAttach*> m_parts;

private:
	QString m_text;
};

#endif // QMIMEMULTIPART_H
