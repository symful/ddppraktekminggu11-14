#include "../types/transaction_category.h"
#include "../types/transaction_type.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERS_DIR "./users"

struct User {
  char username[50];
  char userDir[256];
  char hashFile[256];
  int isAdmin;
};

struct User *currentUser = NULL;

void setup_test_user() {
  system("mkdir -p ./users/testuser");

  currentUser = (struct User *)malloc(sizeof(struct User));
  strcpy(currentUser->username, "testuser");
  strcpy(currentUser->userDir, "./users/testuser");
  currentUser->isAdmin = 0;
}

void cleanup_test_user() {
  if (currentUser != NULL) {
    free(currentUser);
    currentUser = NULL;
  }
  system("rm -rf ./users");
}

void test_create_category_list() {
  printf("\n=== Test: Create Category List ===\n");

  struct CategoryList *list = createCategoryList();
  assert(list != NULL);
  assert(list->count == 0);
  assert(list->capacity == 10);
  assert(list->items != NULL);

  freeCategoryList(list);
  printf("✓ Category list creation successful\n");
}

void test_initialize_default_categories() {
  printf("\n=== Test: Initialize Default Categories ===\n");

  struct CategoryList *list = initializeDefaultCategories();
  assert(list != NULL);
  assert(list->count == 7);

  int incomeCount = 0;
  int expenseCount = 0;

  for (int i = 0; i < list->count; i++) {
    assert(list->items[i] != NULL);
    assert(strlen(list->items[i]->internalName) > 0);
    assert(strlen(list->items[i]->displayName) > 0);
    assert(list->items[i]->isDefault == 1);

    if (list->items[i]->type == TRANSACTION_INCOME) {
      incomeCount++;
    } else {
      expenseCount++;
    }

    printf("  - %s (%s) - %s\n", list->items[i]->internalName,
           list->items[i]->displayName,
           list->items[i]->type == TRANSACTION_INCOME ? "INCOME" : "EXPENSE");
  }

  assert(incomeCount == 1);
  assert(expenseCount == 6);

  freeCategoryList(list);
  printf("✓ Default categories initialized correctly\n");
}

void test_add_custom_category() {
  printf("\n=== Test: Add Custom Category ===\n");

  struct CategoryList *list = createCategoryList();

  int result =
      addCategoryToList(list, "freelance", "Freelance", TRANSACTION_INCOME, 0);
  assert(result == 1);
  assert(list->count == 1);

  struct CategoryItem *item = list->items[0];
  assert(strcmp(item->internalName, "freelance") == 0);
  assert(strcmp(item->displayName, "Freelance") == 0);
  assert(item->type == TRANSACTION_INCOME);
  assert(item->isDefault == 0);

  freeCategoryList(list);
  printf("✓ Custom category added successfully\n");
}

void test_normalize_category_name() {
  printf("\n=== Test: Normalize Category Name ===\n");

  char name1[] = "FOOD";
  normalizeCategoryName(name1);
  assert(strcmp(name1, "food") == 0);

  char name2[] = "Transportation";
  normalizeCategoryName(name2);
  assert(strcmp(name2, "transportation") == 0);

  char name3[] = "FrEeLaNcE";
  normalizeCategoryName(name3);
  assert(strcmp(name3, "freelance") == 0);

  printf("✓ Category names normalized correctly\n");
}

void test_find_category() {
  printf("\n=== Test: Find Category ===\n");

  struct CategoryList *list = initializeDefaultCategories();

  struct CategoryItem *found = findCategoryByInternalName(list, "food");
  assert(found != NULL);
  assert(strcmp(found->internalName, "food") == 0);

  found = findCategoryByInternalName(list, "FOOD");
  assert(found != NULL);

  found = findCategoryByInternalName(list, "nonexistent");
  assert(found == NULL);

  freeCategoryList(list);
  printf("✓ Category finding works correctly\n");
}

void test_category_exists() {
  printf("\n=== Test: Category Exists ===\n");

  struct CategoryList *list = initializeDefaultCategories();

  assert(categoryExistsInList(list, "food") == 1);
  assert(categoryExistsInList(list, "income") == 1);
  assert(categoryExistsInList(list, "nonexistent") == 0);

  freeCategoryList(list);
  printf("✓ Category existence check works\n");
}

void test_remove_category() {
  printf("\n=== Test: Remove Category ===\n");

  struct CategoryList *list = createCategoryList();
  addCategoryToList(list, "custom1", "Custom 1", TRANSACTION_EXPENSE, 0);
  addCategoryToList(list, "custom2", "Custom 2", TRANSACTION_EXPENSE, 0);
  addCategoryToList(list, "default1", "Default 1", TRANSACTION_EXPENSE, 1);

  assert(list->count == 3);

  int result = removeCategoryFromList(list, "custom1");
  assert(result == 1);
  assert(list->count == 2);
  assert(categoryExistsInList(list, "custom1") == 0);

  result = removeCategoryFromList(list, "default1");
  assert(result == 0);
  assert(list->count == 2);

  freeCategoryList(list);
  printf("✓ Category removal works (protects defaults)\n");
}

