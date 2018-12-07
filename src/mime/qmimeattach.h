#ifndef QMIMEATTACH_H
#define QMIMEATTACH_H

#include "qmimeobject.h"

class QMimeAttach : public QMimeObject
{
public:
    QMimeAttach(const QString& attachPath);
    virtual ~QMimeAttach();

    virtual QString toString() const override;

protected:
    virtual QString content() const override;

private:
	QString m_attachPath;
};

#endif // QMIMEATTACH_H
