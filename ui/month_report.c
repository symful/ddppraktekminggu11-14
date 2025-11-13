#include "./month_report.h"
#include "../db/include.c"
#include "../types/include.c"
#include "../utils/date.c"
#include "../utils/date.h"
#include "./master.h"
#include <string.h>

#ifndef UI_MONTH_REPORT_C
#define UI_MONTH_REPORT_C

void showMonthlyMenu(struct MonthReportList *monthReportList) {
  (void)monthReportList; // Suppress unused parameter warning
  clearScreen();
  printf("Menu Bulanan:\n");
  printf("1. Lihat Daftar Bulanan\n");
  printf("2. Tambah Laporan Bulanan\n");
  printf("3. Kembali\n");
  printf("Pilih opsi: ");
}

void showMonthlyList(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("Daftar Bulanan:\n");
  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    printf("%d. %s\n", i + 1, dateToMonthYearString(report->date));
  }
}

void openMonthReport(struct MonthReport *monthReport) {
  while (1) {
    showMonthReport(monthReport);

    int choice;
    printf("Pilih opsi: ");
    if (scanf("%d", &choice) != 1) {
      clearInputBuffer();
      printf("Input tidak valid. ");
      waitForEnter();
      continue;
    }

    switch (choice) {
    case 1:
      openTransactionEditMenu(monthReport);
      break;
    case 2:
      openTransactionDeleteMenu(monthReport);
      break;
    case 3:
      openTransactionAddToReportMenu(monthReport);
      break;
    case 4:
      printf("Fitur ringkasan akan ditampilkan di menu utama. ");
      waitForEnter();
      break;
    case 5:
      return;
    default:
      printf("Pilihan tidak valid. ");
      waitForEnter();
      break;
    }
  }
}

void openMonthlyMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMonthlyMenu(monthReportList);

    int choice;
    if (scanf("%d", &choice) != 1) {
      clearInputBuffer();
      printf("Input tidak valid. ");
      waitForEnter();
      continue;
    }

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
      printf("Pilihan tidak valid. ");
      waitForEnter();
      break;
    }
  }
}

void openTransactionAddMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("Tambah Laporan Bulanan Baru:\n");

  struct MonthReport *newReport = newMonthReport();

  printf("Masukkan bulan dan tahun (MM/YYYY, contoh: 01/2024): ");
  char input[20];
  clearInputBuffer();
  if (fgets(input, sizeof(input), stdin) == NULL) {
    printf("Input tidak valid. ");
    waitForEnter();
    free(newReport);
    return;
  }

  // Remove newline if present
  input[strcspn(input, "\n")] = 0;

  time_t newDate = dateFromMMYYYY(input);
  if (newDate == -1) {
    printf("Format tanggal tidak valid. Gunakan format MM/YYYY. ");
    waitForEnter();
    free(newReport);
    return;
  }

  newReport->date = newDate;
  addMonthReport(monthReportList, newReport);
  saveMonthReportToFile(newReport);

  printf("Laporan bulanan berhasil ditambahkan. ");
  waitForEnter();
}

void openMonthlyListMenu(struct MonthReportList *monthReportList) {
  while (1) {
    clearScreen();

    printf("Laporan Bulanan:\n");
    printf("-----------------\n");
    printf("1. Lihat Laporan Bulanan\n");
    printf("2. Tambah Laporan Bulanan\n");
    printf("3. Edit Laporan Bulanan\n");
    printf("4. Hapus Laporan Bulanan\n");
    printf("5. Kembali\n");
    printf("-----------------\n");

    int choice;
    printf("Pilih opsi: ");
    if (scanf("%d", &choice) != 1) {
      clearInputBuffer();
      printf("Input tidak valid. ");
      waitForEnter();
      continue;
    }

    switch (choice) {
    case 1:
      if (monthReportList->amount == 0) {
        printf("Tidak ada laporan bulanan. ");
        waitForEnter();
        break;
      }
      printf("Pilih nomor laporan: ");
      int reportIndex;
      if (scanf("%d", &reportIndex) != 1) {
        clearInputBuffer();
        printf("Input tidak valid. ");
        waitForEnter();
        break;
      }
      if (reportIndex > 0 && reportIndex <= monthReportList->amount) {
        openMonthReport(monthReportList->reports[reportIndex - 1]);
      } else {
        printf("Nomor laporan tidak valid. ");
        waitForEnter();
      }
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
      printf("Pilihan tidak valid. ");
      waitForEnter();
      break;
    }
  }
}

void openTransactionEditMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("Edit Transaksi:\n");

  if (monthReport->transactionsAmount == 0) {
    printf("Tidak ada transaksi untuk diedit. ");
    waitForEnter();
    return;
  }

  printf("Pilih nomor transaksi yang ingin diubah: ");
  int transactionIndex;
  if (scanf("%d", &transactionIndex) != 1) {
    clearInputBuffer();
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }

  if (transactionIndex > 0 &&
      transactionIndex <= monthReport->transactionsAmount) {
    struct Transaction *transaction =
        &monthReport->transactions[transactionIndex - 1];

    printf("Edit transaksi ID %d:\n", transaction->id);
    printf("Masukkan deskripsi baru: ");
    scanf("%s", transaction->description);
    printf("Masukkan nominal baru: ");
    scanf("%lld", &transaction->realCost);

    saveMonthReportToFile(monthReport);
    printf("Transaksi berhasil diubah. ");
    waitForEnter();
  } else {
    printf("Nomor transaksi tidak valid. ");
    waitForEnter();
  }
}

void openTransactionDeleteMenu(struct MonthReport *monthReport) {
  clearScreen();
  printf("Hapus Transaksi:\n");

  if (monthReport->transactionsAmount == 0) {
    printf("Tidak ada transaksi untuk dihapus. ");
    waitForEnter();
    return;
  }

  printf("Pilih nomor transaksi yang ingin dihapus: ");
  int transactionIndex;
  if (scanf("%d", &transactionIndex) != 1) {
    clearInputBuffer();
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }

  if (transactionIndex > 0 &&
      transactionIndex <= monthReport->transactionsAmount) {
    removeMonthReportTransaction(monthReport, transactionIndex - 1);
    saveMonthReportToFile(monthReport);
    printf("Transaksi berhasil dihapus. ");
    waitForEnter();
  } else {
    printf("Nomor transaksi tidak valid. ");
    waitForEnter();
  }
}

void openTransactionAddToReportMenu(struct MonthReport *monthReport) {
  clearScreen();
  struct Transaction newTransaction;

  printf("Tambah Transaksi:\n");
  newTransaction.id = monthReport->transactionsAmount + 1;
  printf("Masukkan deskripsi: ");
  scanf("%s", newTransaction.description);
  printf("Masukkan nominal: ");
  if (scanf("%lld", &newTransaction.realCost) != 1) {
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }

  printf("\nPilih jenis transaksi:\n");
  printf("0 = Pengeluaran\n");
  printf("1 = Pemasukan\n");
  printf("Pilihan: ");
  int type;
  if (scanf("%d", &type) != 1 || type < 0 || type > 1) {
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }
  newTransaction.type = (enum TransactionType)type;

  printf("\nPilih kategori:\n");
  printf("0 = Transportasi\n");
  printf("1 = Pendapatan\n");
  printf("2 = Pekerjaan\n");
  printf("3 = Makanan\n");
  printf("4 = Hiburan\n");
  printf("5 = Belanja\n");
  printf("6 = Lainnya\n");
  printf("Pilihan: ");
  int category;
  if (scanf("%d", &category) != 1 || category < 0 || category > 6) {
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }
  newTransaction.category = (enum TransactionCategory)category;

  newTransaction.date = monthReport->date;
  newTransaction.maximumCost = newTransaction.realCost;

  addMonthReportTransaction(monthReport, &newTransaction);
  saveMonthReportToFile(monthReport);

  printf("Transaksi berhasil ditambahkan. ");
  waitForEnter();
}

void openMonthReportEditMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  showMonthlyList(monthReportList);
  printf("Edit Laporan Bulanan:\n");

  if (monthReportList->amount == 0) {
    printf("Tidak ada laporan bulanan. ");
    waitForEnter();
    return;
  }

  printf("Pilih nomor laporan yang ingin diubah: ");
  int reportIndex;
  if (scanf("%d", &reportIndex) != 1) {
    clearInputBuffer();
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }

  if (reportIndex > 0 && reportIndex <= monthReportList->amount) {
    struct MonthReport *report = monthReportList->reports[reportIndex - 1];
    printf("Masukkan tanggal baru (MM/YYYY, contoh: 01/2024): ");
    char input[20];
    clearInputBuffer();
    if (fgets(input, sizeof(input), stdin) != NULL) {
      input[strcspn(input, "\n")] = 0;
      time_t newDate = dateFromMMYYYY(input);
      if (newDate != -1) {
        report->date = newDate;
        saveMonthReportToFile(report);
        printf("Laporan berhasil diubah. ");
      } else {
        printf("Format tanggal tidak valid. Gunakan format MM/YYYY. ");
      }
    } else {
      printf("Input tidak valid. ");
    }
    waitForEnter();
  } else {
    printf("Nomor laporan tidak valid. ");
    waitForEnter();
  }
}

void openMonthReportDeleteMenu(struct MonthReportList *monthReportList) {
  clearScreen();
  printf("Hapus Laporan Bulanan:\n");

  if (monthReportList->amount == 0) {
    printf("Tidak ada laporan bulanan. ");
    waitForEnter();
    return;
  }

  showMonthlyList(monthReportList);

  printf("Pilih nomor laporan yang ingin dihapus: ");
  int reportIndex;
  if (scanf("%d", &reportIndex) != 1) {
    clearInputBuffer();
    printf("Input tidak valid. ");
    waitForEnter();
    return;
  }

  if (reportIndex > 0 && reportIndex <= monthReportList->amount) {
    struct MonthReport *reportToDelete =
        monthReportList->reports[reportIndex - 1];
    deleteMonthReportFile(reportToDelete);

    // Free memory if needed and shift reports
    for (int i = reportIndex - 1; i < monthReportList->amount - 1; i++) {
      monthReportList->reports[i] = monthReportList->reports[i + 1];
    }
    monthReportList->amount--;

    printf("Laporan berhasil dihapus. ");
    waitForEnter();
  } else {
    printf("Nomor laporan tidak valid. ");
    waitForEnter();
  }
}

