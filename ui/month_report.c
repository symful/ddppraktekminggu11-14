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

/* ---------------------------------------------------------
   PROSEDUR : renderMonthlyMenuHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu bulanan dengan bingkai dan judul
   I.S.       : Terminal bersih
   F.S.       : Header menu bulanan ditampilkan
   INPUT      : boxWidth = lebar kotak tampilan
                contentLines = jumlah baris vertikal yang ingin di-clear sebelum
render
---------------------------------------------------------*/
void renderMonthlyMenuHeader(int boxWidth, int contentLines) {
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
}

/* ---------------------------------------------------------
   PROSEDUR : renderMonthlyListHeader(boxWidth)
   DESKRIPSI : Menampilkan header daftar laporan bulanan
   I.S.       : Terminal bebas
   F.S.       : Header daftar laporan bulanan ditampilkan
   INPUT      : boxWidth = lebar kotak tampilan
---------------------------------------------------------*/
void renderMonthlyListHeader(int boxWidth) {
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
}

/* ---------------------------------------------------------
   PROSEDUR : renderMonthlyMenuOptions(boxWidth)
   DESKRIPSI : Menampilkan opsi menu bulanan (lihat daftar, buat baru, kembali)
   I.S.       : Terminal sudah menampilkan header menu
   F.S.       : Opsi menu ditampilkan
   INPUT      : boxWidth = lebar kotak tampilan
---------------------------------------------------------*/
void renderMonthlyMenuOptions(int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 📋 Lihat Daftar Laporan Bulanan                     │\n");

  printCentered("│  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" ➕ Buat Laporan Bulanan Baru                        │\n");

  printCentered("│  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali ke Menu Utama                            │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : renderMonthlyMenuFooter(boxWidth, totalReports)
   DESKRIPSI : Menampilkan footer menu bulanan beserta jumlah laporan tersimpan
   I.S.       : Menu bulanan dan opsi sudah ditampilkan
   F.S.       : Footer dan prompt pilihan ditampilkan
   INPUT      : boxWidth = lebar kotak tampilan
                totalReports = jumlah laporan bulanan yang tersimpan
---------------------------------------------------------*/
void renderMonthlyMenuFooter(int boxWidth, int totalReports) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printInfo("📈 Total laporan tersimpan: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, totalReports, COLOR_RESET);

  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

/* ---------------------------------------------------------
   PROSEDUR : renderEmptyMonthlyList(boxWidth)
   DESKRIPSI : Menampilkan pesan ketika belum ada laporan bulanan
   I.S.       : Terminal bebas
   F.S.       : Pesan empty list ditampilkan
   INPUT      : boxWidth = lebar kotak tampilan
---------------------------------------------------------*/
void renderEmptyMonthlyList(int boxWidth) {
  printCentered("", boxWidth);
  printf("📭 Belum ada laporan bulanan.\n");

  printCentered("", boxWidth);
  printInfo("💡 Tip: Buat laporan baru untuk memulai!\n");
}

/* ---------------------------------------------------------
   PROSEDUR : renderSingleMonthlyListItem(index, report, boxWidth)
   DESKRIPSI : Menampilkan satu item laporan bulanan dengan jumlah transaksi dan
status saldo I.S.       : Terminal sudah menampilkan header daftar F.S.       :
Satu item laporan bulanan ditampilkan INPUT      : index = nomor urut laporan
(0-based) report = pointer ke laporan bulanan boxWidth = lebar kotak tampilan
---------------------------------------------------------*/
void renderSingleMonthlyListItem(int index, struct MonthReport *report,
                                 int boxWidth) {
  int totalTransactions = getTotalTransactions(report);

  printCentered("  ", boxWidth);
  printColored(index + 1 < 10 ? " " : "", COLOR_BRIGHT_CYAN);
  printf("%s%d.%s", COLOR_BRIGHT_CYAN, index + 1, COLOR_RESET);

  printf(" 📊 %s", dateToMonthYearString(report->date));
  printf(" (%d transaksi)", totalTransactions);

  if (report->balance > 0) {
    printf(" %s💚 Surplus:%s %sRp %lld%s", COLOR_BRIGHT_GREEN, COLOR_RESET,
           COLOR_BRIGHT_GREEN, report->balance, COLOR_RESET);
  } else if (report->balance < 0) {
    printf(" %s❤️  Defisit:%s %sRp %lld%s", COLOR_BRIGHT_RED, COLOR_RESET,
           COLOR_BRIGHT_RED, -report->balance, COLOR_RESET);
  } else {
    printf(" %s⚖️  Seimbang%s", COLOR_BRIGHT_YELLOW, COLOR_RESET);
  }

  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : showMonthlyMenu(monthReportList)
   DESKRIPSI : Menampilkan menu utama bulanan beserta jumlah laporan tersimpan
   I.S.       : Terminal bebas
   F.S.       : Menu bulanan ditampilkan
   INPUT      : monthReportList = daftar laporan bulanan user/admin
---------------------------------------------------------*/
void showMonthlyMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 11;

  renderMonthlyMenuHeader(boxWidth, contentLines);
  renderMonthlyMenuOptions(boxWidth);
  renderMonthlyMenuFooter(boxWidth, monthReportList->count);
}

/* ---------------------------------------------------------
   PROSEDUR : showMonthlyList(monthReportList)
   DESKRIPSI : Menampilkan seluruh daftar laporan bulanan, atau pesan kosong
jika tidak ada I.S.       : Terminal bebas F.S.       : Daftar laporan bulanan
ditampilkan INPUT      : monthReportList = daftar laporan bulanan user/admin
---------------------------------------------------------*/
void showMonthlyList(struct MonthReportList *monthReportList) {
  int boxWidth = 59;

  renderMonthlyListHeader(boxWidth);

  if (monthReportList->count == 0) {
    renderEmptyMonthlyList(boxWidth);
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    renderSingleMonthlyListItem(i, monthReportList->reports[i], boxWidth);
  }

  printf("\n");
  printCenteredColored(
      "──────────────────────────────────────────────────────────\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void renderTransactionGroupsHeader(int boxWidth) {
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
  printf(" │ %s%-15s%s │ %s%-12s%s │ %s%-12s%s │ %s%-12s%s │ %s%-8s%s │ "
         "%s%-12s%s ║\n",
         COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET, COLOR_BRIGHT_CYAN,
         "BUDGET", COLOR_RESET, COLOR_BRIGHT_CYAN, "TERPAKAI", COLOR_RESET,
         COLOR_BRIGHT_CYAN, "SISA", COLOR_RESET, COLOR_BRIGHT_CYAN, "TRANSAKSI",
         COLOR_RESET, COLOR_BRIGHT_CYAN, "STATUS", COLOR_RESET);

  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

void renderEmptyTransactionGroups(int boxWidth) {
  printCentered("║", boxWidth);
  printColored("                           📭 Belum ada transaksi              "
               "              ",
               COLOR_BRIGHT_YELLOW);
  printf("║\n");
}

void renderTransactionGroupRow(int index, struct TransactionGroup *group,
                               int boxWidth) {
  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, group->category);

  const char *status = getGroupBudgetStatus(group);
  long long remaining =
      group->budget > 0 ? group->budget - group->totalAmount : 0;
  double percentage = group->budget > 0
                          ? ((double)group->totalAmount / group->budget) * 100.0
                          : 0.0;

  printCentered("", boxWidth);
  printf("║ %-3d │ %-15s │ ", index + 1, displayName);
  printf("%s%-12lld%s │ ", COLOR_BRIGHT_CYAN,
         group->budget > 0 ? group->budget : group->totalAmount, COLOR_RESET);
  printf("%s%-12lld%s │ ", COLOR_BRIGHT_YELLOW,
         group->budget > 0 ? group->totalAmount : 0, COLOR_RESET);

  printAmount(remaining);
  printf("%-12s │ %-9d │ ", "", group->transactionCount);

  printBudgetStatus(status, percentage);
  printf("%-12s ║\n", "");
}

void renderTransactionGroupsFooter(int boxWidth) {
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
}

void showTransactionGroupsList(struct MonthReport *monthReport) {
  int boxWidth = 100;

  renderTransactionGroupsHeader(boxWidth);

  if (monthReport->groupCount == 0) {
    renderEmptyTransactionGroups(boxWidth);
  } else {
    for (int i = 0; i < monthReport->groupCount; i++) {
      renderTransactionGroupRow(i, monthReport->groups[i], boxWidth);
    }
  }
  renderTransactionGroupsFooter(boxWidth);
}

void renderMonthReportMenu(struct MonthReport *monthReport) {
  showMonthReport(monthReport);
}

int handleMonthReportChoice(struct MonthReport *monthReport, int choice) {
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
    return 0; // Selesai
  default:
    showErrorMessage("Pilihan tidak valid.");
    break;
  }
  return 1; // lanjut loop
}

void openMonthReport(struct MonthReport *monthReport) {
  while (1) {
    renderMonthReportMenu(monthReport);

    int choice = getValidatedMenuChoice(1, 7);
    if (choice == -1)
      continue;

    if (!handleMonthReportChoice(monthReport, choice))
      return;
  }
}

void renderMonthlyMainMenu(struct MonthReportList *monthReportList) {
  showMonthlyMenu(monthReportList);
}

int handleMonthlyMenuChoice(struct MonthReportList *monthReportList,
                            int choice) {
  switch (choice) {
  case 1:
    openMonthlyListMenu(monthReportList);
    break;
  case 2:
    openTransactionAddMenu(monthReportList);
    break;
  case 3:
    return 0; // kembali
  default:
    showErrorMessage("Pilihan tidak valid.");
    break;
  }
  return 1;
}

void openMonthlyMenu(struct MonthReportList *monthReportList) {
  while (1) {
    renderMonthlyMainMenu(monthReportList);

    int choice = getValidatedMenuChoice(1, 3);
    if (choice == -1)
      continue;

    if (!handleMonthlyMenuChoice(monthReportList, choice))
      return;
  }
}

void renderTransactionAddMenuHeader(int boxWidth, int contentLines) {
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
}

int readNewMonthYear(char *buffer, size_t bufferSize, time_t *outDate,
                     int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);

  if (!readAndValidateString("📅 Masukkan bulan dan tahun (MM/YYYY): ", buffer,
                             bufferSize)) {
    showErrorMessage("Input tidak valid.");
    return 0;
  }

  if (!validateMonthYearFormat(buffer, outDate)) {
    showErrorMessage("Format tanggal tidak valid. Gunakan format MM/YYYY.");
    return 0;
  }

  return 1;
}

int validateMonthUniqueness(struct MonthReportList *list, time_t date) {
  if (validateDuplicateMonthReport(list, date)) {
    showErrorMessage("Laporan untuk bulan ini sudah ada.");
    return 0;
  }
  return 1;
}

void saveNewMonthReport(struct MonthReportList *list,
                        struct MonthReport *report) {
  addMonthReportToList(list, report);
  saveUserMonthReport(report);
  showSuccessMessage("Laporan bulanan berhasil dibuat! 🎉");
}

void openTransactionAddMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 10;

  renderTransactionAddMenuHeader(boxWidth, contentLines);

  struct MonthReport *newReport = newMonthReport();
  if (newReport == NULL) {
    showErrorMessage("Gagal membuat laporan baru.");
    return;
  }

  char temp_input[100];
  time_t newDate;

  if (!readNewMonthYear(temp_input, sizeof(temp_input), &newDate, boxWidth)) {
    freeMonthReport(newReport);
    return;
  }

  if (!validateMonthUniqueness(monthReportList, newDate)) {
    freeMonthReport(newReport);
    return;
  }

  newReport->date = newDate;

  saveNewMonthReport(monthReportList, newReport);
}

void renderMonthlyListMenuHeader(int boxWidth) {
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("             📋 KELOLA LAPORAN BULANAN                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

void renderMonthlyListMenuOptions(int boxWidth) {
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
}

void renderMonthlyListMenuFooter(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");

  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

int handleMonthlyListMenuChoice(int choice, struct MonthReportList *list) {
  switch (choice) {
  case 1:
    openSelectReportMenu(list);
    break;
  case 2:
    openTransactionAddMenu(list);
    break;
  case 3:
    openMonthReportEditMenu(list);
    break;
  case 4:
    openMonthReportDeleteMenu(list);
    break;
  case 5:
    return 0;
  default:
    showErrorMessage("Pilihan tidak valid.");
    break;
  }
  return 1;
}

void openMonthlyListMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;

  while (1) {
    clearScreen();
    showMonthlyList(monthReportList);

    renderMonthlyListMenuHeader(boxWidth);
    renderMonthlyListMenuOptions(boxWidth);
    renderMonthlyListMenuFooter(boxWidth);

    int choice = getValidatedMenuChoice(1, 5);
    if (choice == -1)
      continue;

    if (!handleMonthlyListMenuChoice(choice, monthReportList))
      return;
  }
}

int ensureReportsExist(struct MonthReportList *list) {
  if (list->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return 0;
  }
  return 1;
}

void renderReportSelectionList(struct MonthReportList *list) {
  showMonthlyList(list);
}

int readReportSelection(int *index, struct MonthReportList *list) {
  return readAndValidateInteger("\n📊 Pilih nomor laporan: ", 1, list->count,
                                index);
}

void openSelectedMonthReport(struct MonthReportList *list, int index) {
  openMonthReport(list->reports[index - 1]);
}

void openSelectReportMenu(struct MonthReportList *monthReportList) {
  if (!ensureReportsExist(monthReportList))
    return;

  renderReportSelectionList(monthReportList);

  int reportIndex;
  if (!readReportSelection(&reportIndex, monthReportList)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  openSelectedMonthReport(monthReportList, reportIndex);
}

void renderTransactionViewHeader(int boxWidth) {
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
}

void renderTransactionGroupList(struct MonthReport *report) {
  showTransactionGroupsList(report);
}

int ensureTransactionGroupsExist(struct MonthReport *report) {
  if (report->groupCount == 0) {
    showInfoMessage("Belum ada transaksi di laporan ini.");
    return 0;
  }
  return 1;
}

int readTransactionGroupSelection(struct MonthReport *report, int *index) {
  return readAndValidateInteger(
      "\n📂 Pilih nomor kategori untuk melihat detail: ", 1, report->groupCount,
      index);
}

void showSelectedGroupDetails(struct MonthReport *report, int index) {
  struct TransactionGroup *group = report->groups[index - 1];
  showTransactionDetails(group);
}

void openTransactionViewMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;

  renderTransactionViewHeader(boxWidth);
  renderTransactionGroupList(monthReport);

  if (!ensureTransactionGroupsExist(monthReport))
    return;

  int groupIndex;
  if (!readTransactionGroupSelection(monthReport, &groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  showSelectedGroupDetails(monthReport, groupIndex);
}

void renderTransactionDetailsHeader(int boxWidth, const char *categoryStr) {
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

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
}

void renderTransactionSummary(struct TransactionGroup *group, int boxWidth) {
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

  double percentage = (group->budget > 0)
                          ? ((double)group->totalAmount / group->budget) * 100.0
                          : 0.0;

  printBudgetStatus(getGroupBudgetStatus(group), percentage);
  printf("\n");
}

int renderEmptyTransactionsMessage(int boxWidth) {
  if (boxWidth) { /* avoid unused param */
  }

  printf("\n");
  printCentered("", boxWidth);
  printf("📭 Belum ada transaksi di kategori ini.\n");

  waitForEnter();
  return 1;
}

void renderTransactionTableHeader(int tableWidth) {
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);

  printCentered("", tableWidth);
  printf("║ %sNo.%s │ %s%-12s%s │ %s%-15s%s │ %s%-10s%s │ "
         "%s%-15s%s │ %s%-30s%s ║\n",
         COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_BRIGHT_CYAN, "TANGGAL",
         COLOR_RESET, COLOR_BRIGHT_CYAN, "NAMA", COLOR_RESET, COLOR_BRIGHT_CYAN,
         "JENIS", COLOR_RESET, COLOR_BRIGHT_CYAN, "NOMINAL", COLOR_RESET,
         COLOR_BRIGHT_CYAN, "DESKRIPSI", COLOR_RESET);

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);
}

void renderTransactionRow(struct Transaction *t, int index, int tableWidth) {
  const char *typeIcon =
      (t->type == TRANSACTION_INCOME) ? "💚 Masuk" : "❤️  Keluar";
  char *dateStr = dateToString(t->date);

  printCentered("", tableWidth);
  printf("║ %-3d │ %-12s │ %-15s │ %-15s │ Rp %-12lld │ %-30s ║\n", index + 1,
         dateStr, t->name, typeIcon, t->amount, t->description);

  free(dateStr);
}

void renderTransactionTableFooter(int tableWidth) {
  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n",
      tableWidth, COLOR_BRIGHT_CYAN);
}

void showTransactionDetails(struct TransactionGroup *group) {
  int boxWidth = 59;
  int tableWidth = 100;

  struct CategoryList *categories = getUserCategoriesCache();
  const char *categoryStr = getCategoryDisplayName(categories, group->category);

  renderTransactionDetailsHeader(boxWidth, categoryStr);
  renderTransactionSummary(group, boxWidth);

  if (group->transactionCount == 0) {
    renderEmptyTransactionsMessage(boxWidth);
    return;
  }

  renderTransactionTableHeader(tableWidth);

  for (int i = 0; i < group->transactionCount; i++) {
    renderTransactionRow(group->transactions[i], i, tableWidth);
  }

  renderTransactionTableFooter(tableWidth);

  waitForEnter();
}

void renderTransactionEditHeader(int boxWidth) {
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
}

struct TransactionGroup *
handleTransactionGroupSelection(struct MonthReport *monthReport) {
  showTransactionGroupsList(monthReport);
  if (monthReport->groupCount == 0) {
    showInfoMessage("Belum ada transaksi untuk diedit.");
    return NULL;
  }
  int groupIndex;
  if (!readAndValidateInteger("\n📂 Pilih nomor kategori: ", 1,
                              monthReport->groupCount, &groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }
  return monthReport->groups[groupIndex - 1];
}

struct Transaction *handleTransactionSelection(struct TransactionGroup *group) {
  if (group->transactionCount == 0) {
    showInfoMessage("Tidak ada transaksi di kategori ini.");
    return NULL;
  }

  showTransactionDetails(group);
  int transactionIndex;

  if (!readAndValidateInteger("\n💳 Pilih nomor transaksi yang ingin diedit: ",
                              1, group->transactionCount, &transactionIndex)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }
  return group->transactions[transactionIndex - 1];
}

void renderTransactionEditTitle(struct Transaction *transaction, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printColored("✏️  Edit Transaksi ID ", COLOR_BRIGHT_CYAN);
  printf("%d:\n", transaction->id);
  printCenteredColored("─────────────────────────\n", boxWidth,
                       COLOR_BRIGHT_CYAN);
}

void editTransactionDate(struct Transaction *transaction, int boxWidth) {
  char choice;
  printCentered("", boxWidth);
  printColored("📅 Edit tanggal? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &choice);
  clearInputBuffer();

  if (choice == 'y' || choice == 'Y') {
    time_t newDate;
    if (readAndValidateDate("📅 Tanggal baru (DD/MM/YYYY): ", &newDate)) {
      transaction->date = newDate;
    }
  }
}

void editTransactionName(struct Transaction *transaction) {
  char newName[20];
  if (readAndValidateString("💼 Nama baru (kosongkan jika tidak ingin ubah): ",
                            newName, sizeof(newName))) {
    strcpy(transaction->name, newName);
  }
}

void editTransactionDescription(struct Transaction *transaction) {
  char newDescription[200];
  if (readAndValidateString(
          "📝 Deskripsi baru (kosongkan jika tidak ingin ubah): ",
          newDescription, sizeof(newDescription))) {
    strcpy(transaction->description, newDescription);
  }
}

void editTransactionAmount(struct Transaction *transaction, int boxWidth) {
  char choice;
  printCentered("", boxWidth);
  printColored("💰 Edit nominal? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &choice);
  clearInputBuffer();

  if (choice == 'y' || choice == 'Y') {
    long long newAmount;
    InputResult result =
        promptForTransaction("💰 Nominal baru (Rp): ", &newAmount);

    if (result == INPUT_SUCCESS) {
      transaction->amount = newAmount;
    }
  }
}

void performTransactionEditing(struct Transaction *transaction, int boxWidth) {
  renderTransactionEditTitle(transaction, boxWidth);
  editTransactionDate(transaction, boxWidth);
  editTransactionName(transaction);
  editTransactionDescription(transaction);
  editTransactionAmount(transaction, boxWidth);
}

void openTransactionEditMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;

  renderTransactionEditHeader(boxWidth);

  struct TransactionGroup *group = handleTransactionGroupSelection(monthReport);
  if (!group)
    return;

  struct Transaction *transaction = handleTransactionSelection(group);
  if (!transaction)
    return;

  performTransactionEditing(transaction, boxWidth);

  updateGroupCalculations(group);
  updateReportCalculations(monthReport);
  saveUserMonthReport(monthReport);

  showSuccessMessage("Transaksi berhasil diubah! 📝");
}

void renderDeleteTransactionHeader(int boxWidth) {
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
}

bool selectTransactionGroupForDelete(struct MonthReport *monthReport,
                                     int *groupIndex) {
  if (monthReport->groupCount == 0) {
    showInfoMessage("Belum ada transaksi untuk dihapus.");
    return false;
  }

  if (!readAndValidateInteger("\n📂 Pilih nomor kategori: ", 1,
                              monthReport->groupCount, groupIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  return true;
}

bool selectTransactionInGroupForDelete(struct TransactionGroup *group,
                                       int *transactionIndex) {
  if (group->transactionCount == 0) {
    showInfoMessage("Tidak ada transaksi di kategori ini.");
    return false;
  }

  if (!readAndValidateInteger(
          "\n🗑️  Pilih nomor transaksi yang ingin dihapus: ", 1,
          group->transactionCount, transactionIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  return true;
}

bool renderDeleteConfirmation(struct Transaction *transaction, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  KONFIRMASI HAPUS\n");
  printCenteredColored("───────────────────\n", boxWidth, COLOR_BRIGHT_CYAN);

  char *dateStr = dateToString(transaction->date);
  printCentered("", boxWidth);
  printf("Tanggal: %s\n", dateStr);
  printCentered("", boxWidth);
  printf("Transaksi: %s - Rp %lld\n", transaction->name, transaction->amount);
  free(dateStr);

  char confirmation;
  printf("\n");
  printCentered("", boxWidth);
  printColored("❓ Yakin ingin menghapus? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &confirmation);
  clearInputBuffer();

  return (confirmation == 'y' || confirmation == 'Y');
}

void performTransactionDeletion(struct MonthReport *monthReport, int groupIndex,
                                int transactionIndex) {
  removeUserMonthReportTransaction(monthReport, groupIndex, transactionIndex);
  saveUserMonthReport(monthReport);
}

void openTransactionDeleteMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;
  renderDeleteTransactionHeader(boxWidth);
  showTransactionGroupsList(monthReport);

  int groupIndex;
  if (!selectTransactionGroupForDelete(monthReport, &groupIndex))
    return;

  struct TransactionGroup *group = monthReport->groups[groupIndex - 1];
  showTransactionDetails(group);
  int transactionIndex;
  if (!selectTransactionInGroupForDelete(group, &transactionIndex))
    return;

  struct Transaction *transaction = group->transactions[transactionIndex - 1];
  if (!renderDeleteConfirmation(transaction, boxWidth)) {
    showInfoMessage("Penghapusan dibatalkan.");
    return;
  }
  performTransactionDeletion(monthReport, groupIndex - 1, transactionIndex - 1);
  showSuccessMessage("Transaksi berhasil dihapus! 🗑️");
}

void renderAddTransactionHeader(int boxWidth) {
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
}

bool inputTransactionBasicData(struct Transaction *t) {
  if (!readAndValidateString("💼 Nama transaksi: ", t->name, sizeof(t->name))) {
    showErrorMessage("Nama transaksi tidak valid.");
    return false;
  }

  if (!readAndValidateDate("📅 Tanggal transaksi (DD/MM/YYYY): ", &t->date)) {
    showErrorMessage("Format tanggal tidak valid.");
    return false;
  }

  if (!readAndValidateString("📝 Deskripsi: ", t->description,
                             sizeof(t->description))) {
    showErrorMessage("Deskripsi tidak valid.");
    return false;
  }

  InputResult result = promptForTransaction("💰 Nominal (Rp): ", &t->amount);

  if (result != INPUT_SUCCESS) {
    showErrorMessage("Nominal transaksi tidak valid atau kurang dari minimum.");
    return false;
  }

  return true;
}

void renderTransactionTypeSelector(int boxWidth) {
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
  printf(" ❤️ Pengeluaran\n\n");
}

bool selectTransactionType(enum TransactionType *typeOut) {
  int choice;
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &choice)) {
    showErrorMessage("Jenis transaksi tidak valid.");
    return false;
  }

  *typeOut = (choice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;
  return true;
}

struct CategoryList *getFilteredCategories(int transactionType) {
  struct CategoryList *all = getUserCategoriesCache();
  struct CategoryList *filtered = getCategoriesByType(all, transactionType);

  if (filtered == NULL || filtered->count == 0) {
    showErrorMessage("Tidak ada kategori untuk jenis transaksi ini.");
    if (filtered != NULL)
      freeCategoryList(filtered);
    return NULL;
  }

  return filtered;
}

void renderCategorySelectionHeader(int boxWidth) {
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
}

void renderCategoryList(struct CategoryList *filtered, int boxWidth) {
  for (int i = 0; i < filtered->count; i++) {
    printCentered("  ", boxWidth);
    printf("%s%d.%s %s\n", COLOR_BRIGHT_CYAN, i + 1, COLOR_RESET,
           filtered->items[i]->displayName);
  }
  printf("\n");
}

bool promptCategoryChoice(int *categoryChoice, int maxChoice) {
  if (!readAndValidateInteger("🎯 Pilihan: ", 1, maxChoice, categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    return false;
  }
  return true;
}

void assignCategoryToTransaction(struct Transaction *t,
                                 struct CategoryList *filtered,
                                 int categoryChoice) {
  strncpy(t->category, filtered->items[categoryChoice - 1]->internalName,
          sizeof(t->category) - 1);
  t->category[sizeof(t->category) - 1] = '\0';
}

bool selectTransactionCategory(struct Transaction *t, int boxWidth,
                               int transactionType) {

  struct CategoryList *filtered = getFilteredCategories(transactionType);
  if (filtered == NULL)
    return false;

  renderCategorySelectionHeader(boxWidth);
  renderCategoryList(filtered, boxWidth);

  int categoryChoice;
  if (!promptCategoryChoice(&categoryChoice, filtered->count)) {
    freeCategoryList(filtered);
    return false;
  }

  assignCategoryToTransaction(t, filtered, categoryChoice);

  freeCategoryList(filtered);
  return true;
}

bool validateBudgetForExpense(struct MonthReport *monthReport,
                              struct Transaction *t, int boxWidth) {
  if (t->type != TRANSACTION_EXPENSE)
    return true;

  struct TransactionGroup *group =
      findGroupByCategory(monthReport, t->category);

  if (group == NULL)
    return true;

  if (!validateBudgetExceeded(group, t->amount))
    return true;

  showWarningMessage("⚠️  Transaksi ini akan melebihi budget kategori!");
  printCentered("", boxWidth);

  char proceed;
  printColored("❓ Lanjutkan? (y/n): ", COLOR_BRIGHT_YELLOW);
  scanf(" %c", &proceed);
  clearInputBuffer();

  return (proceed == 'y' || proceed == 'Y');
}

void performTransactionInsertion(struct MonthReport *monthReport,
                                 struct Transaction *t) {
  addUserMonthReportTransaction(monthReport, t);
  saveUserMonthReport(monthReport);
}

void openTransactionAddToReportMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;
  struct Transaction t;

  renderAddTransactionHeader(boxWidth);

  if (!inputTransactionBasicData(&t))
    return;

  renderTransactionTypeSelector(boxWidth);

  if (!selectTransactionType(&t.type))
    return;

  if (!selectTransactionCategory(&t, boxWidth, t.type))
    return;

  t.id = getTotalTransactions(monthReport) + 1;

  if (!validateBudgetForExpense(monthReport, &t, boxWidth))
    return;

  performTransactionInsertion(monthReport, &t);
  showSuccessMessage("Transaksi berhasil ditambahkan! 🎉");
}

void renderCategoryBudgetHeader(int boxWidth) {
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
}

bool listAllCategories(struct CategoryList *categories) {
  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    return false;
  }

  printf("\n📂 Pilih kategori untuk mengatur budget:\n");
  for (int i = 0; i < categories->count; i++) {
    printf("  %d. %s\n", i + 1, categories->items[i]->displayName);
  }
  return true;
}

bool promptCategorySelection(struct CategoryList *categories,
                             char *outCategoryBuffer, size_t bufferSize) {
  int categoryChoice;

  if (!readAndValidateInteger("🎯 Pilihan: ", 1, categories->count,
                              &categoryChoice)) {
    showErrorMessage("Kategori tidak valid.");
    return false;
  }

  strncpy(outCategoryBuffer,
          categories->items[categoryChoice - 1]->internalName, bufferSize - 1);
  outCategoryBuffer[bufferSize - 1] = '\0';
  return true;
}

bool promptNewBudget(long long *budget) {
  return readAndValidateLongLong("💰 Budget baru (Rp): ", 1, 1000000000000LL,
                                 budget);
}

void showBudgetSuccess(struct CategoryList *categories, const char *category,
                       long long budget) {

  char successMsg[200];
  const char *displayName = getCategoryDisplayName(categories, category);
  snprintf(successMsg, sizeof(successMsg),
           "Budget kategori '%s' berhasil diatur menjadi Rp %lld", displayName,
           budget);
  showSuccessMessage(successMsg);
}

void openCategoryBudgetMenu(struct MonthReport *monthReport) {
  int boxWidth = 59;
  clearScreen();
  renderCategoryBudgetHeader(boxWidth);

  showTransactionGroupsList(monthReport);

  struct CategoryList *categories = getUserCategoriesCache();
  if (!listAllCategories(categories))
    return;

  char category[50];
  if (!promptCategorySelection(categories, category, sizeof(category)))
    return;

  long long newBudget;
  if (!promptNewBudget(&newBudget)) {
    showErrorMessage("Budget tidak valid.");
    return;
  }

  setBudgetForCategory(monthReport, category, newBudget);
  saveUserMonthReport(monthReport);

  showBudgetSuccess(categories, category, newBudget);
}

void renderReportEditHeader(int boxWidth) {
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
}

bool selectReportFromList(struct MonthReportList *list, int *outIndex) {
  if (list->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return false;
  }
  if (!readAndValidateInteger("📅 Pilih nomor laporan: ", 1, list->count,
                              outIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }
  return true;
}

bool promptForNewMonthYear(char *buffer, size_t size, time_t *outDate) {
  if (!readAndValidateString("📅 Tanggal baru (MM/YYYY): ", buffer, size)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }
  if (!validateMonthYearFormat(buffer, outDate)) {
    showErrorMessage("Format tanggal tidak valid.");
    return false;
  }
  return true;
}

void applyNewReportDate(struct MonthReport *report, time_t newDate) {
  deleteUserMonthReport(report);
  report->date = newDate;
  saveUserMonthReport(report);
}

void openMonthReportEditMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  clearScreen();
  showMonthlyList(monthReportList);
  renderReportEditHeader(boxWidth);

  int reportIndex;
  if (!selectReportFromList(monthReportList, &reportIndex))
    return;

  struct MonthReport *report = monthReportList->reports[reportIndex - 1];

  char newDateStr[20];
  time_t newDate;
  if (!promptForNewMonthYear(newDateStr, sizeof(newDateStr), &newDate))
    return;

  applyNewReportDate(report, newDate);
  showSuccessMessage("Tanggal laporan berhasil diubah! 📅");
}

void renderDeleteReportHeader(int boxWidth) {
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
}

bool promptSelectReportToDelete(struct MonthReportList *list, int *outIndex) {
  if (list->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return false;
  }

  showMonthlyList(list);

  if (!readAndValidateInteger("\n🗑️  Pilih nomor laporan yang ingin dihapus: ",
                              1, list->count, outIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  return true;
}

void renderDeleteConfirmationScreen(struct MonthReport *report, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  KONFIRMASI HAPUS\n");
  printCenteredColored("───────────────────\n", boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("", boxWidth);
  printf("Laporan: %s\n", dateToMonthYearString(report->date));

  printCentered("", boxWidth);
  printf("Total transaksi: %d\n", getTotalTransactions(report));
}

bool confirmDelete(int boxWidth) {
  char confirmation;

  printf("\n");
  printCentered("", boxWidth);
  printColored("❓ Yakin ingin menghapus? (y/n): ", COLOR_BRIGHT_YELLOW);

  scanf(" %c", &confirmation);
  clearInputBuffer();

  if (confirmation != 'y' && confirmation != 'Y') {
    showInfoMessage("Penghapusan dibatalkan.");
    return false;
  }

  return true;
}

void deleteReportFromList(struct MonthReportList *list, int index) {
  struct MonthReport *report = list->reports[index];

  deleteUserMonthReport(report);
  freeMonthReport(report);

  for (int i = index; i < list->count - 1; i++) {
    list->reports[i] = list->reports[i + 1];
  }

  list->count--;
}

void openMonthReportDeleteMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  clearScreen();
  renderDeleteReportHeader(boxWidth);

  int reportIndex;
  if (!promptSelectReportToDelete(monthReportList, &reportIndex))
    return;

  struct MonthReport *reportToDelete =
      monthReportList->reports[reportIndex - 1];

  renderDeleteConfirmationScreen(reportToDelete, boxWidth);

  if (!confirmDelete(boxWidth))
    return;

  deleteReportFromList(monthReportList, reportIndex - 1);

  showSuccessMessage("Laporan berhasil dihapus! 🗑️");
}

void renderAllSummaryHeader(int boxWidth) {
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
}

bool allocateSummaryArrays(struct CategoryList *categories,
                           long long **categoryTotals,
                           long long **categoryMaximums,
                           int **categoryTransactions) {
  *categoryTotals = calloc(categories->count, sizeof(long long));
  *categoryMaximums = calloc(categories->count, sizeof(long long));
  *categoryTransactions = calloc(categories->count, sizeof(int));

  if (!*categoryTotals || !*categoryMaximums || !*categoryTransactions) {
    free(*categoryTotals);
    free(*categoryMaximums);
    free(*categoryTransactions);
    return false;
  }
  return true;
}

void accumulateReportData(struct MonthReportList *list,
                          struct CategoryList *categories,
                          long long *totalIncome, long long *totalExpense,
                          int *totalTransactions, long long *categoryTotals,
                          long long *categoryMaximums,
                          int *categoryTransactions) {

  for (int i = 0; i < list->count; i++) {
    struct MonthReport *report = list->reports[i];

    *totalIncome += report->totalIncome;
    *totalExpense += report->totalExpenses;
    *totalTransactions += getTotalTransactions(report);

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
}

void renderFinancialSummary(long long totalIncome, long long totalExpense,
                            int totalTransactions, int boxWidth) {

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
  printf("%sRp %lld%s\n", COLOR_BRIGHT_GREEN, totalIncome, COLOR_RESET);

  printCentered("", boxWidth);
  printError("❤️  Total Pengeluaran: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_RED, totalExpense, COLOR_RESET);

  printCentered("", boxWidth);
  printf("💵 Saldo Akhir: ");
  printAmount(finalBalance);
  printf("\n");

  printCentered("", boxWidth);
  printInfo("📊 Total Transaksi: ");
  printf("%s%d%s\n", COLOR_BRIGHT_CYAN, totalTransactions, COLOR_RESET);

  printCentered("", boxWidth);
  if (finalBalance > 0)
    printSuccess("✅ Status: Surplus (Kondisi Baik)\n");
  else if (finalBalance < 0)
    printError("⚠️  Status: Defisit (Perlu Perhatian)\n");
  else
    printWarning("⚖️  Status: Seimbang\n");
}

void renderCategoryBudgetHeader2(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printSuccess("📂 BUDGET PER KATEGORI:\n");

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║ ", boxWidth);
  printf("%s%-20s%s │ ", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf("%s%-15s%s │ ", COLOR_BRIGHT_CYAN, "BUDGET", COLOR_RESET);
  printf("%s%-15s%s │ ", COLOR_BRIGHT_CYAN, "TERPAKAI", COLOR_RESET);
  printf("%s%-15s%s │ ", COLOR_BRIGHT_CYAN, "SISA", COLOR_RESET);
  printf("%s%-10s%s │ ", COLOR_BRIGHT_CYAN, "TRANSAKSI", COLOR_RESET);
  printf("%s%-10s%s ║\n", COLOR_BRIGHT_CYAN, "STATUS", COLOR_RESET);

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

void renderCategoryBudgetSummary(struct CategoryList *categories,
                                 long long *categoryTotals,
                                 long long *categoryMaximums,
                                 int *categoryTransactions, int boxWidth) {

  for (int i = 0; i < categories->count; i++) {
    if (categoryTransactions[i] == 0 && categoryMaximums[i] == 0)
      continue;

    long long remaining = categoryMaximums[i] - categoryTotals[i];
    double percentage =
        (categoryMaximums[i] > 0)
            ? (double)categoryTotals[i] / categoryMaximums[i] * 100.0
            : 0.0;

    const char *displayName =
        getCategoryDisplayName(categories, categories->items[i]->internalName);

    printCentered("║ ", boxWidth);
    printf("%-20s │ ", displayName);

    printf("%s%-15lld%s │ ", COLOR_BRIGHT_CYAN, categoryMaximums[i],
           COLOR_RESET);

    printf("%s%-15lld%s │ ", COLOR_BRIGHT_YELLOW, categoryTotals[i],
           COLOR_RESET);

    printAmount(remaining);

    printf("%-15s │ %-10d │ ", "", categoryTransactions[i]);

    printBudgetStatus(remaining >= 0 ? "Aman" : "Melebihi", percentage);

    printf("%-10s ║\n", "");
  }

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

void showAllMonthReportSummary(struct MonthReportList *monthReportList) {
  int boxWidth = 88;
  clearScreen();

  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  renderAllSummaryHeader(boxWidth);

  struct CategoryList *categories = getUserCategoriesCache();
  if (!categories)
    return;

  long long *categoryTotals;
  long long *categoryMaximums;
  int *categoryTransactions;

  if (!allocateSummaryArrays(categories, &categoryTotals, &categoryMaximums,
                             &categoryTransactions))
    return;

  long long totalIncome = 0, totalExpense = 0;
  int totalTransactions = 0;

  accumulateReportData(monthReportList, categories, &totalIncome, &totalExpense,
                       &totalTransactions, categoryTotals, categoryMaximums,
                       categoryTransactions);

  renderFinancialSummary(totalIncome, totalExpense, totalTransactions,
                         boxWidth);

  renderCategoryBudgetHeader2(boxWidth);

  renderCategoryBudgetSummary(categories, categoryTotals, categoryMaximums,
                              categoryTransactions, boxWidth);

  free(categoryTotals);
  free(categoryMaximums);
  free(categoryTransactions);

  waitForEnter();
}

/* ================= HEADER RINGKASAN ================= */

void renderMonthSummaryHeader(struct MonthReport *monthReport, int boxWidth) {
  printCenteredColored(
      "╔════════════════════════════════════════════════════════════════════"
      "══════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char *monthStr = dateToMonthYearString(monthReport->date);
  int monthStrLen = strlen(monthStr);
  int totalWidth = boxWidth;
  int prefixLen = 21;
  int remainingSpaces = totalWidth - prefixLen - monthStrLen;

  printCentered("║", boxWidth);
  printWithBg("                    📊 RINGKASAN LAPORAN ", COLOR_BRIGHT_WHITE,
              BG_BLUE);
  printf("%s", monthStr);

  for (int i = 0; i < remainingSpaces; i++)
    printf(" ");

  printf("║\n");

  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  free(monthStr);
}

/* ================= RINGKASAN FINANSIAL ================= */

void renderMonthFinancialSummary(struct MonthReport *monthReport,
                                 int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 RINGKASAN FINANSIAL:\n");

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
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
}

/* ================= ANALISIS KONDISI ================= */

void analyzeMonthFinancialCondition(struct MonthReport *monthReport,
                                    const char **condition,
                                    const char **conclusion) {

  if (monthReport->balance < 0) {
    double deficitPercent =
        ((double)(-monthReport->balance) / monthReport->totalIncome) * 100;

    if (deficitPercent > 10) {
      *condition = "⚠️  Defisit besar (Pengeluaran >> Pemasukan)";
      *conclusion = "🚨 Kondisi keuangan kurang sehat. Kurangi pengeluaran dan "
                    "cari tambahan pemasukan.";
    } else {
      *condition = "⚠️  Defisit ringan (Pengeluaran > Pemasukan)";
      *conclusion = "💡 Anda mulai boros, perhatikan pengeluaran harian.";
    }
  }

  else if (monthReport->balance == 0) {
    *condition = "⚖️  Seimbang (Tanpa sisa uang)";
    *conclusion = "✅ Keuangan Anda seimbang, tetap waspada terhadap "
                  "pengeluaran tidak perlu.";
  }

  else {
    double surplusPercent =
        ((double)monthReport->balance / monthReport->totalIncome) * 100;

    if (surplusPercent > 25) {
      *condition = "💚 Surplus besar (Memiliki sisa uang)";
      *conclusion = "🎉 Anda termasuk mahasiswa hemat dan produktif.";
    } else {
      *condition = "💚 Surplus kecil (Memiliki sisa uang)";
      *conclusion = "✅ Keuangan Anda seimbang, tetap waspada terhadap "
                    "pengeluaran tidak perlu.";
    }
  }
}

/* ================= RENDER BAGIAN ANALISIS ================= */

void renderMonthConditionSection(struct MonthReport *monthReport,
                                 const char *condition, const char *conclusion,
                                 int boxWidth) {

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
  } else if (monthReport->balance == 0) {
    printWarning("📊 Status: ");
  } else {
    printSuccess("📊 Status: ");
  }
  printf("%s\n", condition);

  printCentered("", boxWidth);
  printInfo("💡 Kesimpulan: ");
  printf("%s\n", conclusion);

  printCenteredColored(
      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      "━━━━━━━━━━━━━━━━━━\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

/* ================= FUNGSI UTAMA ================= */

void showMonthReportSummary2(struct MonthReport *monthReport) {
  int boxWidth = 88;
  clearScreen();

  renderMonthSummaryHeader(monthReport, boxWidth);
  renderMonthFinancialSummary(monthReport, boxWidth);

  showTransactionGroupsList(monthReport);

  const char *condition;
  const char *conclusion;

  analyzeMonthFinancialCondition(monthReport, &condition, &conclusion);
  renderMonthConditionSection(monthReport, condition, conclusion, boxWidth);

  waitForEnter();
}

/* ================= HEADER LAPORAN ================= */

void renderMonthReportHeader(struct MonthReport *monthReport, int boxWidth) {
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
  for (int i = 0; i < remainingSpaces; i++)
    printf(" ");

  printf("║\n");

  printCenteredColored(
      "╚════════════════════════════════════════════════════════════════════"
      "══════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  free(monthStr);
}

/* ================= RINGKASAN SINGKAT ================= */

void renderMonthBriefSummary(struct MonthReport *monthReport, int boxWidth) {
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
}

/* ================= MENU LAPORAN ================= */

void renderMonthReportMenu2(int menuBoxWidth) {
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

/* ================= FUNGSI UTAMA ================= */

void showMonthReport(struct MonthReport *monthReport) {
  int boxWidth = 88;
  int menuBoxWidth = 59;

  clearScreen();

  renderMonthReportHeader(monthReport, boxWidth);
  renderMonthBriefSummary(monthReport, boxWidth);

  showTransactionGroupsList(monthReport);

  printf("\n");
  renderMonthReportMenu2(menuBoxWidth);
}

char temp_input[100];

#endif
