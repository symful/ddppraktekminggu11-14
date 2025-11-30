#include "../types/transaction.h"
#include "../types/transaction_type.h"
#include "../types/transaction_category.h"
#include "../types/transaction_status.h"
#include "../types/transaction_group.h"
#include "../types/month_report.h"
#include "../types/month_report_list.h"
#include "./month_report.h"
#include "./month_report_list.h"


char *getUserConfigPath();
void createDefaultUserConfig();
int initializeUserWorkspace(const char *username);
void cleanupUserSession();
void resetUserBudgets(struct MonthReportList *monthReportList);
void viewAllUserReports();
void showSystemStatistics();
void runAdminDiagnostics();
void fixAdminSystem();


void showEnhancedAdminMenu();
int handleEnhancedAdminMenu();
int validateAdminPrivileges(const char *operation);
int forcePasswordReset(const char *targetUsername);
void showAdminAccessSummary();
