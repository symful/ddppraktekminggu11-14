#include <stdbool.h>
#include "../types/include.h"
void showMainMenu();
void clearScreen();
void waitForEnter();
void showSuccessMessage(const char *message);
void showErrorMessage(const char *message);
void showWarningMessage(const char *message);
void showInfoMessage(const char *message);
int getValidatedMenuChoice(int minChoice, int maxChoice);
void openSummaryMenu(struct MonthReportList *monthReportList);
void openBudgetSettingsMenu(struct MonthReportList *monthReportList);
void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList);
void openViewBudgetMenu(struct MonthReportList *monthReportList);
void openResetBudgetMenu(struct MonthReportList *monthReportList);
void openConfigurationMenu(struct MonthReportList *monthReportList);
void openSetMinimumAmountMenu();
void openSetMinimumBudgetMenu();
void openSetMinimumTransactionMenu();
void openViewConfigurationMenu();
void openResetConfigurationMenu();
void openMainMenu(struct MonthReportList *monthReportList);
void printMainMenuHeader();
void printFiturMenu();
void printInfoUser();
void printFiturMenuRole();
void printMainMenuFooter();
void printBudgetHeader();
int validasiInputBudget();
void handleInputBudget(int choice, struct MonthReportList *monthReportList, bool *shouldExit);
bool hasMonthReports(struct MonthReportList *monthReportList);
void printBudgetCategoryHeader();
struct MonthReport *selectMonthReport(struct MonthReportList *monthReportList);
bool selectCategory(char *outCategory, size_t bufferSize);
bool inputNewBudget(long long *outBudget);
void showSetBudgetSuccessMessage(const char *category, long long budget);
bool hasMonthReports(struct MonthReportList *monthReportList);
bool hasMonthReportsView(struct MonthReportList *monthReportList);
void printCurrentBudgetHeader();
struct MonthReport *selectReportForBudgetView(struct MonthReportList *monthReportList);
void printBudgetPerCategoryHeader();
void printBudgetTableRow(const struct TransactionGroup *group);
void printBudgetTableFooter();
void printBudgetReport(const struct MonthReport *report);
void openViewBudgetMenu(struct MonthReportList *monthReportList);
bool ensureHasMonthReports(struct MonthReportList *monthReportList);
void printUserIcon(int boxWidth, bool isAdmin);
void printUserLabel(bool isAdmin);
void printUsernameAligned(const char *username, bool isAdmin);
void printAdminNoFolderMessage(int boxWidth);
void printMenuHeaderLine(int boxWidth);
void printAdminFiturMenu(int boxWidth);
void printUserFiturMenuRows(int boxWidth);
void printAdminAuthMenu(int boxWidth);
void printUserAuthMenu(int boxWidth);
void printMessagePrefix(const char *prefix, void (*colorFunc)(const char *));
void printBudgetHeaderLayout();
void printBudgetMenuOptions();
void printBudgetFooter();
void printBudgetPrompt();

void printSelectMonthReportHeader();
bool readMonthReportChoice(int max, int *outIndex);

void printCategoryList(struct CategoryList *categories);
bool readCategorySelection(int max, int *choice);

void printCategoryList(struct CategoryList *categories);
bool readCategorySelection(int max, int *choice);

// validation
bool hasCategories(struct CategoryList *categories);
bool ensureCategoriesAvailable(struct CategoryList *categories);

// header printing
void printCategoryHeaderTop(int boxWidth);
void printCategoryHeaderTitle(int boxWidth);
void printCategoryHeaderBottom(int boxWidth);
void printCategoryHeader();

// list printing
void printSingleCategoryItem(struct CategoryItem *item, int index, int boxWidth);
void printCategoryList(struct CategoryList *categories);

// input
bool readCategorySelection(int max, int *choice);

// category data transfer
void copyCategoryInternalName(char *outCategory,
                              const char *internalName,
                              size_t bufferSize);

// success message helpers
const char* getBudgetCategoryDisplayName(const char *category);
void formatBudgetSuccessMessage(char *buffer, size_t size,const char *displayName, long long budget);

// === Budget Apply + Save ===
void applyBudgetToReport(struct MonthReport *report,
                         const char *category,
                         long long budget);
void saveUpdatedReport(struct MonthReport *report);

// === Month Reports View Validation ===
bool hasMonthReportsOnly(struct MonthReportList *list);
void showNoMonthReportsMessage();

// === Report Selection for View ===
bool readReportChoice(int max, int *choice);
struct MonthReport* getReportByIndex(struct MonthReportList *list, int index);

// Hitung sisa budget
long long calculateRemainingBudget(const struct TransactionGroup *group);

// Hitung persentase pemakaian budget
double calculateBudgetPercentage(const struct TransactionGroup *group);

//
// =========================================================
//  BUDGET TABLE PRINTING (COLUMN-LEVEL)
// =========================================================
//

// Print kolom nama kategori
void printBudgetColumnCategory(const char *displayName);

// Print kolom total amount (warna cyan)
void printBudgetColumnTotal(long long total);

// Print kolom remaining (warna green/red)
void printBudgetColumnRemaining(long long remaining);

// Print kolom status (normal wrapper)
void printBudgetColumnStatus(const char *status, double percentage);

//
// =========================================================
//  BUDGET TABLE PRINTING (ROW & STRUCTURE)
// =========================================================
//

// Print satu baris detail budget kategori
void printBudgetTableRow(const struct TransactionGroup *group);

// Print footer tabel budget
void printBudgetTableFooter();

// Print header tabel budget
void printBudgetTableHeader();

