#include "../DKV2/creditor.h"
#include "../DKV2/contract.h"
#include "../DKV2/booking.h"

#include "../DKV2/helper.h"
#include "../DKV2/dkdbhelper.h"

#include "test_booking.h"

void test_booking::initTestCase()
{   LOG_CALL;
    init_DKDBStruct();
    initTestDb();
    fill_DkDbDefaultContent();
    if( QFile::copy(testDbFilename, tempFile))
        qInfo() << "speed up test with db copy " << tempFile;
}
void test_booking::cleanupTestCase()
{
    if( QFile::exists(tempFile))
        QFile::remove(tempFile);
}

void test_booking::init()
{   LOG_CALL;
    bool usingTempFile =false;
    if( QFile::exists(tempFile)) {
        closeDbConnection();
        QFile::remove(testDbFilename);
        usingTempFile =QFile::copy(tempFile, testDbFilename);
        openDbConnection(testDbFilename);
    }
    if( !usingTempFile) {
        qWarning() << "temp file not found";
        initTestDb();
        fill_DkDbDefaultContent();
    }
}
void test_booking::cleanup()
{   LOG_CALL;
    cleanupTestDb();
}

void test_booking::test_dateONSettlement_noContracts()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QCOMPARE( QDate(), bookings::dateOfnextSettlement());
}
void test_booking::test_dateONSettlement_activatedContracts()
{
    creditor c(saveRandomCreditor());
    contract cont2(saveRandomContract(c.id()));
    cont2.activate(QDate( 2000,12,31), 1000.);
    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 2000, 12, 31));

    contract cont(saveRandomContract(c.id()));
    cont.activate(QDate( 2000, 1, 1), 1000.);
    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 2000, 12, 31));

    contract cont3(saveRandomContract(c.id()));
    cont3.activate(QDate( 1999,12,31), 1000.);
    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 1999, 12, 31));
}
void test_booking::test_dateONSettelment_contractsW_interestBookings00()
{
    creditor cred(saveRandomCreditor());
    contract cont(saveRandomContract(cred.id()));
    cont.setInterestRate(1.);
    cont.activate(QDate( 2000,6,1), 1000.);
    booking::bookReInvestInterest(cred.id(), QDate(2001,1,1), 5.);
    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 2001, 12, 31));
}
void test_booking::test_dateONSettelment_contractsW_and_wo_interestBookings01()
{
    creditor cred(saveRandomCreditor());
    contract cont(saveRandomContract(cred.id()));
    cont.setInterestRate(1.);
    cont.activate(QDate( 2000,6,1), 1000.);
    booking::bookReInvestInterest(cred.id(), QDate(2001,1,1), 5.);

    contract cont2(saveRandomContract(cred.id()));
    cont2.activate(QDate(2000, 6,1), 1000.); // EARLIER

    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 2000, 12, 31));
}
void test_booking::test_dateONSettelment_contractsW_and_wo_interestBookings02()
{
    creditor cred(saveRandomCreditor());
    contract cont(saveRandomContract(cred.id()));
    cont.setInterestRate(1.);
    cont.activate(QDate( 2000,6,1), 1000.);
    booking::bookReInvestInterest(cred.id(), QDate(2001,1,1), 5.); // EARLIER

    contract cont2(saveRandomContract(cred.id()));
    cont2.activate(QDate(2002, 6,1), 1000.);

    QCOMPARE( bookings::dateOfnextSettlement(), QDate( 2001, 12, 31));
}
void test_booking::test_dateONSettelment_contractsW_and_wo_interestBookings03()
{
    QDate miniActDate/* =EndOfTheFuckingWorld*/;
    {
        dbgTimer timer("create many contracts");
        saveRandomCreditors(70);
        saveRandomContracts(100);    // contract date: 2 years back or less
        miniActDate = activateRandomContracts(90);// activation date: > contract date
    }
    {
        dbgTimer timer("calculate settlement year");
        QCOMPARE( bookings::dateOfnextSettlement(), QDate(miniActDate.year(), 12, 31));
    }
}
