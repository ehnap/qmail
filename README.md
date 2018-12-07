# QMail
A library with Qt framework to send emails.

##Usage
if send a text email:
```
#include <QCoreApplication>
#include "smtp/qsmtp.h"
#include "mime/qemail.h"
#include "mime/qmimetext.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSmtp smtp;
    smtp.connectToServer("smtp server address", port, QSmtp::DefaultConnection, 20000);
    //if use SSL, you should use QSmtp::SSLConnection
    smtp.login("username", "password");
    
    //set from address
    QEmail email;
    email.setFromAddress("from address");
    
    //set the recipient's e-mail address
    EmailContact to;
    to.name = "to Name";
    to.address = "to Address";
	email.addToContact(to);
	
	//set subject of the email
	email.setSubject("email subject");
	
    QMimeText textdata;
    textdata.setText("hello smtp!");
    email.setContentData(&textdata);
    smtp.sendEmail(email);

    return a.exec();
}
```

if send a email with a attach:
```
//...
QMimeAttach attachData("attach path");
QMimeMultipart multipartData;
multipartData.setText("email text content");
multipartData.addAttach(&attachData);
email.setContentData(&multipartData);
smtp.sendEmail(email);
//...
```
##References
* https://github.com/bluetiger9/SmtpClient-for-Qt
* https://github.com/dbzhang800/QtMail