// Print laporan budget penuh
void printBudgetReport(const struct MonthReport *report);

bool hasMonthReportsView(struct MonthReportList *monthReportList);

// Print header untuk menu lihat budget
void printCurrentBudgetHeader();

// Pilih report untuk dilihat budgetnya
struct MonthReport *selectReportForBudgetView(struct MonthReportList *monthReportList);

// Menu utama untuk melihat budget
void openViewBudgetMenu(struct MonthReportList *monthReportList);
void printCategoryListHeader();
void printIncomeHeader(int boxWidth);
void printExpenseHeader(int boxWidth);
void printCategoryRow(struct CategoryItem *cat, int index, int boxWidth);
int printIncomeCategories(struct CategoryList *categories, int boxWidth);
int printExpenseCategories(struct CategoryList *categories, int boxWidth);
void printCategorySummary(int total, int income, int expense, int boxWidth);
void showAllCategories();

void printAddCategoryHeader(int boxWidth, int contentLines);

void printCategoryTypeOptions(int boxWidth);

bool readCategoryType(enum TransactionType *type, int boxWidth);

bool readDisplayCategoryName(char *buffer, size_t size, int boxWidth);

void generateInternalCategoryName(char *internalName,
                                  const char *displayName,
                                  size_t size);
bool isDuplicateCategory(struct CategoryList *list,
                         const char *internalName);
bool addNewCategory(struct CategoryList *categories,
                    const char *internalName,
                    const char *displayName,
                    enum TransactionType type);
void showAddCategorySuccess(const char *displayName);
void openAddCategoryMenu();

void printDeleteCategoryHeader(int boxWidth);
void printCustomCategoryListHeader(int boxWidth);
void printCustomCategoryItem(struct CategoryItem *cat, int index, int boxWidth);
void printDefaultCategoryNote(int boxWidth);
int collectCustomCategoryIndices(struct CategoryList *list, int *outIndices);
void printAllCustomCategories(struct CategoryList *list,
                              int *indices,
                              int count,
                              int boxWidth);
bool readDeleteCategoryChoice(int max, int *choice, int boxWidth);
bool readDeleteConfirmation(const char *categoryName, int boxWidth);
bool deleteCategoryByInternalName(const char *internalName);
void openDeleteCategoryMenu();
void printCategoryManagementHeader(int boxWidth, int contentLines);
void printCategoryManagementMenuItem(int index,
                                     const char *emoji,
                                     const char *label,
                                     int boxWidth);
void printCategoryManagementFooter(int boxWidth);
int readCategoryManagementChoice();
void openCategoryManagementMenu();

void printResetBudgetHeader(int boxWidth, int contentLines);
void printResetBudgetWarnings(int boxWidth);
bool readResetBudgetConfirmation(int boxWidth);
void resetBudgetForReport(struct MonthReport *report,
                          struct CategoryList *categories);
void resetBudgetForAllReports(struct MonthReportList *monthReportList,
                              struct CategoryList *categories);
void openResetBudgetMenu(struct MonthReportList *monthReportList);
void printConfigurationHeader(int boxWidth, int contentLines);
void printConfigurationMenuItem(int index, const char *emoji,
                                const char *label, int boxWidth);
void printConfigurationFooter(int boxWidth);
int readConfigurationChoice();
void openConfigurationMenu(struct MonthReportList *monthReportList);


// Header printers
void printMinimumAmountHeader(int boxWidth, int contentLines);
void printMinimumBudgetHeader(int boxWidth, int contentLines);

// Display current values
void showCurrentMinimumAmount(int boxWidth);
void showCurrentMinimumBudget(int boxWidth);

// Input handlers
bool readNewMinimumAmount(long long *outValue);
bool readNewMinimumBudget(long long *outValue);

// Apply updates
void applyNewMinimumAmount(long long value);
void applyNewMinimumBudget(long long value);

// Main menu wrappers
void openSetMinimumAmountMenu();
void openSetMinimumBudgetMenu();

// === MINIMUM TRANSACTION ===
void printMinimumTransactionHeader(int boxWidth, int contentLines);
void showCurrentMinimumTransaction(int boxWidth);
bool readNewMinimumTransaction(long long *outValue);
void applyNewMinimumTransaction(long long value);
void openSetMinimumTransactionMenu();

// === VIEW CONFIGURATION ===
void printConfigurationHeaderKeuangan(int boxWidth);
void printFinanceConfigHeader(int boxWidth);
void printFinanceConfigRows(int boxWidth);
void printValidationConfigHeader(int boxWidth);
void printValidationConfigRows(int boxWidth);
void openViewConfigurationMenu();

// === RESET CONFIGURATION MENU ===
void printResetConfigHeader(int boxWidth, int contentLines);
void printResetConfigWarning(int boxWidth);
bool readResetConfigConfirmation(int boxWidth, char *outConfirm);
void applyResetConfiguration();
void printResetCancelledMessage();
void openResetConfigurationMenu();

// Role utilities
bool isAdmin();
int getMainMenuMaxChoice();

// User menu handlers
void handleMenuChoiceUser(int choice, struct MonthReportList *monthReportList);
void printUserLogoutScreen();
void printUserExitScreen();

// Admin menu handlers
void handleMenuChoiceAdmin(int choice, struct MonthReportList *monthReportList);
void printAdminMenuScreen();
bool readAdminMenuChoice(int *outChoice);
void handleAdminPanelSelection(int adminChoice);
void printAdminLogoutScreen();
void printAdminExitScreen();

// Main wrapper
void openMainMenu(struct MonthReportList *monthReportList);
