#include "./month_report.h"
#include "../auth/auth.c"
#include "../db/include.c"
#include "../db/transaction_group.c"
#include "../types/include.c"
#include "../utils/date.c"
#include "../utils/date.h"
#include "../utils/validation.c"
#include "./colors.c"
#include "./master.h"
#include "./screen.c"
#include <string.h>

#ifndef UI_MONTH_REPORT_C
#define UI_MONTH_REPORT_C

void showMonthlyMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 11;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                📊 MENU BULANAN                          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 📋 Lihat Daftar Laporan Bulanan                     │\n");
  printCentered("│  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" ➕ Buat Laporan Bulanan Baru                        │\n");
  printCentered("│  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali ke Menu Utama                            │\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📈 Total laporan tersimpan: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, monthReportList->count, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

void showMonthlyList(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  printf("\n");
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("              📅 DAFTAR LAPORAN BULANAN                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");

  if (monthReportList->count == 0) {
    printCentered("", boxWidth);
    printf("📭 Belum ada laporan bulanan.\n");
    printCentered("", boxWidth);
    printInfo("💡 Tip: Buat laporan baru untuk memulai!\n");
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    int totalTransactions = getTotalTransactions(report);
    printCentered("  ", boxWidth);
    printColored(i + 1 < 10 ? " " : "", COLOR_BRIGHT_CYAN);
    printf("%s%d.%s", COLOR_BRIGHT_CYAN, i + 1, COLOR_RESET);
    printf(" 📊 %s", dateToMonthYearString(report->date));
    printf(" (%d transaksi)", totalTransactions);

    if (report->balance > 0) {
      printf(" ");
      printf("%s💚 Surplus:%s", COLOR_BRIGHT_GREEN, COLOR_RESET);
      printf(" %sRp %lld%s", COLOR_BRIGHT_GREEN, report->balance, COLOR_RESET);
    } else if (report->balance < 0) {
      printf(" ");
      printf("%s❤️  Defisit:%s", COLOR_BRIGHT_RED, COLOR_RESET);
      printf(" %sRp %lld%s", COLOR_BRIGHT_RED, -report->balance, COLOR_RESET);
    } else {
      printf(" ");
      printf("%s⚖️  Seimbang%s", COLOR_BRIGHT_YELLOW, COLOR_RESET);
    }
    printf("\n");
  }
  printf("\n");
  printCenteredColored(
      "──────────────────────────────────────────────────────────\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void showTransactionGroupsList(struct MonthReport *monthReport) {
  int boxWidth = 100;
  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                                    📂 KATEGORI TRANSAKSI       "
              "                               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║ ", boxWidth);
  printColored("No.", COLOR_BRIGHT_CYAN);
  printf(" │ ");
  printf("%s%-15s%s", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-12s%s", COLOR_BRIGHT_CYAN, "BUDGET", COLOR_RESET);
  printf(" │ ");
  printf("%s%-12s%s", COLOR_BRIGHT_CYAN, "TERPAKAI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-12s%s", COLOR_BRIGHT_CYAN, "SISA", COLOR_RESET);
  printf(" │ ");
  printf("%s%-8s%s", COLOR_BRIGHT_CYAN, "TRANSAKSI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-12s%s", COLOR_BRIGHT_CYAN, "STATUS", COLOR_RESET);
  printf(" ║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  if (monthReport->groupCount == 0) {
    printCentered("║", boxWidth);
    printColored("                           📭 Belum ada transaksi            "
                 "                       ",
                 COLOR_BRIGHT_YELLOW);
    printf("║\n");
  } else {
    for (int i = 0; i < monthReport->groupCount; i++) {
      struct TransactionGroup *group = monthReport->groups[i];
      const char *status = getGroupBudgetStatus(group);
      long long remaining = group->budget - group->totalAmount;
      double percentage =
          group->budget > 0
              ? ((double)group->totalAmount / group->budget) * 100.0
              : 0.0;

      struct CategoryList *categories = getUserCategoriesCache();
      const char *displayName =
          getCategoryDisplayName(categories, group->category);

      printCentered("", boxWidth);
      printf("║ %-3d │ %-15s │ ", i + 1, displayName);
      printf("%s%-12lld%s", COLOR_BRIGHT_CYAN, group->budget, COLOR_RESET);
      printf(" │ ");
      printf("%s%-12lld%s", COLOR_BRIGHT_YELLOW, group->totalAmount,
             COLOR_RESET);
      printf(" │ ");
      printAmount(remaining);
      printf("%-12s │ %-9d │ ", "", group->transactionCount);
      printBudgetStatus(status, percentage);
      printf("%-12s ║\n", "");
    }
  }
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
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
  int boxWidth = 59;
  int contentLines = 10;
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("              ➕ BUAT LAPORAN BULANAN BARU               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  struct MonthReport *newReport = newMonthReport();
  if (newReport == NULL) {
    showErrorMessage("Gagal membuat laporan baru.");
    return;
  }

  char temp_input[100];
  time_t newDate;
  printf("\n");
  printCentered("", boxWidth);
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
  int boxWidth = 59;
  while (1) {
    clearScreen();
    showMonthlyList(monthReportList);

    printf("\n");
    printCenteredColored(
        "┌─────────────────────────────────────────────────────────┐\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│", boxWidth);
    printWithBg("             📋 KELOLA LAPORAN BULANAN                   ",
                COLOR_BRIGHT_WHITE, BG_BLUE);
    printf("│\n");
    printCenteredColored(
        "├─────────────────────────────────────────────────────────┤\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printCentered("│  ", boxWidth);
    printColored("1.", COLOR_BRIGHT_CYAN);
    printf(" 👁️  Lihat Detail Laporan                             │\n");
    printCentered("│  ", boxWidth);
    printColored("2.", COLOR_BRIGHT_CYAN);
    printf(" ➕ Buat Laporan Baru                                │\n");
    printCentered("│  ", boxWidth);
    printColored("3.", COLOR_BRIGHT_CYAN);
    printf(" ✏️  Edit Tanggal Laporan                             │\n");
    printCentered("│  ", boxWidth);
    printColored("4.", COLOR_BRIGHT_CYAN);
    printf(" 🗑️  Hapus Laporan                                    │\n");
    printCentered("│  ", boxWidth);
    printColored("5.", COLOR_BRIGHT_CYAN);
    printf(" ⬅️  Kembali                                          │\n");
    printCenteredColored(
        "└─────────────────────────────────────────────────────────┘\n",
        boxWidth, COLOR_BRIGHT_CYAN);
    printf("\n");
    printCentered("", boxWidth);
    printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);

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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                 👁️  LIHAT TRANSAKSI                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  struct CategoryList *categories = getUserCategoriesCache();
  const char *categoryStr = getCategoryDisplayName(categories, group->category);

  printCentered("│", boxWidth);
  printWithBg("           💳 DETAIL TRANSAKSI - ", COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("%s", categoryStr);
  int remainingSpaces = 27 - strlen(categoryStr);
  for (int i = 0; i < remainingSpaces; i++) {
    printf(" ");
  }
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printInfo("💰 Budget: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, group->budget, COLOR_RESET);
  printCentered("", boxWidth);
  printWarning("💸 Terpakai: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_YELLOW, group->totalAmount, COLOR_RESET);
  printCentered("", boxWidth);
  printf("💵 Sisa: ");
  printAmount(group->budget - group->totalAmount);
  printf("\n");
  printCentered("", boxWidth);
  printf("📊 Status: ");
  double percentage = group->budget > 0
                          ? ((double)group->totalAmount / group->budget) * 100.0
                          : 0.0;
  printBudgetStatus(getGroupBudgetStatus(group), percentage);
  printf("\n");

  if (group->transactionCount == 0) {
    printf("\n");
    printCentered("", boxWidth);
    printf("📭 Belum ada transaksi di kategori ini.\n");
    waitForEnter();
    return;
  }

  int tableWidth = 100;
  printf("\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);
  printCentered("", tableWidth);
  printf("║ %sNo.%s │ %s%-12s%s │ %s%-15s%s │ %s%-10s%s │ %s%-15s%s │ "
         "%s%-30s%s ║\n",
         COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_BRIGHT_CYAN, "TANGGAL",
         COLOR_RESET, COLOR_BRIGHT_CYAN, "NAMA", COLOR_RESET, COLOR_BRIGHT_CYAN,
         "JENIS", COLOR_RESET, COLOR_BRIGHT_CYAN, "NOMINAL", COLOR_RESET,
         COLOR_BRIGHT_CYAN, "DESKRIPSI", COLOR_RESET);
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);

  for (int i = 0; i < group->transactionCount; i++) {
    struct Transaction *t = group->transactions[i];
    const char *typeIcon =
        (t->type == TRANSACTION_INCOME) ? "💚 Masuk" : "❤️  Keluar";
    char *dateStr = dateToString(t->date);

    printCentered("", tableWidth);
    printf("║ %-3d │ %-12s │ %-15s │ %-15s │ Rp %-12lld │ %-30s ║\n", i + 1,
           dateStr, t->name, typeIcon, t->amount, t->description);

    free(dateStr);
  }
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);

  waitForEnter();
}

void openTransactionEditMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                ✏️  EDIT TRANSAKSI                        ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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

  printf("\n");
  printCentered("", boxWidth);
  printColored("✏️  Edit Transaksi ID ", COLOR_BRIGHT_CYAN);
  printf("%d:\n", transaction->id);
  printCenteredColored("─────────────────────────\n", boxWidth,
                       COLOR_BRIGHT_CYAN);
  printf("\n");
  char dateChoice;
  printCentered("", boxWidth);
  printColored("📅 Edit tanggal? (y/n): ", COLOR_BRIGHT_YELLOW);
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
  printCentered("", boxWidth);
  printColored("💰 Edit nominal? (y/n): ", COLOR_BRIGHT_YELLOW);
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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("               🗑️  HAPUS TRANSAKSI                       ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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

  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  KONFIRMASI HAPUS\n");
  printCenteredColored("───────────────────\n", boxWidth, COLOR_BRIGHT_CYAN);
  char *dateStr = dateToString(group->transactions[transactionIndex - 1]->date);
  printCentered("", boxWidth);
  printf("Tanggal: %s\n", dateStr);
  printCentered("", boxWidth);
  printf("Transaksi: %s - Rp %lld\n",
         group->transactions[transactionIndex - 1]->name,
         group->transactions[transactionIndex - 1]->amount);
  free(dateStr);

  char confirmation;
  printf("\n");
  printCentered("", boxWidth);
  printColored("❓ Yakin ingin menghapus? (y/n): ", COLOR_BRIGHT_YELLOW);
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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("               ➕ TAMBAH TRANSAKSI                        ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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

  printf("\n");
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                  💳 JENIS TRANSAKSI                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 💚 Pemasukan\n");
  printCentered("  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" ❤️ Pengeluaran\n");
  printf("\n");

  int typeChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &typeChoice)) {
    showErrorMessage("Jenis transaksi tidak valid.");
    return;
  }
  newTransaction.type =
      (typeChoice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;

  struct CategoryList *allCategories = getUserCategoriesCache();
  struct CategoryList *filteredCategories =
      getCategoriesByType(allCategories, newTransaction.type);

  if (filteredCategories == NULL || filteredCategories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia untuk tipe transaksi ini.");
    if (filteredCategories != NULL) {
      freeCategoryList(filteredCategories);
    }
    return;
  }

  printf("\n");
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                📂 KATEGORI TRANSAKSI                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  for (int i = 0; i < filteredCategories->count; i++) {
    printCentered("  ", boxWidth);
    printf("%s%d.%s %s\n", COLOR_BRIGHT_CYAN, i + 1, COLOR_RESET,
           filteredCategories->items[i]->displayName);
  }
  printf("\n");

  int categoryChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, filteredCategories->count,
                              &categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    freeCategoryList(filteredCategories);
    return;
  }

  strncpy(newTransaction.category,
          filteredCategories->items[categoryChoice - 1]->internalName,
          sizeof(newTransaction.category) - 1);
  newTransaction.category[sizeof(newTransaction.category) - 1] = '\0';

  freeCategoryList(filteredCategories);

  newTransaction.id = getTotalTransactions(monthReport) + 1;

  if (newTransaction.type == TRANSACTION_EXPENSE) {
    struct TransactionGroup *group =
        findGroupByCategory(monthReport, newTransaction.category);
    if (group != NULL && validateBudgetExceeded(group, newTransaction.amount)) {
      showWarningMessage("⚠️  Transaksi ini akan melebihi budget kategori!");

      char proceed;
      printCentered("", boxWidth);
      printColored("❓ Lanjutkan? (y/n): ", COLOR_BRIGHT_YELLOW);
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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("            🏷️  KELOLA BUDGET KATEGORI                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  showTransactionGroupsList(monthReport);

  struct CategoryList *categories = getUserCategoriesCache();

  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    return;
  }

  printf("\n📂 Pilih kategori untuk mengatur budget:\n");
  for (int i = 0; i < categories->count; i++) {
    printf("  %d. %s\n", i + 1, categories->items[i]->displayName);
  }

  int categoryChoice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, categories->count,
                              &categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    return;
  }

  char category[50];
  strncpy(category, categories->items[categoryChoice - 1]->internalName,
          sizeof(category) - 1);
  category[sizeof(category) - 1] = '\0';

  long long newBudget;
  if (!readAndValidateLongLong("💰 Budget baru (Rp): ", 1, 1000000000000LL,
                               &newBudget)) {
    showErrorMessage("Budget tidak valid.");
    return;
  }

  setBudgetForCategory(monthReport, category, newBudget);
  saveUserMonthReport(monthReport);

  char successMsg[200];
  const char *displayName = getCategoryDisplayName(categories, category);
  snprintf(successMsg, sizeof(successMsg),
           "Budget kategori '%s' berhasil diatur menjadi Rp %lld", displayName,
           newBudget);
  showSuccessMessage(successMsg);
}

void openMonthReportEditMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  clearScreen();
  showMonthlyList(monthReportList);

  printf("\n");
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("              ✏️  EDIT TANGGAL LAPORAN                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("               🗑️  HAPUS LAPORAN                         ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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

  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  KONFIRMASI HAPUS\n");
  printCenteredColored("───────────────────\n", boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("", boxWidth);
  printf("Laporan: %s\n", dateToMonthYearString(reportToDelete->date));
  printCentered("", boxWidth);
  printf("Total transaksi: %d\n", getTotalTransactions(reportToDelete));

  char confirmation;
  printf("\n");
  printCentered("", boxWidth);
  printColored("❓ Yakin ingin menghapus? (y/n): ", COLOR_BRIGHT_YELLOW);
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
  int boxWidth = 88;
  clearScreen();
  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  printCenteredColored(
      "╔════════════════════════════════════════════════════════════════════"
      "══════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg(
      "                           📊 RINGKASAN KEUANGAN KESELURUHAN        "
      "             ",
      COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  long long totalIncome = 0, totalExpense = 0;
  int totalTransactions = 0;

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    return;
  }

  long long *categoryTotals =
      (long long *)calloc(categories->count, sizeof(long long));
  long long *categoryMaximums =
      (long long *)calloc(categories->count, sizeof(long long));
  int *categoryTransactions = (int *)calloc(categories->count, sizeof(int));

  if (!categoryTotals || !categoryMaximums || !categoryTransactions) {
    free(categoryTotals);
    free(categoryMaximums);
    free(categoryTransactions);
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    totalIncome += report->totalIncome;
    totalExpense += report->totalExpenses;
    totalTransactions += getTotalTransactions(report);

    for (int j = 0; j < report->groupCount; j++) {
      struct TransactionGroup *group = report->groups[j];

      for (int k = 0; k < categories->count; k++) {
        if (strcmp(categories->items[k]->internalName, group->category) == 0) {
          categoryTotals[k] += group->totalAmount;
          categoryMaximums[k] += group->budget;
          categoryTransactions[k] += group->transactionCount;
          break;
        }
      }
    }
  }

  long long finalBalance = totalIncome - totalExpense;

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 RINGKASAN FINANSIAL:\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("", boxWidth);
  printSuccess("💚 Total Pemasukan: ");
  printf("%s", COLOR_BRIGHT_GREEN);
  printf("Rp %lld", totalIncome);
  printf("%s\n", COLOR_RESET);
  printCentered("", boxWidth);
  printError("❤️  Total Pengeluaran: ");
  printf("%s", COLOR_BRIGHT_RED);
  printf("Rp %lld", totalExpense);
  printf("%s\n", COLOR_RESET);
  printCentered("", boxWidth);
  printf("💵 Saldo Akhir: ");
  printAmount(finalBalance);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total Transaksi: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, totalTransactions, COLOR_RESET);

  printCentered("", boxWidth);
  if (finalBalance > 0) {
    printSuccess("✅ Status: Surplus (Kondisi Baik)\n");
  } else if (finalBalance < 0) {
    printError("⚠️  Status: Defisit (Perlu Perhatian)\n");
  } else {
    printWarning("⚖️  Status: Seimbang\n");
  }

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("📂 BUDGET PER KATEGORI:\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║ ", boxWidth);
  printf("%s%-20s%s", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-15s%s", COLOR_BRIGHT_CYAN, "BUDGET", COLOR_RESET);
  printf(" │ ");
  printf("%s%-15s%s", COLOR_BRIGHT_CYAN, "TERPAKAI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-15s%s", COLOR_BRIGHT_CYAN, "SISA", COLOR_RESET);
  printf(" │ ");
  printf("%s%-10s%s", COLOR_BRIGHT_CYAN, "TRANSAKSI", COLOR_RESET);
  printf(" │ ");
  printf("%s%-10s%s", COLOR_BRIGHT_CYAN, "STATUS", COLOR_RESET);
  printf(" ║\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  for (int category = 0; category < categories->count; category++) {
    if (categoryTransactions[category] > 0 || categoryMaximums[category] > 0) {
      long long remaining =
          categoryMaximums[category] - categoryTotals[category];
      const char *status = remaining >= 0 ? "✅ Aman" : "⚠️ Melebihi";
      double percentage = categoryMaximums[category] > 0
                              ? ((double)categoryTotals[category] /
                                 categoryMaximums[category]) *
                                    100.0
                              : 0.0;

      const char *displayName = getCategoryDisplayName(
          categories, categories->items[category]->internalName);
      printCentered("║ ", boxWidth);
      printf("%-20s │ ", displayName);
      printf("%s%-15lld%s", COLOR_BRIGHT_CYAN, categoryMaximums[category],
             COLOR_RESET);
      printf(" │ ");
      printf("%s%-15lld%s", COLOR_BRIGHT_YELLOW, categoryTotals[category],
             COLOR_RESET);
      printf(" │ ");
      printAmount(remaining);
      printf("%-15s │ %-10d │ ", "", categoryTransactions[category]);
      printBudgetStatus(status, percentage);
      printf("%-10s ║\n", "");
    }
  }
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  free(categoryTotals);
  free(categoryMaximums);
  free(categoryTransactions);

  waitForEnter();
}

void showMonthReportSummary2(struct MonthReport *monthReport) {
  int boxWidth = 88;
  clearScreen();
  printCenteredColored(
      "╔════════════════════════════════════════════════════════════════════"
      "══════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char *monthStr = dateToMonthYearString(monthReport->date);
  int monthStrLen = strlen(monthStr);
  int totalWidth = 88;
  int prefixLen = 21;
  int remainingSpaces = totalWidth - prefixLen - monthStrLen;

  printCentered("║", boxWidth);
  printWithBg("                    📊 RINGKASAN LAPORAN ", COLOR_BRIGHT_WHITE,
              BG_BLUE);
  printf("%s", monthStr);
  for (int i = 0; i < remainingSpaces; i++) {
    printf(" ");
  }
  printf("║\n");

  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 RINGKASAN FINANSIAL:\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("", boxWidth);
  printSuccess("💚 Total Pemasukan: ");
  printf("%s", COLOR_BRIGHT_GREEN);
  printf("Rp %lld", monthReport->totalIncome);
  printf("%s\n", COLOR_RESET);
  printCentered("", boxWidth);
  printError("❤️  Total Pengeluaran: ");
  printf("%s", COLOR_BRIGHT_RED);
  printf("Rp %lld", monthReport->totalExpenses);
  printf("%s\n", COLOR_RESET);
  printCentered("", boxWidth);
  printf("💵 Saldo: ");
  printAmount(monthReport->balance);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total Transaksi: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, getTotalTransactions(monthReport),
         COLOR_RESET);

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

  printf("\n");
  printCentered("", boxWidth);
  printInfo("🏥 KONDISI KEUANGAN:\n");
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("", boxWidth);
  if (monthReport->balance < 0) {
    printError("📊 Status: ");
    printf("%s\n", financialCondition);
  } else if (monthReport->balance == 0) {
    printWarning("📊 Status: ");
    printf("%s\n", financialCondition);
  } else {
    printSuccess("📊 Status: ");
    printf("%s\n", financialCondition);
  }
  printCentered("", boxWidth);
  printInfo("💡 Kesimpulan: ");
  printf("%s\n", financialConclusion);
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  waitForEnter();
}

void showMonthReport(struct MonthReport *monthReport) {
  int boxWidth = 88;
  clearScreen();
  printCenteredColored(
      "╔════════════════════════════════════════════════════════════════════"
      "══════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char *monthStr = dateToMonthYearString(monthReport->date);
  printCentered("║", boxWidth);
  printWithBg("                           📊 LAPORAN ", COLOR_BRIGHT_WHITE,
              BG_BLUE);
  printf("%s", monthStr);
  int remainingSpaces = 45 - strlen(monthStr);
  for (int i = 0; i < remainingSpaces; i++) {
    printf(" ");
  }
  printf("║\n");

  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 RINGKASAN:\n");
  printCenteredColored(
      "─────────────────────────────────────────────────────────────────\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("", boxWidth);
  printSuccess("💚 Total Pemasukan: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_GREEN, monthReport->totalIncome,
         COLOR_RESET);
  printCentered("", boxWidth);
  printError("❤️  Total Pengeluaran: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_RED, monthReport->totalExpenses,
         COLOR_RESET);
  printCentered("", boxWidth);
  printf("💵 Saldo: ");
  printAmount(monthReport->balance);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total Transaksi: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, getTotalTransactions(monthReport),
         COLOR_RESET);

  showTransactionGroupsList(monthReport);

  printf("\n");
  int menuBoxWidth = 59;
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n",
      menuBoxWidth, COLOR_BRIGHT_CYAN);
  printCentered("│", menuBoxWidth);
  printWithBg("                   ⚙️  MENU LAPORAN                       ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n",
      menuBoxWidth, COLOR_BRIGHT_CYAN);
  printCentered("│  ", menuBoxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 👁️  Lihat Detail Transaksi                           │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" ➕ Tambah Transaksi Baru                            │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" ✏️  Edit Transaksi                                   │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" 🗑️  Hapus Transaksi                                  │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" 💰 Atur Budget Kategori                             │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 📊 Ringkasan Detail                                 │\n");
  printCentered("│  ", menuBoxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali                                          │\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n",
      menuBoxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", menuBoxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

char temp_input[100];

#endif
