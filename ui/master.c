#include "../auth/auth.h"
#include "../db/transaction_group.c"
#include "../types/include.c"
#include "../utils/validation.c"
#include "./month_report.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* =======================================================
        MODUL : tampilkanMenu()
        DESKRIPSI : Membersihkan Layar
        INITIAL STATE : Terdapat teks pada layar
        FINAL STATE : Layar dibersihkan menjadi kosong
   =======================================================*/
void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

/* =======================================================
        MODUL : waitForEnter()
        DESKRIPSI : Meminta User Untuk Menekan Enter Untuk Lanjut
        INITIAL STATE :
        FINAL STATE :
   =======================================================*/
void waitForEnter() {
  printf("\n📱 Tekan Enter untuk melanjutkan...");
  clearInputBuffer();
}

/* =======================================================
        MODUL : printMainMenuHeader()
        DESKRIPSI : Mencetak Header Menu Utama
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Header Menu Utama Ditampilkan Ke Layar
   =======================================================*/
void printMainMenuHeader() {
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                    🏠 MENU UTAMA                        │\n");
}

/* =======================================================
        MODUL : printInfoUser()
        DESKRIPSI : Mencetak Informasi User (Username dan Hak Akses)
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Informasi User Ditampilkan Ke Layar
   =======================================================*/
void printInfoUser() {
  if (currentUser == NULL)
    return;

  if (currentUser->isAdmin) {
    printf("│              👑 Admin: %-30s │\n", currentUser->username);
    printf("│              🚫 Tanpa Folder Pribadi                   │\n");
  } else {
    printf("│              👤 Pengguna: %-27s │\n", currentUser->username);
  }
}

/* =======================================================
        MODUL : printFiturMenu()
        DESKRIPSI : Mencetak Fitur-Fitur Pada Menu Sesuai Role
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Fitur-Fitur Pada Menu Dicetak Sesuai Dengan Role
   =======================================================*/
void printFiturMenu() {
  printf("├─────────────────────────────────────────────────────────┤\n");
  if (currentUser != NULL && currentUser->isAdmin) {
    printf("│  1. 🚫 Kelola Laporan Bulanan (Admin tidak memiliki)    │\n");
    printf("│  2. 🚫 Lihat Ringkasan Keuangan (Admin tidak memiliki)  │\n");
    printf("│  3. 🚫 Pengaturan Budget (Admin tidak memiliki)         │\n");
    printf("│  4. 🚫 Kelola Kategori (Admin tidak memiliki)           │\n");
    printf("│  5. ⚙️  Pengaturan Sistem                                │\n");
  } else {
    printf("│  1. 📊 Kelola Laporan Bulanan                           │\n");
    printf("│  2. 📈 Lihat Ringkasan Keuangan                         │\n");
    printf("│  3. ⚙️  Pengaturan Budget                                │\n");
    printf("│  4. 🏷️  Kelola Kategori                                 │\n");
    printf("│  5. 🔧 Pengaturan Sistem                                │\n");
  }
}

/* =======================================================
        MODUL : printFiturMenuAuth()
        DESKRIPSI : Mencetak Fitur-Fitur Pada Menu Yang Berkaitan Autentikasi
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Fitur-Fitur Pada Menu Yang Berkaitan Autentikasi Dicetak
   Sesuai Dengan Role
   =======================================================*/
void printFiturMenuAuth() {
  if (currentUser != NULL && currentUser->isAdmin) {
    printf("│  6. 👑 Admin Panel                                      │\n");
    printf("│  7. 🔓 Logout                                           │\n");
    printf("│  8. ❌ Keluar                                           │\n");
  } else {
    printf("│  6. 🔓 Logout                                           │\n");
    printf("│  7. ❌ Keluar                                           │\n");
  }
}

/* =======================================================
        MODUL : printMainMenuFooter()
        DESKRIPSI : Mencetak Footer Main Menu
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Footer Main Menu Dicetak Pada Layar
   =======================================================*/