void test_filter_categories_by_type() {
  printf("\n=== Test: Filter Categories by Type ===\n");

  struct CategoryList *list = initializeDefaultCategories();

  struct CategoryList *incomeCategories =
      getCategoriesByType(list, TRANSACTION_INCOME);
  assert(incomeCategories != NULL);
  assert(incomeCategories->count == 1);

  for (int i = 0; i < incomeCategories->count; i++) {
    assert(incomeCategories->items[i]->type == TRANSACTION_INCOME);
    printf("  Income: %s\n", incomeCategories->items[i]->displayName);
  }

  struct CategoryList *expenseCategories =
      getCategoriesByType(list, TRANSACTION_EXPENSE);
  assert(expenseCategories != NULL);
  assert(expenseCategories->count == 6);

  for (int i = 0; i < expenseCategories->count; i++) {
    assert(expenseCategories->items[i]->type == TRANSACTION_EXPENSE);
    printf("  Expense: %s\n", expenseCategories->items[i]->displayName);
  }

  freeCategoryList(incomeCategories);
  freeCategoryList(expenseCategories);
  freeCategoryList(list);
  printf("✓ Category filtering by type works\n");
}

void test_validate_category_name() {
  printf("\n=== Test: Validate Category Name ===\n");

  assert(validateCategoryName("food") == 1);
  assert(validateCategoryName("my category") == 1);
  assert(validateCategoryName("category_1") == 1);
  assert(validateCategoryName("") == 0);
  assert(validateCategoryName(NULL) == 0);

  char longName[60];
  memset(longName, 'a', 55);
  longName[55] = '\0';
  assert(validateCategoryName(longName) == 0);

  assert(validateCategoryName("invalid@name") == 0);
  assert(validateCategoryName("invalid#name") == 0);

  printf("✓ Category name validation works\n");
}

void test_save_and_load_categories() {
  printf("\n=== Test: Save and Load Categories ===\n");

  setup_test_user();

  struct CategoryList *original = initializeDefaultCategories();
  addCategoryToList(original, "freelance", "Freelance", TRANSACTION_INCOME, 0);
  addCategoryToList(original, "investment", "Investasi", TRANSACTION_INCOME, 0);

  int originalCount = original->count;

  saveUserCategories(original);

  refreshCategoryCache();

  struct CategoryList *loaded = loadUserCategories();
  assert(loaded != NULL);
  assert(loaded->count == originalCount);

  assert(categoryExistsInList(loaded, "freelance") == 1);
  assert(categoryExistsInList(loaded, "investment") == 1);
  assert(categoryExistsInList(loaded, "food") == 1);

  struct CategoryItem *item = findCategoryByInternalName(loaded, "freelance");
  assert(item != NULL);
  assert(strcmp(item->displayName, "Freelance") == 0);
  assert(item->type == TRANSACTION_INCOME);
  assert(item->isDefault == 0);

  freeCategoryList(original);
  freeCategoryList(loaded);
  cleanup_test_user();

  printf("✓ Save and load categories works\n");
}

void test_get_display_name() {
  printf("\n=== Test: Get Display Name ===\n");

  struct CategoryList *list = initializeDefaultCategories();

  const char *displayName = getCategoryDisplayName(list, "food");
  assert(strcmp(displayName, "Makanan") == 0);

  displayName = getCategoryDisplayName(list, "income");
  assert(strcmp(displayName, "Pendapatan") == 0);

  displayName = getCategoryDisplayName(list, "nonexistent");
  assert(strcmp(displayName, "nonexistent") == 0);

  freeCategoryList(list);
  printf("✓ Display name retrieval works\n");
}

void test_category_cache() {
  printf("\n=== Test: Category Cache ===\n");

  setup_test_user();

  struct CategoryList *cat1 = getUserCategoriesCache();
  struct CategoryList *cat2 = getUserCategoriesCache();

  assert(cat1 == cat2);
  assert(cat1 != NULL);

  int originalCount = cat1->count;

  refreshCategoryCache();

  struct CategoryList *cat3 = getUserCategoriesCache();
  assert(cat3 != NULL);
  assert(cat3->count == originalCount);

  cleanup_test_user();

  printf("✓ Category cache works correctly\n");
}

void test_buffer_overflow_protection() {
  printf("\n=== Test: Buffer Overflow Protection ===\n");

  struct CategoryList *list = createCategoryList();

  char longName[100];
  memset(longName, 'a', 99);
  longName[99] = '\0';

  int result =
      addCategoryToList(list, longName, "Display", TRANSACTION_EXPENSE, 0);
  assert(result == 1);

  struct CategoryItem *item = list->items[0];
  assert(strlen(item->internalName) < 50);
  assert(item->internalName[49] == '\0');

  freeCategoryList(list);
  printf("✓ Buffer overflow protection works\n");
}

int main() {
  printf("\n");
  printf("╔══════════════════════════════════════════════════════╗\n");
  printf("║      CATEGORY SYSTEM TEST SUITE                     ║\n");
  printf("╚══════════════════════════════════════════════════════╝\n");

  test_create_category_list();
  test_initialize_default_categories();
  test_add_custom_category();
  test_normalize_category_name();
  test_find_category();
  test_category_exists();
  test_remove_category();
  test_filter_categories_by_type();
  test_validate_category_name();
  test_save_and_load_categories();
  test_get_display_name();
  test_category_cache();
  test_buffer_overflow_protection();

  printf("\n");
  printf("╔══════════════════════════════════════════════════════╗\n");
  printf("║      ✓ ALL TESTS PASSED                             ║\n");
  printf("╚══════════════════════════════════════════════════════╝\n");
  printf("\n");

  return 0;
}
