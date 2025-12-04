#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================
   MOCK STRUCTS (mengikuti project)
   ================================ */
struct MonthReport {
  int month;
};
struct MonthReportList {
  struct MonthReport **reports;
  int count;
};
struct CategoryList {
  int count;
};

/* ================================
   MOCK FUNCTIONS (UI Dependent)
   ================================ */
int userMockInput = 0;

bool readAndValidateInteger(const char *msg, int min, int max, int *out) {
  if (userMockInput < min || userMockInput > max)
    return false;
  *out = userMockInput;
  return true;
}

void showErrorMessage(const char *msg) { printf("[ERROR] %s\n", msg); }

void showMonthlyList(struct MonthReportList *list) {
  printf("[SHOW LIST] (%d reports)\n", list->count);
}

bool readMonthReportChoice(int max, int *out) {
  if (userMockInput < 1 || userMockInput > max)
    return false;
  *out = userMockInput;
  return true;
}

/* ==========================================
   IMPORT FUNGSI ASLI YANG AKAN DI-TEST
   ========================================== */
int getValidatedMenuChoice(int minChoice, int maxChoice) {
  int choice;
  if (!readAndValidateInteger("", minChoice, maxChoice, &choice)) {
    showErrorMessage("Input tidak valid atau terlalu banyak percobaan.");
    return -1;
  }
  return choice;
}
bool hasMonthReports(struct MonthReportList *monthReportList) {
  return monthReportList != NULL && monthReportList->count > 0;
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

/* ================================
       TEST RESULT FORMAT
   ================================ */

void testResult(const char *modul, const char *namaTC, const char *input,
                const char *output, const char *intended) {

  printf("===========================================\n");
  printf("Modul       : %s\n", modul);
  printf("Nama TC     : %s\n", namaTC);
  printf("Input       : %s\n", input);
  printf("Output      : %s\n", output);
  printf("Intended    : %s\n", intended);
  printf("Status      : %s\n", strcmp(output, intended) == 0 ? "PASS" : "FAIL");
  printf("===========================================\n\n");
}

/* ================================
   MAIN TEST
   ================================ */

int main() {
  char buf[50];

  /* ====================================================
     1. getValidatedMenuChoice() — Total: 10 Test Case
     ==================================================== */

  // TC 1 - Input valid tengah
  userMockInput = 5;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Input Valid Tengah", "5", buf, "5");

  // TC 2 - Input minimal boundary
  userMockInput = 1;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Boundary Min", "1", buf, "1");

  // TC 3 - Input maksimal boundary
  userMockInput = 10;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Boundary Max", "10", buf, "10");

  // TC 4 - Input < min
  userMockInput = -5;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Input Kurang Dari Min", "-5", buf,
             "-1");

  // TC 5 - Input > max
  userMockInput = 999;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Input Lebih Dari Max", "999", buf,
             "-1");

  // TC 6 - Input nol untuk range mulai 1
  userMockInput = 0;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Input Nol", "0", buf, "-1");

  // TC 7 - Input negatif besar
  userMockInput = -9999;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 10));
  testResult("getValidatedMenuChoice", "Input Negatif Besar", "-9999", buf,
             "-1");

  // TC 8 - Input berupa 'karakter' (simulate failed)
  userMockInput = 123456; // mock karakternya invalid
  sprintf(buf, "%d", getValidatedMenuChoice(1, 5));
  testResult("getValidatedMenuChoice", "Input Karakter (simulasi gagal)", "X",
             buf, "-1");

  // TC 9 - Range kecil (1–1), input benar
  userMockInput = 1;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 1));
  testResult("getValidatedMenuChoice", "Range 1–1 Valid", "1", buf, "1");

  // TC 10 - Range kecil (1–1), input salah
  userMockInput = 2;
  sprintf(buf, "%d", getValidatedMenuChoice(1, 1));
  testResult("getValidatedMenuChoice", "Range 1–1 Invalid", "2", buf, "-1");

  /* ====================================================
     2. hasMonthReports() — Total: 6 Test Case
     ==================================================== */

  struct MonthReport dummy;
  struct MonthReport *arrOne[1] = {&dummy};

  struct MonthReportList emptyList = {NULL, 0};
  struct MonthReportList oneList = {arrOne, 1};
  struct MonthReportList bigList = {arrOne, 9999}; // simulate banyak data

  testResult("hasMonthReports", "NULL Pointer", "NULL",
             hasMonthReports(NULL) ? "true" : "false", "false");

  testResult("hasMonthReports", "List Kosong", "count=0",
             hasMonthReports(&emptyList) ? "true" : "false", "false");

  testResult("hasMonthReports", "List Ada 1", "count=1",
             hasMonthReports(&oneList) ? "true" : "false", "true");

  testResult("hasMonthReports", "List Banyak", "count=9999",
             hasMonthReports(&bigList) ? "true" : "false", "true");

  emptyList.count = -1;
  testResult("hasMonthReports", "Count Negatif", "count=-1",
             hasMonthReports(&emptyList) ? "true" : "false", "false");

  emptyList.count = 9999999;
  testResult("hasMonthReports", "Count Sangat Besar", "count=9999999",
             hasMonthReports(&emptyList) ? "true" : "false", "true");

  /* ====================================================
     3. selectMonthReport() — Total: 7 Test Case
     ==================================================== */

  userMockInput = 1;
  testResult("selectMonthReport", "Select Index Valid", "1",
             selectMonthReport(&oneList) != NULL ? "Report Returned" : "NULL",
             "Report Returned");

  userMockInput = 0;
  testResult("selectMonthReport", "Select 0", "0",
             selectMonthReport(&oneList) != NULL ? "Report Returned" : "NULL",
             "NULL");

  userMockInput = -5;
  testResult("selectMonthReport", "Select Negatif", "-5",
             selectMonthReport(&oneList) != NULL ? "Report Returned" : "NULL",
             "NULL");

  userMockInput = 99;
  testResult("selectMonthReport", "Select Over Index", "99",
             selectMonthReport(&oneList) != NULL ? "Report Returned" : "NULL",
             "NULL");

  struct MonthReportList twoList;
  struct MonthReport r1, r2;
  struct MonthReport *twArr[2] = {&r1, &r2};
  twoList.count = 2;
  twoList.reports = twArr;

  userMockInput = 2;
  testResult("selectMonthReport", "Select Element Kedua", "2",
             selectMonthReport(&twoList) != NULL ? "Report Returned" : "NULL",
             "Report Returned");

  struct MonthReportList zeroList = {NULL, 0};
  userMockInput = 1;
  testResult("selectMonthReport", "Select on Empty List", "1",
             selectMonthReport(&zeroList) != NULL ? "Report Returned" : "NULL",
             "NULL");

  userMockInput = 1;
  testResult("selectMonthReport", "Select dengan Pointer NULL", "1",
             selectMonthReport(NULL) != NULL ? "Report Returned" : "NULL",
             "NULL");

  /* ====================================================
     4. hasCategories() — 5 Test Case
     ==================================================== */
  struct CategoryList c0 = {0};
  struct CategoryList c1 = {1};
  struct CategoryList cBig = {999};

  testResult("hasCategories", "NULL Pointer", "NULL",
             hasCategories(NULL) ? "true" : "false", "false");
  testResult("hasCategories", "0 Kategori", "count=0",
             hasCategories(&c0) ? "true" : "false", "false");
  testResult("hasCategories", "1 Kategori", "count=1",
             hasCategories(&c1) ? "true" : "false", "true");
  testResult("hasCategories", "Kategori Banyak", "count=999",
             hasCategories(&cBig) ? "true" : "false", "true");
  c0.count = -1;
  testResult("hasCategories", "Count Negatif", "count=-1",
             hasCategories(&c0) ? "true" : "false", "false");

  /* ====================================================
     5. ensureCategoriesAvailable() — 6 Test Case
     ==================================================== */

  struct CategoryList none = {0};
  struct CategoryList some = {5};

  testResult("ensureCategoriesAvailable", "List Kosong", "count=0",
             ensureCategoriesAvailable(&none) ? "true" : "false", "false");

  testResult("ensureCategoriesAvailable", "List Ada", "count=5",
             ensureCategoriesAvailable(&some) ? "true" : "false", "true");

  testResult("ensureCategoriesAvailable", "Pointer NULL", "NULL",
             ensureCategoriesAvailable(NULL) ? "true" : "false", "false");

  none.count = -10;
  testResult("ensureCategoriesAvailable", "Count Negatif", "count=-10",
             ensureCategoriesAvailable(&none) ? "true" : "false", "false");

  some.count = 999;
  testResult("ensureCategoriesAvailable", "Kategori Banyak", "count=999",
             ensureCategoriesAvailable(&some) ? "true" : "false", "true");

  some.count = 1;
  testResult("ensureCategoriesAvailable", "Kategori Satu", "count=1",
             ensureCategoriesAvailable(&some) ? "true" : "false", "true");

  return 0;
}
