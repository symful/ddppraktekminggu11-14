#include "./month_report.h"
#include "../auth/auth.c"
#include "../db/include.c"
#include "../db/transaction_group.c"
#include "../types/include.c"
#include "../utils/date.c"
#include "../utils/date.h"
#include "../utils/validation.c"
#include "./master.h"
#include <string.h>

#ifndef UI_MONTH_REPORT_C
#define UI_MONTH_REPORT_C

void showMonthlyMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                📊 MENU BULANAN                          │\n");
  printf("├─────────────────────────────────────────────────────────┤\n");
  printf("│  1. 📋 Lihat Daftar Laporan Bulanan                     │\n");
  printf("│  2. ➕ Buat Laporan Bulanan Baru                        │\n");
  printf("│  3. ⬅️  Kembali ke Menu Utama                            │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
  printf("\n📈 Total laporan tersimpan: %d\n", monthReportList->count);
  printf("🎯 Pilihan Anda: ");
}

void showMonthlyList(struct MonthReportList *monthReportList) {
  printf("\n📅 DAFTAR LAPORAN BULANAN:\n");
  printf("──────────────────────────────────────────────────────────\n");

  if (monthReportList->count == 0) {
    printf("  📭 Belum ada laporan bulanan.\n");
    printf("  💡 Tip: Buat laporan baru untuk memulai!\n");
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    int totalTransactions = getTotalTransactions(report);
    printf("  %d. 📊 %s", i + 1, dateToMonthYearString(report->date));
    printf(" (%d transaksi)", totalTransactions);

    if (report->balance > 0) {
      printf(" 💚 Surplus: Rp %lld", report->balance);
    } else if (report->balance < 0) {
      printf(" ❤️  Defisit: Rp %lld", -report->balance);
    } else {
      printf(" ⚖️  Seimbang");
    }
    printf("\n");
  }
  printf("──────────────────────────────────────────────────────────\n");
}

void showTransactionGroupsList(struct MonthReport *monthReport) {
  printf("\n📂 KATEGORI TRANSAKSI:\n");
  printf("═════════════════════════════════════════════════════════════════════"
         "════════════════════════════\n");
  printf("║ No. │ %-15s │ %-12s │ %-12s │ %-12s │ %-8s │ %-12s ║\n", "KATEGORI",
         "BUDGET", "TERPAKAI", "SISA", "TRANSAKSI", "STATUS");
  printf("═════════════════════════════════════════════════════════════════════"
         "════════════════════════════\n");

  if (monthReport->groupCount == 0) {
    printf("║                                   📭 Belum ada transaksi        "
           "    "
           "            "
           "              ║\n");
  } else {
    for (int i = 0; i < monthReport->groupCount; i++) {
      struct TransactionGroup *group = monthReport->groups[i];
      const char *status = getGroupBudgetStatus(group);

      printf("║ %-3d │ %-15s │ %-12lld │ %-12lld │ %-12lld │ %-9d │ %-12s ║\n",
             i + 1, transactionCategoryToString(group->category), group->budget,
             group->totalAmount, group->budget - group->totalAmount,
             group->transactionCount, status);
    }
  }
  printf("═════════════════════════════════════════════════════════════════════"
         "════════════════════════════\n");
}

void openMonthReport(struct MonthReport *monthReport) {
  while (1) {
    showMonthReport(monthReport);

    int choice = getValidatedMenuChoice(1, 7);
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      openTransactionViewMenu(monthReport);
      break;
    case 2:
      openTransactionAddToReportMenu(monthReport);
      break;
    case 3:
      openTransactionEditMenu(monthReport);
      break;
    case 4:
      openTransactionDeleteMenu(monthReport);
      break;
    case 5:
      openCategoryBudgetMenu(monthReport);
      break;
    case 6:
      showMonthReportSummary2(monthReport);
      break;
    case 7:
      return;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}

void openMonthlyMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMonthlyMenu(monthReportList);

    int choice = getValidatedMenuChoice(1, 3);
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      openMonthlyListMenu(monthReportList);
      break;
    case 2:
      openTransactionAddMenu(monthReportList);
      break;
    case 3:
      return;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}

void openTransactionAddMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│              ➕ BUAT LAPORAN BULANAN BARU               │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  struct MonthReport *newReport = newMonthReport();
  if (newReport == NULL) {
    showErrorMessage("Gagal membuat laporan baru.");
    return;
  }

  char temp_input[100];
  time_t newDate;
  if (!readAndValidateString("📅 Masukkan bulan dan tahun (MM/YYYY): ",
                             temp_input, sizeof(temp_input))) {
    showErrorMessage("Input tidak valid.");
    freeMonthReport(newReport);
    return;
  }

  if (!validateMonthYearFormat(temp_input, &newDate)) {
    showErrorMessage("Format tanggal tidak valid. Gunakan format MM/YYYY.");
    freeMonthReport(newReport);
    return;
  }

  if (validateDuplicateMonthReport(monthReportList, newDate)) {
    showErrorMessage("Laporan untuk bulan ini sudah ada.");
    freeMonthReport(newReport);
    return;
  }

  newReport->date = newDate;
  addMonthReportToList(monthReportList, newReport);
  saveUserMonthReport(newReport);

  showSuccessMessage("Laporan bulanan berhasil dibuat! 🎉");
}

void openMonthlyListMenu(struct MonthReportList *monthReportList) {
  while (1) {
    clearScreen();
    showMonthlyList(monthReportList);

    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│             📋 KELOLA LAPORAN BULANAN                   │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│  1. 👁️  Lihat Detail Laporan                             │\n");
    printf("│  2. ➕ Buat Laporan Baru                                │\n");
    printf("│  3. ✏️  Edit Tanggal Laporan                             │\n");
    printf("│  4. 🗑️  Hapus Laporan                                    │\n");
    printf("│  5. ⬅️  Kembali                                          │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    printf("\n🎯 Pilihan Anda: ");

    int choice = getValidatedMenuChoice(1, 5);
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      openSelectReportMenu(monthReportList);
      break;
    case 2:
      openTransactionAddMenu(monthReportList);
      break;
    case 3:
      openMonthReportEditMenu(monthReportList);
      break;
    case 4:
      openMonthReportDeleteMenu(monthReportList);
      break;
    case 5:
      return;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}

void openSelectReportMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return;
  }

  showMonthlyList(monthReportList);

  int reportIndex;
  if (!readAndValidateInteger("\n📊 Pilih nomor laporan: ", 1,
                              monthReportList->count, &reportIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  openMonthReport(monthReportList->reports[reportIndex - 1]);
}

void openTransactionViewMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                 👁️  LIHAT TRANSAKSI                      │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showTransactionGroupsList(monthReport);

  if (monthReport->groupCount == 0) {
    showInfoMessage("Belum ada transaksi di laporan ini.");
    return;
  }

  int groupIndex;
  if (!readAndValidateInteger(
          "\n📂 Pilih nomor kategori untuk melihat detail: ", 1,
          monthReport->groupCount, &groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct TransactionGroup *group = monthReport->groups[groupIndex - 1];
  showTransactionDetails(group);
}

void showTransactionDetails(struct TransactionGroup *group) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");

  const char *categoryStr = transactionCategoryToString(group->category);
  int categoryLen = strlen(categoryStr);
  int totalWidth = 59;
  int prefixLen = 21;
  int remainingSpaces = totalWidth - prefixLen - categoryLen;

  printf("│           💳 DETAIL TRANSAKSI - %s", categoryStr);
  for (int i = 0; i < remainingSpaces; i++) {
    printf(" ");
  }
  printf("│\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  printf("\n💰 Budget: Rp %lld\n", group->budget);
  printf("💸 Terpakai: Rp %lld\n", group->totalAmount);
  printf("💵 Sisa: Rp %lld\n", group->budget - group->totalAmount);
  printf("📊 Status: %s\n", getGroupBudgetStatus(group));

  if (group->transactionCount == 0) {
    printf("\n📭 Belum ada transaksi di kategori ini.\n");
    waitForEnter();
    return;
  }

  printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━\n");
  printf("║ No. │ %-12s │ %-15s │ %-10s │ %-15s │ %-30s ║\n", "TANGGAL", "NAMA",
         "JENIS", "NOMINAL", "DESKRIPSI");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━\n");

  for (int i = 0; i < group->transactionCount; i++) {
    struct Transaction *t = group->transactions[i];
    const char *typeIcon =
        (t->type == TRANSACTION_INCOME) ? "💚 Masuk" : "❤️  Keluar";
    char *dateStr = dateToString(t->date);

    printf("║ %-3d │ %-12s │ %-15s │ %-15s │ Rp %-12lld │ %-30s ║\n", i + 1,
           dateStr, t->name, typeIcon, t->amount, t->description);

    free(dateStr);
  }
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━\n");

  waitForEnter();
}

void openTransactionEditMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                ✏️  EDIT TRANSAKSI                        │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showTransactionGroupsList(monthReport);

  if (monthReport->groupCount == 0) {
    showInfoMessage("Belum ada transaksi untuk diedit.");
    return;
  }

  int groupIndex;
  if (!readAndValidateInteger("\n📂 Pilih nomor kategori: ", 1,
                              monthReport->groupCount, &groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct TransactionGroup *group = monthReport->groups[groupIndex - 1];

  if (group->transactionCount == 0) {
    showInfoMessage("Tidak ada transaksi di kategori ini.");
    return;
  }

  showTransactionDetails(group);

  int transactionIndex;
  if (!readAndValidateInteger("\n💳 Pilih nomor transaksi yang ingin diedit: ",
                              1, group->totalAmount, &transactionIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct Transaction *transaction = group->transactions[transactionIndex - 1];

  printf("\n✏️  Edit Transaksi ID %d:\n", transaction->id);
  printf("─────────────────────────\n");

  char dateChoice;
  printf("📅 Edit tanggal? (y/n): ");
  scanf(" %c", &dateChoice);
  clearInputBuffer();

  if (dateChoice == 'y' || dateChoice == 'Y') {
    time_t newDate;
    if (readAndValidateDate("📅 Tanggal baru (DD/MM/YYYY): ", &newDate)) {
      transaction->date = newDate;
    }
  }

  char newName[20];
  if (readAndValidateString("💼 Nama baru (kosongkan jika tidak ingin ubah): ",
                            newName, sizeof(newName))) {
    strcpy(transaction->name, newName);
  }

  char newDescription[200];
  if (readAndValidateString(
          "📝 Deskripsi baru (kosongkan jika tidak ingin ubah): ",
          newDescription, sizeof(newDescription))) {
    strcpy(transaction->description, newDescription);
  }

  long long newAmount;
  printf("💰 Edit nominal? (y/n): ");
  char editChoice;
  scanf(" %c", &editChoice);
  clearInputBuffer();

  if (editChoice == 'y' || editChoice == 'Y') {
    InputResult result =
        promptForTransaction("💰 Nominal baru (Rp): ", &newAmount);
    if (result == INPUT_SUCCESS) {
      transaction->amount = newAmount;
    }
  }

  updateGroupCalculations(group);
  updateReportCalculations(monthReport);
  saveUserMonthReport(monthReport);

  showSuccessMessage("Transaksi berhasil diubah! 📝");
}

void openTransactionDeleteMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│               🗑️  HAPUS TRANSAKSI                       │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showTransactionGroupsList(monthReport);

  if (monthReport->groupCount == 0) {
    showInfoMessage("Belum ada transaksi untuk dihapus.");
    return;
  }

  int groupIndex;
  if (!readAndValidateInteger("\n📂 Pilih nomor kategori: ", 1,
                              monthReport->groupCount, &groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct TransactionGroup *group = monthReport->groups[groupIndex - 1];

  if (group->transactionCount == 0) {
    showInfoMessage("Tidak ada transaksi di kategori ini.");
    return;
  }

  showTransactionDetails(group);

  int transactionIndex;
  if (!readAndValidateInteger(
          "\n🗑️  Pilih nomor transaksi yang ingin dihapus: ", 1,
          group->transactionCount, &transactionIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  printf("\n⚠️  KONFIRMASI HAPUS\n");
  printf("───────────────────\n");
  char *dateStr = dateToString(group->transactions[transactionIndex - 1]->date);
  printf("Tanggal: %s\n", dateStr);
  printf("Transaksi: %s - Rp %lld\n",
         group->transactions[transactionIndex - 1]->name,
         group->transactions[transactionIndex - 1]->amount);
  free(dateStr);

  char confirmation;
  printf("❓ Yakin ingin menghapus? (y/n): ");
  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Penghapusan dibatalkan.");
    return;
  }

  removeUserMonthReportTransaction(monthReport, groupIndex - 1,
                                   transactionIndex - 1);
  saveUserMonthReport(monthReport);

  showSuccessMessage("Transaksi berhasil dihapus! 🗑️");
}

void openTransactionAddToReportMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│               ➕ TAMBAH TRANSAKSI                        │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  struct Transaction newTransaction;

  if (!readAndValidateString("💼 Nama transaksi: ", newTransaction.name,
                             sizeof(newTransaction.name))) {
    showErrorMessage("Nama transaksi tidak valid.");
    return;
  }

  if (!readAndValidateDate("📅 Tanggal transaksi (DD/MM/YYYY): ",
                           &newTransaction.date)) {
    showErrorMessage("Format tanggal tidak valid.");
    return;
  }

  if (!readAndValidateString("📝 Deskripsi: ", newTransaction.description,
                             sizeof(newTransaction.description))) {
    showErrorMessage("Deskripsi tidak valid.");
    return;
  }

  InputResult result =
      promptForTransaction("💰 Nominal (Rp): ", &newTransaction.amount);
  if (result != INPUT_SUCCESS) {
    showErrorMessage(
        "Nominal transaksi tidak valid atau di bawah minimum yang diizinkan.");
    return;
  }

  printf("\n💳 JENIS TRANSAKSI:\n");
  printf("──────────────────\n");
  printf("  1. 💚 Pemasukan\n");
  printf("  2. ❤️ Pengeluaran\n");

  int typeChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &typeChoice)) {
    showErrorMessage("Jenis transaksi tidak valid.");
    return;
  }
  newTransaction.type =
      (typeChoice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;

  printf("\n📂 KATEGORI TRANSAKSI:\n");
  printf("─────────────────────\n");
  for (int i = 0; i <= TC_OTHER; i++) {
    printf("  %d. %s\n", i + 1,
           transactionCategoryToString((enum TransactionCategory)i));
  }

  int categoryChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, TC_OTHER + 1,
                              &categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    return;
  }
  newTransaction.category = (enum TransactionCategory)(categoryChoice - 1);

  newTransaction.id = getTotalTransactions(monthReport) + 1;

  if (newTransaction.type == TRANSACTION_EXPENSE) {
    struct TransactionGroup *group =
        findGroupByCategory(monthReport, newTransaction.category);
    if (group != NULL && validateBudgetExceeded(group, newTransaction.amount)) {
      showWarningMessage("⚠️  Transaksi ini akan melebihi budget kategori!");

      char proceed;
      printf("❓ Lanjutkan? (y/n): ");
      scanf(" %c", &proceed);
      clearInputBuffer();

      if (proceed != 'y' && proceed != 'Y') {
        showInfoMessage("Transaksi dibatalkan.");
        return;
      }
    }
  }

  addUserMonthReportTransaction(monthReport, &newTransaction);
  saveUserMonthReport(monthReport);

  showSuccessMessage("Transaksi berhasil ditambahkan! 🎉");
}

void openCategoryBudgetMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│            🏷️  KELOLA BUDGET KATEGORI                   │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showTransactionGroupsList(monthReport);

  printf("\n📂 Pilih kategori untuk mengatur budget:\n");
  for (int i = 0; i <= TC_OTHER; i++) {
    printf("  %d. %s\n", i + 1,
           transactionCategoryToString((enum TransactionCategory)i));
  }

  int categoryChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, TC_OTHER + 1,
                              &categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    return;
  }

  enum TransactionCategory category =
      (enum TransactionCategory)(categoryChoice - 1);

  long long newBudget;
  if (!readAndValidateLongLong("💰 Budget baru (Rp): ", 1, 1000000000000LL,
                               &newBudget)) {
    showErrorMessage("Budget tidak valid.");
    return;
  }

  setBudgetForCategory(monthReport, category, newBudget);
  saveUserMonthReport(monthReport);

  char successMsg[200];
  snprintf(successMsg, sizeof(successMsg),
           "Budget kategori '%s' berhasil diatur menjadi Rp %lld",
           transactionCategoryToString(category), newBudget);
  showSuccessMessage(successMsg);
}

void openMonthReportEditMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  showMonthlyList(monthReportList);

  printf("\n┌─────────────────────────────────────────────────────────┐\n");
  printf("│              ✏️  EDIT TANGGAL LAPORAN                   │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  int reportIndex;
  if (!readAndValidateInteger("📅 Pilih nomor laporan: ", 1,
                              monthReportList->count, &reportIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct MonthReport *report = monthReportList->reports[reportIndex - 1];

  char newDateStr[20];
  if (!readAndValidateString("📅 Tanggal baru (MM/YYYY): ", newDateStr,
                             sizeof(newDateStr))) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  time_t newDate;
  if (!validateMonthYearFormat(newDateStr, &newDate)) {
    showErrorMessage("Format tanggal tidak valid.");
    return;
  }

  deleteUserMonthReport(report);
  report->date = newDate;
  saveUserMonthReport(report);

  showSuccessMessage("Tanggal laporan berhasil diubah! 📅");
}

void openMonthReportDeleteMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│               🗑️  HAPUS LAPORAN                         │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  showMonthlyList(monthReportList);

  int reportIndex;
  if (!readAndValidateInteger("\n🗑️  Pilih nomor laporan yang ingin dihapus: ",
                              1, monthReportList->count, &reportIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct MonthReport *reportToDelete =
      monthReportList->reports[reportIndex - 1];

  printf("\n⚠️  KONFIRMASI HAPUS\n");
  printf("───────────────────\n");
  printf("Laporan: %s\n", dateToMonthYearString(reportToDelete->date));
  printf("Total transaksi: %d\n", getTotalTransactions(reportToDelete));

  char confirmation;
  printf("❓ Yakin ingin menghapus? (y/n): ");
  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Penghapusan dibatalkan.");
    return;
  }

  deleteUserMonthReport(reportToDelete);
  freeMonthReport(reportToDelete);

  for (int i = reportIndex - 1; i < monthReportList->count - 1; i++) {
    monthReportList->reports[i] = monthReportList->reports[i + 1];
  }
  monthReportList->count--;

  showSuccessMessage("Laporan berhasil dihapus! 🗑️");
}

void showAllMonthReportSummary(struct MonthReportList *monthReportList) {
  clearScreen();
  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  printf("╔════════════════════════════════════════════════════════════════════"
         "══════════════╗\n");
  printf("║                           📊 RINGKASAN KEUANGAN KESELURUHAN        "
         "             ║\n");
  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");

  long long totalIncome = 0, totalExpense = 0;
  int totalTransactions = 0;
  long long categoryTotals[TC_OTHER + 1] = {0};
  long long categoryMaximums[TC_OTHER + 1] = {0};
  int categoryTransactions[TC_OTHER + 1] = {0};

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    totalIncome += report->totalIncome;
    totalExpense += report->totalExpenses;
    totalTransactions += getTotalTransactions(report);

    for (int j = 0; j < report->groupCount; j++) {
      struct TransactionGroup *group = report->groups[j];
      categoryTotals[group->category] += group->totalAmount;
      categoryMaximums[group->category] += group->budget;
      categoryTransactions[group->category] += group->transactionCount;
    }
  }

  long long finalBalance = totalIncome - totalExpense;

  printf("\n💰 RINGKASAN FINANSIAL:\n");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");
  printf("💚 Total Pemasukan: Rp %lld\n", totalIncome);
  printf("❤️  Total Pengeluaran: Rp %lld\n", totalExpense);
  printf("💵 Saldo Akhir: Rp %lld\n", finalBalance);
  printf("📊 Total Transaksi: %d\n", totalTransactions);

  if (finalBalance > 0) {
    printf("✅ Status: Surplus (Kondisi Baik)\n");
  } else if (finalBalance < 0) {
    printf("⚠️  Status: Defisit (Perlu Perhatian)\n");
  } else {
    printf("⚖️  Status: Seimbang\n");
  }

  printf("\n📂 BUDGET PER KATEGORI:\n");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");
  printf("║ %-20s │ %-15s │ %-15s │ %-15s │ %-10s │ %-10s ║\n", "KATEGORI",
         "BUDGET", "TERPAKAI", "SISA", "TRANSAKSI", "STATUS");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");

  for (int category = 0; category <= TC_OTHER; category++) {
    if (categoryTransactions[category] > 0 || categoryMaximums[category] > 0) {
      long long remaining =
          categoryMaximums[category] - categoryTotals[category];
      const char *status = remaining >= 0 ? "✅ Aman" : "⚠️ Melebihi";

      printf("║ %-20s │ %-15lld │ %-15lld │ %-15lld │ %-10d │ %-10s ║\n",
             transactionCategoryToString((enum TransactionCategory)category),
             categoryMaximums[category], categoryTotals[category], remaining,
             categoryTransactions[category], status);
    }
  }
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");

  waitForEnter();
}

void showMonthReportSummary2(struct MonthReport *monthReport) {
  clearScreen();
  printf("╔════════════════════════════════════════════════════════════════════"
         "══════════════╗\n");

  char *monthStr = dateToMonthYearString(monthReport->date);
  int monthStrLen = strlen(monthStr);
  int totalWidth = 88;
  int prefixLen = 21;
  int remainingSpaces = totalWidth - prefixLen - monthStrLen;

  printf("║                    📊 RINGKASAN LAPORAN %s", monthStr);
  for (int i = 0; i < remainingSpaces; i++) {
    printf(" ");
  }
  printf("║\n");

  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");

  printf("\n💰 RINGKASAN FINANSIAL:\n");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");
  printf("💚 Total Pemasukan: Rp %lld\n", monthReport->totalIncome);
  printf("❤️  Total Pengeluaran: Rp %lld\n", monthReport->totalExpenses);
  printf("💵 Saldo: Rp %lld\n", monthReport->balance);
  printf("📊 Total Transaksi: %d\n", getTotalTransactions(monthReport));

  showTransactionGroupsList(monthReport);

  const char *financialCondition;
  const char *financialConclusion;

  if (monthReport->balance < 0) {
    double deficitPercentage =
        ((double)(-monthReport->balance) / monthReport->totalIncome) * 100;
    if (deficitPercentage > 10) {
      financialCondition = "⚠️  Defisit besar (Pengeluaran >> Pemasukan)";
      financialConclusion = "🚨 Kondisi keuangan kurang sehat. Kurangi "
                            "pengeluaran dan cari tambahan pemasukan.";
    } else {
      financialCondition = "⚠️  Defisit ringan (Pengeluaran > Pemasukan)";
      financialConclusion =
          "💡 Anda mulai boros, perhatikan pengeluaran harian.";
    }
  } else if (monthReport->balance == 0) {
    financialCondition = "⚖️  Seimbang (Tanpa sisa uang)";
    financialConclusion = "✅ Keuangan Anda seimbang, tetap waspada terhadap "
                          "pengeluaran tidak perlu.";
  } else {
    double surplusPercentage =
        ((double)monthReport->balance / monthReport->totalIncome) * 100;
    if (surplusPercentage > 25) {
      financialCondition = "💚 Surplus besar (Memiliki sisa uang)";
      financialConclusion = "🎉 Anda termasuk mahasiswa hemat dan produktif.";
    } else {
      financialCondition = "💚 Surplus kecil (Memiliki sisa uang)";
      financialConclusion = "✅ Keuangan Anda seimbang, tetap waspada terhadap "
                            "pengeluaran tidak perlu.";
    }
  }

  printf("\n🏥 KONDISI KEUANGAN:\n");
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");
  printf("📊 Status: %s\n", financialCondition);
  printf("💡 Kesimpulan: %s\n", financialConclusion);
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
         "━━━━━━━━━━━━━━━━━━\n");

  waitForEnter();
}

void showMonthReport(struct MonthReport *monthReport) {
  clearScreen();
  printf("╔════════════════════════════════════════════════════════════════════"
         "══════════════╗\n");
  char *monthStr = dateToMonthYearString(monthReport->date);
  int monthStrLen = strlen(monthStr);
  int totalWidth = 83;
  int textLen = 10 + monthStrLen;
  int totalSpaces = totalWidth - 2 - textLen;
  int leftSpaces = totalSpaces / 2;
  int rightSpaces = totalSpaces - leftSpaces;

  printf("║");
  for (int i = 0; i < leftSpaces; i++) {
    printf(" ");
  }
  printf("📊 LAPORAN %s", monthStr);
  for (int i = 0; i < rightSpaces; i++) {
    printf(" ");
  }
  printf("║\n");

  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");

  printf("💰 Saldo: Rp %lld | 📊 Transaksi: %d | 💚 Masuk: Rp %lld | ❤️  "
         "Keluar: Rp %lld\n",
         monthReport->balance, getTotalTransactions(monthReport),
         monthReport->totalIncome, monthReport->totalExpenses);

  showTransactionGroupsList(monthReport);

  printf("\n┌─────────────────────────────────────────────────────────┐\n");
  printf("│                    🛠️  MENU AKSI                         │\n");
  printf("├─────────────────────────────────────────────────────────┤\n");
  printf("│  1. 👁️  Lihat Detail Transaksi                           │\n");
  printf("│  2. ➕ Tambah Transaksi Baru                            │\n");
  printf("│  3. ✏️  Edit Transaksi                                   │\n");
  printf("│  4. 🗑️  Hapus Transaksi                                  │\n");
  printf("│  5. 🏷️  Kelola Budget Kategori                           │\n");
  printf("│  6. 📈 Lihat Ringkasan Detail                           │\n");
  printf("│  7. ⬅️  Kembali                                          │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
  printf("\n🎯 Pilihan Anda: ");
}

char temp_input[100];

#endif