void showMonthReportSummary(struct MonthReportList *monthReportList) {
  clearScreen();
  if (monthReportList->amount == 0) {
    printf("Tidak ada laporan bulanan. ");
    waitForEnter();
    return;
  }

  printf("======== LAPORAN KEUANGAN %s =========\n",
         dateToMonthYearString(monthReportList->reports[0]->date));
  long long totalIncome = 0, totalExpense = 0;
  int incomeTransactions = 0, expenseTransactions = 0;

  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    for (int j = 0; j < report->transactionsAmount; j++) {
      struct Transaction *transaction = &report->transactions[j];
      if (transaction->type == TT_INCOME) {
        totalIncome += transaction->realCost;
        incomeTransactions++;
      } else if (transaction->type == TT_EXPENSE) {
        totalExpense += transaction->realCost;
        expenseTransactions++;
      }
    }
  }

  long long finalBalance = totalIncome - totalExpense;
  long long averageExpense =
      expenseTransactions > 0 ? totalExpense / expenseTransactions : 0;

  printf("Total Pemasukan (%d Transaksi) : Rp %lld\n", incomeTransactions,
         totalIncome);
  printf("Total Pengeluaran (%d Transaksi) : Rp %lld\n", expenseTransactions,
         totalExpense);
  printf("Saldo Akhir : Rp %lld\n", finalBalance);
  printf("Rata-rata pengeluaran : Rp %lld\n", averageExpense);
  printf("----------------------------------------------------\n");
  printf("POS ANGGARAN\n");
  printf("%-15s | %-15s | %-15s | %-15s | %-20s | %-10s\n", "POS", "Batas (Rp)",
         "Realisasi (Rp)", "Sisa (Rp)", "Jumlah Transaksi", "Status");
  long long categoryTotals[7] = {0};
  long long categoryMaximums[7] = {0};
  int categoryTransactions[7] = {0};

  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];
    for (int j = 0; j < report->transactionsAmount; j++) {
      struct Transaction *transaction = &report->transactions[j];
      if (transaction->type == TT_EXPENSE) {
        categoryTotals[transaction->category] += transaction->realCost;
        categoryMaximums[transaction->category] += transaction->maximumCost;
        categoryTransactions[transaction->category]++;
      }
    }
  }

  for (int category = 0; category < 7; category++) {
    long long remaining = categoryMaximums[category] - categoryTotals[category];
    const char *status = remaining >= 0 ? "Aman" : "Melebihi";
    printf("%-15s | %-15lld | %-15lld | %-15lld | %-20d | %-10s\n",
           transactionCategoryToString((enum TransactionCategory)category),
           categoryMaximums[category], categoryTotals[category], remaining,
           categoryTransactions[category], status);
  }

  printf("----------------------------------------------------\n");
  const char *financialCondition;
  const char *financialConclusion;

  if (finalBalance < 0) {
    double deficitPercentage = ((double)(-finalBalance) / totalIncome) * 100;
    if (deficitPercentage > 10) {
      financialCondition =
          "Defisit besar (Pengeluaran lebih besar dari pemasukan)";
      financialConclusion = "Kondisi keuangan kurang sehat. Kurangi "
                            "pengeluaran dan cari tambahan pemasukan.";
    } else {
      financialCondition =
          "Defisit ringan (Pengeluaran lebih besar dari pemasukan)";
      financialConclusion = "Anda mulai boros, perhatikan pengeluaran harian.";
    }
  } else if (finalBalance == 0) {
    financialCondition = "Seimbang (Tanpa sisa uang)";
    financialConclusion = "Keuangan Anda seimbang, tetap waspada terhadap "
                          "pengeluaran tidak perlu.";
  } else {
    double surplusPercentage = ((double)finalBalance / totalIncome) * 100;
    if (surplusPercentage > 25) {
      financialCondition = "Surplus besar (Masih memiliki sisa uang)";
      financialConclusion = "Anda termasuk mahasiswa hemat dan produktif.";
    } else {
      financialCondition = "Surplus kecil (Masih memiliki sisa uang)";
      financialConclusion = "Keuangan Anda seimbang, tetap waspada terhadap "
                            "pengeluaran tidak perlu.";
    }
  }

  printf("Kondisi Keuangan : %s\n", financialCondition);
  printf("Kesimpulan : %s\n", financialConclusion);
  printf("====================================================\n");
  waitForEnter();
}

void showMonthReportTransactions(struct MonthReport *monthReport) {
  clearScreen();
  printf("Laporan Keuangan Bulanan: %s\n", ctime(&monthReport->date));
  printf("==================================================================="
         "==\n");
  printf("| %-3s | %-5s | %-12s | %-15s | %-12s | %-15s | %-30s |\n", "No.",
         "ID", "Tanggal", "Pos Anggaran", "Jenis", "Nominal (Rp)", "Deskripsi");
  printf("==================================================================="
         "==\n");
  for (int j = 0; j < monthReport->transactionsAmount; j++) {
    struct Transaction *transaction = &monthReport->transactions[j];
    printf("| %-3d | %-5d | %-12s | %-15s | %-12s | %-15lld | %-30s |\n", j + 1,
           transaction->id, dateToString(transaction->date),
           transactionCategoryToString(transaction->category),
           transactionTypeToString(transaction->type), transaction->realCost,
           transaction->description);
  }
  printf("==================================================================="
         "==\n\n");
}

void showMonthReport(struct MonthReport *monthReport) {
  showMonthReportTransactions(monthReport);

  printf("---------------\n");
  printf("1. Ubah Transaksi\n");
  printf("2. Hapus Transaksi\n");
  printf("3. Tambah Transaksi\n");
  printf("4. Lihat Ringkasan\n");
  printf("5. Kembali\n");
  printf("---------------\n");
}

#endif
