#include "../auth/auth.h"
#include "../db/transaction_group.c"
#include "../types/include.c"
#include "../utils/validation.c"
#include "./admin_menu_ui.c"
#include "./auth_ui.c"
#include "./colors.c"
#include "./month_report.c"
#include "./screen.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void waitForEnter() {
  printf("\n");
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  clearInputBuffer();
}

void printMainMenuHeader() {
  int boxWidth = 59;
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                    🏠 MENU UTAMA                        ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
}

void printUserIcon(int boxWidth, bool isAdmin) {
  if (isAdmin)
    printCentered("│              👑 ", boxWidth);
  else
    printCentered("│              👤 ", boxWidth);
}

void printUserLabel(bool isAdmin) {
  if (isAdmin)
    printColored("Admin: ", COLOR_BRIGHT_YELLOW);
  else
    printColored("Pengguna: ", COLOR_BRIGHT_YELLOW);
}

void printUsernameAligned(const char *username, bool isAdmin) {
  int targetLength = isAdmin ? 30 : 27;
  printColored(username, COLOR_BRIGHT_CYAN);
  int len = strlen(username);
  for (int i = len; i < targetLength; i++)
    printf(" ");
  printf(" │\n");
}

void printAdminNoFolderMessage(int boxWidth) {
  printCenteredColored(
      "│              🚫 Tanpa Folder Pribadi                   │\n", boxWidth,
      COLOR_BRIGHT_WHITE);
}

void printInfoUser() {
  int boxWidth = 59;

  if (currentUser == NULL)
    return;

  bool isAdmin = currentUser->isAdmin;

  printUserIcon(boxWidth, isAdmin);
  printUserLabel(isAdmin);
  printUsernameAligned(currentUser->username, isAdmin);

  if (isAdmin)
    printAdminNoFolderMessage(boxWidth);
}

void printMenuHeaderLine(int boxWidth) {
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printAdminFiturMenu(int boxWidth) {
  printCenteredColored(
      "│  1. 🚫 Kelola Laporan Bulanan (Admin tidak memiliki)    │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  2. 🚫 Lihat Ringkasan Keuangan (Admin tidak memiliki)  │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  3. 🚫 Pengaturan Budget (Admin tidak memiliki)         │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  4. 🚫 Kelola Kategori (Admin tidak memiliki)           │\n", boxWidth,
      COLOR_DIM);

  printCentered("│  ", boxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" ⚙️  Pengaturan Sistem                                │\n");
}
void printUserFiturMenuRows(int boxWidth) {
  const char *labels[] = {
      " 📊 Kelola Laporan Bulanan", " 📈 Lihat Ringkasan Keuangan",
      " ⚙️  Pengaturan Budget", " 🏷️  Kelola Kategori", " 🔧 Pengaturan Sistem"};

  char numBuf[8];

  for (int i = 0; i < 5; i++) {
    snprintf(numBuf, sizeof(numBuf), "%d.", i + 1);

    printCentered("│  ", boxWidth);
    printColored(numBuf, COLOR_BRIGHT_CYAN);
    printf("%s                                │\n", labels[i]);
  }
}

void printFiturMenu() {
  int boxWidth = 59;
  printMenuHeaderLine(boxWidth);

  if (currentUser != NULL && currentUser->isAdmin)
    printAdminFiturMenu(boxWidth);
  else
    printUserFiturMenuRows(boxWidth);
}

void printAdminAuthMenu(int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 👑 Admin Panel                                      │\n");

  printCentered("│  ", boxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           │\n");

  printCentered("│  ", boxWidth);
  printColored("8.", COLOR_BRIGHT_CYAN);
  printf(" ❌ Keluar                                           │\n");
}

void printUserAuthMenu(int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           │\n");

  printCentered("│  ", boxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" ❌ Keluar                                           │\n");
}

void printFiturMenuAuth() {
  int boxWidth = 59;

  if (currentUser != NULL && currentUser->isAdmin)
    printAdminAuthMenu(boxWidth);
  else
    printUserAuthMenu(boxWidth);
}

void printMainMenuFooter() {
  int boxWidth = 59;
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Tip: Pilih nomor menu yang diinginkan\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

void showMainMenu() {
  int contentLines = 14;
  clearAndCenterVertically(contentLines);
  printMainMenuHeader();
  printInfoUser();
  printFiturMenu();
  printFiturMenuAuth();
  printMainMenuFooter();
}

void printMessagePrefix(const char *prefix, void (*colorFunc)(const char *)) {
  printf("\n");
  colorFunc(prefix);
}

void showSuccessMessage(const char *message) {
  printMessagePrefix("✅ ", printSuccess);
  printf("%s", message);
  waitForEnter();
}

void showErrorMessage(const char *message) {
  printMessagePrefix("❌ ", printError);
  printf("%s", message);
  waitForEnter();
}

void showWarningMessage(const char *message) {
  printMessagePrefix("⚠️  ", printWarning);
  printf("%s", message);
  waitForEnter();
}

void showInfoMessage(const char *message) {
  printMessagePrefix("ℹ️  ", printInfo);
  printf("%s", message);
  waitForEnter();
}

int getValidatedMenuChoice(int minChoice, int maxChoice) {
  int choice;

  if (!readAndValidateInteger("", minChoice, maxChoice, &choice)) {
    showErrorMessage("Input tidak valid atau terlalu banyak percobaan.");
    return -1;
  }

  return choice;
}

void openSummaryMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan untuk ditampilkan. Buat laporan "
                    "terlebih dahulu.");
    return;
  }

  showAllMonthReportSummary(monthReportList);
}

