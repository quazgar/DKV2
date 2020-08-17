#include <QtTest>

#include "../DKV2/helper.h"
#include "../DKV2/helpersql.h"
#include "../DKV2/dkdbhelper.h"
#include "../DKV2/creditor.h"
#include "../DKV2/contract.h"

#include "test_contract.h"

void test_contract::initTestCase()
{   LOG_CALL;
    init_DKDBStruct();
}
void test_contract::cleanupTestCase()
{   LOG_CALL;
}
void test_contract::init()
{   LOG_CALL;
    initTestDb();
    QVERIFY(create_DK_TablesAndContent());
}
void test_contract::cleanup()
{   LOG_CALL;
    cleanupTestDb();
}

void test_contract::test_createUninit_Contract()
{   LOG_CALL;
    contract c;
}

void test_contract::test_set_get_interest()
{   LOG_CALL;
    contract c;
    c.setInterestRate(1.5);
    QCOMPARE( c.interestRate(), 1.5);
    c.setInterestRate(1.49);
    QCOMPARE( c.interestRate(), 1.49);
    creditor cre = saveRandomCreditor();
    c.setCreditorId(cre.id());
    c.saveNewContract();
    contract d(c.id());
    QCOMPARE(d.interestRate(), 1.49);
}

void test_contract::test_activateContract()
{   LOG_CALL;
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QVERIFY(cont.isActive() == false);
    QVERIFY(cont.activate(QDate::currentDate(), cont.plannedInvest()));
    QVERIFY(cont.isActive() == true);
    // activating an active contract fails:
    QVERIFY(false == cont.activate(QDate::currentDate(), cont.plannedInvest()));
}

void test_contract::test_randomContract()
{   LOG_CALL;
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QCOMPARE(rowCount("Vertraege"), 1);
}

void test_contract::test_randomContracts()
{   LOG_CALL;
    int count = 50;
    dbgTimer t(QString::number(count) + " contracts");
    saveRandomCreditors(count/3);
    saveRandomContracts(count);
    QCOMPARE(rowCount("Vertraege"), count);
}

void test_contract::test_write_read_contract()
{   LOG_CALL;
    creditor c(saveRandomCreditor());
    contract cont_write(saveRandomContract(c.id()));
    contract cont_loaded(cont_write.id());
    QCOMPARE(cont_write, cont_loaded);
}

void test_contract::deposit_inactive_contract_fails()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QVERIFY( ! cont.deposit(QDate::currentDate(), 1000.));
    QVERIFY( ! cont.payout(QDate::currentDate(), 1000.));
    QDate aDate = QDate(2019, 1, 1);
    cont.activate(aDate, 1000.);
    QVERIFY(cont.deposit(aDate.addMonths(6), 1000.));
    QVERIFY(cont.payout(aDate.addMonths(6).addDays(1), 100.));
}

void test_contract::too_high_payout_fails()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.activate(QDate::currentDate(), 1000.);
    QVERIFY( ! cont.payout(QDate::currentDate().addDays(1), 1001.));
}

void test_contract::unsequenced_bookings_fail()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QDate aDate = QDate::currentDate();
    cont.activate(aDate, 1000.);
    QVERIFY( ! cont.deposit(aDate, 1000.));
    QVERIFY( ! cont.deposit(aDate.addDays(-1), 1000.));
    QVERIFY( ! cont.payout(aDate, 100.));
    QVERIFY( ! cont.payout(aDate.addDays(-1), 100.));
}

void test_contract::test_annualSettlement_inactive_fails()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(1.);
    cont.setReinvesting(true);
    QVERIFY( ! cont.annualSettlement(2019));

    cont.activate(QDate(2019, 7, 1), 1000.);
    QCOMPARE(cont.annualSettlement(2020), 2020);
    QCOMPARE(cont.value(), 1005.);
}

void test_contract::test_annualSettlement_fullYear()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(1.);
    cont.setReinvesting(true);

    cont.activate(QDate(2019, 1, 1), 1000.);
    QCOMPARE(cont.annualSettlement(2020), 2020);
    QCOMPARE(cont.value(), 1010.);
}

void test_contract::test_annualSettlement_twoYear()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(1.);
    cont.setReinvesting(true);

    cont.activate(QDate(2019, 1, 1), 1000.);
    QCOMPARE(cont.annualSettlement(2020), 2020);
    QCOMPARE(cont.annualSettlement(2021), 2021);
    QCOMPARE(cont.value(), 1020.1);
}

void test_contract::test_deposit01()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(2.0);
    cont.setReinvesting(true);
    cont.activate(QDate(2019, 1, 1), 1000.);
    cont.deposit(QDate(2019, 7, 1), 1000.);
    QCOMPARE(cont.value(), 2010.);
}

void test_contract::test_depositFailsOn_1_1()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(2.0);
    cont.setReinvesting(true);
    cont.activate(QDate(2019, 1, 1), 1000.);
    QVERIFY( ! cont.deposit(QDate(2020, 1, 1), 1000.));
    QVERIFY( ! cont.payout(QDate(2020, 1, 1), 100));
}

void test_contract::test_deposit_wSettlement()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(2.0);
    cont.setReinvesting(true);
    cont.activate(QDate(2019, 1, 1), 1000.);
    cont.deposit(QDate(2020, 7, 1), 1000.);
    QCOMPARE(cont.value(), 2030.2);
    // booking 1: activation (deposit)
    // booking 2: annual settlement 2019 (1.1.2020)
    // booking 3: interest deposit (1.7. 2020)
    // booking 4: deposit
    QCOMPARE(bookings::getBookings(cont.id()).count(), 4);
}

