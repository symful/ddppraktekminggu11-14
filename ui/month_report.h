#include "../db/include.h"

#ifndef UI_MONTH_REPORT_H
#define UI_MONTH_REPORT_H

void showMonthReport(struct MonthReport *monthReport);
void openTransactionEditMenu(struct MonthReport *monthReport);
void openTransactionDeleteMenu(struct MonthReport *monthReport);
void openTransactionAddToReportMenu(struct MonthReport *monthReport);
void openMonthReportEditMenu(struct MonthReportList *monthReportList);
void openMonthReportDeleteMenu(struct MonthReportList *monthReportList);
void openMonthlyListMenu(struct MonthReportList *monthReportList);
void openMonthlyMenu(struct MonthReportList *monthReportList);
void openMonthReport(struct MonthReport *monthReport);
void openTransactionAddMenu(struct MonthReportList *monthReportList);
void showAllMonthReportSummary(struct MonthReportList *monthReportList);
void showMonthlyMenu(struct MonthReportList *monthReportList);
void showMonthlyList(struct MonthReportList *monthReportList);
void showMonthReportTransactions(struct MonthReport *monthReport);
void showTransactionGroupsList(struct MonthReport *monthReport);
void openTransactionViewMenu(struct MonthReport *monthReport);
void showTransactionDetails(struct TransactionGroup *group);
void openSelectReportMenu(struct MonthReportList *monthReportList);
void openCategoryBudgetMenu(struct MonthReport *monthReport);
void showMonthReportSummary2(struct MonthReport *monthReport);

void renderMonthlyMenuHeader(int boxWidth, int contentLines);
void renderMonthlyMenuOptions(int boxWidth);
void renderMonthlyMenuFooter(int boxWidth, int totalReports);

void renderMonthlyListHeader(int boxWidth);
void renderEmptyMonthlyList(int boxWidth);
void renderSingleMonthlyListItem(int index,
                                 struct MonthReport *report,
                                 int boxWidth);

void showMonthlyMenu(struct MonthReportList *monthReportList);
void showMonthlyList(struct MonthReportList *monthReportList);

void renderTransactionGroupsHeader(int boxWidth);

// Renderer jika list kosong
void renderEmptyTransactionGroups(int boxWidth);

// Renderer satu baris transaksi
void renderTransactionGroupRow(int index, struct TransactionGroup *group, int boxWidth);

// Renderer footer tabel
void renderTransactionGroupsFooter(int boxWidth);

// Fungsi utama setelah modularisasi
void showTransactionGroupsList(struct MonthReport *monthReport);

void renderMonthReportMenu(struct MonthReport *monthReport);
int handleMonthReportChoice(struct MonthReport *monthReport, int choice);

// Monthly main menu
void renderMonthlyMainMenu(struct MonthReportList *monthReportList);
int handleMonthlyMenuChoice(struct MonthReportList *monthReportList, int choice);
void renderTransactionAddMenuHeader(int boxWidth, int contentLines);
int readNewMonthYear(char *buffer, size_t bufferSize, time_t *outDate, int boxWidth);
int validateMonthUniqueness(struct MonthReportList *list, time_t date);
void saveNewMonthReport(struct MonthReportList *list, struct MonthReport *report);
void openTransactionAddMenu(struct MonthReportList *list);

// UI
void renderMonthlyListMenuHeader(int boxWidth);
void renderMonthlyListMenuOptions(int boxWidth);
void renderMonthlyListMenuFooter(int boxWidth);

// handler
int handleMonthlyListMenuChoice(int choice, struct MonthReportList *list);

// main
void openMonthlyListMenu(struct MonthReportList *list);

int ensureReportsExist(struct MonthReportList *list);
void renderReportSelectionList(struct MonthReportList *list);
int readReportSelection(int *index, struct MonthReportList *list);
void openSelectedMonthReport(struct MonthReportList *list, int index);

void openSelectReportMenu(struct MonthReportList *monthReportList);

void renderTransactionViewHeader(int boxWidth);
void renderTransactionGroupList(struct MonthReport *report);
int ensureTransactionGroupsExist(struct MonthReport *report);
int readTransactionGroupSelection(struct MonthReport *report, int *index);
void showSelectedGroupDetails(struct MonthReport *report, int index);

void openTransactionViewMenu(struct MonthReport *monthReport);

// Header tampilan
void renderTransactionDetailsHeader(int boxWidth,
                                    const char *categoryStr);

// Ringkasan budget/sisa/status
void renderTransactionSummary(struct TransactionGroup *group,
                              int boxWidth);

// Pesan jika tidak ada transaksi
int renderEmptyTransactionsMessage(int boxWidth);

// Header tabel transaksi
void renderTransactionTableHeader(int tableWidth);

// Baris transaksi tunggal
void renderTransactionRow(struct Transaction *t,
                          int index,
                          int tableWidth);
void renderTransactionTableFooter(int tableWidth);

// Fungsi utama
void showTransactionDetails(struct TransactionGroup *group);

void renderTransactionEditHeader(int boxWidth);
struct TransactionGroup *handleTransactionGroupSelection(struct MonthReport *monthReport);
struct Transaction *handleTransactionSelection(struct TransactionGroup *group);
void performTransactionEditing(struct Transaction *transaction, int boxWidth);
void openTransactionEditMenu(struct MonthReport *monthReport);