void printBudgetHeaderLayout() {
  int boxWidth = 59;
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                 ⚙️  PENGATURAN BUDGET                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printBudgetMenuOptions() {
  int boxWidth = 59;
  printCentered("│  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 🏷️  Atur Budget per Kategori                         │\n");

  printCentered("│  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 📋 Lihat Budget Saat Ini                            │\n");

  printCentered("│  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" 🔄 Reset Semua Budget                               │\n");

  printCentered("│  ", boxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali ke Menu Utama                            │\n");
}

void printBudgetFooter() {
  int boxWidth = 59;
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printBudgetPrompt() {
  int boxWidth = 59;
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

void printBudgetHeader() {
  int contentLines = 10;
  clearAndCenterVertically(contentLines);

  printBudgetHeaderLayout();
  printBudgetMenuOptions();
  printBudgetFooter();
  printBudgetPrompt();
}

int validasiInputBudget() { return getValidatedMenuChoice(1, 4); }

void handleInputBudget(int choice, struct MonthReportList *monthReportList,
                       bool *shouldExit) {
  switch (choice) {
  case 1:
    openSetCategoryBudgetMenu(monthReportList);
    break;
  case 2:
    openViewBudgetMenu(monthReportList);
    break;
  case 3:
    openResetBudgetMenu(monthReportList);
    break;
  case 4:
    *shouldExit = true;
    break;
  default:
    showErrorMessage("Pilihan tidak valid.");
    break;
  }
}

void openBudgetSettingsMenu(struct MonthReportList *monthReportList) {
  bool shouldExit = false;

  while (!shouldExit) {
    printBudgetHeader();

    int choice = validasiInputBudget();
    if (choice == -1) {
      continue;
    }

    handleInputBudget(choice, monthReportList, &shouldExit);
  }
}

bool hasMonthReports(struct MonthReportList *monthReportList) {
  return monthReportList != NULL && monthReportList->count > 0;
}

void printBudgetCategoryHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("             🏷️  ATUR BUDGET PER KATEGORI                ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

bool readMonthReportChoice(int max, int *outIndex) {
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1, max, outIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }
  return true;
}

struct MonthReport *selectMonthReport(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readMonthReportChoice(monthReportList->count, &reportChoice))
    return NULL;

  return monthReportList->reports[reportChoice - 1];
}

bool hasCategories(struct CategoryList *categories) {
  return categories != NULL && categories->count > 0;
}

bool ensureCategoriesAvailable(struct CategoryList *categories) {
  if (!hasCategories(categories)) {
    showErrorMessage("Tidak ada kategori tersedia.");
    return false;
  }
  return true;
}

void printCategoryHeaderTop(int boxWidth) {
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCategoryHeaderTitle(int boxWidth) {
  printCentered("│", boxWidth);
  printWithBg("        📂 Pilih Kategori untuk Mengatur Budget          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
}

void printCategoryHeaderBottom(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCategoryHeader() {
  int boxWidth = 59;
  printf("\n");
  printCategoryHeaderTop(boxWidth);
  printCategoryHeaderTitle(boxWidth);
  printCategoryHeaderBottom(boxWidth);
  printf("\n");
}

void printSingleCategoryItem(struct CategoryItem *item, int index,
                             int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%s%d.%s %s\n", COLOR_BRIGHT_CYAN, index + 1, COLOR_RESET,
         item->displayName);
}
void printCategoryList(struct CategoryList *categories) {
  int boxWidth = 59;

  for (int i = 0; i < categories->count; i++) {
    printSingleCategoryItem(categories->items[i], i, boxWidth);
  }
  printf("\n");
}

bool readCategorySelection(int max, int *choice) {
  return readAndValidateInteger("\n🎯 Pilihan kategori: ", 1, max, choice);
}

void copyCategoryInternalName(char *outCategory, const char *internalName,
                              size_t bufferSize) {

  strncpy(outCategory, internalName, bufferSize - 1);
  outCategory[bufferSize - 1] = '\0';
}

bool selectCategory(char *outCategory, size_t bufferSize) {
  struct CategoryList *categories = getUserCategoriesCache();
  if (!ensureCategoriesAvailable(categories))
    return false;
  printCategoryHeader();
  printCategoryList(categories);
  int categoryChoice;
  if (!readCategorySelection(categories->count, &categoryChoice)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  copyCategoryInternalName(outCategory,
                           categories->items[categoryChoice - 1]->internalName,
                           bufferSize);
  return true;
}

bool inputNewBudget(long long *outBudget) {
  InputResult result =
      promptForBudget("💰 Masukkan budget baru (Rp): ", outBudget);
  if (result != INPUT_SUCCESS) {
    showErrorMessage(
        "Budget tidak valid atau di bawah minimum yang diizinkan.");
    return false;
  }
  return true;
}

const char *getBudgetCategoryDisplayName(const char *category) {
  struct CategoryList *categories = getUserCategoriesCache();
  return getCategoryDisplayName(categories, category);
}

void formatBudgetSuccessMessage(char *buffer, size_t size,
                                const char *displayName, long long budget) {

  snprintf(buffer, size,
           "Budget untuk kategori '%s' berhasil diatur menjadi Rp %lld",
           displayName, budget);
}

void showSetBudgetSuccessMessage(const char *category, long long budget) {
  const char *displayName = getBudgetCategoryDisplayName(category);

  char successMsg[256];
  formatBudgetSuccessMessage(successMsg, sizeof(successMsg), displayName,
                             budget);
  showSuccessMessage(successMsg);
}

bool ensureHasMonthReports(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return false;
  }
  return true;
}

void applyBudgetToReport(struct MonthReport *report, const char *category,
                         long long budget) {

  setBudgetForCategory(report, category, budget);
}

void saveUpdatedReport(struct MonthReport *report) {
  saveUserMonthReport(report);
}

void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList) {

  if (!ensureHasMonthReports(monthReportList))
    return;

  printBudgetCategoryHeader();
  struct MonthReport *report = selectMonthReport(monthReportList);
  if (report == NULL)
    return;

  char category[50];
  if (!selectCategory(category, sizeof(category)))
    return;

  long long budget;
  if (!inputNewBudget(&budget))
    return;

  applyBudgetToReport(report, category, budget);
  saveUpdatedReport(report);
  showSetBudgetSuccessMessage(category, budget);
}

bool hasMonthReportsOnly(struct MonthReportList *list) {
  return list != NULL && list->count > 0;
}
void showNoMonthReportsMessage() {
  showInfoMessage("Tidak ada laporan bulanan.");
}

bool hasMonthReportsView(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsOnly(monthReportList)) {
    showNoMonthReportsMessage();
    return false;
  }
  return true;
}

void printCurrentBudgetHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("               📋 LIHAT BUDGET SAAT INI                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

bool readReportChoice(int max, int *choice) {
  return readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1, max, choice);
}
struct MonthReport *getReportByIndex(struct MonthReportList *list, int index) {
  return list->reports[index];
}

struct MonthReport *
selectReportForBudgetView(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);
  int reportChoice;
  if (!readReportChoice(monthReportList->count, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }
  return getReportByIndex(monthReportList, reportChoice - 1);
}

void printBudgetPerCategoryHeader() {
  int boxWidth = 88;
  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "════════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                              💰 DETAIL BUDGET PER KATEGORI     "
              "                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║ ", boxWidth);
  printColored("KATEGORI", COLOR_BRIGHT_CYAN);
  printf("            │ ");
  printColored("BUDGET (Rp)", COLOR_BRIGHT_CYAN);
  printf("     │ ");
  printColored("TERPAKAI (Rp)", COLOR_BRIGHT_CYAN);
  printf("   │ ");
  printColored("SISA (Rp)", COLOR_BRIGHT_CYAN);
  printf("       │ ");
  printColored("STATUS", COLOR_BRIGHT_CYAN);
  printf("     ║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

long long calculateRemainingBudget(const struct TransactionGroup *group) {
  return group->budget - group->totalAmount;
}

double calculateBudgetPercentage(const struct TransactionGroup *group) {
  if (group->budget <= 0)
    return 0.0;
  return ((double)group->totalAmount / group->budget) * 100.0;
}

void printBudgetColumnCategory(const char *displayName) {
  printf("%-20s │ ", displayName);
}

void printBudgetColumnTotal(long long total) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%-15lld", total);
  printColored(buffer, COLOR_BRIGHT_CYAN);
  printf(" │ ");
}

void printBudgetColumnRemaining(long long remaining) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%-15lld", remaining);

  const char *color = (remaining >= 0) ? COLOR_BRIGHT_GREEN : COLOR_BRIGHT_RED;

  printColored(buffer, color);
  printf(" │ ");
}

void printBudgetColumnStatus(const char *status, double percentage) {
  printBudgetStatus(status, percentage);
}

void printBudgetTableRow(const struct TransactionGroup *group) {
  int boxWidth = 88;

  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, group->category);

  long long remaining = calculateRemainingBudget(group);
  double percentage = calculateBudgetPercentage(group);
  const char *status = getGroupBudgetStatus(group);

  printCentered("║ ", boxWidth);
  printBudgetColumnCategory(displayName);
  printBudgetColumnTotal(group->totalAmount);
  printBudgetColumnRemaining(remaining);
  printAmount(remaining);
  printf("%-15s │ ", "");
  printBudgetColumnStatus(status, percentage);
  printf("%-10s ║\n", "");
}

void printBudgetTableHeader() { printBudgetPerCategoryHeader(); }

void printBudgetTableFooter() {
  int boxWidth = 88;
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "════════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

void printBudgetReport(const struct MonthReport *report) {
  printBudgetTableHeader();

  for (int i = 0; i < report->groupCount; i++) {
    printBudgetTableRow(report->groups[i]);
  }

  printBudgetTableFooter();
}

void openViewBudgetMenu(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsView(monthReportList))
    return;

  printCurrentBudgetHeader();

  struct MonthReport *report = selectReportForBudgetView(monthReportList);
  if (report == NULL)
    return;
  printBudgetReport(report);
  waitForEnter();
}

void printCategoryListHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                📋 DAFTAR KATEGORI                       ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printIncomeHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 KATEGORI PENDAPATAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printExpenseHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printError("💸 KATEGORI PENGELUARAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCategoryRow(struct CategoryItem *cat, int index, int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%d. %-30s ", index + 1, cat->displayName);

  if (cat->isDefault)
    printColored("[Default]", COLOR_BRIGHT_WHITE);
  else
    printColored("[Custom]", COLOR_BRIGHT_YELLOW);

  printf("\n");
}

int printIncomeCategories(struct CategoryList *categories, int boxWidth) {
  int count = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_INCOME) {
      printCategoryRow(categories->items[i], i, boxWidth);
      count++;
    }
  }
  if (count == 0)
    printCentered("  📭 Tidak ada kategori pendapatan\n", boxWidth);

  return count;
}