void test_contract::test_payout()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(2.0);
    cont.setReinvesting(true);
    cont.activate(QDate(2019, 1, 1), 1000.);
    cont.payout(QDate(2019, 7, 1), 500.);
    QCOMPARE(cont.value(), 510.);
}

void test_contract::test_payout_wSettlement()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(2.0);
    cont.setReinvesting(true);
    cont.activate(QDate(2019, 1, 1), 1000.);
    cont.payout(QDate(2020, 7, 1), 500.);
    QCOMPARE(cont.value(), 530.2);
    QCOMPARE(bookings::getBookings(cont.id()).count(), 4);
}

void test_contract::test_activationDate()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QDate aDate = QDate(2020, 5, 1);
    QCOMPARE(cont.activationDate(), QDate());
    cont.activate(aDate, 1000.);
    QCOMPARE(cont.activationDate(), aDate);
    cont.deposit(aDate.addMonths(6), 1000.);
    QCOMPARE(cont.activationDate(), aDate);
    booking::investInterest(cont.id(), QDate(2021, 1, 1), 10.);
    QCOMPARE(cont.activationDate(), aDate);
}

//void test_contract::test_latestSettlementDate()
//{
//    creditor c(saveRandomCreditor());
//    contract cont(saveRandomContract(c.id()));

//    QDate aDate = QDate(2020, 5, 1);
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate());

//    cont.activate(1000., aDate);
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate());

//    cont.deposit(1000., aDate.addMonths(6)); // same year
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate(2020, 11, 1));

//    QCOMPARE(cont.annualSettlement(), 2021); // +1y
//    QVERIFY( cont.deposit(1000., QDate(2021, 2, 1)));
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate(2021, 1, 1));

//    QCOMPARE(cont.annualSettlement(), 2022);
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate(2022, 1, 1));

//    cont.deposit(1000., QDate(2024, 2, 1));
//    QCOMPARE(cont.latestInterestPaymentDate(), QDate(2024, 1, 1));
//}

void test_contract::test_getValue_byDate()
{
    dbgTimer t("8x contract::getValue()");
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(1.0);
    cont.setReinvesting();
    QDate aDate = QDate(2020, 5, 1);

    cont.activate(aDate, 1000.);
    QCOMPARE(cont.value(aDate), 1000.);
    QCOMPARE(cont.value(aDate.addMonths(1)), 1000.);

    cont.deposit(aDate.addMonths(6), 1000.); // 1.11.
    QCOMPARE(cont.value(aDate.addMonths(7)), 2005.);
    cont.payout(aDate.addYears(2), 100);
    QCOMPARE(cont.value(), 1935.18); // verified in excel
}

void test_contract::test_getValue_byDate_wInterestPayout()
{
    dbgTimer t("8x contract::getValue()");
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    cont.setInterestRate(1.0);
    cont.setReinvesting(false);
    QDate anydate = QDate(2020, 5, 1);

    // booking 1
    cont.activate(anydate, 1000.); // 1.5.2020
    QCOMPARE(cont.value(anydate), 1000.);
    QCOMPARE(cont.value(anydate.addMonths(1)), 1000.); // 1.6.2020

    cont.deposit(anydate.addMonths(6), 1000.); // 1.11.2020
    // deposit forces interest depoits, even for not reinvesting contracts
    // booking 2: interest deposit
    // booking 3: deposit
    QCOMPARE(cont.value(anydate.addMonths(7)), 2005.); // 1.12.2020
    QCOMPARE(bookings::getBookings(cont.id()).count(), 3);

    // payout forces settlement of 2021
    // bookint 4: interest deposit 1.11.2020 - 1.1.2021
    // booking 5: payout of interest (3.34 Euro)
    // booking 6: interest deposit 1.1.2021 - 1.1.2022
    // booking 7: payout of interest (20.05)
    // booking 8: interest deposit 1.1.2022 - 1.5.2022
    // booking 9: payout 100 (4 month -> 6.68 Euro)
    cont.payout(anydate.addYears(2), 100); // 1.5.2022
    QCOMPARE(cont.value(), 1911.68); // verified in excel
    QCOMPARE(bookings::getBookings(cont.id()).count(), 9);
}

void test_contract::test_finalize()
{
    creditor c(saveRandomCreditor());
    contract cont(saveRandomContract(c.id()));
    QDate aDate = QDate(2020, 5, 1);
    cont.activate(aDate, 1000.);
    cont.deposit(aDate.addMonths(1), 1000.);
    QCOMPARE(tableRecordCount("Vertraege"), 1);
    QCOMPARE(tableRecordCount("Buchungen"), 3);
    QString contractId = QString::number(cont.id());
    double fi =0., fp =0.;
    cont.finalize(false, aDate.addMonths(2), fi, fp);
    // finalize should reset the cont object
    QCOMPARE(cont.id(), -1);
    QCOMPARE(tableRecordCount("Vertraege"), 0);
    QCOMPARE(tableRecordCount("Buchungen"), 0);
    QCOMPARE(tableRecordCount("exVertraege"), 1);
    QCOMPARE(tableRecordCount("exBuchungen"), 5);
    QCOMPARE(executeSingleValueSql(
             contract::getTableDef_deletedContracts()["LaufzeitEnde"],
             "id=" +contractId), QDate(aDate.addMonths(2)));
}
