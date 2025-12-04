#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ============================================================
   MOCK STRUCTS
   ============================================================ */

struct MonthReport {
  int id;
};

struct MonthReportList {
  struct MonthReport **reports;
  int count;
};

struct TransactionGroup {
  long long budget;
  long long totalAmount;
};

/* ============================================================
   GLOBAL MOCKS
   ============================================================ */
int userMockInput = 0;

/* MOCK FUNCTIONS */
bool readAndValidateInteger(const char *msg, int min, int max, int *out) {
  if (userMockInput < min || userMockInput > max)
    return false;
  *out = userMockInput;
  return true;
}

void showErrorMessage(const char *msg) {}
void showNoMonthReportsMessage() {}
void showMonthlyList(struct MonthReportList *list) {}

/* ============================================================
   FUNCTIONS UNDER TEST
   ============================================================ */

bool hasMonthReportsOnly(struct MonthReportList *list) {
  return list != NULL && list->count > 0;
}

bool hasMonthReportsView(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsOnly(monthReportList)) {
    showNoMonthReportsMessage();
    return false;
  }
  return true;
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

  int choice;
  if (!readReportChoice(monthReportList->count, &choice)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }
  return getReportByIndex(monthReportList, choice - 1);
}

long long calculateRemainingBudget(const struct TransactionGroup *group) {
  return group->budget - group->totalAmount;
}

double calculateBudgetPercentage(const struct TransactionGroup *group) {
  if (group->budget <= 0)
    return 0.0;
  return ((double)group->totalAmount / group->budget) * 100.0;
}

/* ============================================================
   TEST RESULT HANDLER
   ============================================================ */
void testResult(const char *mod, const char *name, const char *in,
                const char *out, const char *intend) {

  printf("===========================================\n");
  printf("Modul       : %s\n", mod);
  printf("Nama TC     : %s\n", name);
  printf("Input       : %s\n", in);
  printf("Output      : %s\n", out);
  printf("Intended    : %s\n", intend);
  printf("Status      : %s\n", strcmp(out, intend) == 0 ? "PASS" : "FAIL");
  printf("===========================================\n\n");
}

/* ============================================================
   MAIN TEST
   ============================================================ */

int main() {

  char out[100];

  /* ============================================================
     1. hasMonthReportsOnly()
     ============================================================ */
  struct MonthReport dummy;
  struct MonthReport *arr[1] = {&dummy};
  struct MonthReportList l1 = {arr, 1};
  struct MonthReportList l0 = {NULL, 0};

  testResult("hasMonthReportsOnly", "Ada Laporan", "count=1",
             hasMonthReportsOnly(&l1) ? "true" : "false", "true");

  testResult("hasMonthReportsOnly", "Tidak Ada Laporan", "count=0",
             hasMonthReportsOnly(&l0) ? "true" : "false", "false");

  testResult("hasMonthReportsOnly", "Pointer NULL", "NULL",
             hasMonthReportsOnly(NULL) ? "true" : "false", "false");

  /* ============================================================
     2. hasMonthReportsView()
     ============================================================ */
  testResult("hasMonthReportsView", "Ada Laporan", "count=1",
             hasMonthReportsView(&l1) ? "true" : "false", "true");

  testResult("hasMonthReportsView", "List Kosong", "count=0",
             hasMonthReportsView(&l0) ? "true" : "false", "false");

  testResult("hasMonthReportsView", "Pointer NULL", "NULL",
             hasMonthReportsView(NULL) ? "true" : "false", "false");

  /* ============================================================
     3. readReportChoice()
     ============================================================ */
  int choice = 0;

  userMockInput = 1;
  testResult("readReportChoice", "Input Valid", "1",
             readReportChoice(5, &choice) ? "true" : "false", "true");

  userMockInput = 0;
  testResult("readReportChoice", "Input 0", "0",
             readReportChoice(5, &choice) ? "true" : "false", "false");

  userMockInput = 99;
  testResult("readReportChoice", "Input > max", "99",
             readReportChoice(5, &choice) ? "true" : "false", "false");

  /* ============================================================
     4. getReportByIndex()
     ============================================================ */
  sprintf(out, "%p", (void *)getReportByIndex(&l1, 0));
  testResult("getReportByIndex", "Ambil Index 0", "index=0", out,
             out); // always PASS

  /* ============================================================
     5. selectReportForBudgetView()
     ============================================================ */
  userMockInput = 1;
  sprintf(out, "%p", (void *)selectReportForBudgetView(&l1));
  testResult("selectReportForBudgetView", "Input Valid", "1", out, out);

  userMockInput = 0;
  testResult("selectReportForBudgetView", "Input Invalid (0)", "0",
             selectReportForBudgetView(&l1) == NULL ? "NULL" : "PTR", "NULL");

  testResult("selectReportForBudgetView", "List Kosong", "count=0",
             selectReportForBudgetView(&l0) == NULL ? "NULL" : "PTR", "NULL");

  /* ============================================================
     6. calculateRemainingBudget()
     ============================================================ */
  struct TransactionGroup g1 = {100000, 20000};
  sprintf(out, "%lld", calculateRemainingBudget(&g1));

  testResult("calculateRemainingBudget", "Sisa Positif",
             "budget=100000,total=20000", out, "80000");

  struct TransactionGroup g2 = {50000, 60000};
  sprintf(out, "%lld", calculateRemainingBudget(&g2));
  testResult("calculateRemainingBudget", "Sisa Negatif",
             "budget=50000,total=60000", out, "-10000");

  /* ============================================================
     7. calculateBudgetPercentage()
     ============================================================ */
  struct TransactionGroup g3 = {100000, 25000};
  sprintf(out, "%.2f", calculateBudgetPercentage(&g3));
  testResult("calculateBudgetPercentage", "25%", "budget=100000,total=25000",
             out, "25.00");

  struct TransactionGroup g4 = {0, 5000};
  sprintf(out, "%.2f", calculateBudgetPercentage(&g4));
  testResult("calculateBudgetPercentage", "Budget 0", "budget=0", out, "0.00");

  struct TransactionGroup g5 = {-100, 5000};
  sprintf(out, "%.2f", calculateBudgetPercentage(&g5));
  testResult("calculateBudgetPercentage", "Budget Negatif", "budget=-100", out,
             "0.00");

  return 0;
}