void printMainMenuFooter() {
  printf("└─────────────────────────────────────────────────────────┘\n");
  printf("\n💡 Tip: Pilih nomor menu yang diinginkan\n");
  printf("🎯 Pilihan Anda: ");
}

/* =======================================================
        MODUL : showMainMenu()
        DESKRIPSI : Menampung Seluruh Modul Yang Menampilkan Main Menu
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Melakukan Pemanggilan
   Modul) FINAL STATE : Modul yang Terkait Dengan Mencetak Main Menu Dipanggil
   =======================================================*/
void showMainMenu() {
  printMainMenuHeader();
  printInfoUser();
  printFiturMenu();
  printFiturMenuAuth();
  printMainMenuFooter();
}

/* =======================================================
        MODUL : showSuccessMessage()
        DESKRIPSI : Menampilkan Pesan Sukses
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Pesan Sukses Dicetak Ke Layar, Sesuai Dengan Message Yang
   Dikirimkan Lewat Parameter
   =======================================================*/
void showSuccessMessage(const char *message) {
  printf("\n✅ %s", message);
  waitForEnter();
}

/* =======================================================
        MODUL : showErrorMessage()
        DESKRIPSI : Menampilkan Pesan Error
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Pesan Error Dicetak Ke Layar, Sesuai Dengan Message Yang
   Dikirimkan Lewat Parameter
   =======================================================*/
void showErrorMessage(const char *message) {
  printf("\n❌ %s", message);
  waitForEnter();
}

/* =======================================================
        MODUL : showWarningMessage()
        DESKRIPSI : Menampilkan Pesan Warning
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Pesan Warning Dicetak Ke Layar, Sesuai Dengan Message Yang
   Dikirimkan Lewat Parameter
   =======================================================*/
void showWarningMessage(const char *message) {
  printf("\n⚠️  %s", message);
  waitForEnter();
}

/* =======================================================
        MODUL : showInfoMessage()
        DESKRIPSI : Menampilkan Pesan Informasi
        INITIAL STATE : (Tidak Ada Data Yang Diubah, Hanya Menampilkan Teks)
        FINAL STATE : Pesan Informasi Dicetak Ke Layar, Sesuai Dengan Message
   Yang Dikirimkan Lewat Parameter
   =======================================================*/
void showInfoMessage(const char *message) {
  printf("\nℹ️  %s", message);
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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                 ⚙️  PENGATURAN BUDGET                    │\n");
  printf("├─────────────────────────────────────────────────────────┤\n");
  printf("│  1. 🏷️  Atur Budget per Kategori                         │\n");
  printf("│  2. 📋 Lihat Budget Saat Ini                            │\n");
  printf("│  3. 🔄 Reset Semua Budget                               │\n");
  printf("│  4. ⬅️  Kembali ke Menu Utama                            │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
  printf("\n🎯 Pilihan Anda: ");
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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│             🏷️  ATUR BUDGET PER KATEGORI                │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
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

  printf("\n📂 Pilih Kategori untuk Mengatur Budget:\n");
  printf("─────────────────────────────────────────\n");
  for (int i = 0; i < categories->count; i++) {
    printf("  %d. %s\n", i + 1, categories->items[i]->displayName);
  }

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                📋 BUDGET SAAT INI                      │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
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
  printf("\n╔══════════════════════════════════════════════════════════════════"
         "════════════════╗\n");
  printf("║                              💰 DETAIL BUDGET PER KATEGORI         "
         "               ║\n");
  printf("╠════════════════════════════════════════════════════════════════════"
         "══════════════╣\n");
  printf("║ %-20s │ %-15s │ %-15s │ %-15s │ %-10s ║\n", "KATEGORI",
         "BUDGET (Rp)", "TERPAKAI (Rp)", "SISA (Rp)", "STATUS");
  printf("╠════════════════════════════════════════════════════════════════════"
         "══════════════╣\n");
}

void printBudgetTableRow(const struct TransactionGroup *group) {
  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, group->category);
  printf("║ %-20s │ %-15lld │ %-15lld │ %-15lld │ %-10s ║\n", displayName,
         group->budget, group->totalAmount, group->budget - group->totalAmount,
         getGroupBudgetStatus(group));
}

