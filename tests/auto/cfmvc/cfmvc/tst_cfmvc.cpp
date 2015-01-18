#include <QtTest/QtTest>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qdir.h>
#include <QtCore/qtemporarydir.h>
#include <QtCore/qfileinfo.h>

class tst_CFMvc : public QObject
{
    Q_OBJECT
public:
    tst_CFMvc()

private slots:
    void initTestCase();
};

void tst_CFMvc::initTestCase()
{
}

QTEST_MAIN(tst_CFMvc)

#include "tst_cfmvc.moc"
