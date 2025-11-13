#include "../types/include.c"
#include "../utils/validation.c"
#include "./month_report.c"
#include <stdio.h>
#include <stdlib.h>

void clearScreen() { system("clear"); }

void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void waitForEnter() {
  printf("\n📱 Tekan Enter untuk melanjutkan...");
  clearInputBuffer();
}

void showMainMenu() {
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                    🏠 MENU UTAMA                        │\n");
  printf("├─────────────────────────────────────────────────────────┤\n");
  printf("│  1. 📊 Kelola Laporan Bulanan                           │\n");
  printf("│  2. 📈 Lihat Ringkasan Keuangan                         │\n");
  printf("│  3. ⚙️  Pengaturan Budget                                │\n");
  printf("│  4. ❌ Keluar                                           │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");
  printf("\n💡 Tip: Pilih nomor menu yang diinginkan\n");
  printf("🎯 Pilihan Anda: ");
}

void showSuccessMessage(const char *message) {
  printf("\n✅ %s", message);
  waitForEnter();
}

void showErrorMessage(const char *message) {
  printf("\n❌ %s", message);
  waitForEnter();
}

void showWarningMessage(const char *message) {
  printf("\n⚠️  %s", message);
  waitForEnter();
}

void showInfoMessage(const char *message) {
  printf("\nℹ️  %s", message);
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

void showLoadingMessage(const char *action) {
  printf("\n⏳ %s", action);
  fflush(stdout);

  for (int i = 0; i < 3; i++) {
    printf(".");
    fflush(stdout);

    for (int j = 0; j < 300000; j++) {
    }
  }
  printf(" Selesai!\n");
}

void openSummaryMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->amount == 0) {
    showInfoMessage("Tidak ada laporan bulanan untuk ditampilkan. Buat laporan "
                    "terlebih dahulu.");
    return;
  }

  showLoadingMessage("Menghitung ringkasan keuangan");
  showMonthReportSummary(monthReportList);
}

void openBudgetSettingsMenu(struct MonthReportList *monthReportList) {
  while (1) {
    clearScreen();
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│                 ⚙️  PENGATURAN BUDGET                   │\n");
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│  1. 🏷️  Atur Budget per Kategori                        │\n");
    printf("│  2. 📋 Lihat Budget Saat Ini                            │\n");
    printf("│  3. 🔄 Reset Semua Budget                               │\n");
    printf("│  4. ⬅️  Kembali ke Menu Utama                          │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    printf("\n🎯 Pilihan Anda: ");

    int choice = getValidatedMenuChoice(1, 4);
    if (choice == -1)
      continue;

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
      return;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}

void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->amount == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return;
  }

  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│             🏷️  ATUR BUDGET PER KATEGORI                │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1,
                              monthReportList->amount, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct MonthReport *report = monthReportList->reports[reportChoice - 1];

  printf("\n📂 Pilih Kategori untuk Mengatur Budget:\n");
  printf("─────────────────────────────────────────\n");
  for (int i = 0; i <= TC_OTHER; i++) {
    printf("  %d. %s\n", i + 1,
           transactionCategoryToString((enum TransactionCategory)i));
  }

  int categoryChoice;
  if (!readAndValidateInteger("\n🎯 Pilihan kategori: ", 1, TC_OTHER + 1,
                              &categoryChoice)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  enum TransactionCategory category =
      (enum TransactionCategory)(categoryChoice - 1);

  long long budget;
  if (!readAndValidateLongLong("💰 Masukkan budget baru (Rp): ", 1,
                               1000000000000LL, &budget)) {
    showErrorMessage("Input budget tidak valid.");
    return;
  }

  setBudgetForCategory(report, category, budget);
  saveMonthReportToFile(report);

  char successMsg[200];
  snprintf(successMsg, sizeof(successMsg),
           "Budget untuk kategori '%s' berhasil diatur menjadi Rp %lld",
           transactionCategoryToString(category), budget);
  showSuccessMessage(successMsg);
}

void openViewBudgetMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->amount == 0) {
    showInfoMessage("Tidak ada laporan bulanan.");
    return;
  }

  clearScreen();
  printf("┌─────────────────────────────────────────────────────────┐\n");
  printf("│                📋 BUDGET SAAT INI                      │\n");
  printf("└─────────────────────────────────────────────────────────┘\n");

  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1,
                              monthReportList->amount, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  struct MonthReport *report = monthReportList->reports[reportChoice - 1];

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

  for (int i = 0; i < report->groupsAmount; i++) {
    struct TransactionGroup *group = &report->groups[i];
    printf("║ %-20s │ %-15lld │ %-15lld │ %-15lld │ %-10s ║\n",
           transactionCategoryToString(group->category), group->maximumCost,
           group->totalRealCost, group->remainingCost,
           getGroupBudgetStatus(group));
  }

  printf("╚════════════════════════════════════════════════════════════════════"
         "══════════════╝\n");
  waitForEnter();
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

  showLoadingMessage("Mereset semua budget");

  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j <= TC_OTHER; j++) {
      setBudgetForCategory(report, (enum TransactionCategory)j, 1000000);
    }

    saveMonthReportToFile(report);
  }

  showSuccessMessage("Semua budget berhasil direset ke Rp 1,000,000!");
}

void openMainMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMainMenu();

    int choice = getValidatedMenuChoice(1, 4);
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      showLoadingMessage("Membuka menu bulanan");
      openMonthlyMenu(monthReportList);
      break;
    case 2:
      openSummaryMenu(monthReportList);
      break;
    case 3:
      openBudgetSettingsMenu(monthReportList);
      break;
    case 4:
      clearScreen();
      printf("╔══════════════════════════════════════════════════════════╗\n");
      printf("║                    👋 TERIMA KASIH!                     ║\n");
      printf("║                                                          ║\n");
      printf("║          Semoga keuangan Anda selalu terjaga! 💰        ║\n");
      printf("║                                                          ║\n");
      printf("║                   Sampai jumpa lagi! 😊                 ║\n");
      printf("╚══════════════════════════════════════════════════════════╝\n");
      printf("\n");
      exit(0);
      break;
    default:
      showErrorMessage("Pilihan tidak valid.");
      break;
    }
  }
}
