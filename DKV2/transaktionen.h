#ifndef TRANSAKTIONEN_H
#define TRANSAKTIONEN_H

#include "contract.h"

bool checkSchema_ConvertIfneeded(const QString& origDbFile);

void activateContract(qlonglong vid);
void changeContractValue(qlonglong vid);

void deleteInactiveContract(qlonglong vid);
void terminateContract(qlonglong cid);
void terminateContract_Final( contract& v);
void cancelContract( contract& v);

void annualSettlement();

void editCreditor(qlonglong creditorId);
void newCreditorAndContract();

void createInvestment();

#endif // TRANSAKTIONEN_H
