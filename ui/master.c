#include "../auth/auth.h"
#include "../db/transaction_group.c"
#include "../types/include.c"
#include "../utils/validation.c"
#include "./admin_menu_ui.c"
#include "./auth_ui.c"
#include "./colors.c"
#include "./month_report.c"
#include "./screen.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------------------------------------------------------
   PROSEDUR : clearScreen()
   DESKRIPSI :
      Membersihkan layar agar tampilan berikutnya lebih rapi.
   I.S.  : Layar masih berisi tampilan sebelumnya.
   F.S.  : Layar bersih.
   ---------------------------------------------------------*/
void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

/* ---------------------------------------------------------
   PROSEDUR : waitForEnter()
   DESKRIPSI :
      Menunggu pengguna menekan Enter agar mereka sempat
      membaca informasi sebelum program berlanjut.
   I.S.  : Program berhenti menunggu input.
   F.S.  : Enter ditekan, program dilanjutkan
   ---------------------------------------------------------*/
void waitForEnter() {
  printf("\n");
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  clearInputBuffer();
}

/* ---------------------------------------------------------
   PROSEDUR : printMainMenuHeader()
   DESKRIPSI :
      Menampilkan header menu utama.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printMainMenuHeader() {
  int boxWidth = 59;
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                    🏠 MENU UTAMA                        ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printUserIcon(boxWidth, isAdmin)
   DESKRIPSI :
      Menampilkan ikon sesuai jenis pengguna.
   I.S.  : Ikon belum tampil.
   F.S.  : Ikon tampil.
   ---------------------------------------------------------*/
void printUserIcon(int boxWidth, bool isAdmin) {
  if (isAdmin)
    printCentered("│              👑 ", boxWidth);
  else
    printCentered("│              👤 ", boxWidth);
}

/* ---------------------------------------------------------
   PROSEDUR : printUserLabel(isAdmin)
   DESKRIPSI :
      Menampilkan label user (Admin/Pengguna).
   I.S.  : Label belum tampil.
   F.S.  : Label tampil.
   ---------------------------------------------------------*/
void printUserLabel(bool isAdmin) {
  if (isAdmin)
    printColored("Admin: ", COLOR_BRIGHT_YELLOW);
  else
    printColored("Pengguna: ", COLOR_BRIGHT_YELLOW);
}

/* ---------------------------------------------------------
   PROSEDUR : printUsernameAligned(username, isAdmin)
   DESKRIPSI :
      Menampilkan username dengan perataan kolom yang sesuai.
   I.S.  : Username belum tampil.
   F.S.  : Username tampil rapi.
   ---------------------------------------------------------*/
