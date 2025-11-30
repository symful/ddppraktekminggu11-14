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

void printInfoUser() {
  int boxWidth = 59;

  if (currentUser == NULL)
    return;

  if (currentUser->isAdmin) {
    printCentered("│              👑 ", boxWidth);
    printColored("Admin: ", COLOR_BRIGHT_YELLOW);
    printColored(currentUser->username, COLOR_BRIGHT_CYAN);
    for (int i = strlen(currentUser->username); i < 30; i++)
      printf(" ");
    printf(" │\n");
    printCenteredColored(
        "│              🚫 Tanpa Folder Pribadi                   │\n",
        boxWidth, COLOR_BRIGHT_WHITE);
  } else {
    printCentered("│              👤 ", boxWidth);
    printColored("Pengguna: ", COLOR_BRIGHT_YELLOW);
    printColored(currentUser->username, COLOR_BRIGHT_CYAN);
    for (int i = strlen(currentUser->username); i < 27; i++)
      printf(" ");
    printf(" │\n");
  }
}

void printFiturMenu() {
  int boxWidth = 59;
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  if (currentUser != NULL && currentUser->isAdmin) {
    printCenteredColored(
        "│  1. 🚫 Kelola Laporan Bulanan (Admin tidak memiliki)    │\n",
        boxWidth, COLOR_DIM);
    printCenteredColored(
        "│  2. 🚫 Lihat Ringkasan Keuangan (Admin tidak memiliki)  │\n",
        boxWidth, COLOR_DIM);
    printCenteredColored(
        "│  3. 🚫 Pengaturan Budget (Admin tidak memiliki)         │\n",
        boxWidth, COLOR_DIM);
    printCenteredColored(
        "│  4. 🚫 Kelola Kategori (Admin tidak memiliki)           │\n",
        boxWidth, COLOR_DIM);
    printCentered("│  ", boxWidth);
    printColored("5.", COLOR_BRIGHT_CYAN);
    printf(" ⚙️  Pengaturan Sistem                                │\n");
  } else {
    printCentered("│  ", boxWidth);
    printColored("1.", COLOR_BRIGHT_CYAN);
    printf(" 📊 Kelola Laporan Bulanan                           │\n");
    printCentered("│  ", boxWidth);
    printColored("2.", COLOR_BRIGHT_CYAN);
    printf(" 📈 Lihat Ringkasan Keuangan                         │\n");
    printCentered("│  ", boxWidth);
    printColored("3.", COLOR_BRIGHT_CYAN);
    printf(" ⚙️  Pengaturan Budget                                │\n");
    printCentered("│  ", boxWidth);
    printColored("4.", COLOR_BRIGHT_CYAN);
    printf(" 🏷️  Kelola Kategori                                 │\n");
    printCentered("│  ", boxWidth);
    printColored("5.", COLOR_BRIGHT_CYAN);
    printf(" 🔧 Pengaturan Sistem                                │\n");
  }
}

