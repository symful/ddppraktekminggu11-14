#ifndef TRANSACTION_CATEGORY_H
#define TRANSACTION_CATEGORY_H

#include <stddef.h>
#include "./transaction_type.h"

// Category item structure
struct CategoryItem {
    char internalName[50];  // English, lowercase, for code comparison
    char displayName[50];   // Indonesian, for UI display
    enum TransactionType type;  // TRANSACTION_INCOME or TRANSACTION_EXPENSE
    int isDefault;  // 1 for default categories, 0 for custom
};

// Category list structure
struct CategoryList {
    struct CategoryItem** items;
    int count;
    int capacity;
};

// Category management functions
struct CategoryList* createCategoryList();
void freeCategoryList(struct CategoryList* list);
struct CategoryList* initializeDefaultCategories();
struct CategoryList* loadUserCategories();
void saveUserCategories(struct CategoryList* categories);
void refreshCategoryCache();

// Category operations
int addCategoryToList(struct CategoryList* list, const char* internalName, 
                      const char* displayName, enum TransactionType type, int isDefault);
int removeCategoryFromList(struct CategoryList* list, const char* internalName);
struct CategoryItem* findCategoryByInternalName(struct CategoryList* list, const char* internalName);
int categoryExistsInList(struct CategoryList* list, const char* internalName);

// Category filtering
struct CategoryList* getCategoriesByType(struct CategoryList* source, enum TransactionType type);

// Category validation
int validateCategoryName(const char* name);
int canDeleteCategory(const char* internalName);
int isCategoryInUse(const char* internalName);

// Utility functions
void normalizeCategoryName(char* name);
int readCategoryNameSafe(char* buffer, size_t bufferSize, const char* prompt);
const char* getCategoryDisplayName(struct CategoryList* list, const char* internalName);

// Global category cache
struct CategoryList* getUserCategoriesCache();

#endif