void printUsernameAligned(const char *username, bool isAdmin) {
  int targetLength = isAdmin ? 30 : 27;
  printColored(username, COLOR_BRIGHT_CYAN);
  int len = strlen(username);
  for (int i = len; i < targetLength; i++)
    printf(" ");
  printf(" │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminNoFolderMessage(boxWidth)
   DESKRIPSI :
      Menampilkan pesan bahwa admin tidak memiliki folder pribadi.
   I.S.  : Pesan belum tampil.
   F.S.  : Pesan tampil.
   ---------------------------------------------------------*/
void printAdminNoFolderMessage(int boxWidth) {
  printCenteredColored(
      "│              🚫 Tanpa Folder Pribadi                   │\n", boxWidth,
      COLOR_BRIGHT_WHITE);
}

/* ---------------------------------------------------------
   PROSEDUR : printInfoUser()
   DESKRIPSI :
      Menampilkan informasi user yang sedang login.
   I.S.  : Data user tersedia.
   F.S.  : Info user tampil.
   ---------------------------------------------------------*/
void printInfoUser() {
  int boxWidth = 59;

  if (currentUser == NULL)
    return;

  bool isAdmin = currentUser->isAdmin;

  printUserIcon(boxWidth, isAdmin);
  printUserLabel(isAdmin);
  printUsernameAligned(currentUser->username, isAdmin);

  if (isAdmin)
    printAdminNoFolderMessage(boxWidth);
}

/* ---------------------------------------------------------
   PROSEDUR : printMenuHeaderLine(boxWidth)
   DESKRIPSI :
      Menampilkan garis pemisah menu.
   I.S.  : Garis belum tampil.
   F.S.  : Garis tampil.
   ---------------------------------------------------------*/
void printMenuHeaderLine(int boxWidth) {
  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminFiturMenu(boxWidth)
   DESKRIPSI :
      Menampilkan daftar fitur khusus admin.
   I.S.  : Menu belum tampil.
   F.S.  : Menu tampil.
   ---------------------------------------------------------*/
void printAdminFiturMenu(int boxWidth) {
  printCenteredColored(
      "│  1. 🚫 Kelola Laporan Bulanan (Admin tidak memiliki)    │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  2. 🚫 Lihat Ringkasan Keuangan (Admin tidak memiliki)  │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  3. 🚫 Pengaturan Budget (Admin tidak memiliki)         │\n", boxWidth,
      COLOR_DIM);
  printCenteredColored(
      "│  4. 🚫 Kelola Kategori (Admin tidak memiliki)           │\n", boxWidth,
      COLOR_DIM);

  printCentered("│  ", boxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" ⚙️  Pengaturan Sistem                                │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printUserFiturMenuRows(boxWidth)
   DESKRIPSI :
      Menampilkan daftar fitur yang tersedia untuk pengguna biasa.
   I.S.  : Menu fitur belum tampil.
   F.S.  : Baris-baris menu fitur pengguna tampil lengkap.
   ---------------------------------------------------------*/
void printUserFiturMenuRows(int boxWidth) {
  const char *labels[] = {
      " 📊 Kelola Laporan Bulanan", " 📈 Lihat Ringkasan Keuangan",
      " ⚙️  Pengaturan Budget", " 🏷️  Kelola Kategori", " 🔧 Pengaturan Sistem"};

  char numBuf[8];

  for (int i = 0; i < 5; i++) {
    snprintf(numBuf, sizeof(numBuf), "%d.", i + 1);

    printCentered("│  ", boxWidth);
    printColored(numBuf, COLOR_BRIGHT_CYAN);
    printf("%s                                │\n", labels[i]);
  }
}

/* ---------------------------------------------------------
   PROSEDUR : printFiturMenu()
   DESKRIPSI :
      Menampilkan menu fitur sesuai tipe user
      (admin atau pengguna biasa).
   I.S.  : Belum ada menu fitur yang ditampilkan.
   F.S.  : Menu fitur yang sesuai tampil di layar.
   ---------------------------------------------------------*/
void printFiturMenu() {
  int boxWidth = 59;
  printMenuHeaderLine(boxWidth);

  if (currentUser != NULL && currentUser->isAdmin)
    printAdminFiturMenu(boxWidth);
  else
    printUserFiturMenuRows(boxWidth);
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminAuthMenu(boxWidth)
   DESKRIPSI :
      Menampilkan pilihan menu autentikasi khusus admin
      seperti Admin Panel, Logout, dan Keluar.
   I.S.  : Menu autentikasi belum tampil.
   F.S.  : Menu autentikasi admin tampil.
   ---------------------------------------------------------*/
void printAdminAuthMenu(int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 👑 Admin Panel                                      │\n");

  printCentered("│  ", boxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           │\n");

  printCentered("│  ", boxWidth);
  printColored("8.", COLOR_BRIGHT_CYAN);
  printf(" ❌ Keluar                                           │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printUserAuthMenu(boxWidth)
   DESKRIPSI :
      Menampilkan pilihan autentikasi untuk pengguna biasa
      (Logout dan Keluar).
   I.S.  : Menu autentikasi belum tampil.
   F.S.  : Menu autentikasi pengguna tampil.
   ---------------------------------------------------------*/
void printUserAuthMenu(int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           │\n");

  printCentered("│  ", boxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" ❌ Keluar                                           │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printFiturMenuAuth()
   DESKRIPSI :
      Menampilkan menu autentikasi sesuai tipe user
      (admin atau pengguna).
   I.S.  : Menu autentikasi belum tampil.
   F.S.  : Menu autentikasi sesuai user tampil.
   ---------------------------------------------------------*/
void printFiturMenuAuth() {
  int boxWidth = 59;

  if (currentUser != NULL && currentUser->isAdmin)
    printAdminAuthMenu(boxWidth);
  else
    printUserAuthMenu(boxWidth);
}

/* ---------------------------------------------------------
   PROSEDUR : printMainMenuFooter()
   DESKRIPSI :
      Menampilkan footer menu utama beserta tip penggunaan.
   I.S.  : Footer belum tampil.
   F.S.  : Footer menu utama tampil di layar.
   ---------------------------------------------------------*/
void printMainMenuFooter() {
  int boxWidth = 59;
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Tip: Pilih nomor menu yang diinginkan\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}
/* ---------------------------------------------------------
   PROSEDUR : showMainMenu()
   DESKRIPSI :
      Menampilkan menu utama lengkap dengan header, info user,
      daftar fitur, autentikasi, dan footer.
   I.S.  : Program sedang berjalan, menu belum tampil.
   F.S.  : Menu utama tampil di layar.
   ---------------------------------------------------------*/
void showMainMenu() {
  int contentLines = 14;
  clearAndCenterVertically(contentLines);
  printMainMenuHeader();
  printInfoUser();
  printFiturMenu();
  printFiturMenuAuth();
  printMainMenuFooter();
}

/* ---------------------------------------------------------
   PROSEDUR : printMessagePrefix(prefix, colorFunc)
   DESKRIPSI :
      Mencetak prefix pesan dengan warna tertentu.
   I.S.  : Prefix dan fungsi warna sudah tersedia.
   F.S.  : Prefix dicetak di layar sebelum pesan.
   ---------------------------------------------------------*/
void printMessagePrefix(const char *prefix, void (*colorFunc)(const char *)) {
  printf("\n");
  colorFunc(prefix);
}

/* ---------------------------------------------------------
   PROSEDUR : showSuccessMessage(message)
   DESKRIPSI :
      Menampilkan pesan sukses dengan ikon dan warna hijau.
   I.S.  : Pesan sudah siap ditampilkan.
   F.S.  : Pesan sukses tampil di layar, menunggu user tekan enter.
   ---------------------------------------------------------*/
void showSuccessMessage(const char *message) {
  printMessagePrefix("✅ ", printSuccess);
  printf("%s", message);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : showErrorMessage(message)
   DESKRIPSI :
      Menampilkan pesan error dengan ikon dan warna merah.
   I.S.  : Pesan sudah siap ditampilkan.
   F.S.  : Pesan error tampil di layar, menunggu user tekan enter.
   ---------------------------------------------------------*/
void showErrorMessage(const char *message) {
  printMessagePrefix("❌ ", printError);
  printf("%s", message);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : showWarningMessage(message)
   DESKRIPSI :
      Menampilkan pesan peringatan dengan ikon dan warna kuning.
   I.S.  : Pesan sudah siap ditampilkan.
   F.S.  : Pesan peringatan tampil di layar, menunggu user tekan enter.
   ---------------------------------------------------------*/
void showWarningMessage(const char *message) {
  printMessagePrefix("⚠️  ", printWarning);
  printf("%s", message);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : showInfoMessage(message)
   DESKRIPSI :
      Menampilkan pesan informasi dengan ikon dan warna biru.
   I.S.  : Pesan sudah siap ditampilkan.
   F.S.  : Pesan informasi tampil di layar, menunggu user tekan enter.
   ---------------------------------------------------------*/
void showInfoMessage(const char *message) {
  printMessagePrefix("ℹ️  ", printInfo);
  printf("%s", message);
  waitForEnter();
}

/* ---------------------------------------------------------
   FUNGSI : getValidatedMenuChoice(minChoice, maxChoice)
   DESKRIPSI :
      Membaca input user untuk menu, memvalidasi sesuai rentang
      yang diberikan, dan menangani input tidak valid.
   INPUT  : minChoice (nilai minimal pilihan)
            maxChoice (nilai maksimal pilihan)
   OUTPUT : Pilihan user yang valid, atau -1 jika gagal.
   ---------------------------------------------------------*/
int getValidatedMenuChoice(int minChoice, int maxChoice) {
  int choice;
  if (!readAndValidateInteger("", minChoice, maxChoice, &choice)) {
    showErrorMessage("Input tidak valid atau terlalu banyak percobaan.");
    return -1;
  }
  return choice;
}

/* ---------------------------------------------------------
   PROSEDUR : openSummaryMenu(monthReportList)
   DESKRIPSI :
      Menampilkan ringkasan laporan bulanan jika ada.
   I.S.  : Daftar laporan bulanan sudah tersedia.
   F.S.  : Ringkasan laporan tampil, atau info jika kosong.
   ---------------------------------------------------------*/
void openSummaryMenu(struct MonthReportList *monthReportList) {
  if (monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan untuk ditampilkan. Buat laporan "
                    "terlebih dahulu.");
    return;
  }
  showAllMonthReportSummary(monthReportList);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetHeaderLayout()
   DESKRIPSI :
      Mencetak layout header menu pengaturan budget dengan warna.
   I.S.  : Header belum tampil.
   F.S.  : Header menu budget tampil di layar.
   ---------------------------------------------------------*/
void printBudgetHeaderLayout() {
  int boxWidth = 59;
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                 ⚙️  PENGATURAN BUDGET                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetMenuOptions()
   DESKRIPSI :
      Menampilkan daftar pilihan menu pengaturan budget.
   I.S.  : Menu belum tampil.
   F.S.  : Pilihan menu budget tampil di layar.
   ---------------------------------------------------------*/
void printBudgetMenuOptions() {
  int boxWidth = 59;
  printCentered("│  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 🏷️  Atur Budget per Kategori                         │\n");

  printCentered("│  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 📋 Lihat Budget Saat Ini                            │\n");

  printCentered("│  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" 🔄 Reset Semua Budget                               │\n");

  printCentered("│  ", boxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali ke Menu Utama                            │\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetFooter()
   DESKRIPSI :
      Menampilkan garis footer menu budget.
   I.S.  : Footer belum tampil.
   F.S.  : Footer menu budget tampil di layar.
   ---------------------------------------------------------*/
void printBudgetFooter() {
  int boxWidth = 59;
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetPrompt()
   DESKRIPSI :
      Mencetak prompt input pilihan user untuk menu budget.
   I.S.  : Prompt belum tampil.
   F.S.  : Prompt muncul di layar.
   ---------------------------------------------------------*/
void printBudgetPrompt() {
  int boxWidth = 59;
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetHeader()
   DESKRIPSI :
      Menampilkan seluruh layout menu budget: header, opsi, footer, dan prompt.
   I.S.  : Menu budget belum tampil.
   F.S.  : Menu budget lengkap tampil di layar.
   ---------------------------------------------------------*/
void printBudgetHeader() {
  int contentLines = 10;
  clearAndCenterVertically(contentLines);

  printBudgetHeaderLayout();
  printBudgetMenuOptions();
  printBudgetFooter();
  printBudgetPrompt();
}

/* ---------------------------------------------------------
   FUNGSI : validasiInputBudget()
   DESKRIPSI :
      Memvalidasi input user untuk menu budget (1-4).
   OUTPUT : Pilihan user yang valid, atau -1 jika gagal.
   ---------------------------------------------------------*/
int validasiInputBudget() { return getValidatedMenuChoice(1, 4); }

/* ---------------------------------------------------------
   PROSEDUR : handleInputBudget(choice, monthReportList, shouldExit)
   DESKRIPSI :
      Menangani aksi sesuai pilihan user di menu budget.
   I.S.  : Pilihan user sudah tersedia.
   F.S.  : Menu terkait dijalankan, atau keluar jika pilih 4.
   ---------------------------------------------------------*/
void handleInputBudget(int choice, struct MonthReportList *monthReportList,
                       bool *shouldExit) {
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
    *shouldExit = true;
    break;
  default:
    showErrorMessage("Pilihan tidak valid.");
    break;
  }
}

/* ---------------------------------------------------------
   PROSEDUR : openBudgetSettingsMenu(monthReportList)
   DESKRIPSI :
      Menampilkan menu pengaturan budget dan menunggu input user.
   I.S.  : Program sedang berjalan.
   F.S.  : Menu budget tampil dan dijalankan hingga user keluar.
   ---------------------------------------------------------*/
void openBudgetSettingsMenu(struct MonthReportList *monthReportList) {
  bool shouldExit = false;

  while (!shouldExit) {
    printBudgetHeader();

    int choice = validasiInputBudget();
    if (choice == -1) {
      continue;
    }

    handleInputBudget(choice, monthReportList, &shouldExit);
  }
}

/* ---------------------------------------------------------
   FUNGSI : hasMonthReports(monthReportList)
   DESKRIPSI :
      Mengecek apakah terdapat laporan bulanan.
   OUTPUT : true jika ada laporan, false jika kosong atau NULL.
   ---------------------------------------------------------*/
bool hasMonthReports(struct MonthReportList *monthReportList) {
  return monthReportList != NULL && monthReportList->count > 0;
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetCategoryHeader()
   DESKRIPSI :
      Menampilkan header menu "Atur Budget per Kategori".
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printBudgetCategoryHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("             🏷️  ATUR BUDGET PER KATEGORI                ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   FUNGSI : readMonthReportChoice(max, outIndex)
   DESKRIPSI :
      Membaca input user untuk memilih laporan bulanan.
   INPUT  : max = jumlah laporan
            outIndex = pointer untuk menyimpan pilihan
   OUTPUT : true jika valid, false jika gagal.
   ---------------------------------------------------------*/
bool readMonthReportChoice(int max, int *outIndex) {
  if (!readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1, max, outIndex)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }
  return true;
}

/* ---------------------------------------------------------
   FUNGSI : selectMonthReport(monthReportList)
   DESKRIPSI :
      Memilih laporan bulanan dari daftar.
   OUTPUT : Pointer ke MonthReport yang dipilih, NULL jika gagal.
   ---------------------------------------------------------*/
struct MonthReport *selectMonthReport(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);

  int reportChoice;
  if (!readMonthReportChoice(monthReportList->count, &reportChoice))
    return NULL;

  return monthReportList->reports[reportChoice - 1];
}

/* ---------------------------------------------------------
   FUNGSI : hasCategories(categories)
   DESKRIPSI :
      Mengecek apakah ada kategori dalam daftar.
   OUTPUT : true jika ada, false jika kosong atau NULL.
   ---------------------------------------------------------*/
bool hasCategories(struct CategoryList *categories) {
  return categories != NULL && categories->count > 0;
}

/* ---------------------------------------------------------
   FUNGSI : ensureCategoriesAvailable(categories)
   DESKRIPSI :
      Memastikan setidaknya ada satu kategori tersedia.
   OUTPUT : true jika tersedia, false jika tidak.
   ---------------------------------------------------------*/
bool ensureCategoriesAvailable(struct CategoryList *categories) {
  if (!hasCategories(categories)) {
    showErrorMessage("Tidak ada kategori tersedia.");
    return false;
  }
  return true;
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderTop(boxWidth)
   DESKRIPSI :
      Mencetak garis atas header kategori.
   I.S.  : Belum tampil.
   F.S.  : Garis atas header tampil.
   ---------------------------------------------------------*/
void printCategoryHeaderTop(int boxWidth) {
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderTitle(boxWidth)
   DESKRIPSI :
      Mencetak judul header kategori dengan warna dan background.
   I.S.  : Belum tampil.
   F.S.  : Judul header tampil.
   ---------------------------------------------------------*/
void printCategoryHeaderTitle(int boxWidth) {
  printCentered("│", boxWidth);
  printWithBg("        📂 Pilih Kategori untuk Mengatur Budget          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderBottom(boxWidth)
   DESKRIPSI :
      Mencetak garis bawah header kategori.
   I.S.  : Belum tampil.
   F.S.  : Garis bawah header tampil.
   ---------------------------------------------------------*/
void printCategoryHeaderBottom(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeader()
   DESKRIPSI :
      Menampilkan header lengkap untuk menu kategori.
   I.S.  : Header belum tampil.
   F.S.  : Header kategori tampil di layar.
   ---------------------------------------------------------*/
void printCategoryHeader() {
  int boxWidth = 59;
  printf("\n");
  printCategoryHeaderTop(boxWidth);
  printCategoryHeaderTitle(boxWidth);
  printCategoryHeaderBottom(boxWidth);
  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printSingleCategoryItem(item, index, boxWidth)
   DESKRIPSI :
      Menampilkan satu item kategori dengan nomor urut.
   I.S.  : Item kategori tersedia.
   F.S.  : Item kategori dicetak ke layar.
   ---------------------------------------------------------*/
void printSingleCategoryItem(struct CategoryItem *item, int index,
                             int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%s%d.%s %s\n", COLOR_BRIGHT_CYAN, index + 1, COLOR_RESET,
         item->displayName);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryList(categories)
   DESKRIPSI :
      Menampilkan daftar semua kategori.
   I.S.  : Daftar kategori tersedia.
   F.S.  : Semua kategori tampil di layar.
   ---------------------------------------------------------*/
void printCategoryList(struct CategoryList *categories) {
  int boxWidth = 59;

  for (int i = 0; i < categories->count; i++) {
    printSingleCategoryItem(categories->items[i], i, boxWidth);
  }
  printf("\n");
}

/* ---------------------------------------------------------
   FUNGSI : readCategorySelection(max, choice)
   DESKRIPSI :
      Membaca input user untuk memilih kategori.
   INPUT  : max = jumlah kategori
            choice = pointer untuk menyimpan pilihan
   OUTPUT : true jika valid, false jika gagal.
   ---------------------------------------------------------*/
bool readCategorySelection(int max, int *choice) {
  return readAndValidateInteger("\n🎯 Pilihan kategori: ", 1, max, choice);
}

/* ---------------------------------------------------------
   PROSEDUR : copyCategoryInternalName(outCategory, internalName, bufferSize)
   DESKRIPSI :
      Menyalin nama internal kategori ke buffer dengan aman.
   I.S.  : Nama internal tersedia.
   F.S.  : Nama internal tersalin di outCategory.
   ---------------------------------------------------------*/
void copyCategoryInternalName(char *outCategory, const char *internalName,
                              size_t bufferSize) {
  strncpy(outCategory, internalName, bufferSize - 1);
  outCategory[bufferSize - 1] = '\0';
}

/* ---------------------------------------------------------
   FUNGSI : selectCategory(outCategory, bufferSize)
   DESKRIPSI :
      Menampilkan daftar kategori dan membiarkan user memilih.
   OUTPUT : true jika kategori berhasil dipilih, false jika gagal.
   ---------------------------------------------------------*/
bool selectCategory(char *outCategory, size_t bufferSize) {
  struct CategoryList *categories = getUserCategoriesCache();
  if (!ensureCategoriesAvailable(categories))
    return false;
  printCategoryHeader();
  printCategoryList(categories);
  int categoryChoice;
  if (!readCategorySelection(categories->count, &categoryChoice)) {
    showErrorMessage("Input tidak valid.");
    return false;
  }

  copyCategoryInternalName(outCategory,
                           categories->items[categoryChoice - 1]->internalName,
                           bufferSize);
  return true;
}

/* ---------------------------------------------------------
   FUNGSI : inputNewBudget(outBudget)
   DESKRIPSI :
      Meminta user memasukkan budget baru dan memvalidasinya.
   OUTPUT : true jika input valid, false jika gagal.
   ---------------------------------------------------------*/
bool inputNewBudget(long long *outBudget) {
  InputResult result =
      promptForBudget("💰 Masukkan budget baru (Rp): ", outBudget);
  if (result != INPUT_SUCCESS) {
    showErrorMessage(
        "Budget tidak valid atau di bawah minimum yang diizinkan.");
    return false;
  }
  return true;
}

/* ---------------------------------------------------------
   FUNGSI : getBudgetCategoryDisplayName(category)
   DESKRIPSI :
      Mengambil nama kategori yang ditampilkan untuk user.
   OUTPUT : Pointer ke nama display kategori.
   ---------------------------------------------------------*/
const char *getBudgetCategoryDisplayName(const char *category) {
  struct CategoryList *categories = getUserCategoriesCache();
  return getCategoryDisplayName(categories, category);
}

/* ---------------------------------------------------------
   PROSEDUR : formatBudgetSuccessMessage(buffer, size, displayName, budget)
   DESKRIPSI :
      Membuat pesan sukses ketika budget berhasil diatur.
   I.S.  : Display name dan budget tersedia.
   F.S.  : Pesan sukses tersimpan di buffer.
   ---------------------------------------------------------*/
void formatBudgetSuccessMessage(char *buffer, size_t size,
                                const char *displayName, long long budget) {

  snprintf(buffer, size,
           "Budget untuk kategori '%s' berhasil diatur menjadi Rp %lld",
           displayName, budget);
}

/* ---------------------------------------------------------
   PROSEDUR : showSetBudgetSuccessMessage(category, budget)
   DESKRIPSI :
      Menampilkan pesan sukses setelah budget kategori diatur.
   I.S.  : Nama kategori dan budget tersedia.
   F.S.  : Pesan sukses tampil di layar.
   ---------------------------------------------------------*/
void showSetBudgetSuccessMessage(const char *category, long long budget) {
  const char *displayName = getBudgetCategoryDisplayName(category);

  char successMsg[256];
  formatBudgetSuccessMessage(successMsg, sizeof(successMsg), displayName,
                             budget);
  showSuccessMessage(successMsg);
}

/* ---------------------------------------------------------
   FUNGSI : ensureHasMonthReports(monthReportList)
   DESKRIPSI :
      Mengecek apakah terdapat laporan bulanan, menampilkan info jika kosong.
   OUTPUT : true jika ada laporan, false jika tidak.
   ---------------------------------------------------------*/
bool ensureHasMonthReports(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || monthReportList->count == 0) {
    showInfoMessage("Tidak ada laporan bulanan. Buat laporan terlebih dahulu.");
    return false;
  }
  return true;
}

/* ---------------------------------------------------------
   PROSEDUR : applyBudgetToReport(report, category, budget)
   DESKRIPSI :
      Menerapkan budget untuk kategori tertentu di laporan bulanan.
   I.S.  : Laporan dan kategori tersedia.
   F.S.  : Budget tersimpan di laporan.
   ---------------------------------------------------------*/
void applyBudgetToReport(struct MonthReport *report, const char *category,
                         long long budget) {

  setBudgetForCategory(report, category, budget);
}

/* ---------------------------------------------------------
   PROSEDUR : saveUpdatedReport(report)
   DESKRIPSI :
      Menyimpan laporan bulanan yang sudah diperbarui.
   I.S.  : Laporan sudah diperbarui.
   F.S.  : Laporan tersimpan di sistem.
   ---------------------------------------------------------*/
void saveUpdatedReport(struct MonthReport *report) {
  saveUserMonthReport(report);
}

/* ---------------------------------------------------------
   PROSEDUR : openSetCategoryBudgetMenu(monthReportList)
   DESKRIPSI :
      Menjalankan alur pengaturan budget kategori:
      pilih laporan, pilih kategori, input budget, simpan, tampilkan sukses.
   I.S.  : Daftar laporan tersedia.
   F.S.  : Budget kategori tersimpan, pesan sukses tampil.
   ---------------------------------------------------------*/
void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList) {

  if (!ensureHasMonthReports(monthReportList))
    return;

  printBudgetCategoryHeader();
  struct MonthReport *report = selectMonthReport(monthReportList);
  if (report == NULL)
    return;

  char category[50];
  if (!selectCategory(category, sizeof(category)))
    return;

  long long budget;
  if (!inputNewBudget(&budget))
    return;

  applyBudgetToReport(report, category, budget);
  saveUpdatedReport(report);
  showSetBudgetSuccessMessage(category, budget);
}

/* ---------------------------------------------------------
   FUNGSI : hasMonthReportsOnly(list)
   DESKRIPSI :
      Mengecek apakah ada laporan bulanan tanpa menampilkan pesan.
   OUTPUT : true jika ada, false jika tidak.
   ---------------------------------------------------------*/
bool hasMonthReportsOnly(struct MonthReportList *list) {
  return list != NULL && list->count > 0;
}

/* ---------------------------------------------------------
   PROSEDUR : showNoMonthReportsMessage()
   DESKRIPSI :
      Menampilkan pesan bahwa tidak ada laporan bulanan.
   I.S.  : Belum ada info ditampilkan.
   F.S.  : Pesan info tampil di layar.
   ---------------------------------------------------------*/
void showNoMonthReportsMessage() {
  showInfoMessage("Tidak ada laporan bulanan.");
}

/* ---------------------------------------------------------
   FUNGSI : hasMonthReportsView(monthReportList)
   DESKRIPSI :
      Mengecek laporan bulanan untuk keperluan tampilan,
      menampilkan pesan jika kosong.
   OUTPUT : true jika ada laporan, false jika tidak.
   ---------------------------------------------------------*/
bool hasMonthReportsView(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsOnly(monthReportList)) {
    showNoMonthReportsMessage();
    return false;
  }
  return true;
}

/* ---------------------------------------------------------
   PROSEDUR : printCurrentBudgetHeader()
   DESKRIPSI :
      Menampilkan header menu "Lihat Budget Saat Ini".
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printCurrentBudgetHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("               📋 LIHAT BUDGET SAAT INI                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   FUNGSI : readReportChoice(max, choice)
   DESKRIPSI :
      Membaca input user untuk memilih laporan bulanan.
   OUTPUT : true jika valid, false jika gagal.
   ---------------------------------------------------------*/
bool readReportChoice(int max, int *choice) {
  return readAndValidateInteger("\n📅 Pilih nomor laporan: ", 1, max, choice);
}

/* ---------------------------------------------------------
   FUNGSI : getReportByIndex(list, index)
   DESKRIPSI :
      Mengambil laporan bulanan berdasarkan indeks.
   OUTPUT : Pointer ke MonthReport yang dipilih.
   ---------------------------------------------------------*/
struct MonthReport *getReportByIndex(struct MonthReportList *list, int index) {
  return list->reports[index];
}

/* ---------------------------------------------------------
   FUNGSI : selectReportForBudgetView(monthReportList)
   DESKRIPSI :
      Menampilkan daftar laporan bulanan, meminta user memilih, dan
   mengembalikan laporan. OUTPUT : Pointer ke laporan yang dipilih, NULL jika
   gagal.
   ---------------------------------------------------------*/
struct MonthReport *
selectReportForBudgetView(struct MonthReportList *monthReportList) {
  showMonthlyList(monthReportList);
  int reportChoice;
  if (!readReportChoice(monthReportList->count, &reportChoice)) {
    showErrorMessage("Input tidak valid.");
    return NULL;
  }
  return getReportByIndex(monthReportList, reportChoice - 1);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetPerCategoryHeader()
   DESKRIPSI :
      Menampilkan header tabel budget per kategori lengkap dengan kolom.
   I.S.  : Header belum tampil.
   F.S.  : Header tabel tampil di layar.
   ---------------------------------------------------------*/
void printBudgetPerCategoryHeader() {
  int boxWidth = 88;
  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "════════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                              💰 DETAIL BUDGET PER KATEGORI     "
              "                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║ ", boxWidth);
  printColored("KATEGORI", COLOR_BRIGHT_CYAN);
  printf("            │ ");
  printColored("BUDGET (Rp)", COLOR_BRIGHT_CYAN);
  printf("     │ ");
  printColored("TERPAKAI (Rp)", COLOR_BRIGHT_CYAN);
  printf("   │ ");
  printColored("SISA (Rp)", COLOR_BRIGHT_CYAN);
  printf("       │ ");
  printColored("STATUS", COLOR_BRIGHT_CYAN);
  printf("     ║\n");
  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "════════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   FUNGSI : calculateRemainingBudget(group)
   DESKRIPSI :
      Menghitung sisa budget untuk kategori.
   OUTPUT : sisa budget (budget - totalAmount)
   ---------------------------------------------------------*/
long long calculateRemainingBudget(const struct TransactionGroup *group) {
  return group->budget - group->totalAmount;
}

/* ---------------------------------------------------------
   FUNGSI : calculateBudgetPercentage(group)
   DESKRIPSI :
      Menghitung persentase penggunaan budget.
   OUTPUT : persentase (0.0 jika budget <= 0)
   ---------------------------------------------------------*/
double calculateBudgetPercentage(const struct TransactionGroup *group) {
  if (group->budget <= 0)
    return 0.0;
  return ((double)group->totalAmount / group->budget) * 100.0;
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnCategory(displayName)
   DESKRIPSI :
      Menampilkan kolom nama kategori pada tabel budget.
   I.S.  : Nama kategori tersedia.
   F.S.  : Kolom kategori tampil di layar.
   ---------------------------------------------------------*/
void printBudgetColumnCategory(const char *displayName) {
  printf("%-20s │ ", displayName);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnTotal(total)
   DESKRIPSI :
      Menampilkan total pengeluaran/pendapatan kategori.
   I.S.  : Total tersedia.
   F.S.  : Kolom total tampil di layar.
   ---------------------------------------------------------*/
void printBudgetColumnTotal(long long total) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%-15lld", total);
  printColored(buffer, COLOR_BRIGHT_CYAN);
  printf(" │ ");
}

/* ---------------------------------------------------------
PROSEDUR : printBudgetColumnRemaining(remaining)
DESKRIPSI :
Menampilkan sisa budget dengan warna hijau jika positif, merah jika negatif.
I.S.  : Sisa budget tersedia.
F.S.  : Kolom sisa tampil di layar.
---------------------------------------------------------*/
void printBudgetColumnRemaining(long long remaining) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%-15lld", remaining);

  const char *color = (remaining >= 0) ? COLOR_BRIGHT_GREEN : COLOR_BRIGHT_RED;

  printColored(buffer, color);
  printf(" │ ");
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnStatus(status, percentage)
   DESKRIPSI :
      Menampilkan status budget berdasarkan persentase penggunaan.
   I.S.  : Status dan persentase tersedia.
   F.S.  : Kolom status tampil di layar.
   ---------------------------------------------------------*/
void printBudgetColumnStatus(const char *status, double percentage) {
  printBudgetStatus(status, percentage);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableRow(group)
   DESKRIPSI :
      Menampilkan satu baris tabel budget untuk kategori tertentu.
   I.S.  : Data group tersedia.
   F.S.  : Baris tabel tampil di layar.
   ---------------------------------------------------------*/
void printBudgetTableRow(const struct TransactionGroup *group) {
  int boxWidth = 88;

  struct CategoryList *categories = getUserCategoriesCache();
  const char *displayName = getCategoryDisplayName(categories, group->category);

  long long remaining = calculateRemainingBudget(group);
  double percentage = calculateBudgetPercentage(group);
  const char *status = getGroupBudgetStatus(group);

  printCentered("║ ", boxWidth);
  printBudgetColumnCategory(displayName);
  printBudgetColumnTotal(group->totalAmount);
  printBudgetColumnRemaining(remaining);
  printAmount(remaining);
  printf("%-15s │ ", "");
  printBudgetColumnStatus(status, percentage);
  printf("%-10s ║\n", "");
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableHeader()
   DESKRIPSI :
      Menampilkan header tabel budget per kategori.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printBudgetTableHeader() { printBudgetPerCategoryHeader(); }

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableFooter()
   DESKRIPSI :
      Menampilkan footer tabel budget per kategori.
   I.S.  : Footer belum tampil.
   F.S.  : Footer tampil di layar.
   ---------------------------------------------------------*/
void printBudgetTableFooter() {
  int boxWidth = 88;
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "════════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printBudgetReport(report)
   DESKRIPSI :
      Menampilkan seluruh tabel budget untuk satu laporan bulanan.
   I.S.  : Laporan tersedia.
   F.S.  : Tabel budget tampil lengkap di layar.
   ---------------------------------------------------------*/
void printBudgetReport(const struct MonthReport *report) {
  printBudgetTableHeader();

  for (int i = 0; i < report->groupCount; i++) {
    printBudgetTableRow(report->groups[i]);
  }

  printBudgetTableFooter();
}

/* ---------------------------------------------------------
   PROSEDUR : openViewBudgetMenu(monthReportList)
   DESKRIPSI :
      Menampilkan menu untuk melihat budget saat ini per laporan bulanan.
   I.S.  : Daftar laporan tersedia.
   F.S.  : Tabel budget tampil di layar.
   ---------------------------------------------------------*/
void openViewBudgetMenu(struct MonthReportList *monthReportList) {
  if (!hasMonthReportsView(monthReportList))
    return;

  printCurrentBudgetHeader();

  struct MonthReport *report = selectReportForBudgetView(monthReportList);
  if (report == NULL)
    return;
  printBudgetReport(report);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryListHeader()
   DESKRIPSI :
      Menampilkan header daftar kategori.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printCategoryListHeader() {
  int boxWidth = 59;
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                📋 DAFTAR KATEGORI                       ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printIncomeHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header kategori pendapatan.
   I.S.  : Header belum tampil.
   F.S.  : Header pendapatan tampil di layar.
   ---------------------------------------------------------*/
void printIncomeHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printSuccess("💰 KATEGORI PENDAPATAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printExpenseHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header kategori pengeluaran.
   I.S.  : Header belum tampil.
   F.S.  : Header pengeluaran tampil di layar.
   ---------------------------------------------------------*/
void printExpenseHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printError("💸 KATEGORI PENGELUARAN:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryRow(cat, index, boxWidth)
   DESKRIPSI :
      Menampilkan satu baris kategori dengan tipe Default/Custom.
   I.S.  : Item kategori tersedia.
   F.S.  : Baris kategori tampil di layar.
   ---------------------------------------------------------*/
void printCategoryRow(struct CategoryItem *cat, int index, int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%d. %-30s ", index + 1, cat->displayName);

  if (cat->isDefault)
    printColored("[Default]", COLOR_BRIGHT_WHITE);
  else
    printColored("[Custom]", COLOR_BRIGHT_YELLOW);

  printf("\n");
}

/* ---------------------------------------------------------
   FUNGSI : printIncomeCategories(categories, boxWidth)
   DESKRIPSI :
      Menampilkan semua kategori pendapatan.
   OUTPUT : jumlah kategori pendapatan yang tampil.
   ---------------------------------------------------------*/
int printIncomeCategories(struct CategoryList *categories, int boxWidth) {
  int count = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_INCOME) {
      printCategoryRow(categories->items[i], i, boxWidth);
      count++;
    }
  }
  if (count == 0)
    printCentered("  📭 Tidak ada kategori pendapatan\n", boxWidth);

  return count;
}

/* ---------------------------------------------------------
   FUNGSI : printExpenseCategories(categories, boxWidth)
   DESKRIPSI :
      Menampilkan semua kategori pengeluaran.
   OUTPUT : jumlah kategori pengeluaran yang tampil.
   ---------------------------------------------------------*/
int printExpenseCategories(struct CategoryList *categories, int boxWidth) {
  int count = 0;
  for (int i = 0; i < categories->count; i++) {
    if (categories->items[i]->type == TRANSACTION_EXPENSE) {
      printCategoryRow(categories->items[i], i, boxWidth);
      count++;
    }
  }
  if (count == 0)
    printCentered("  📭 Tidak ada kategori pengeluaran\n", boxWidth);

  return count;
}

/* ---------------------------------------------------------
   PROSEDUR : printCategorySummary(total, income, expense, boxWidth)
   DESKRIPSI :
      Menampilkan ringkasan jumlah kategori pendapatan & pengeluaran.
   I.S.  : Data kategori tersedia.
   F.S.  : Ringkasan tampil di layar.
   ---------------------------------------------------------*/
void printCategorySummary(int total, int income, int expense, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📊 Total: ");
  printf("%d kategori (", total);
  printf("%s%d%s", COLOR_BRIGHT_GREEN, income, COLOR_RESET);
  printf(" pendapatan, ");
  printf("%s%d%s", COLOR_BRIGHT_RED, expense, COLOR_RESET);
  printf(" pengeluaran)\n");
}

/* ---------------------------------------------------------
   PROSEDUR : showAllCategories()
   DESKRIPSI :
      Menampilkan seluruh daftar kategori dan ringkasan.
   I.S.  : Kategori tersedia.
   F.S.  : Daftar kategori dan ringkasan tampil di layar.
   ---------------------------------------------------------*/
void showAllCategories() {
  int boxWidth = 59;

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL || categories->count == 0) {
    showErrorMessage("Tidak ada kategori tersedia.");
    waitForEnter();
    return;
  }

  printCategoryListHeader();
  printIncomeHeader(boxWidth);
  int incomeCount = printIncomeCategories(categories, boxWidth);
  printExpenseHeader(boxWidth);
  int expenseCount = printExpenseCategories(categories, boxWidth);
  printCategorySummary(categories->count, incomeCount, expenseCount, boxWidth);
}

/* ---------------------------------------------------------
   PROSEDUR : printAddCategoryHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu tambah kategori baru.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printAddCategoryHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("               ➕ TAMBAH KATEGORI BARU                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryTypeOptions(boxWidth)
   DESKRIPSI :
      Menampilkan pilihan tipe kategori (pendapatan atau pengeluaran).
   I.S.  : Belum tampil.
   F.S.  : Pilihan tipe tampil di layar.
   ---------------------------------------------------------*/
void printCategoryTypeOptions(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📝 TIPE KATEGORI:\n");

  printCentered("  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 💰 Pendapatan (Income)\n");

  printCentered("  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 💸 Pengeluaran (Expense)\n");
}

/* ---------------------------------------------------------
   FUNGSI : readCategoryType(type, boxWidth)
   DESKRIPSI :
      Membaca input user untuk memilih tipe kategori.
   OUTPUT : true jika input valid, false jika gagal.
   ---------------------------------------------------------*/
bool readCategoryType(enum TransactionType *type, int boxWidth) {
  int choice;

  printf("\n");
  printCentered("", boxWidth);

  if (!readAndValidateInteger("🎯 Pilihan: ", 1, 2, &choice)) {
    return false;
  }

  *type = (choice == 1) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;
  return true;
}

/* ---------------------------------------------------------
   FUNGSI : readDisplayCategoryName(buffer, size, boxWidth)
   DESKRIPSI :
      Membaca nama kategori dari user dengan aman.
   OUTPUT : true jika nama valid, false jika gagal.
   ---------------------------------------------------------*/
bool readDisplayCategoryName(char *buffer, size_t size, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);

  return readCategoryNameSafe(buffer, size, "📝 Nama kategori (Indonesia): ");
}

/* ---------------------------------------------------------
   PROSEDUR : generateInternalCategoryName(internalName, displayName, size)
   DESKRIPSI :
      Menghasilkan nama internal kategori dari nama display dan menormalisasi.
   I.S.  : Nama display tersedia.
   F.S.  : Nama internal siap dipakai.
   ---------------------------------------------------------*/
void generateInternalCategoryName(char *internalName, const char *displayName,
                                  size_t size) {
  strncpy(internalName, displayName, size - 1);
  internalName[size - 1] = '\0';
  normalizeCategoryName(internalName);
}

/* ---------------------------------------------------------
   FUNGSI : isDuplicateCategory(list, internalName)
   DESKRIPSI :
      Mengecek apakah kategori sudah ada di daftar.
   OUTPUT : true jika duplikat, false jika tidak.
   ---------------------------------------------------------*/
bool isDuplicateCategory(struct CategoryList *list, const char *internalName) {
  return categoryExistsInList(list, internalName);
}

/* ---------------------------------------------------------
   FUNGSI : addNewCategory(categories, internalName, displayName, type)
   DESKRIPSI :
      Menambahkan kategori baru ke daftar dan menyimpan perubahan.
   OUTPUT : true jika berhasil, false jika gagal.
   ---------------------------------------------------------*/
bool addNewCategory(struct CategoryList *categories, const char *internalName,
                    const char *displayName, enum TransactionType type) {
  bool success =
      addCategoryToList(categories, internalName, displayName, type, 0);

  if (success) {
    saveUserCategories(categories);
    refreshCategoryCache();
  }

  return success;
}

/* ---------------------------------------------------------
   PROSEDUR : showAddCategorySuccess(displayName)
   DESKRIPSI :
      Menampilkan pesan sukses setelah kategori berhasil ditambahkan.
   I.S.  : Nama kategori tersedia.
   F.S.  : Pesan sukses tampil di layar.
   ---------------------------------------------------------*/
void showAddCategorySuccess(const char *displayName) {
  char msg[256];
  snprintf(msg, sizeof(msg), "Kategori '%s' berhasil ditambahkan!",
           displayName);
  showSuccessMessage(msg);
}

/* ---------------------------------------------------------
   PROSEDUR : openAddCategoryMenu()
   DESKRIPSI :
      Menjalankan alur tambah kategori baru
   I.S.  : Kategori baru belum ditambahkan
   F.S.  : Kategori baru ditambahkan jika valid
   ---------------------------------------------------------*/
void openAddCategoryMenu() {
  int boxWidth = 59;
  int contentLines = 12;

  printAddCategoryHeader(boxWidth, contentLines);
  printCategoryTypeOptions(boxWidth);

  // 1. Baca tipe kategori
  enum TransactionType type;
  if (!readCategoryType(&type, boxWidth)) {
    showErrorMessage("Tipe tidak valid.");
    waitForEnter();
    return;
  }

  // 2. Baca nama display
  char displayName[50];
  if (!readDisplayCategoryName(displayName, sizeof(displayName), boxWidth)) {
    showErrorMessage("Nama kategori tidak valid.");
    waitForEnter();
    return;
  }

  // 3. Generate internal name
  char internalName[50];
  generateInternalCategoryName(internalName, displayName, sizeof(internalName));

  // 4. Load category list
  struct CategoryList *categories = loadUserCategories();

  // 5. Cek duplikat
  if (isDuplicateCategory(categories, internalName)) {
    showErrorMessage("Kategori dengan nama tersebut sudah ada.");
    freeCategoryList(categories);
    waitForEnter();
    return;
  }

  // 6. Tambah kategori
  if (addNewCategory(categories, internalName, displayName, type)) {
    showAddCategorySuccess(displayName);
  } else {
    showErrorMessage("Gagal menambahkan kategori.");
  }

  freeCategoryList(categories);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : printDeleteCategoryHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header menu hapus kategori custom.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printDeleteCategoryHeader(int boxWidth) {
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("               🗑️  HAPUS KATEGORI CUSTOM                 ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCustomCategoryListHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header daftar kategori custom.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printCustomCategoryListHeader(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📋 KATEGORI CUSTOM:\n");
  printCenteredColored(
      "═══════════════════════════════════════════════════════════\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCustomCategoryItem(cat, index, boxWidth)
   DESKRIPSI :
      Menampilkan satu kategori custom beserta tipe (pendapatan/pengeluaran).
   I.S.  : Item kategori tersedia.
   F.S.  : Baris kategori tampil di layar.
   ---------------------------------------------------------*/
void printCustomCategoryItem(struct CategoryItem *cat, int index,
                             int boxWidth) {
  printCentered("  ", boxWidth);
  printf("%d. %s ", index + 1, cat->displayName);

  if (cat->type == TRANSACTION_INCOME)
    printSuccess("(Pendapatan)\n");
  else
    printError("(Pengeluaran)\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printDefaultCategoryNote(boxWidth)
   DESKRIPSI :
      Menampilkan catatan bahwa kategori default tidak bisa dihapus.
   I.S.  : Catatan belum tampil.
   F.S.  : Catatan tampil di layar.
   ---------------------------------------------------------*/
void printDefaultCategoryNote(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Catatan: Kategori default tidak dapat dihapus\n");
}

/* ---------------------------------------------------------
   FUNGSI : collectCustomCategoryIndices(list, outIndices)
   DESKRIPSI :
      Mengumpulkan indeks semua kategori custom.
   INPUT  : list = daftar kategori.
            outIndices = array untuk menampung indeks custom.
   OUTPUT : jumlah kategori custom.
   ---------------------------------------------------------*/
int collectCustomCategoryIndices(struct CategoryList *list, int *outIndices) {
  int count = 0;

  for (int i = 0; i < list->count; i++) {
    if (!list->items[i]->isDefault) {
      outIndices[count++] = i;
    }
  }

  return count;
}

/* ---------------------------------------------------------
   PROSEDUR : printAllCustomCategories(list, indices, count, boxWidth)
   DESKRIPSI :
      Menampilkan semua kategori custom berdasarkan indeks.
   I.S.  : Data kategori custom tersedia.
   F.S.  : Semua kategori custom tampil di layar.
   ---------------------------------------------------------*/
void printAllCustomCategories(struct CategoryList *list, int *indices,
                              int count, int boxWidth) {
  for (int i = 0; i < count; i++) {
    struct CategoryItem *item = list->items[indices[i]];
    printCustomCategoryItem(item, indices[i], boxWidth);
  }
}

/* ---------------------------------------------------------
   FUNGSI : readDeleteCategoryChoice(max, choice, boxWidth)
   DESKRIPSI :
      Membaca input user untuk memilih kategori yang ingin dihapus.
   INPUT  : max = jumlah kategori custom, boxWidth = lebar tampilan.
   OUTPUT : true jika input valid, false jika gagal.
            choice = indeks pilihan kategori (0 untuk batal).
   ---------------------------------------------------------*/
bool readDeleteCategoryChoice(int max, int *choice, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  return readAndValidateInteger(
      "🎯 Pilih kategori untuk dihapus (0 untuk batal): ", 0, max, choice);
}

/* ---------------------------------------------------------
   FUNGSI : readDeleteConfirmation(categoryName, boxWidth)
   DESKRIPSI :
      Meminta konfirmasi user sebelum menghapus kategori.
   INPUT  : categoryName = nama kategori, boxWidth = lebar tampilan.
   OUTPUT : true jika user konfirmasi y/Y, false jika n/N.
   ---------------------------------------------------------*/
bool readDeleteConfirmation(const char *categoryName, int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  Anda yakin ingin menghapus kategori '");
  printf("%s'? (y/n): ", categoryName);

  char c;
  scanf(" %c", &c);
  clearInputBuffer();

  return (c == 'y' || c == 'Y');
}

/* ---------------------------------------------------------
   FUNGSI : deleteCategoryByInternalName(internalName)
   DESKRIPSI :
      Menghapus kategori dari daftar berdasarkan internal name.
   INPUT  : internalName = nama internal kategori.
   OUTPUT : true jika berhasil, false jika gagal.
   ---------------------------------------------------------*/
bool deleteCategoryByInternalName(const char *internalName) {
  struct CategoryList *fullList = loadUserCategories();
  bool success = false;

  if (removeCategoryFromList(fullList, internalName)) {
    saveUserCategories(fullList);
    refreshCategoryCache();
    success = true;
  }

  freeCategoryList(fullList);
  return success;
}

/* ---------------------------------------------------------
   PROSEDUR : openDeleteCategoryMenu()
   DESKRIPSI :
      Menjalankan alur hapus kategori custom
   I.S.  : Daftar kategori tersedia.
   F.S.  : Kategori berhasil dihapus jika valid
   ---------------------------------------------------------*/
void openDeleteCategoryMenu() {
  int boxWidth = 59;
  struct CategoryList *categories = getUserCategoriesCache();

  printDeleteCategoryHeader(boxWidth);
  printCustomCategoryListHeader(boxWidth);

  int customIndices[100];
  int customCount = collectCustomCategoryIndices(categories, customIndices);

  if (customCount == 0) {
    showWarningMessage("Tidak ada kategori custom untuk dihapus.");
    waitForEnter();
    return;
  }

  printAllCustomCategories(categories, customIndices, customCount, boxWidth);
  printDefaultCategoryNote(boxWidth);

  int choice;
  if (!readDeleteCategoryChoice(customCount, &choice, boxWidth)) {
    showErrorMessage("Pilihan tidak valid.");
    waitForEnter();
    return;
  }

  if (choice == 0) {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  int index = customIndices[choice - 1];
  const char *internalName = categories->items[index]->internalName;
  const char *displayName = categories->items[index]->displayName;

  if (!canDeleteCategory(internalName)) {
    showErrorMessage("Kategori sedang digunakan dan tidak dapat dihapus.");
    waitForEnter();
    return;
  }

  if (!readDeleteConfirmation(displayName, boxWidth)) {
    showInfoMessage("Penghapusan dibatalkan.");
    waitForEnter();
    return;
  }

  if (deleteCategoryByInternalName(internalName)) {
    showSuccessMessage("Kategori berhasil dihapus!");
  } else {
    showErrorMessage("Gagal menghapus kategori.");
  }
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu kelola kategori.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printCategoryManagementHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                🏷️  KELOLA KATEGORI                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementMenuItem(index, emoji, label, boxWidth)
   DESKRIPSI :
      Menampilkan satu item menu kelola kategori dengan nomor dan emoji.
   I.S.  : Menu belum tampil.
   F.S.  : Item menu tampil di layar.
   ---------------------------------------------------------*/
void printCategoryManagementMenuItem(int index, const char *emoji,
                                     const char *label, int boxWidth) {
  printCentered("│  ", boxWidth);
  printf("%s%d.%s", COLOR_BRIGHT_CYAN, index, COLOR_RESET);
  printf(" %s %s", emoji, label);

  int pad = 51 - (int)strlen(label);
  for (int i = 0; i < pad; i++)
    printf(" ");

  printf("│\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementFooter(boxWidth)
   DESKRIPSI :
      Menampilkan footer menu kelola kategori dan prompt input.
   I.S.  : Footer belum tampil.
   F.S.  : Footer tampil dan siap input.
   ---------------------------------------------------------*/
void printCategoryManagementFooter(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

/* ---------------------------------------------------------
   FUNGSI : readCategoryManagementChoice()
   DESKRIPSI :
      Membaca pilihan menu kelola kategori (1-4).
   INPUT  : -
   OUTPUT : pilihan valid (1-4), -1 jika gagal.
   ---------------------------------------------------------*/
int readCategoryManagementChoice() { return getValidatedMenuChoice(1, 4); }

/* ---------------------------------------------------------
   PROSEDUR : openCategoryManagementMenu()
   DESKRIPSI :
      Menjalankan alur menu kelola kategori (lihat, tambah, hapus, kembali).
   I.S.  : Menu kategori belum tampil.
   F.S.  : Menu selesai dijalankan sesuai pilihan.
   ---------------------------------------------------------*/
void openCategoryManagementMenu() {
  int boxWidth = 59;

  while (1) {
    int contentLines = 10;

    printCategoryManagementHeader(boxWidth, contentLines);
    printCategoryManagementMenuItem(1, "📋", "Lihat Semua Kategori", boxWidth);
    printCategoryManagementMenuItem(2, "➕", "Tambah Kategori Baru", boxWidth);
    printCategoryManagementMenuItem(3, "🗑️", "Hapus Kategori Custom", boxWidth);
    printCategoryManagementMenuItem(4, "⬅️", "Kembali ke Menu Utama", boxWidth);
    printCategoryManagementFooter(boxWidth);

    int choice = readCategoryManagementChoice();
    if (choice == -1)
      continue;
    switch (choice) {
    case 1:
      showAllCategories();
      waitForEnter();
      break;
    case 2:
      openAddCategoryMenu();
      break;
    case 3:
      openDeleteCategoryMenu();
      break;
    case 4:
      return;
    }
  }
}

/* ---------------------------------------------------------
   PROSEDUR : printResetBudgetHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu reset semua budget.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printResetBudgetHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("              🔄 RESET SEMUA BUDGET                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printResetBudgetWarnings(boxWidth)
   DESKRIPSI :
      Menampilkan peringatan dan info default budget.
   I.S.  : Peringatan belum tampil.
   F.S.  : Peringatan tampil di layar.
   ---------------------------------------------------------*/
void printResetBudgetWarnings(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning(
      "⚠️  PERINGATAN: Ini akan mengatur semua budget ke nilai default!\n");

  printCentered("", boxWidth);
  printInfo("📊 Budget default untuk setiap kategori: ");
  printf("%sRp 1,000,000%s\n\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
}

/* ---------------------------------------------------------
   FUNGSI : readResetBudgetConfirmation(boxWidth)
   DESKRIPSI :
      Membaca konfirmasi user untuk reset semua budget.
   INPUT  : boxWidth = lebar tampilan.
   OUTPUT : true jika user yakin (y/Y), false jika tidak.
   ---------------------------------------------------------*/
bool readResetBudgetConfirmation(int boxWidth) {
  char c;

  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);

  scanf(" %c", &c);
  clearInputBuffer();

  return (c == 'y' || c == 'Y');
}

/* ---------------------------------------------------------
   PROSEDUR : resetBudgetForReport(report, categories)
   DESKRIPSI :
      Mengatur semua kategori pada satu laporan menjadi default Rp 1,000,000.
   I.S.  : Laporan dan kategori tersedia.
   F.S.  : Semua budget di laporan direset dan tersimpan.
   ---------------------------------------------------------*/
void resetBudgetForReport(struct MonthReport *report,
                          struct CategoryList *categories) {
  for (int i = 0; i < categories->count; i++) {
    struct CategoryItem *cat = categories->items[i];
    if (cat != NULL) {
      setBudgetForCategory(report, cat->internalName, 1000000);
    }
  }
  saveUserMonthReport(report);
}

/* ---------------------------------------------------------
   PROSEDUR : resetBudgetForAllReports(monthReportList, categories)
   DESKRIPSI :
      Mengatur semua budget di semua laporan menjadi default.
   I.S.  : Daftar laporan dan kategori tersedia.
   F.S.  : Semua budget di semua laporan direset.
   ---------------------------------------------------------*/
void resetBudgetForAllReports(struct MonthReportList *monthReportList,
                              struct CategoryList *categories) {
  for (int i = 0; i < monthReportList->count; i++) {
    resetBudgetForReport(monthReportList->reports[i], categories);
  }
}

/* ---------------------------------------------------------
   PROSEDUR : openResetBudgetMenu(monthReportList)
   DESKRIPSI :
      Menjalankan alur reset semua budget:
      menampilkan header, peringatan, konfirmasi, reset, dan pesan sukses.
   I.S.  : Laporan dan kategori tersedia.
   F.S.  : Semua budget direset jika user konfirmasi, pesan tampil.
   ---------------------------------------------------------*/
void openResetBudgetMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;
  int contentLines = 10;

  printResetBudgetHeader(boxWidth, contentLines);
  printResetBudgetWarnings(boxWidth);

  if (!readResetBudgetConfirmation(boxWidth)) {
    showInfoMessage("Reset budget dibatalkan.");
    return;
  }

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    showErrorMessage("Kategori tidak tersedia.");
    return;
  }

  resetBudgetForAllReports(monthReportList, categories);

  showSuccessMessage("Semua budget berhasil direset ke Rp 1,000,000!");
}

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu pengaturan sistem.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printConfigurationHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("                🔧 PENGATURAN SISTEM                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "├─────────────────────────────────────────────────────────┤\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationMenuItem(index, emoji, label, boxWidth)
   DESKRIPSI :
      Menampilkan satu item menu pengaturan sistem.
   I.S.  : Item belum tampil.
   F.S.  : Item tampil di layar.
   ---------------------------------------------------------*/
void printConfigurationMenuItem(int index, const char *emoji, const char *label,
                                int boxWidth) {
  printCentered("│  ", boxWidth);
  printColored("%d.", COLOR_BRIGHT_CYAN);
  printf(" %s %s", emoji, label);

  int padding = 51 - (int)strlen(label);
  for (int i = 0; i < padding; i++)
    printf(" ");

  printf("│\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationFooter(boxWidth)
   DESKRIPSI :
      Menampilkan footer menu pengaturan sistem dan prompt input.
   I.S.  : Footer belum tampil.
   F.S.  : Footer tampil dan siap input.
   ---------------------------------------------------------*/
void printConfigurationFooter(int boxWidth) {
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Pilihan Anda: ", COLOR_BRIGHT_YELLOW);
}

/* ---------------------------------------------------------
   FUNGSI : readConfigurationChoice()
   DESKRIPSI :
      Membaca pilihan menu pengaturan sistem (1-6).
   INPUT  : -
   OUTPUT : pilihan valid (1-6), -1 jika gagal.
   ---------------------------------------------------------*/
int readConfigurationChoice() { return getValidatedMenuChoice(1, 6); }

/* ---------------------------------------------------------
   PROSEDUR : openConfigurationMenu(monthReportList)
   DESKRIPSI :
      Menjalankan alur menu pengaturan sistem sesuai pilihan:
      set minimum amount/budget/transaction, lihat pengaturan, reset, kembali.
   I.S.  : Menu belum tampil.
   F.S.  : Menu selesai dijalankan sesuai pilihan user.
   ---------------------------------------------------------*/
void openConfigurationMenu(struct MonthReportList *monthReportList) {
  int boxWidth = 59;

  while (1) {
    printConfigurationHeader(boxWidth, 12);

    printConfigurationMenuItem(1, "💰", "Atur Minimum Jumlah Uang", boxWidth);
    printConfigurationMenuItem(2, "📊", "Atur Minimum Budget", boxWidth);
    printConfigurationMenuItem(3, "💳", "Atur Minimum Transaksi", boxWidth);
    printConfigurationMenuItem(4, "📋", "Lihat Pengaturan Saat Ini", boxWidth);
    printConfigurationMenuItem(5, "🔄", "Reset ke Pengaturan Default",
                               boxWidth);
    printConfigurationMenuItem(6, "⬅️", "Kembali ke Menu Utama", boxWidth);

    printConfigurationFooter(boxWidth);

    int choice = readConfigurationChoice();
    if (choice == -1)
      continue;

    switch (choice) {
    case 1:
      openSetMinimumAmountMenu();
      break;
    case 2:
      openSetMinimumBudgetMenu();
      break;
    case 3:
      openSetMinimumTransactionMenu();
      break;
    case 4:
      openViewConfigurationMenu();
      break;
    case 5:
      openResetConfigurationMenu();
      break;
    case 6:
      return;
    }
  }
}

/* ---------------------------------------------------------
   PROSEDUR : printMinimumAmountHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu untuk mengatur minimum jumlah uang.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printMinimumAmountHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("             💰 ATUR MINIMUM JUMLAH UANG                 ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printMinimumBudgetHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu untuk mengatur minimum budget.
   I.S.  : Header belum tampil.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printMinimumBudgetHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("                📊 ATUR MINIMUM BUDGET                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumAmount(boxWidth)
   DESKRIPSI :
      Menampilkan nilai minimum jumlah uang saat ini.
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Nilai minimum saat ini ditampilkan.
   ---------------------------------------------------------*/
void showCurrentMinimumAmount(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("💰 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumAmount(), COLOR_RESET);
}

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumBudget(boxWidth)
   DESKRIPSI :
      Menampilkan nilai minimum budget saat ini.
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Nilai minimum saat ini ditampilkan.
   ---------------------------------------------------------*/
void showCurrentMinimumBudget(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("📊 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, getMinimumBudget(), COLOR_RESET);
}

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumAmount(outValue)
   DESKRIPSI :
      Membaca input user untuk nilai minimum jumlah uang baru.
   INPUT  : outValue = pointer untuk menyimpan nilai.
   OUTPUT : true jika input valid, false jika gagal.
   ---------------------------------------------------------*/
bool readNewMinimumAmount(long long *outValue) {
  return readAndValidateLongLong("💰 Masukkan minimum baru (minimal 1): ", 1,
                                 getMaximumAmount(), outValue);
}

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumBudget(outValue)
   DESKRIPSI :
      Membaca input user untuk nilai minimum budget baru.
   INPUT  : outValue = pointer untuk menyimpan nilai.
   OUTPUT : true jika input valid, false jika gagal.
   ---------------------------------------------------------*/
bool readNewMinimumBudget(long long *outValue) {
  return readAndValidateLongLong("📊 Masukkan minimum baru (minimal 1): ", 1,
                                 getMaximumBudget(), outValue);
}

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumAmount(value)
   DESKRIPSI :
      Menyimpan nilai minimum jumlah uang baru dan menampilkan pesan sukses.
   INPUT  : value = nilai minimum baru.
   F.S.  : Konfigurasi tersimpan di file dan pesan sukses ditampilkan.
   ---------------------------------------------------------*/
void applyNewMinimumAmount(long long value) {
  setMinimumAmount(value);
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg),
           "Minimum jumlah uang berhasil diatur menjadi Rp %lld", value);
  showSuccessMessage(msg);
}

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumBudget(value)
   DESKRIPSI :
      Menyimpan nilai minimum budget baru dan menampilkan pesan sukses.
   INPUT  : value = nilai minimum baru.
   F.S.  : Konfigurasi tersimpan di file dan pesan sukses ditampilkan.
   ---------------------------------------------------------*/
void applyNewMinimumBudget(long long value) {
  setMinimumBudget(value);
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg), "Minimum budget berhasil diatur menjadi Rp %lld",
           value);
  showSuccessMessage(msg);
}

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumAmountMenu()
   DESKRIPSI :
      Menjalankan alur menu untuk mengatur minimum jumlah uang:
      menampilkan header, nilai saat ini, membaca input, dan menyimpan.
   I.S.  : Menu belum tampil.
   F.S.  : Nilai minimum diupdate sesuai input user.
   ---------------------------------------------------------*/
void openSetMinimumAmountMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumAmountHeader(boxWidth, contentLines);
  showCurrentMinimumAmount(boxWidth);

  long long value;
  if (!readNewMinimumAmount(&value)) {
    showErrorMessage("Input tidak valid.");
    return;
  }

  applyNewMinimumAmount(value);
}

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumBudgetMenu()
   DESKRIPSI :
      Menjalankan alur menu untuk mengatur minimum budget:
      menampilkan header, nilai saat ini, membaca input, dan menyimpan.
   I.S.  : Menu belum tampil.
   F.S.  : Nilai minimum budget diupdate sesuai input user.
   ---------------------------------------------------------*/
void openSetMinimumBudgetMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumBudgetHeader(boxWidth, contentLines);
  showCurrentMinimumBudget(boxWidth);

  long long value;
  if (!readNewMinimumBudget(&value)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  applyNewMinimumBudget(value);
}

/* ---------------------------------------------------------
   PROSEDUR : printMinimumTransactionHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu untuk mengatur minimum transaksi.
   INPUT  :
      boxWidth = lebar tampilan.
      contentLines = jumlah baris kosong vertikal di atas.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printMinimumTransactionHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);
  printCentered("│", boxWidth);
  printWithBg("              💳 ATUR MINIMUM TRANSAKSI                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumTransaction(boxWidth)
   DESKRIPSI :
      Menampilkan nilai minimum transaksi saat ini.
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Nilai minimum transaksi ditampilkan.
   ---------------------------------------------------------*/
void showCurrentMinimumTransaction(int boxWidth) {
  printCentered("", boxWidth);
  printInfo("💳 Minimum saat ini: ");
  printf("%sRp %lld%s\n", COLOR_BRIGHT_CYAN, globalConfig.money.minimumBudget,
         COLOR_RESET);
}

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumTransaction(outValue)
   DESKRIPSI :
      Membaca input user untuk nilai minimum transaksi baru.
   INPUT  : outValue = pointer untuk menyimpan nilai.
   OUTPUT : true jika input valid, false jika gagal.
   ---------------------------------------------------------*/
bool readNewMinimumTransaction(long long *outValue) {
  return readAndValidateLongLong("💳 Masukkan minimum baru (minimal 1): ", 1,
                                 globalConfig.money.minimumBudget, outValue);
}

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumTransaction(value)
   DESKRIPSI :
      Menyimpan nilai minimum transaksi baru dan menampilkan pesan sukses.
   INPUT  : value = nilai minimum transaksi baru.
   F.S.  : Konfigurasi tersimpan di file dan pesan sukses ditampilkan.
   ---------------------------------------------------------*/
void applyNewMinimumTransaction(long long value) {
  globalConfig.money.minimumBudget = value;
  saveConfigToFile("./config.txt");

  char msg[200];
  snprintf(msg, sizeof(msg),
           "Minimum transaksi berhasil diatur menjadi Rp %lld", value);
  showSuccessMessage(msg);
}

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumTransactionMenu()
   DESKRIPSI :
      Menjalankan alur menu untuk mengatur minimum transaksi:
      menampilkan header, nilai saat ini, membaca input, dan menyimpan.
   I.S.  : Menu belum tampil.
   F.S.  : Nilai minimum transaksi diupdate sesuai input user.
   ---------------------------------------------------------*/
void openSetMinimumTransactionMenu() {
  int boxWidth = 59;
  int contentLines = 8;

  printMinimumTransactionHeader(boxWidth, contentLines);
  showCurrentMinimumTransaction(boxWidth);

  long long newMin;
  if (!readNewMinimumTransaction(&newMin)) {
    showErrorMessage("Input tidak valid.");
    return;
  }
  applyNewMinimumTransaction(newMin);
}

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationHeaderKeuangan(boxWidth)
   DESKRIPSI :
      Menampilkan header umum menu pengaturan saat ini (keuangan & validasi).
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Header tampil di layar.
   ---------------------------------------------------------*/
void printConfigurationHeaderKeuangan(int boxWidth) {
  clearScreen();
  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", 59,
      COLOR_BRIGHT_CYAN);
  printCentered("│", 59);
  printWithBg("              📋 PENGATURAN SAAT INI                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");
  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", 59,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printFinanceConfigHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header tabel pengaturan keuangan.
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Header tabel tampil di layar.
   ---------------------------------------------------------*/
void printFinanceConfigHeader(int boxWidth) {
  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "══════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║", boxWidth);
  printWithBg("                              💰 PENGATURAN KEUANGAN            "
              "                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");

  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║ ", boxWidth);
  printf("%s%-30s%s │ ", COLOR_BRIGHT_CYAN, "KATEGORI", COLOR_RESET);
  printf("%s%-20s%s │ ", COLOR_BRIGHT_CYAN, "MINIMUM", COLOR_RESET);
  printf("%s%-20s%s ║\n", COLOR_BRIGHT_CYAN, "MAXIMUM", COLOR_RESET);
}

/* ---------------------------------------------------------
   PROSEDUR : printFinanceConfigRows(boxWidth)
   DESKRIPSI :
      Menampilkan nilai minimum dan maksimum untuk jumlah uang, budget, dan
   transaksi. INPUT  : boxWidth = lebar tampilan. F.S.  : Semua nilai keuangan
   ditampilkan.
   ---------------------------------------------------------*/
void printFinanceConfigRows(int boxWidth) {
  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Jumlah Uang");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_GREEN, getMinimumAmount(),
         COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_GREEN, getMaximumAmount(),
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Budget");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_CYAN, getMinimumBudget(),
         COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_CYAN, getMaximumBudget(),
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-30s │ ", "Transaksi");
  printf("%sRp %-17lld%s │ ", COLOR_BRIGHT_YELLOW,
         globalConfig.money.minimumTransactionAmount, COLOR_RESET);
  printf("%sRp %-17lld%s ║\n", COLOR_BRIGHT_YELLOW,
         globalConfig.money.maximumTransactionAmount, COLOR_RESET);
}

/* ---------------------------------------------------------
   PROSEDUR : printValidationConfigHeader(boxWidth)
   DESKRIPSI :
      Menampilkan header tabel pengaturan validasi input.
   INPUT  : boxWidth = lebar tampilan.
   F.S.  : Header validasi tampil di layar.
   ---------------------------------------------------------*/
void printValidationConfigHeader(int boxWidth) {
  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printf("\n");
  printCenteredColored("╔══════════════════════════════════════════════════════"
                       "══════════════════════════════╗\n",
                       boxWidth, COLOR_BRIGHT_CYAN);

  printCentered("║", boxWidth);
  printWithBg("                             ⚙️  PENGATURAN VALIDASI             "
              "                    ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");

  printCenteredColored("╠══════════════════════════════════════════════════════"
                       "══════════════════════════════╣\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printValidationConfigRows(boxWidth)
   DESKRIPSI :
      Menampilkan pengaturan validasi input, seperti panjang nama, deskripsi,
   percobaan, dan tahun. INPUT  : boxWidth = lebar tampilan. F.S.  : Semua
   pengaturan validasi ditampilkan.
   ---------------------------------------------------------*/
void printValidationConfigRows(int boxWidth) {
  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Panjang Nama");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxNameLength, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Panjang Deskripsi");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxDescriptionLength, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Maksimal Percobaan Input");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN,
         globalConfig.validation.maxValidationAttempts, COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Tahun Minimum");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN, globalConfig.validation.minYear,
         COLOR_RESET);

  printCentered("║ ", boxWidth);
  printf("%-40s │ ", "Tahun Maksimum");
  printf("%s%-40d%s ║\n", COLOR_BRIGHT_CYAN, globalConfig.validation.maxYear,
         COLOR_RESET);

  printCenteredColored("╚══════════════════════════════════════════════════════"
                       "══════════════════════════════╝\n",
                       boxWidth, COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : openViewConfigurationMenu()
   DESKRIPSI :
      Menampilkan seluruh konfigurasi saat ini:
      1. Keuangan (minimum & maksimum jumlah uang, budget, transaksi)
      2. Validasi input
      Menunggu user menekan enter untuk kembali.
   I.S.  : Menu belum tampil.
   F.S.  : Semua pengaturan saat ini ditampilkan.
   ---------------------------------------------------------*/
void openViewConfigurationMenu() {
  int boxWidth = 88;

  printConfigurationHeaderKeuangan(boxWidth);
  printFinanceConfigHeader(boxWidth);
  printFinanceConfigRows(boxWidth);
  printValidationConfigHeader(boxWidth);
  printValidationConfigRows(boxWidth);
  waitForEnter();
}

/* ---------------------------------------------------------
   PROSEDUR : printResetConfigHeader(boxWidth, contentLines)
   DESKRIPSI :
      Menampilkan header menu reset pengaturan ke default.
   INPUT  :
      boxWidth = lebar tampilan
      contentLines = jumlah baris kosong vertikal
   F.S.  : Header reset tampil di layar
   ---------------------------------------------------------*/
void printResetConfigHeader(int boxWidth, int contentLines) {
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "┌─────────────────────────────────────────────────────────┐\n", boxWidth,
      COLOR_BRIGHT_CYAN);

  printCentered("│", boxWidth);
  printWithBg("           🔄 RESET PENGATURAN DEFAULT                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("│\n");

  printCenteredColored(
      "└─────────────────────────────────────────────────────────┘\n", boxWidth,
      COLOR_BRIGHT_CYAN);
}

/* ---------------------------------------------------------
   PROSEDUR : printResetConfigWarning(boxWidth)
   DESKRIPSI :
      Menampilkan peringatan bahwa semua konfigurasi akan dikembalikan ke
   default. INPUT  : boxWidth = lebar tampilan F.S.  : Peringatan ditampilkan di
   layar
   ---------------------------------------------------------*/
void printResetConfigWarning(int boxWidth) {
  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  PERINGATAN: Ini akan mengembalikan semua pengaturan ke "
               "nilai default!\n\n");
}

/* ---------------------------------------------------------
   FUNGSI : readResetConfigConfirmation(boxWidth, outConfirm)
   DESKRIPSI :
      Membaca konfirmasi user untuk reset konfigurasi (y/n)
   INPUT  :
      boxWidth = lebar tampilan
      outConfirm = pointer char untuk menyimpan input user
   OUTPUT : true jika user mengonfirmasi 'y' atau 'Y', false jika tidak
   ---------------------------------------------------------*/
bool readResetConfigConfirmation(int boxWidth, char *outConfirm) {
  printCentered("", boxWidth);
  printColored("❓ Apakah Anda yakin? (y/n): ", COLOR_BRIGHT_YELLOW);

  scanf(" %c", outConfirm);
  clearInputBuffer();

  return (*outConfirm == 'y' || *outConfirm == 'Y');
}

/* ---------------------------------------------------------
   PROSEDUR : applyResetConfiguration()
   DESKRIPSI :
      Mengembalikan semua pengaturan ke nilai default, menyimpan ke file,
      dan menampilkan pesan sukses.
   INPUT  : -
   F.S.  : Konfigurasi tersimpan ke default, pesan sukses tampil
   ---------------------------------------------------------*/
void applyResetConfiguration() {
  initializeDefaultConfig();
  saveConfigToFile("./config.txt");

  showSuccessMessage("Pengaturan berhasil direset ke nilai default! Silakan "
                     "restart aplikasi untuk melihat perubahan.");
}

/* ---------------------------------------------------------
   PROSEDUR : printResetCancelledMessage()
   DESKRIPSI :
      Menampilkan pesan bahwa reset konfigurasi dibatalkan.
   INPUT  : -
   F.S.  : Pesan ditampilkan di layar
   ---------------------------------------------------------*/
void printResetCancelledMessage() {
  showInfoMessage("Reset pengaturan dibatalkan.");
}

/* ---------------------------------------------------------
   PROSEDUR : openResetConfigurationMenu()
   DESKRIPSI :
      Menjalankan alur menu reset konfigurasi
   I.S.  : Konfigurasi belum direset
   F.S.  : Konfigurasi direset atau dibatalkan sesuai pilihan user
   ---------------------------------------------------------*/
void openResetConfigurationMenu() {
  int boxWidth = 59;
  int contentLines = 10;

  printResetConfigHeader(boxWidth, contentLines);
  printResetConfigWarning(boxWidth);

  char confirm;
  bool confirmed = readResetConfigConfirmation(boxWidth, &confirm);

  if (!confirmed) {
    printResetCancelledMessage();
    return;
  }
  applyResetConfiguration();
}

/* ---------------------------------------------------------
   FUNGSI : isAdmin()
   DESKRIPSI :
      Mengecek apakah user saat ini adalah admin.
   OUTPUT : true jika admin, false jika bukan
   ---------------------------------------------------------*/
bool isAdmin() { return currentUser && currentUser->isAdmin; }

/* ---------------------------------------------------------
   FUNGSI : getMainMenuMaxChoice()
   DESKRIPSI :
      Mengembalikan jumlah pilihan menu utama berdasarkan status user.
   OUTPUT : 8 jika admin, 7 jika user biasa
   ---------------------------------------------------------*/
int getMainMenuMaxChoice() { return isAdmin() ? 8 : 7; }

/* ---------------------------------------------------------
   PROSEDUR : handleMenuChoiceUser(choice, monthReportList)
   DESKRIPSI :
      Menangani pilihan menu utama untuk user biasa.
   I.S :
      choice = pilihan menu
      monthReportList = pointer daftar laporan bulanan
   F.S.  : Menu terkait dijalankan, logout/exit jika dipilih
   ---------------------------------------------------------*/
void handleMenuChoiceUser(int choice, struct MonthReportList *monthReportList) {
  switch (choice) {
  case 1:
    openMonthlyMenu(monthReportList);
    break;
  case 2:
    openSummaryMenu(monthReportList);
    break;
  case 3:
    openBudgetSettingsMenu(monthReportList);
    break;
  case 4:
    openCategoryManagementMenu();
    break;
  case 5:
    openConfigurationMenu(monthReportList);
    break;
  case 6:
    printUserLogoutScreen();
    return;
  case 7:
    printUserExitScreen();
    exit(0);
  default:
    showErrorMessage("Pilihan tidak valid.");
  }
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminMenuScreen()
   DESKRIPSI : Menampilkan layar menu admin.
   I.S.       : Layar bisa berisi data apa saja
   F.S.       : Layar menampilkan menu admin
---------------------------------------------------------*/
void printAdminMenuScreen() {
  clearScreen();
  showAdminMenu();
}

/* ---------------------------------------------------------
   FUNGSI : readAdminMenuChoice(outChoice)
   DESKRIPSI : Membaca pilihan menu admin dari input user.
   INPUT  : outChoice = pointer ke variabel untuk menyimpan pilihan
   OUTPUT : true jika input valid (integer), false jika tidak
---------------------------------------------------------*/
bool readAdminMenuChoice(int *outChoice) {
  int ch;
  if (scanf("%d", outChoice) != 1)
    return false;

  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
  return true;
}

/* ---------------------------------------------------------
PROSEDUR : handleAdminPanelSelection(adminChoice)
DESKRIPSI : Menjalankan aksi sesuai pilihan panel admin.
I.S.       : Admin menu ditampilkan
F.S.       : Aksi panel admin dijalankan sesuai pilihan
---------------------------------------------------------*/
void handleAdminPanelSelection(int adminChoice) {
  switch (adminChoice) {
  case 1:
    handleAdminUserManagement();
    break;
  case 2:
    viewAllUserReports();
    break;
  case 3:
    showSystemStatistics();
    break;
  case 4:
    break;
  case 5:
    printAdminLogoutScreen();
    break;
  }
}

/* ---------------------------------------------------------
   PROSEDUR : printUserLogoutScreen()
   DESKRIPSI : Menampilkan layar logout untuk user biasa.
   I.S.       : User masih login
   F.S.       : Layar menampilkan logout user dan info kembali ke login
---------------------------------------------------------*/
void printUserLogoutScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      🔓 LOGOUT                          ║\n");
  printf("║                                                          ║\n");
  printf("║               Logout pengguna: %-21s     ║\n",
         currentUser ? currentUser->username : "Tidak Diketahui");
  printf("║                                                          ║\n");
  printf("║                 Kembali ke layar login...               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminLogoutScreen()
   DESKRIPSI : Menampilkan layar logout untuk admin.
   I.S.       : Admin masih login
   F.S.       : Layar menampilkan logout admin dan info kembali ke login
---------------------------------------------------------*/
void printAdminLogoutScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      🔓 LOGOUT                          ║\n");
  printf("║                                                          ║\n");
  printf("║               Logout admin: %-23s     ║\n",
         currentUser ? currentUser->username : "Tidak Diketahui");
  printf("║                                                          ║\n");
  printf("║                 Kembali ke layar login...               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printUserExitScreen()
   DESKRIPSI : Menampilkan layar ucapan terima kasih untuk user saat keluar
aplikasi. I.S.       : User masih menggunakan aplikasi F.S.       : Layar
menampilkan pesan perpisahan
---------------------------------------------------------*/
void printUserExitScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👋 TERIMA KASIH!                     ║\n");
  printf("║                                                          ║\n");
  printf("║          Semoga keuangan Anda selalu terjaga! 💰        ║\n");
  printf("║                                                          ║\n");
  printf("║                   Sampai jumpa lagi! 😊                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : printAdminExitScreen()
   DESKRIPSI : Menampilkan layar ucapan terima kasih untuk admin saat keluar
aplikasi. I.S.       : Admin masih menggunakan aplikasi F.S.       : Layar
menampilkan pesan perpisahan
---------------------------------------------------------*/
void printAdminExitScreen() {
  clearScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👋 TERIMA KASIH!                     ║\n");
  printf("║                                                          ║\n");
  printf("║          Semoga keuangan Anda selalu terjaga! 💰👑      ║\n");
  printf("║                                                          ║\n");
  printf("║                   Sampai jumpa lagi! 😊                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\n");
}

/* ---------------------------------------------------------
   PROSEDUR : handleMenuChoiceAdmin(choice, monthReportList)
   DESKRIPSI : Menangani pilihan menu utama untuk admin.
   I.S.       : Admin menu ditampilkan
   F.S.       : Pilihan dijalankan
---------------------------------------------------------*/
void handleMenuChoiceAdmin(int choice,
                           struct MonthReportList *monthReportList) {
  switch (choice) {
  case 1:
    showErrorMessage("Admin tidak memiliki laporan pribadi!");
    break;
  case 2:
    showErrorMessage("Admin tidak memiliki ringkasan pribadi!");
    break;
  case 3:
    showErrorMessage("Admin tidak memiliki budget pribadi!");
    break;
  case 4:
    showErrorMessage("Admin tidak memiliki kategori pribadi!");
    break;
  case 5:
    openConfigurationMenu(monthReportList);
    break;
  case 6: {
    printAdminMenuScreen();
    int adminChoice;
    if (readAdminMenuChoice(&adminChoice))
      handleAdminPanelSelection(adminChoice);
    break;
  }
  case 7:
    printAdminLogoutScreen();
    return;
  case 8:
    printAdminExitScreen();
    exit(0);
  default:
    showErrorMessage("Pilihan tidak valid.");
  }
}

/* ---------------------------------------------------------
   PROSEDUR : openMainMenu(monthReportList)
   DESKRIPSI : Menampilkan menu utama
   I.S.       : Layar Kosong
   F.S.       : Menu utama Ditampilkan
---------------------------------------------------------*/
void openMainMenu(struct MonthReportList *monthReportList) {
  while (1) {
    showMainMenu();

    int maxChoice = getMainMenuMaxChoice();
    int choice = getValidatedMenuChoice(1, maxChoice);

    if (choice == -1)
      continue;

    if (isAdmin())
      handleMenuChoiceAdmin(choice, monthReportList);
    else
      handleMenuChoiceUser(choice, monthReportList);
  }
}