void printFiturMenuAuth() {
  int boxWidth = 59;

  if (currentUser != NULL && currentUser->isAdmin) {
    printCentered("│  ", boxWidth);
    printColored("6.", COLOR_BRIGHT_CYAN);
    printf(" 👑 Admin Panel                                      │\n");
    printCentered("│  ", boxWidth);
    printColored("7.", COLOR_BRIGHT_CYAN);
    printf(" 🔓 Logout                                           │\n");
    printCentered("│  ", boxWidth);
    printColored("8.", COLOR_BRIGHT_CYAN);
    printf(" ❌ Keluar                                           │\n");
  } else {
    printCentered("│  ", boxWidth);
    printColored("6.", COLOR_BRIGHT_CYAN);
    printf(" 🔓 Logout                                           │\n");
    printCentered("│  ", boxWidth);
    printColored("7.", COLOR_BRIGHT_CYAN);
    printf(" ❌ Keluar                                           │\n");
  }
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

void showSuccessMessage(const char *message) {
  printf("\n");
  printSuccess("✅ ");
  printf("%s", message);
  waitForEnter();
}

void showErrorMessage(const char *message) {
  printf("\n");
  printError("❌ ");
  printf("%s", message);
  waitForEnter();
}

void showWarningMessage(const char *message) {
  printf("\n");
  printWarning("⚠️  ");
  printf("%s", message);
  waitForEnter();
}

void showInfoMessage(const char *message) {
  printf("\n");
  printInfo("ℹ️  ");
  printf("%s", message);
  waitForEnter();
}

/* =======================================================
    INI
   BELUUUUUUUUUUUUUUUUUUUUUMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
         MODUL : getValidatedMenuChoice()
         DESKRIPSI : Melakukan Validasi Terhadap Input Yang Diberikan Oleh User
         INPUT :
           mhs[] ? array berisi data mahasiswa
           n ? jumlah mahasiswa
         OUTPUT :
           Mengembalikan nilai rata-rata (float).
   =======================================================*/
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

void printBudgetHeader() {
  int boxWidth = 59;
  int contentLines = 10;
  clearAndCenterVertically(contentLines);

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
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
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

struct MonthReport *selectMonthReport(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1,
                              monthReportList->count, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }

  return monthReportList->reports[reportChoice - 1];
}

bool selectCategory(char *outCategory, size_t bufferSize) {
  struct CategoryList *categories = getUserCategoriesCache();

  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    return false;
  }

  int boxWidth = 59;
  printf("\n");
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("        📂 Pilih Kategori untuk Mengatur Budget          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  for (int i = 0; i < categories->count; i++) {
    printCentered("  ", boxWidth);
    printf("%s%d.%s %s\n", COLOR_BRIGHT_CYAN, i + 1, COLOR_RESET,
           categories->items[i]->displayName);
  }
  printf("\n");

  int categoryChoice;
  if (!readAndValidateInteger("\n🎯 Pilihan kategori: ", 1, categories->count,
                              &categoryChoice)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  strncpy(outCategory, categories->items[categoryChoice - 1]->internalName,
          bufferSize - 1);
  outCategory[bufferSize - 1] = '\0';
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

void showSetBudgetSuccessMessage(const char *category, long long budget) {
  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, category);

  char successMsg[256];
  snprintf(successMsg, sizeof(successMsg),
           "Budget untuk kategori '%s' berhasil diatur menjadi Rp %lld",
           displayName, budget);
  showSuccessMessage(successMsg);
}

bool ensureHasMonthReports(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return false;
  }
  return true;
}

void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList) {
  if (!ensureHasMonthReports(monthReportList)) {
    return;
  }

  printBudgetCategoryHeader();

  struct MonthReport *report = selectMonthReport(monthReportList);
  if (report == NULL) {
    return;
  }

  char category[50];
  if (!selectCategory(category, sizeof(category))) {
    return;
  }

  long long budget;
  if (!inputNewBudget(&budget)) {
    return;
  }

  setBudgetForCategory(report, category, budget);
  saveUserMonthReport(report);

  showSetBudgetSuccessMessage(category, budget);
}

bool hasMonthReportsView(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
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

struct MonthReport *
selectReportForBudgetView(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1,
                              monthReportList->count, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }

  return monthReportList->reports[reportChoice - 1];
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

void printBudgetTableRow(const struct TransactionGroup *group) {
  int boxWidth = 88;
  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, group->category);
  long long remaining = group->budget - group->totalAmount;
  double percentage = group->budget > 0
                          ? ((double)group->totalAmount / group->budget) * 100.0
                          : 0.0;
  const char *status = getGroupBudgetStatus(group);

  printCentered("║ ", boxWidth);
  printf("%-20s │ ", displayName);
  printColored("%-15lld", COLOR_BRIGHT_CYAN);
  printf(" │ ");
  printColored("%-15lld",
               remaining >= 0 ? COLOR_BRIGHT_GREEN : COLOR_BRIGHT_RED);
  printf(" │ ");
  printAmount(remaining);
  printf("%-15s │ ", "");
  printBudgetStatus(status, percentage);
  printf("%-10s ║\n", "");
}

void printBudgetTableFooter() {
  int boxWidth = 88;
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "════════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

void printBudgetReport(const struct MonthReport *report) {
  printBudgetPerCategoryHeader();

  for (int i = 0; i < report->groupCount; i++) {
    const struct TransactionGroup *group = report->groups[i];
    printBudgetTableRow(group);
  }

  printBudgetTableFooter();
}

void openViewBudgetMenu(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsView(monthReportList)) {
    return;
  }

  printCurrentBudgetHeader();

  struct MonthReport *report = selectReportForBudgetView(monthReportList);
  if (report == NULL) {
    return;
  }

  printBudgetReport(report);

  waitForEnter();
}