void printBudgetTableFooter() {
  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");
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
  clearScreen();
  struct CategoryList *categories = getUserCategoriesCache();

  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    waitForEnter();
    return;
  }

  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                📋 DAFTAR KATEGORI                       │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("\n💰 KATEGORI PENDAPATAN:\n");
  printf("═══════════════════════════════════════════════════════════\n");

  int incomeCount = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_INCOME) {
      printf("  %d. %-30s %s\n", ++incomeCount,
             categories->items[i]->displayName,
             categories->items[i]->isDefault ? "[Default]" : "[Custom]");
    }
  }
  if (incomeCount == 0) {
    printf("  📭 Tidak ada kategori pendapatan\n");
  }

  printf("\n💸 KATEGORI PENGELUARAN:\n");
  printf("═══════════════════════════════════════════════════════════\n");

  int expenseCount = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_EXPENSE) {
      printf("  %d. %-30s %s\n", ++expenseCount,
             categories->items[i]->displayName,
             categories->items[i]->isDefault ? "[Default]" : "[Custom]");
    }
  }
  if (expenseCount == 0) {
    printf("  📭 Tidak ada kategori pengeluaran\n");
  }

  printf("\n📊 Total: %d kategori (%d pendapatan, %d pengeluaran)\n",
         categories->count, incomeCount, expenseCount);
}

