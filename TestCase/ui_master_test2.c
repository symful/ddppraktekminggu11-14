#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===============================================================
   MOCK STRUCTS
   =============================================================== */

struct CategoryItem {
  char internalName[50];
  char displayName[50];
};

struct CategoryList {
  struct CategoryItem **items;
  int count;
};

struct MonthReportList {
  int count;
};

/* ===============================================================
   GLOBAL MOCKS & FLAGS
   =============================================================== */

int userMockInput = 0;
long long mockBudgetInput = 0;
int mockBudgetResult = 0; // INPUT_SUCCESS = 0

/* ===============================================================
   MOCKED FUNCTIONS
   =============================================================== */

bool readAndValidateInteger(const char *msg, int min, int max, int *out) {
  if (userMockInput < min || userMockInput > max)
    return false;
  *out = userMockInput;
  return true;
}

void showErrorMessage(const char *msg) {}
void showInfoMessage(const char *msg) {}

void printCategoryHeader() {}
void printCategoryList(struct CategoryList *c) {}

void copyCategoryInternalName(char *out, const char *src, size_t size) {
  strncpy(out, src, size);
}

struct CategoryList *mockCategoryCache = NULL;
struct CategoryList *getUserCategoriesCache() { return mockCategoryCache; }

const char *getCategoryDisplayName(struct CategoryList *list, const char *key) {
  if (!list)
    return NULL;
  for (int i = 0; i < list->count; i++) {
    if (strcmp(list->items[i]->internalName, key) == 0)
      return list->items[i]->displayName;
  }
  return NULL;
}

typedef enum { INPUT_SUCCESS = 0, INPUT_FAIL = 1 } InputResult;

InputResult promptForBudget(const char *msg, long long *out) {
  if (mockBudgetResult != INPUT_SUCCESS)
    return INPUT_FAIL;
  *out = mockBudgetInput;
  return INPUT_SUCCESS;
}

/* ===============================================================
   FUNCTIONS TO TEST
   =============================================================== */

bool readCategorySelection(int max, int *choice) {
  return readAndValidateInteger("\n🎯 Pilihan kategori: ", 1, max, choice);
}

bool selectCategory(char *outCategory, size_t bufferSize) {
  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL || categories->count == 0)
    return false;

  int categoryChoice;
  if (!readCategorySelection(categories->count, &categoryChoice))
    return false;

  copyCategoryInternalName(outCategory,
                           categories->items[categoryChoice - 1]->internalName,
                           bufferSize);
  return true;
}

bool inputNewBudget(long long *outBudget) {
  InputResult r = promptForBudget("💰 Masukkan budget baru (Rp): ", outBudget);
  if (r != INPUT_SUCCESS)
    return false;
  return true;
}

const char *getBudgetCategoryDisplayName(const char *category) {
  struct CategoryList *categories = getUserCategoriesCache();
  return getCategoryDisplayName(categories, category);
}

bool ensureHasMonthReports(struct MonthReportList *list) {
  if (list == NULL || list->count == 0)
    return false;
  return true;
}

/* ===============================================================
   TEST RESULT PRINTER
   =============================================================== */

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

/* ===============================================================
   MAIN TEST CASES
   =============================================================== */

int main() {

  char out[100];

  /* ===========================================================
     1. readCategorySelection()
     =========================================================== */

  int choice;
  userMockInput = 2;
  testResult("readCategorySelection", "Input Valid", "2",
             readCategorySelection(5, &choice) ? "true" : "false", "true");

  userMockInput = 0;
  testResult("readCategorySelection", "Input 0", "0",
             readCategorySelection(5, &choice) ? "true" : "false", "false");

  userMockInput = 99;
  testResult("readCategorySelection", "Input > max", "99",
             readCategorySelection(5, &choice) ? "true" : "false", "false");

  userMockInput = -3;
  testResult("readCategorySelection", "Input negatif", "-3",
             readCategorySelection(5, &choice) ? "true" : "false", "false");

  /* ===========================================================
     2. selectCategory()
     =========================================================== */

  struct CategoryItem c1 = {"food", "Makanan"};
  struct CategoryItem c2 = {"drink", "Minuman"};
  struct CategoryItem *arr[2] = {&c1, &c2};
  struct CategoryList list = {arr, 2};

  mockCategoryCache = &list;

  userMockInput = 1;
  memset(out, 0, sizeof(out));
  selectCategory(out, 100);
  testResult("selectCategory", "Pilih Kategori 1", "1", out, "food");

  userMockInput = 2;
  memset(out, 0, sizeof(out));
  selectCategory(out, 100);
  testResult("selectCategory", "Pilih Kategori 2", "2", out, "drink");

  mockCategoryCache = NULL;
  memset(out, 0, sizeof(out));
  testResult("selectCategory", "Cache NULL", "X",
             selectCategory(out, 100) ? "true" : "false", "false");

  mockCategoryCache = &list;
  list.count = 0;
  testResult("selectCategory", "Tidak Ada Kategori", "X",
             selectCategory(out, 100) ? "true" : "false", "false");

  /* ===========================================================
     3. inputNewBudget()
     =========================================================== */

  mockBudgetInput = 500000;
  mockBudgetResult = INPUT_SUCCESS;
  long long b;
  inputNewBudget(&b);
  sprintf(out, "%lld", b);
  testResult("inputNewBudget", "Budget Valid", "500000", out, "500000");

  mockBudgetResult = INPUT_FAIL;
  testResult("inputNewBudget", "Budget Invalid", "abc",
             inputNewBudget(&b) ? "true" : "false", "false");

  /* ===========================================================
     4. getBudgetCategoryDisplayName()
     =========================================================== */

  mockCategoryCache = &list;
  list.count = 2;

  testResult("getBudgetCategoryDisplayName", "Nama Ada", "food",
             getBudgetCategoryDisplayName("food"), "Makanan");

  testResult("getBudgetCategoryDisplayName", "Nama Tidak Ada", "invalid",
             getBudgetCategoryDisplayName("invalid") ?: "NULL", "NULL");

  mockCategoryCache = NULL;
  testResult("getBudgetCategoryDisplayName", "Cache NULL", "food",
             getBudgetCategoryDisplayName("food") ?: "NULL", "NULL");

  /* ===========================================================
     5. ensureHasMonthReports()
     =========================================================== */

  struct MonthReportList m1 = {5};
  testResult("ensureHasMonthReports", "Ada Laporan", "count=5",
             ensureHasMonthReports(&m1) ? "true" : "false", "true");

  struct MonthReportList m2 = {0};
  testResult("ensureHasMonthReports", "List Kosong", "count=0",
             ensureHasMonthReports(&m2) ? "true" : "false", "false");

  testResult("ensureHasMonthReports", "Pointer NULL", "NULL",
             ensureHasMonthReports(NULL) ? "true" : "false", "false");

  return 0;
}