void showAllCategories() {
  int boxWidth = 59;
  clearScreen();
  struct CategoryList *categories = getUserCategoriesCache();

  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    waitForEnter();
    return;
  }

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

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 KATEGORI PENDAPATAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  int incomeCount = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_INCOME) {
      printCentered("  ", boxWidth);
      printf("%d.", i + 1);
      printf(" %-30s ", categories->items[i]->displayName);
      if (categories->items[i]->isDefault) {
        printColored("[Default]", COLOR_BRIGHT_WHITE);
      } else {
        printColored("[Custom]", COLOR_BRIGHT_YELLOW);
      }
      printf("\n");
      incomeCount++;
    }
  }
  if (incomeCount == 0) {
    printCentered("  📭 Tidak ada kategori pendapatan\n", boxWidth);
  }

  printf("\n");
  printCentered("", boxWidth);
  printError("💸 KATEGORI PENGELUARAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  int expenseCount = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_EXPENSE) {
      printCentered("  ", boxWidth);
      printf("%d.", i + 1);
      printf(" %-30s ", categories->items[i]->displayName);
      if (categories->items[i]->isDefault) {
        printColored("[Default]", COLOR_BRIGHT_WHITE);
      } else {
        printColored("[Custom]", COLOR_BRIGHT_YELLOW);
      }
      printf("\n");
      expenseCount++;
    }
  }
  if (expenseCount == 0) {
    printCentered("  📭 Tidak ada kategori pengeluaran\n", boxWidth);
  }

  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total: ");
  printf("%d kategori (", categories->count);
  printf("%s%d%s", COLOR_BRIGHT_GREEN, incomeCount, COLOR_RESET);
  printf(" pendapatan, ");
  printf("%s%d%s", COLOR_BRIGHT_RED, expenseCount, COLOR_RESET);
  printf(" pengeluaran)\n");
}

void openAddCategoryMenu() {
  int boxWidth = 59;
  int contentLines = 12;
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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("📝 TIPE KATEGORI:\n");
  printCentered("  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 💰 Pendapatan (Income)\n");
  printCentered("  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 💸 Pengeluaran (Expense)\n");

  int typeChoice;
  printf("\n");
  printCentered("", boxWidth);
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &typeChoice)) {
    showErrorMessage("Tipe tidak valid.");
    waitForEnter();
    return;
  }

  enum TransactionType type =
      (typeChoice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;

  char displayName[50];
  printf("\n");
  printCentered("", boxWidth);
  if (!readCategoryNameSafe(displayName, sizeof(displayName),
                            "📝 Nama kategori (Indonesia): ")) {
    showErrorMessage("Nama kategori tidak valid.");
    waitForEnter();
    return;
  }

  char internalName[50];
  strncpy(internalName, displayName, sizeof(internalName) - 1);
  internalName[sizeof(internalName) - 1] = '\0';
  normalizeCategoryName(internalName);

  struct CategoryList *categories = loadUserCategories();

  if (categoryExistsInList(categories, internalName)) {
    showErrorMessage("Kategori dengan nama tersebut sudah ada.");
    freeCategoryList(categories);
    waitForEnter();
    return;
  }

  if (addCategoryToList(categories, internalName, displayName, type, 0)) {
    saveUserCategories(categories);
    refreshCategoryCache();

    char successMsg[256];
    snprintf(successMsg, sizeof(successMsg),
             "Kategori '%s' berhasil ditambahkan!", displayName);
    showSuccessMessage(successMsg);
  } else {
    showErrorMessage("Gagal menambahkan kategori.");
  }

  freeCategoryList(categories);
  waitForEnter();
}

