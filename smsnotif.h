#ifndef SMSNOTIF_H
#define SMSNOTIF_H

#include <QString>

class SmsNotif
{
public:
    SmsNotif();
    bool sendSMS(const QString &to, const QString &message);
};

#endif // SMSNOTIF_H
