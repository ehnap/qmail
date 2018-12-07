#include "qmimeattach.h"
#include <QFileInfo>

QMimeAttach::QMimeAttach(const QString & attachPath)
	: QMimeObject()
	, m_attachPath(attachPath)
{
	m_type = "application/octet-stream";
	m_encodingType = QMimeObject::Base64;
}

QMimeAttach::~QMimeAttach()
{

}

QString QMimeAttach::toString() const
{
	QString contentText;
	contentText.append("Content-Type: ").append(m_type).append(";\r\n");
	QFileInfo fi = QFileInfo(m_attachPath);
	QString fileName = fi.fileName();
	contentText.append("\tname=\"").append(fileName).append("\"\r\n");
	contentText.append("Content-Transfer-Encoding: ");
	contentText.append(encodingTypeToStr(m_encodingType));
	contentText.append("Content-Disposition:attachment;\r\n");
	contentText.append("\tfilename=\"").append(fileName).append("\"\r\n");
	contentText.append("\r\n");
	contentText.append(content());
	contentText.append("\r\n");
	return contentText;
}

QString QMimeAttach::content() const
{
	QFile f(m_attachPath);
	if (!f.open(QIODevice::ReadOnly))
		return QString();
	return encodeHelper(f.readAll(), m_encodingType);
}
