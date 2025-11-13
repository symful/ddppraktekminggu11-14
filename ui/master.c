#include "../types/include.c"
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
  printf("Tekan Enter untuk melanjutkan...");
  clearInputBuffer();
  clearInputBuffer();
}

void showMainMenu() {
  clearScreen();
  printf("Menu Utama:\n");
  printf("1. Menu Bulanan\n");
  printf("2. Keluar\n");
  printf("Pilih opsi: ");
}
void openMainMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMainMenu();

    int choice;
    if (scanf("%d", &choice) != 1) {
      clearInputBuffer();
      printf("Input tidak valid. ");
      waitForEnter();
      continue;
    }

    switch (choice) {
    case 1:
      openMonthlyMenu(monthReportList);
      break;
    case 2:
      clearScreen();
      printf("Terima kasih!\n");
      exit(0);
      break;
    default:
      printf("Pilihan tidak valid. ");
      waitForEnter();
      break;
    }
  }
}
