#include "./transaction_category.h"
#include "../auth/auth.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef TRANSACTION_CATEGORY_C
#define TRANSACTION_CATEGORY_C

static struct CategoryList *cachedUserCategories = NULL;

struct CategoryList *createCategoryList() {
  struct CategoryList *list =
      (struct CategoryList *)malloc(sizeof(struct CategoryList));
  if (list == NULL) {
    return NULL;
  }

  list->capacity = 10;
  list->count = 0;
  list->items = (struct CategoryItem **)malloc(list->capacity *
                                               sizeof(struct CategoryItem *));

  if (list->items == NULL) {
    free(list);
    return NULL;
  }

  return list;
}

void freeCategoryList(struct CategoryList *list) {
  if (list == NULL) {
    return;
  }

  if (list->items != NULL) {
    for (int i = 0; i < list->count; i++) {
      if (list->items[i] != NULL) {
        free(list->items[i]);
      }
    }
    free(list->items);
  }

  free(list);
}

void normalizeCategoryName(char *name) {
  if (name == NULL)
    return;

  for (int i = 0; name[i]; i++) {
    name[i] = tolower((unsigned char)name[i]);
  }
}

int validateCategoryName(const char *name) {
  if (name == NULL || strlen(name) == 0) {
    return 0;
  }

  size_t len = strlen(name);
  if (len >= 50) {
    return 0;
  }

  for (size_t i = 0; i < len; i++) {
    if (!isalnum((unsigned char)name[i]) && name[i] != ' ' && name[i] != '_') {
      return 0;
    }
  }

  return 1;
}

int readCategoryNameSafe(char *buffer, size_t bufferSize, const char *prompt) {
  printf("%s", prompt);

  if (fgets(buffer, bufferSize, stdin) == NULL) {
    return 0;
  }

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
    len--;
  }

  if (len == 0) {
    return 0;
  }

  if (len >= bufferSize - 1) {
    buffer[bufferSize - 1] = '\0';
  }

  return validateCategoryName(buffer);
}

struct CategoryList *initializeDefaultCategories() {
  struct CategoryList *list = createCategoryList();
  if (list == NULL) {
    return NULL;
  }

  const char *defaults[][3] = {
      {"transportation", "Transportasi", "EXPENSE"},
      {"work", "Pekerjaan", "EXPENSE"},
      {"food", "Makanan", "EXPENSE"},
      {"entertainment", "Hiburan", "EXPENSE"},
      {"shopping", "Belanja", "EXPENSE"},
      {"other", "Lainnya", "EXPENSE"},
      {"income", "Pendapatan", "INCOME"},
  };

  int defaultCount = sizeof(defaults) / sizeof(defaults[0]);

  for (int i = 0; i < defaultCount; i++) {
    enum TransactionType type = (strcmp(defaults[i][2], "INCOME") == 0)
                                    ? TRANSACTION_INCOME
                                    : TRANSACTION_EXPENSE;
    addCategoryToList(list, defaults[i][0], defaults[i][1], type, 1);
  }

  return list;
}

int addCategoryToList(struct CategoryList *list, const char *internalName,
                      const char *displayName, enum TransactionType type,
                      int isDefault) {
  if (list == NULL || internalName == NULL || displayName == NULL) {
    return 0;
  }

  if (list->count >= list->capacity) {
    int newCapacity = list->capacity * 2;
    struct CategoryItem **newItems = (struct CategoryItem **)realloc(
        list->items, newCapacity * sizeof(struct CategoryItem *));

    if (newItems == NULL) {
      return 0;
    }

    list->items = newItems;
    list->capacity = newCapacity;
  }

  struct CategoryItem *item =
      (struct CategoryItem *)malloc(sizeof(struct CategoryItem));
  if (item == NULL) {
    return 0;
  }

  strncpy(item->internalName, internalName, sizeof(item->internalName) - 1);
  item->internalName[sizeof(item->internalName) - 1] = '\0';
  normalizeCategoryName(item->internalName);

  strncpy(item->displayName, displayName, sizeof(item->displayName) - 1);
  item->displayName[sizeof(item->displayName) - 1] = '\0';

  item->type = type;
  item->isDefault = isDefault;

  list->items[list->count] = item;
  list->count++;

  return 1;
}

struct CategoryItem *findCategoryByInternalName(struct CategoryList *list,
                                                const char *internalName) {
  if (list == NULL || internalName == NULL) {
    return NULL;
  }

  char normalizedName[50];
  strncpy(normalizedName, internalName, sizeof(normalizedName) - 1);
  normalizedName[sizeof(normalizedName) - 1] = '\0';
  normalizeCategoryName(normalizedName);

  for (int i = 0; i < list->count; i++) {
    if (list->items[i] != NULL &&
        strcmp(list->items[i]->internalName, normalizedName) == 0) {
      return list->items[i];
    }
  }

  return NULL;
}

int categoryExistsInList(struct CategoryList *list, const char *internalName) {
  return findCategoryByInternalName(list, internalName) != NULL;
}