void openDeleteCategoryMenu() {
  int boxWidth = 59;
  clearScreen();
  struct CategoryList *categories = getUserCategoriesCache();

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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("📋 KATEGORI CUSTOM:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  int customCount = 0;
  int customIndices[100];

  for (int i = 0; i < categories->count; i++) {
    if (!categories->items[i]->isDefault) {
      customIndices[customCount] = i;
      printCentered("  ", boxWidth);
      printf("%d.", i + 1);
      printf(" %s ", categories->items[i]->displayName);
      if (categories->items[i]->type == TRANSACTION_INCOME) {
        printSuccess("(Pendapatan)\n");
      } else {
        printError("(Pengeluaran)\n");
      }
      customCount++;
    }
  }

  if (customCount == 0) {
    showWarningMessage("Tidak ada kategori custom untuk dihapus.");
    waitForEnter();
    return;
  }

  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Catatan: Kategori default tidak dapat dihapus\n");

  int choice;
  printf("\n");
  printCentered("", boxWidth);
  if (!readAndValidateInteger(
          "🎯 Pilih kategori untuk dihapus (0 untuk batal): ", 0, customCount,
          &choice)) {
    showErrorMessage("Pilihan tidak valid.");
    waitForEnter();
    return;
  }

  if (choice == 0) {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  int categoryIndex = customIndices[choice - 1];
  const char *categoryName = categories->items[categoryIndex]->internalName;

  if (!canDeleteCategory(categoryName)) {
    showErrorMessage("Kategori sedang digunakan dan tidak dapat dihapus.");
    waitForEnter();
    return;
  }

  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  Anda yakin ingin menghapus kategori '");
  printf("%s", categories->items[categoryIndex]->displayName);
  printf("'? (y/n): ");

  char confirm;
  scanf(" %c", &confirm);
  clearInputBuffer();

  if (confirm != 'y' && confirm != 'Y') {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  struct CategoryList *fullList = loadUserCategories();
  if (removeCategoryFromList(fullList, categoryName)) {
    saveUserCategories(fullList);
    refreshCategoryCache();
    showSuccessMessage("Kategori berhasil dihapus!");
  } else {
    showErrorMessage("Gagal menghapus kategori.");
  }

  freeCategoryList(fullList);
  waitForEnter();
}

void openCategoryManagementMenu() {
  int boxWidth = 59;
  while (1) {
    int contentLines = 10;
    clearAndCenterVertically(contentLines);
    printCenteredColored(
        "┌─────────────────────────────────────────────────────────┐\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│", boxWidth);
    printWithBg("                🏷️  KELOLA KATEGORI                      ",
                COLOR_BRIGHT_WHITE, BG_BLUE);
    printf("│\n");
    printCenteredColored(
        "├─────────────────────────────────────────────────────────┤\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│  ", boxWidth);
    printColored("1.", COLOR_BRIGHT_CYAN);
    printf(" 📋 Lihat Semua Kategori                             │\n");
    printCentered("│  ", boxWidth);
    printColored("2.", COLOR_BRIGHT_CYAN);
    printf(" ➕ Tambah Kategori Baru                             │\n");
    printCentered("│  ", boxWidth);
    printColored("3.", COLOR_BRIGHT_CYAN);
    printf(" 🗑️  Hapus Kategori Custom                           │\n");
    printCentered("│  ", boxWidth);
    printColored("4.", COLOR_BRIGHT_CYAN);
    printf(" ⬅️  Kembali ke Menu Utama                            │\n");
    printCenteredColored(
        "└─────────────────────────────────────────────────────────┘\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printf("\n");
    printCentered("", boxWidth);
    printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);

    int choice = getValidatedMenuChoice(1, 4);
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

void openResetBudgetMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 10;
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

  printf("\n");
  printCentered("", boxWidth);
  printWarning(
      "⚠️  PERINGATAN: Ini akan mengatur semua budget ke nilai default!\n");
  printCentered("", boxWidth);
  printInfo("📊 Budget default untuk setiap kategori: ");
  printf("%sRp 1,000,000%s\n\n", COLOR_BRIGHT_CYAN, COLOR_RESET);

  char confirmation;
  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Reset budget dibatalkan.");
    return;
  }

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j < categories->count; j++) {
      if (categories->items[j] != NULL) {
        setBudgetForCategory(report, categories->items[j]->internalName,
                             1000000);
      }
    }

    saveUserMonthReport(report);
  }

  showSuccessMessage("Semua budget berhasil direset ke Rp 1,000,000!");
}

void openConfigurationMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  while (1) {
    int contentLines = 12;
    clearAndCenterVertically(contentLines);
    printCenteredColored(
        "┌─────────────────────────────────────────────────────────┐\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│", boxWidth);
    printWithBg("                🔧 PENGATURAN SISTEM                     ",
                COLOR_BRIGHT_WHITE, BG_BLUE);
    printf("│\n");
    printCenteredColored(
        "├─────────────────────────────────────────────────────────┤\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│  ", boxWidth);
    printColored("1.", COLOR_BRIGHT_CYAN);
    printf(" 💰 Atur Minimum Jumlah Uang                         │\n");
    printCentered("│  ", boxWidth);
    printColored("2.", COLOR_BRIGHT_CYAN);
    printf(" 📊 Atur Minimum Budget                              │\n");
    printCentered("│  ", boxWidth);
    printColored("3.", COLOR_BRIGHT_CYAN);
    printf(" 💳 Atur Minimum Transaksi                           │\n");
    printCentered("│  ", boxWidth);
    printColored("4.", COLOR_BRIGHT_CYAN);
    printf(" 📋 Lihat Pengaturan Saat Ini                        │\n");
    printCentered("│  ", boxWidth);
    printColored("5.", COLOR_BRIGHT_CYAN);
    printf(" 🔄 Reset ke Pengaturan Default                      │\n");
    printCentered("│  ", boxWidth);
    printColored("6.", COLOR_BRIGHT_CYAN);
    printf(" ⬅️  Kembali ke Menu Utama                            │\n");
    printCenteredColored(
        "└─────────────────────────────────────────────────────────┘\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printf("\n");
    printCentered("", boxWidth);
    printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);

    int choice = getValidatedMenuChoice(1, 6);
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
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}

void openSetMinimumAmountMenu() {
  int boxWidth = 59;
  int contentLines = 8;
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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("💰 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumAmount(), COLOR_RESET);

  long long newMinimum;
  if (!readAndValidateLongLong("💰 Masukkan minimum baru (minimal 1): ", 1,
                               getMaximumAmount(), &newMinimum)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  setMinimumAmount(newMinimum);
  saveConfigToFile("./config.txt");

  char successMsg[200];
  snprintf(successMsg, sizeof(successMsg),
           "Minimum jumlah uang berhasil diatur menjadi Rp %lld", newMinimum);
  showSuccessMessage(successMsg);
}

void openSetMinimumBudgetMenu() {
  int boxWidth = 59;
  int contentLines = 8;
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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumBudget(), COLOR_RESET);

  long long newMinimum;
  if (!readAndValidateLongLong("📊 Masukkan minimum baru (minimal 1): ", 1,
                               getMaximumBudget(), &newMinimum)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  setMinimumBudget(newMinimum);
  saveConfigToFile("./config.txt");

  char successMsg[200];
  snprintf(successMsg, sizeof(successMsg),
           "Minimum budget berhasil diatur menjadi Rp %lld", newMinimum);
  showSuccessMessage(successMsg);
}

void openSetMinimumTransactionMenu() {
  int boxWidth = 59;
  int contentLines = 8;
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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("💳 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, globalConfig.money.minimumBudget,
         COLOR_RESET);

  long long newMinimum;
  if (!readAndValidateLongLong("💳 Masukkan minimum baru (minimal 1): ", 1,
                               globalConfig.money.minimumBudget, &newMinimum)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  globalConfig.money.minimumBudget = newMinimum;
  saveConfigToFile("./config.txt");

  char successMsg[200];
  snprintf(successMsg, sizeof(successMsg),
           "Minimum transaksi berhasil diatur menjadi Rp %lld", newMinimum);
  showSuccessMessage(successMsg);
}

void openViewConfigurationMenu() {
  int boxWidth = 88;
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

  printf("\n");
  printCenteredColored(
      "╔════════════════════════════════════════════════════════════════════"
      "══════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg(
      "                              💰 PENGATURAN KEUANGAN                "
      "            ",
      COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╠════════════════════════════════════════════════════════════════════"
      "══════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║ ", boxWidth);
  printf("%s%-30s%s", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-20s%s", COLOR_BRIGHT_CYAN, "MINIMUM", COLOR_RESET);
  printf(" │ ");
  printf("%s%-20s%s", COLOR_BRIGHT_CYAN, "MAXIMUM", COLOR_RESET);
  printf(" ║\n");
  printCenteredColored(
      "╠════════════════════════════════════════════════════════════════════"
      "══════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
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
  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  printf("\n");
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════════════"
      "════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg(
      "                             ⚙️  PENGATURAN VALIDASI                 "
      "           ",
      COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╠════════════════════════════════════════════════════════════════════"
      "════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
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
  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  waitForEnter();
}

void openResetConfigurationMenu() {
  int boxWidth = 59;
  int contentLines = 10;
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

  printf("\n");
  printCentered("", boxWidth);
  printWarning(
      "⚠️  PERINGATAN: Ini akan mengembalikan semua pengaturan ke nilai "
      "default!\n\n");

  char confirmation;
  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Reset pengaturan dibatalkan.");
    return;
  }

  initializeDefaultConfig();
  saveConfigToFile("./config.txt");

  showSuccessMessage(
      "Pengaturan berhasil direset ke nilai default! Silakan restart aplikasi "
      "untuk melihat perubahan.");
}

void openMainMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMainMenu();

    int maxChoice = (currentUser && currentUser->isAdmin) ? 8 : 7;
    int choice = getValidatedMenuChoice(1, maxChoice);
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      if (currentUser && currentUser->isAdmin) {
        showErrorMessage("Admin tidak memiliki laporan pribadi! Gunakan Admin "
                         "Panel untuk melihat laporan semua pengguna.");
      } else {

        openMonthlyMenu(monthReportList);
      }
      break;
    case 2:
      if (currentUser && currentUser->isAdmin) {
        showErrorMessage("Admin tidak memiliki ringkasan pribadi! Gunakan "
                         "Admin Panel untuk statistik sistem.");
      } else {
        openSummaryMenu(monthReportList);
      }
      break;
    case 3:
      if (currentUser && currentUser->isAdmin) {
        showErrorMessage("Admin tidak memiliki budget pribadi! Gunakan Admin "
                         "Panel untuk mengelola pengguna.");
      } else {
        openBudgetSettingsMenu(monthReportList);
      }
      break;
    case 4:
      if (currentUser && currentUser->isAdmin) {
        showErrorMessage("Admin tidak memiliki kategori pribadi!");
      } else {
        openCategoryManagementMenu();
      }
      break;
    case 5:
      openConfigurationMenu(monthReportList);
      break;
    case 6:
      if (currentUser && currentUser->isAdmin) {

        clearScreen();
        showAdminMenu();

        int adminChoice;
        if (scanf("%d", &adminChoice) == 1) {
          int c;
          while ((c = getchar()) != '\n' && c != EOF)
            ;

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

            clearScreen();
            printf("╔══════════════════════════════════════════════════════════"
                   "╗\n");
            printf("║                      🔓 LOGOUT                          "
                   "║\n");
            printf("║                                                          "
                   "║\n");
            printf("║               Logout admin: %-23s     ║\n",
                   currentUser ? currentUser->username : "Tidak Diketahui");
            printf("║                                                          "
                   "║\n");
            printf("║                 Kembali ke layar login...               "
                   "║\n");
            printf("╚══════════════════════════════════════════════════════════"
                   "╝\n");
            printf("\n");
            return;
          }
        }
      } else {

        clearScreen();
        printf(
            "╔══════════════════════════════════════════════════════════╗\n");
        printf("║                      🔓 LOGOUT                          ║\n");
        printf(
            "║                                                          ║\n");
        printf("║               Logout pengguna: %-21s     ║\n",
               currentUser ? currentUser->username : "Tidak Diketahui");
        printf(
            "║                                                          ║\n");
        printf("║                 Kembali ke layar login...               ║\n");
        printf(
            "╚══════════════════════════════════════════════════════════╝\n");
        printf("\n");
        return;
      }
      break;
    case 7:
      if (currentUser && currentUser->isAdmin) {

        clearScreen();
        printf(
            "╔══════════════════════════════════════════════════════════╗\n");
        printf("║                      🔓 LOGOUT                          ║\n");
        printf(
            "║                                                          ║\n");
        printf("║               Logout admin: %-23s     ║\n",
               currentUser ? currentUser->username : "Tidak Diketahui");
        printf(
            "║                                                          ║\n");
        printf("║                 Kembali ke layar login...               ║\n");
        printf(
            "╚══════════════════════════════════════════════════════════╝\n");
        printf("\n");
        return;
      } else {

        clearScreen();
        printf(
            "╔══════════════════════════════════════════════════════════╗\n");
        printf("║                    👋 TERIMA KASIH!                     ║\n");
        printf(
            "║                                                          ║\n");
        printf("║          Semoga keuangan Anda selalu terjaga! 💰        ║\n");
        printf(
            "║                                                          ║\n");
        printf("║                   Sampai jumpa lagi! 😊                 ║\n");
        printf(
            "╚══════════════════════════════════════════════════════════╝\n");
        printf("\n");
        exit(0);
      }
      break;
    case 8:
      if (currentUser && currentUser->isAdmin) {

        clearScreen();
        printf(
            "╔══════════════════════════════════════════════════════════╗\n");
        printf("║                    👋 TERIMA KASIH!                     ║\n");
        printf(
            "║                                                          ║\n");
        printf("║          Semoga keuangan Anda selalu terjaga! 💰👑      ║\n");
        printf(
            "║                                                          ║\n");
        printf("║                   Sampai jumpa lagi! 😊                 ║\n");
        printf(
            "╚══════════════════════════════════════════════════════════╝\n");
        printf("\n");
        exit(0);
      }
      break;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}