int printExpenseCategories(struct CategoryList *categories, int boxWidth) {
  int count = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_EXPENSE) {
      printCategoryRow(categories->items[i], i, boxWidth);
      count++;
    }
  }
  if (count == 0)
    printCentered("  📭 Tidak ada kategori pengeluaran\n", boxWidth);

  return count;
}

void printCategorySummary(int total, int income, int expense, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total: ");
  printf("%d kategori (", total);
  printf("%s%d%s", COLOR_BRIGHT_GREEN, income, COLOR_RESET);
  printf(" pendapatan, ");
  printf("%s%d%s", COLOR_BRIGHT_RED, expense, COLOR_RESET);
  printf(" pengeluaran)\n");
}

void showAllCategories() {
  int boxWidth = 59;

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    waitForEnter();
    return;
  }

  printCategoryListHeader();
  printIncomeHeader(boxWidth);
  int incomeCount = printIncomeCategories(categories, boxWidth);
  printExpenseHeader(boxWidth);
  int expenseCount = printExpenseCategories(categories, boxWidth);
  printCategorySummary(categories->count, incomeCount, expenseCount, boxWidth);
}

void printAddCategoryHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("               ➕ TAMBAH KATEGORI BARU                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCategoryTypeOptions(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📝 TIPE KATEGORI:\n");

  printCentered("  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 💰 Pendapatan (Income)\n");

  printCentered("  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 💸 Pengeluaran (Expense)\n");
}

