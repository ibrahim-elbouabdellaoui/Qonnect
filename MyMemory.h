#ifndef MYMEMORY_H
#define MYMEMORY_H
#include<QString>

class MyMemory
{
    QString API_URL ;
public:
    MyMemory();
    QString Traduire(const QString& ,const QString&,const QString&) ;
};

#endif // MYMEMORY_H