void renderTransactionEditTitle(struct Transaction *transaction, int boxWidth);
void editTransactionDate(struct Transaction *transaction, int boxWidth);
void editTransactionName(struct Transaction *transaction);
void editTransactionDescription(struct Transaction *transaction);
void editTransactionAmount(struct Transaction *transaction, int boxWidth);
void performTransactionEditing(struct Transaction *transaction, int boxWidth);

void renderDeleteTransactionHeader(int boxWidth);

bool selectTransactionGroupForDelete(struct MonthReport *monthReport,
                                     int *groupIndex);

bool selectTransactionInGroupForDelete(struct TransactionGroup *group,
                                       int *transactionIndex);

bool renderDeleteConfirmation(struct Transaction *transaction,
                              int boxWidth);

void performTransactionDeletion(struct MonthReport *monthReport,
                                int groupIndex,
                                int transactionIndex);

void openTransactionDeleteMenu(struct MonthReport *monthReport);

void renderAddTransactionHeader(int boxWidth);

bool inputTransactionBasicData(struct Transaction *t);

void renderTransactionTypeSelector(int boxWidth);

bool selectTransactionType(enum TransactionType *typeOut);

bool selectTransactionCategory(struct Transaction *t,
                               int boxWidth,
                               int transactionType);

bool validateBudgetForExpense(struct MonthReport *monthReport,
                              struct Transaction *t,
                              int boxWidth);

void performTransactionInsertion(struct MonthReport *monthReport,
                                 struct Transaction *t);

void openTransactionAddToReportMenu(struct MonthReport *monthReport);

struct CategoryList *getFilteredCategories(int transactionType);

void renderCategorySelectionHeader(int boxWidth);

void renderCategoryList(struct CategoryList *filtered, int boxWidth);

bool promptCategoryChoice(int *categoryChoice, int maxChoice);

void assignCategoryToTransaction(struct Transaction *t,
                                 struct CategoryList *filtered,
                                 int categoryChoice);

bool selectTransactionCategory(struct Transaction *t,
                               int boxWidth,
                               int transactionType);

void renderCategoryBudgetHeader(int boxWidth);

bool listAllCategories(struct CategoryList *categories);

bool promptCategorySelection(struct CategoryList *categories,
                             char *outCategoryBuffer,
                             size_t bufferSize);

bool promptNewBudget(long long *budget);

void showBudgetSuccess(struct CategoryList *categories,
                       const char *category,
                       long long budget);

void openCategoryBudgetMenu(struct MonthReport *monthReport);

void renderReportEditHeader(int boxWidth);

bool selectReportFromList(struct MonthReportList *list, int *outIndex);

bool promptForNewMonthYear(char *buffer, size_t size, time_t *outDate);

void applyNewReportDate(struct MonthReport *report, time_t newDate);

void openMonthReportEditMenu(struct MonthReportList *monthReportList);

void renderDeleteReportHeader(int boxWidth);

bool promptSelectReportToDelete(struct MonthReportList *list, int *outIndex);

void renderDeleteConfirmationScreen(struct MonthReport *report, int boxWidth);

bool confirmDelete(int boxWidth);

void deleteReportFromList(struct MonthReportList *list, int index);

void openMonthReportDeleteMenu(struct MonthReportList *monthReportList);

void renderAllSummaryHeader(int boxWidth);

bool allocateSummaryArrays(struct CategoryList *categories,
                           long long **categoryTotals,
                           long long **categoryMaximums,
                           int **categoryTransactions);

void accumulateReportData(struct MonthReportList *list,
                          struct CategoryList *categories,
                          long long *totalIncome,
                          long long *totalExpense,
                          int *totalTransactions,
                          long long *categoryTotals,
                          long long *categoryMaximums,
                          int *categoryTransactions);

void renderFinancialSummary(long long totalIncome,
                            long long totalExpense,
                            int totalTransactions,
                            int boxWidth);

void renderCategoryBudgetHeader2(int boxWidth);

void renderCategoryBudgetSummary(struct CategoryList *categories,
                                 long long *categoryTotals,
                                 long long *categoryMaximums,
                                 int *categoryTransactions,
                                 int boxWidth);

void showAllMonthReportSummary(struct MonthReportList *monthReportList);

void renderMonthSummaryHeader(struct MonthReport *monthReport, int boxWidth);

void renderMonthFinancialSummary(struct MonthReport *monthReport, int boxWidth);

void analyzeMonthFinancialCondition(struct MonthReport *monthReport,
                                    const char **condition,
                                    const char **conclusion);

void renderMonthConditionSection(struct MonthReport *monthReport,
                                 const char *condition,
                                 const char *conclusion,
                                 int boxWidth);

void showMonthReportSummary2(struct MonthReport *monthReport);
void renderMonthReportHeader(struct MonthReport *monthReport, int boxWidth);
void renderMonthBriefSummary(struct MonthReport *monthReport, int boxWidth);
void renderMonthReportMenu2(int menuBoxWidth);

void showMonthReport(struct MonthReport *monthReport);

extern char temp_input[100];

#endif