bool readCategoryType(enum TransactionType *type, int boxWidth) {
  int choice;

  printf("\n");
  printCentered("", boxWidth);

  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &choice)) {
    return false;
  }

  *type = (choice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;
  return true;
}

bool readDisplayCategoryName(char *buffer, size_t size, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);

  return readCategoryNameSafe(buffer, size, "📝 Nama kategori (Indonesia): ");
}

void generateInternalCategoryName(char *internalName, const char *displayName,
                                  size_t size) {
  strncpy(internalName, displayName, size - 1);
  internalName[size - 1] = '\0';
  normalizeCategoryName(internalName);
}

bool isDuplicateCategory(struct CategoryList *list, const char *internalName) {
  return categoryExistsInList(list, internalName);
}

bool addNewCategory(struct CategoryList *categories, const char *internalName,
                    const char *displayName, enum TransactionType type) {
  bool success =
      addCategoryToList(categories, internalName, displayName, type, 0);

  if (success) {
    saveUserCategories(categories);
    refreshCategoryCache();
  }

  return success;
}

void showAddCategorySuccess(const char *displayName) {
  char msg[256];
  snprintf(msg, sizeof(msg), "Kategori '%s' berhasil ditambahkan!",
           displayName);
  showSuccessMessage(msg);
}

void openAddCategoryMenu() {
  int boxWidth = 59;
  int contentLines = 12;

  printAddCategoryHeader(boxWidth, contentLines);
  printCategoryTypeOptions(boxWidth);

  // 1. Baca tipe kategori
  enum TransactionType type;
  if (!readCategoryType(&type, boxWidth)) {
    showErrorMessage("Tipe tidak valid.");
    waitForEnter();
    return;
  }

  // 2. Baca nama display
  char displayName[50];
  if (!readDisplayCategoryName(displayName, sizeof(displayName), boxWidth)) {
    showErrorMessage("Nama kategori tidak valid.");
    waitForEnter();
    return;
  }

  // 3. Generate internal name
  char internalName[50];
  generateInternalCategoryName(internalName, displayName, sizeof(internalName));

  // 4. Load category list
  struct CategoryList *categories = loadUserCategories();

  // 5. Cek duplikat
  if (isDuplicateCategory(categories, internalName)) {
    showErrorMessage("Kategori dengan nama tersebut sudah ada.");
    freeCategoryList(categories);
    waitForEnter();
    return;
  }

  // 6. Tambah kategori
  if (addNewCategory(categories, internalName, displayName, type)) {
    showAddCategorySuccess(displayName);
  } else {
    showErrorMessage("Gagal menambahkan kategori.");
  }

  freeCategoryList(categories);
  waitForEnter();
}