int removeCategoryFromList(struct CategoryList *list,
                           const char *internalName) {
  if (list == NULL || internalName == NULL) {
    return 0;
  }

  char normalizedName[50];
  strncpy(normalizedName, internalName, sizeof(normalizedName) - 1);
  normalizedName[sizeof(normalizedName) - 1] = '\0';
  normalizeCategoryName(normalizedName);

  for (int i = 0; i < list->count; i++) {
    if (list->items[i] != NULL &&
        strcmp(list->items[i]->internalName, normalizedName) == 0) {

      if (list->items[i]->isDefault) {
        return 0;
      }

      free(list->items[i]);

      for (int j = i; j < list->count - 1; j++) {
        list->items[j] = list->items[j + 1];
      }

      list->count--;
      return 1;
    }
  }

  return 0;
}

struct CategoryList *getCategoriesByType(struct CategoryList *source,
                                         enum TransactionType type) {
  if (source == NULL) {
    return NULL;
  }

  struct CategoryList *filtered = createCategoryList();
  if (filtered == NULL) {
    return NULL;
  }

  for (int i = 0; i < source->count; i++) {
    if (source->items[i] != NULL && source->items[i]->type == type) {
      addCategoryToList(filtered, source->items[i]->internalName,
                        source->items[i]->displayName, source->items[i]->type,
                        source->items[i]->isDefault);
    }
  }

  return filtered;
}

const char *getCategoryDisplayName(struct CategoryList *list,
                                   const char *internalName) {
  struct CategoryItem *item = findCategoryByInternalName(list, internalName);
  if (item != NULL) {
    return item->displayName;
  }
  return internalName;
}

void saveUserCategories(struct CategoryList *categories) {
  if (categories == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char filepath[512];
  char tempPath[512];

  snprintf(filepath, sizeof(filepath), "%s/%s/categories.txt", USERS_DIR,
           currentUser->username);
  snprintf(tempPath, sizeof(tempPath), "%s/%s/categories.tmp", USERS_DIR,
           currentUser->username);

  FILE *file = fopen(tempPath, "w");
  if (file == NULL) {
    perror("Failed to create temp category file");
    return;
  }

  fprintf(file, "CATEGORY_FILE\n");
  fprintf(file, "VERSION=1\n");
  fprintf(file, "COUNT=%d\n", categories->count);

  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i] != NULL) {
      fprintf(file, "CATEGORY_START\n");
      fprintf(file, "INTERNAL_NAME=%s\n", categories->items[i]->internalName);
      fprintf(file, "DISPLAY_NAME=%s\n", categories->items[i]->displayName);
      fprintf(file, "TYPE=%d\n", categories->items[i]->type);
      fprintf(file, "IS_DEFAULT=%d\n", categories->items[i]->isDefault);
      fprintf(file, "CATEGORY_END\n");
    }
  }

  fclose(file);

  if (rename(tempPath, filepath) != 0) {
    perror("Failed to save categories");
    remove(tempPath);
  }
}

struct CategoryList *loadUserCategories() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return initializeDefaultCategories();
  }

  char filepath[512];
  snprintf(filepath, sizeof(filepath), "%s/%s/categories.txt", USERS_DIR,
           currentUser->username);

  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    struct CategoryList *defaults = initializeDefaultCategories();
    saveUserCategories(defaults);
    return defaults;
  }

  char line[256];

  if (fgets(line, sizeof(line), file) == NULL ||
      strncmp(line, "CATEGORY_FILE", 13) != 0) {
    fclose(file);
    return initializeDefaultCategories();
  }

  struct CategoryList *list = createCategoryList();
  if (list == NULL) {
    fclose(file);
    return NULL;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "CATEGORY_START", 14) == 0) {
      char internalName[50] = {0};
      char displayName[50] = {0};
      enum TransactionType type = TRANSACTION_EXPENSE;
      int isDefault = 0;

      while (fgets(line, sizeof(line), file) != NULL &&
             strncmp(line, "CATEGORY_END", 12) != 0) {
        if (strncmp(line, "INTERNAL_NAME=", 14) == 0) {
          strncpy(internalName, line + 14, sizeof(internalName) - 1);
          internalName[strcspn(internalName, "\n")] = 0;
        } else if (strncmp(line, "DISPLAY_NAME=", 13) == 0) {
          strncpy(displayName, line + 13, sizeof(displayName) - 1);
          displayName[strcspn(displayName, "\n")] = 0;
        } else if (strncmp(line, "TYPE=", 5) == 0) {
          type = (enum TransactionType)atoi(line + 5);
        } else if (strncmp(line, "IS_DEFAULT=", 11) == 0) {
          isDefault = atoi(line + 11);
        }
      }

      if (strlen(internalName) > 0 && strlen(displayName) > 0) {
        addCategoryToList(list, internalName, displayName, type, isDefault);
      }
    }
  }

  fclose(file);

  if (list->count == 0) {
    freeCategoryList(list);
    return initializeDefaultCategories();
  }

  return list;
}

void refreshCategoryCache() {
  if (cachedUserCategories != NULL) {
    freeCategoryList(cachedUserCategories);
    cachedUserCategories = NULL;
  }
}

struct CategoryList *getUserCategoriesCache() {
  if (cachedUserCategories == NULL) {
    cachedUserCategories = loadUserCategories();
  }
  return cachedUserCategories;
}

int isCategoryInUse(const char *internalName) { return 0; }

int canDeleteCategory(const char *internalName) {
  struct CategoryList *categories = getUserCategoriesCache();
  struct CategoryItem *item =
      findCategoryByInternalName(categories, internalName);

  if (item == NULL) {
    return 0;
  }

  if (item->isDefault) {
    return 0;
  }

  if (isCategoryInUse(internalName)) {
    return 0;
  }

  return 1;
}

#endif