void openAddCategoryMenu() {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│               ➕ TAMBAH KATEGORI BARU                    │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("\n📝 TIPE KATEGORI:\n");
  printf("  1. 💰 Pendapatan (Income)\n");
  printf("  2. 💸 Pengeluaran (Expense)\n");

  int typeChoice;
  if (!readAndValidateInteger("\n🎯 Pilihan: ", 1, 2, &typeChoice)) {
    showErrorMessage("Tipe tidak valid.");
    waitForEnter();
    return;
  }

  enum TransactionType type =
      (typeChoice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;

  char displayName[50];
  if (!readCategoryNameSafe(displayName, sizeof(displayName),
                            "\n📝 Nama kategori (Indonesia): ")) {
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
  clearScreen();
  struct CategoryList *categories = getUserCategoriesCache();

  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│               🗑️  HAPUS KATEGORI CUSTOM                 │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("\n📋 KATEGORI CUSTOM:\n");
  printf("═══════════════════════════════════════════════════════════\n");

  int customCount = 0;
  int customIndices[100];

  for (int i = 0; i < categories->count; i++) {
    if (!categories->items[i]->isDefault) {
      customIndices[customCount] = i;
      printf("  %d. %s (%s)\n", customCount + 1,
             categories->items[i]->displayName,
             categories->items[i]->type == TRANSACTION_INCOME ? "Pendapatan"
                                                              : "Pengeluaran");
      customCount++;
    }
  }

  if (customCount == 0) {
    showWarningMessage("Tidak ada kategori custom untuk dihapus.");
    waitForEnter();
    return;
  }

  printf("\n💡 Catatan: Kategori default tidak dapat dihapus\n");

  int choice;
  if (!readAndValidateInteger(
          "\n🎯 Pilih kategori untuk dihapus (0 untuk batal): ", 0, customCount,
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

  printf("\n⚠️  Anda yakin ingin menghapus kategori '%s'? (y/n): ",
         categories->items[categoryIndex]->displayName);

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
  while (1) {
    clearScreen();
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│                🏷️  KELOLA KATEGORI                      │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│  1. 📋 Lihat Semua Kategori                             │\n");
    printf("│  2. ➕ Tambah Kategori Baru                             │\n");
    printf("│  3. 🗑️  Hapus Kategori Custom                           │\n");
    printf("│  4. ⬅️  Kembali ke Menu Utama                            │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    printf("\n🎯 Pilihan Anda: ");

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│              🔄 RESET SEMUA BUDGET                      │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("⚠️  PERINGATAN: Ini akan mengatur semua budget ke nilai default!\n");
  printf("📊 Budget default untuk setiap kategori: Rp 1,000,000\n\n");

  char confirmation;
  printf("❓ Apakah Anda yakin? (y/n): ");
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
  while (1) {
    clearScreen();
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│                🔧 PENGATURAN SISTEM                     │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│  1. 💰 Atur Minimum Jumlah Uang                         │\n");
    printf("│  2. 📊 Atur Minimum Budget                              │\n");
    printf("│  3. 💳 Atur Minimum Transaksi                           │\n");
    printf("│  4. 📋 Lihat Pengaturan Saat Ini                        │\n");
    printf("│  5. 🔄 Reset ke Pengaturan Default                      │\n");
    printf("│  6. ⬅️  Kembali ke Menu Utama                            │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    printf("\n🎯 Pilihan Anda: ");

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│             💰 ATUR MINIMUM JUMLAH UANG                 │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("💰 Minimum saat ini: Rp %lld\n", getMinimumAmount());

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                📊 ATUR MINIMUM BUDGET                   │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("📊 Minimum saat ini: Rp %lld\n", getMinimumBudget());

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│              💳 ATUR MINIMUM TRANSAKSI                  │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("💳 Minimum saat ini: Rp %lld\n", globalConfig.money.minimumBudget);

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
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│              📋 PENGATURAN SAAT INI                     │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("╔════════════════════════════════════════════════════════════════════"
         "══════════════╗\n");
  printf("║                              💰 PENGATURAN KEUANGAN                "
         "            ║\n");
  printf("╠════════════════════════════════════════════════════════════════════"
         "══════════════╣\n");
  printf("║ %-30s │ %-20s │ %-20s ║\n", "KATEGORI", "MINIMUM", "MAXIMUM");
  printf("╠════════════════════════════════════════════════════════════════════"
         "══════════════╣\n");
  printf("║ %-30s │ Rp %-17lld │ Rp %-17lld ║\n", "Jumlah Uang",
         getMinimumAmount(), getMaximumAmount());
  printf("║ %-30s │ Rp %-17lld │ Rp %-17lld ║\n", "Budget", getMinimumBudget(),
         getMaximumBudget());
  printf("║ %-30s │ Rp %-17lld │ Rp %-17lld ║\n", "Transaksi",
         globalConfig.money.minimumTransactionAmount,
         globalConfig.money.maximumTransactionAmount);
  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");

  printf("\n╔══════════════════════════════════════════════════════════════════"
         "════════════════╗\n");
  printf("║                             ⚙️  PENGATURAN VALIDASI                 "
         "           ║\n");
  printf("╠════════════════════════════════════════════════════════════════════"
         "══════════════╣\n");
  printf("║ %-40s │ %-40d ║\n", "Maksimal Panjang Nama",
         globalConfig.validation.maxNameLength);
  printf("║ %-40s │ %-40d ║\n", "Maksimal Panjang Deskripsi",
         globalConfig.validation.maxDescriptionLength);
  printf("║ %-40s │ %-40d ║\n", "Maksimal Percobaan Input",
         globalConfig.validation.maxValidationAttempts);
  printf("║ %-40s │ %-40d ║\n", "Tahun Minimum",
         globalConfig.validation.minYear);
  printf("║ %-40s │ %-40d ║\n", "Tahun Maksimum",
         globalConfig.validation.maxYear);
  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");

  waitForEnter();
}

void openResetConfigurationMenu() {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│            🔄 RESET PENGATURAN DEFAULT                  │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf(
      "⚠️  PERINGATAN: Ini akan mereset semua pengaturan ke nilai default!\n\n");

  char confirmation;
  printf("❓ Apakah Anda yakin? (y/n): ");
  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Reset pengaturan dibatalkan.");
    return;
  }

  initializeDefaultConfig();
  saveConfigToFile("./config.txt");

  showSuccessMessage("Semua pengaturan berhasil direset ke nilai default!");
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