void printDeleteCategoryHeader(int boxWidth) {
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("               🗑️  HAPUS KATEGORI CUSTOM                 ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCustomCategoryListHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📋 KATEGORI CUSTOM:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCustomCategoryItem(struct CategoryItem *cat, int index,
                             int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%d. %s ", index + 1, cat->displayName);

  if (cat->type == TRANSACTION_INCOME)
    printSuccess("(Pendapatan)\n");
  else
    printError("(Pengeluaran)\n");
}

void printDefaultCategoryNote(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Catatan: Kategori default tidak dapat dihapus\n");
}

int collectCustomCategoryIndices(struct CategoryList *list, int *outIndices) {
  int count = 0;

  for (int i = 0; i < list->count; i++) {
    if (!list->items[i]->isDefault) {
      outIndices[count++] = i;
    }
  }

  return count;
}

void printAllCustomCategories(struct CategoryList *list, int *indices,
                              int count, int boxWidth) {
  for (int i = 0; i < count; i++) {
    struct CategoryItem *item = list->items[indices[i]];
    printCustomCategoryItem(item, indices[i], boxWidth);
  }
}

bool readDeleteCategoryChoice(int max, int *choice, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  return readAndValidateInteger(
      "🎯 Pilih kategori untuk dihapus (0 untuk batal): ", 0, max, choice);
}

bool readDeleteConfirmation(const char *categoryName, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  Anda yakin ingin menghapus kategori '");
  printf("%s'? (y/n): ", categoryName);

  char c;
  scanf(" %c", &c);
  clearInputBuffer();

  return (c == 'y' || c == 'Y');
}

bool deleteCategoryByInternalName(const char *internalName) {
  struct CategoryList *fullList = loadUserCategories();
  bool success = false;

  if (removeCategoryFromList(fullList, internalName)) {
    saveUserCategories(fullList);
    refreshCategoryCache();
    success = true;
  }

  freeCategoryList(fullList);
  return success;
}

void openDeleteCategoryMenu() {
  int boxWidth = 59;
  struct CategoryList *categories = getUserCategoriesCache();

  printDeleteCategoryHeader(boxWidth);
  printCustomCategoryListHeader(boxWidth);

  int customIndices[100];
  int customCount = collectCustomCategoryIndices(categories, customIndices);

  if (customCount == 0) {
    showWarningMessage("Tidak ada kategori custom untuk dihapus.");
    waitForEnter();
    return;
  }

  printAllCustomCategories(categories, customIndices, customCount, boxWidth);
  printDefaultCategoryNote(boxWidth);

  int choice;
  if (!readDeleteCategoryChoice(customCount, &choice, boxWidth)) {
    showErrorMessage("Pilihan tidak valid.");
    waitForEnter();
    return;
  }

  if (choice == 0) {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  int index = customIndices[choice - 1];
  const char *internalName = categories->items[index]->internalName;
  const char *displayName = categories->items[index]->displayName;

  if (!canDeleteCategory(internalName)) {
    showErrorMessage("Kategori sedang digunakan dan tidak dapat dihapus.");
    waitForEnter();
    return;
  }

  if (!readDeleteConfirmation(displayName, boxWidth)) {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  if (deleteCategoryByInternalName(internalName)) {
    showSuccessMessage("Kategori berhasil dihapus!");
  } else {
    showErrorMessage("Gagal menghapus kategori.");
  }
  waitForEnter();
}

void printCategoryManagementHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                🏷️  KELOLA KATEGORI                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printCategoryManagementMenuItem(int index, const char *emoji,
                                     const char *label, int boxWidth) {
  printCentered("│  ", boxWidth);
  printf("%s%d.%s", COLOR_BRIGHT_CYAN, index, COLOR_RESET);
  printf(" %s %s", emoji, label);

  int pad = 51 - (int)strlen(label);
  for (int i = 0; i < pad; i++)
    printf(" ");

  printf("│\n");
}

void printCategoryManagementFooter(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

int readCategoryManagementChoice() { return getValidatedMenuChoice(1, 4); }

void openCategoryManagementMenu() {
  int boxWidth = 59;

  while (1) {
    int contentLines = 10;

    printCategoryManagementHeader(boxWidth, contentLines);
    printCategoryManagementMenuItem(1, "📋", "Lihat Semua Kategori", boxWidth);
    printCategoryManagementMenuItem(2, "➕", "Tambah Kategori Baru", boxWidth);
    printCategoryManagementMenuItem(3, "🗑️", "Hapus Kategori Custom", boxWidth);
    printCategoryManagementMenuItem(4, "⬅️", "Kembali ke Menu Utama", boxWidth);
    printCategoryManagementFooter(boxWidth);

    int choice = readCategoryManagementChoice();
    if (choice == -1)
      continue;
    switch (choice) {
    case 1:
      showAllCategories();
      waitForEnter();
      break;
    case 2:
      openAddCategoryMenu();
      break;
    case 3:
      openDeleteCategoryMenu();
      break;
    case 4:
      return;
    }
  }
}

void printResetBudgetHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("              🔄 RESET SEMUA BUDGET                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printResetBudgetWarnings(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning(
      "⚠️  PERINGATAN: Ini akan mengatur semua budget ke nilai default!\n");

  printCentered("", boxWidth);
  printInfo("📊 Budget default untuk setiap kategori: ");
  printf("%sRp 1,000,000%s\n\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
}

bool readResetBudgetConfirmation(int boxWidth) {
  char c;

  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);

  scanf(" %c", &c);
  clearInputBuffer();

  return (c == 'y' || c == 'Y');
}

void resetBudgetForReport(struct MonthReport *report,
                          struct CategoryList *categories) {
  for (int i = 0; i < categories->count; i++) {
    struct CategoryItem *cat = categories->items[i];
    if (cat != NULL) {
      setBudgetForCategory(report, cat->internalName, 1000000);
    }
  }
  saveUserMonthReport(report);
}

void resetBudgetForAllReports(struct MonthReportList *monthReportList,
                              struct CategoryList *categories) {
  for (int i = 0; i < monthReportList->count; i++) {
    resetBudgetForReport(monthReportList->reports[i], categories);
  }
}

void openResetBudgetMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 10;

  printResetBudgetHeader(boxWidth, contentLines);
  printResetBudgetWarnings(boxWidth);

  if (!readResetBudgetConfirmation(boxWidth)) {
    showInfoMessage("Reset budget dibatalkan.");
    return;
  }

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    showErrorMessage("Kategori tidak tersedia.");
    return;
  }

  resetBudgetForAllReports(monthReportList, categories);

  showSuccessMessage("Semua budget berhasil direset ke Rp 1,000,000!");
}

void printConfigurationHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                🔧 PENGATURAN SISTEM                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printConfigurationMenuItem(int index, const char *emoji, const char *label,
                                int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("%d.", COLOR_BRIGHT_CYAN);
  printf(" %s %s", emoji, label);

  int padding = 51 - (int)strlen(label);
  for (int i = 0; i < padding; i++)
    printf(" ");

  printf("│\n");
}

void printConfigurationFooter(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

int readConfigurationChoice() { return getValidatedMenuChoice(1, 6); }

void openConfigurationMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;

  while (1) {
    printConfigurationHeader(boxWidth, 12);

    printConfigurationMenuItem(1, "💰", "Atur Minimum Jumlah Uang", boxWidth);
    printConfigurationMenuItem(2, "📊", "Atur Minimum Budget", boxWidth);
    printConfigurationMenuItem(3, "💳", "Atur Minimum Transaksi", boxWidth);
    printConfigurationMenuItem(4, "📋", "Lihat Pengaturan Saat Ini", boxWidth);
    printConfigurationMenuItem(5, "🔄", "Reset ke Pengaturan Default",
                               boxWidth);
    printConfigurationMenuItem(6, "⬅️", "Kembali ke Menu Utama", boxWidth);

    printConfigurationFooter(boxWidth);

    int choice = readConfigurationChoice();
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      openSetMinimumAmountMenu();
      break;
    case 2:
      openSetMinimumBudgetMenu();
      break;
    case 3:
      openSetMinimumTransactionMenu();
      break;
    case 4:
      openViewConfigurationMenu();
      break;
    case 5:
      openResetConfigurationMenu();
      break;
    case 6:
      return;
    }
  }
}

void printMinimumAmountHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("             💰 ATUR MINIMUM JUMLAH UANG                 ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printMinimumBudgetHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                📊 ATUR MINIMUM BUDGET                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void showCurrentMinimumAmount(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("💰 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumAmount(), COLOR_RESET);
}

void showCurrentMinimumBudget(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("📊 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumBudget(), COLOR_RESET);
}

bool readNewMinimumAmount(long long *outValue) {
  return readAndValidateLongLong("💰 Masukkan minimum baru (minimal 1): ", 1,
                                 getMaximumAmount(), outValue);
}

bool readNewMinimumBudget(long long *outValue) {
  return readAndValidateLongLong("📊 Masukkan minimum baru (minimal 1): ", 1,
                                 getMaximumBudget(), outValue);
}

void applyNewMinimumAmount(long long value) {
  setMinimumAmount(value);
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg),
           "Minimum jumlah uang berhasil diatur menjadi Rp %lld", value);
  showSuccessMessage(msg);
}

void applyNewMinimumBudget(long long value) {
  setMinimumBudget(value);
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg), "Minimum budget berhasil diatur menjadi Rp %lld",
           value);
  showSuccessMessage(msg);
}

void openSetMinimumAmountMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumAmountHeader(boxWidth, contentLines);
  showCurrentMinimumAmount(boxWidth);

  long long value;
  if (!readNewMinimumAmount(&value)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  applyNewMinimumAmount(value);
}

void openSetMinimumBudgetMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumBudgetHeader(boxWidth, contentLines);
  showCurrentMinimumBudget(boxWidth);

  long long value;
  if (!readNewMinimumBudget(&value)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  applyNewMinimumBudget(value);
}

void printMinimumTransactionHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("              💳 ATUR MINIMUM TRANSAKSI                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void showCurrentMinimumTransaction(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("💳 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, globalConfig.money.minimumBudget,
         COLOR_RESET);
}

bool readNewMinimumTransaction(long long *outValue) {
  return readAndValidateLongLong("💳 Masukkan minimum baru (minimal 1): ", 1,
                                 globalConfig.money.minimumBudget, outValue);
}

void applyNewMinimumTransaction(long long value) {
  globalConfig.money.minimumBudget = value;
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg),
           "Minimum transaksi berhasil diatur menjadi Rp %lld", value);
  showSuccessMessage(msg);
}

void openSetMinimumTransactionMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumTransactionHeader(boxWidth, contentLines);
  showCurrentMinimumTransaction(boxWidth);

  long long newMin;
  if (!readNewMinimumTransaction(&newMin)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  applyNewMinimumTransaction(newMin);
}

void printConfigurationHeaderKeuangan(int boxWidth) {
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", 59,
      COLOR_BRIGHT_CYAN);
  printCentered("│", 59);
  printWithBg("              📋 PENGATURAN SAAT INI                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", 59,
      COLOR_BRIGHT_CYAN);
}

void printFinanceConfigHeader(int boxWidth) {
  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "══════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║", boxWidth);
  printWithBg("                              💰 PENGATURAN KEUANGAN            "
              "                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");

  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║ ", boxWidth);
  printf("%s%-30s%s │ ", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf("%s%-20s%s │ ", COLOR_BRIGHT_CYAN, "MINIMUM", COLOR_RESET);
  printf("%s%-20s%s ║\n", COLOR_BRIGHT_CYAN, "MAXIMUM", COLOR_RESET);
}

void printFinanceConfigRows(int boxWidth) {
  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Jumlah Uang");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_GREEN, getMinimumAmount(),
         COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_GREEN, getMaximumAmount(),
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Budget");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_CYAN, getMinimumBudget(),
         COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_CYAN, getMaximumBudget(),
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Transaksi");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_YELLOW,
         globalConfig.money.minimumTransactionAmount, COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_YELLOW,
         globalConfig.money.maximumTransactionAmount, COLOR_RESET);
}

void printValidationConfigHeader(int boxWidth) {
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "══════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║", boxWidth);
  printWithBg("                             ⚙️  PENGATURAN VALIDASI             "
              "                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");

  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

void printValidationConfigRows(int boxWidth) {
  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Panjang Nama");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxNameLength, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Panjang Deskripsi");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxDescriptionLength, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Percobaan Input");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxValidationAttempts, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Tahun Minimum");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN, globalConfig.validation.minYear,
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Tahun Maksimum");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN, globalConfig.validation.maxYear,
         COLOR_RESET);

  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

void openViewConfigurationMenu() {
  int boxWidth = 88;

  printConfigurationHeaderKeuangan(boxWidth);
  printFinanceConfigHeader(boxWidth);
  printFinanceConfigRows(boxWidth);
  printValidationConfigHeader(boxWidth);
  printValidationConfigRows(boxWidth);
  waitForEnter();
}

void printResetConfigHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("           🔄 RESET PENGATURAN DEFAULT                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void printResetConfigWarning(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  PERINGATAN: Ini akan mengembalikan semua pengaturan ke "
               "nilai default!\n\n");
}

bool readResetConfigConfirmation(int boxWidth, char *outConfirm) {
  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);

  scanf(" %c", outConfirm);
  clearInputBuffer();

  return (*outConfirm == 'y' || *outConfirm == 'Y');
}

void applyResetConfiguration() {
  initializeDefaultConfig();
  saveConfigToFile("./config.txt");

  showSuccessMessage("Pengaturan berhasil direset ke nilai default! Silakan "
                     "restart aplikasi untuk melihat perubahan.");
}

void printResetCancelledMessage() {
  showInfoMessage("Reset pengaturan dibatalkan.");
}

void openResetConfigurationMenu() {
  int boxWidth = 59;
  int contentLines = 10;

  printResetConfigHeader(boxWidth, contentLines);
  printResetConfigWarning(boxWidth);

  char confirm;
  bool confirmed = readResetConfigConfirmation(boxWidth, &confirm);

  if (!confirmed) {
    printResetCancelledMessage();
    return;
  }
  applyResetConfiguration();
}

bool isAdmin() { return currentUser && currentUser->isAdmin; }

int getMainMenuMaxChoice() { return isAdmin() ? 8 : 7; }

void handleMenuChoiceUser(int choice, struct MonthReportList *monthReportList) {
  switch (choice) {
  case 1:
    openMonthlyMenu(monthReportList);
    break;
  case 2:
    openSummaryMenu(monthReportList);
    break;
  case 3:
    openBudgetSettingsMenu(monthReportList);
    break;
  case 4:
    openCategoryManagementMenu();
    break;
  case 5:
    openConfigurationMenu(monthReportList);
    break;
  case 6:
    printUserLogoutScreen();
    return;
  case 7:
    printUserExitScreen();
    exit(0);
  default:
    showErrorMessage("Pilihan tidak valid.");
  }
}

void printAdminMenuScreen() {
  clearScreen();
  showAdminMenu();
}

bool readAdminMenuChoice(int *outChoice) {
  int ch;
  if (scanf("%d", outChoice) != 1)
    return false;

  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
  return true;
}

void handleAdminPanelSelection(int adminChoice) {
  switch (adminChoice) {
  case 1:
    handleAdminUserManagement();
    break;
  case 2:
    viewAllUserReports();
    break;
  case 3:
    showSystemStatistics();
    break;
  case 4:
    break;
  case 5:
    printAdminLogoutScreen();
    break;
  }
}

void printUserLogoutScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      🔓 LOGOUT                          ║\n");
  printf("║                                                          ║\n");
  printf("║               Logout pengguna: %-21s     ║\n",
         currentUser ? currentUser->username : "Tidak Diketahui");
  printf("║                                                          ║\n");
  printf("║                 Kembali ke layar login...               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

void printAdminLogoutScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      🔓 LOGOUT                          ║\n");
  printf("║                                                          ║\n");
  printf("║               Logout admin: %-23s     ║\n",
         currentUser ? currentUser->username : "Tidak Diketahui");
  printf("║                                                          ║\n");
  printf("║                 Kembali ke layar login...               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

void printUserExitScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👋 TERIMA KASIH!                     ║\n");
  printf("║                                                          ║\n");
  printf("║          Semoga keuangan Anda selalu terjaga! 💰        ║\n");
  printf("║                                                          ║\n");
  printf("║                   Sampai jumpa lagi! 😊                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

void printAdminExitScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👋 TERIMA KASIH!                     ║\n");
  printf("║                                                          ║\n");
  printf("║          Semoga keuangan Anda selalu terjaga! 💰👑      ║\n");
  printf("║                                                          ║\n");
  printf("║                   Sampai jumpa lagi! 😊                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

void handleMenuChoiceAdmin(int choice,
                           struct MonthReportList *monthReportList) {
  switch (choice) {
  case 1:
    showErrorMessage("Admin tidak memiliki laporan pribadi!");
    break;
  case 2:
    showErrorMessage("Admin tidak memiliki ringkasan pribadi!");
    break;
  case 3:
    showErrorMessage("Admin tidak memiliki budget pribadi!");
    break;
  case 4:
    showErrorMessage("Admin tidak memiliki kategori pribadi!");
    break;
  case 5:
    openConfigurationMenu(monthReportList);
    break;
  case 6: {
    printAdminMenuScreen();
    int adminChoice;
    if (readAdminMenuChoice(&adminChoice))
      handleAdminPanelSelection(adminChoice);
    break;
  }
  case 7:
    printAdminLogoutScreen();
    return;
  case 8:
    printAdminExitScreen();
    exit(0);
  default:
    showErrorMessage("Pilihan tidak valid.");
  }
}

void openMainMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMainMenu();

    int maxChoice = getMainMenuMaxChoice();
    int choice = getValidatedMenuChoice(1, maxChoice);

    if (choice == -1)
      continue;

    if (isAdmin())
      handleMenuChoiceAdmin(choice, monthReportList);
    else
      handleMenuChoiceUser(choice, monthReportList);
  }
}
