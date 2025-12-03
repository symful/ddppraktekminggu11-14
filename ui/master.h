#include <stdbool.h>
#include "../types/include.h"
/* ---------------------------------------------------------
   PROSEDUR : showMainMenu()
   DESKRIPSI : Menampilkan menu utama aplikasi
   I.S.       : Layar bebas, bisa kosong atau menampilkan info sebelumnya
   F.S.       : Layar menampilkan menu utama
---------------------------------------------------------*/
void showMainMenu();

/* ---------------------------------------------------------
   PROSEDUR : clearScreen()
   DESKRIPSI : Membersihkan layar terminal
   I.S.       : Layar berisi teks
   F.S.       : Layar kosong
---------------------------------------------------------*/
void clearScreen();

/* ---------------------------------------------------------
   PROSEDUR : waitForEnter()
   DESKRIPSI : Menunggu pengguna menekan tombol Enter
   I.S.       : Terminal menunggu input
   F.S.       : Terminal melanjutkan eksekusi setelah Enter ditekan
---------------------------------------------------------*/
void waitForEnter();

/* ---------------------------------------------------------
   PROSEDUR : showSuccessMessage(message)
   DESKRIPSI : Menampilkan pesan sukses dengan format khusus
   I.S.       : Terminal berisi teks lainnya
   F.S.       : Pesan sukses ditampilkan
---------------------------------------------------------*/
void showSuccessMessage(const char *message);

/* ---------------------------------------------------------
   PROSEDUR : showErrorMessage(message)
   DESKRIPSI : Menampilkan pesan kesalahan
   I.S.       : Terminal berisi teks lainnya
   F.S.       : Pesan error ditampilkan
---------------------------------------------------------*/
void showErrorMessage(const char *message);

/* ---------------------------------------------------------
   PROSEDUR : showWarningMessage(message)
   DESKRIPSI : Menampilkan pesan peringatan
   I.S.       : Terminal berisi teks lainnya
   F.S.       : Pesan warning ditampilkan
---------------------------------------------------------*/
void showWarningMessage(const char *message);

/* ---------------------------------------------------------
   PROSEDUR : showInfoMessage(message)
   DESKRIPSI : Menampilkan pesan informasi
   I.S.       : Terminal berisi teks lainnya
   F.S.       : Pesan info ditampilkan
---------------------------------------------------------*/
void showInfoMessage(const char *message);

/* ---------------------------------------------------------
   FUNGSI : getValidatedMenuChoice(minChoice, maxChoice)
   DESKRIPSI : Membaca input menu dari user dan memvalidasi dalam rentang
   INPUT  : minChoice = nilai minimum yang valid
            maxChoice = nilai maksimum yang valid
   OUTPUT : nilai integer pilihan valid, -1 jika input tidak valid
---------------------------------------------------------*/
int getValidatedMenuChoice(int minChoice, int maxChoice);

/* ---------------------------------------------------------
   PROSEDUR : openSummaryMenu(monthReportList)
   DESKRIPSI : Membuka menu ringkasan laporan bulanan
   I.S.       : Terminal bebas
   F.S.       : Menu ringkasan ditampilkan dan bisa berinteraksi
---------------------------------------------------------*/
void openSummaryMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openBudgetSettingsMenu(monthReportList)
   DESKRIPSI : Membuka menu pengaturan budget
   I.S.       : Terminal bebas
   F.S.       : Menu pengaturan budget ditampilkan
---------------------------------------------------------*/
void openBudgetSettingsMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openSetCategoryBudgetMenu(monthReportList)
   DESKRIPSI : Membuka menu untuk menetapkan budget per kategori
   I.S.       : Terminal bebas
   F.S.       : Menu input budget kategori ditampilkan
---------------------------------------------------------*/
void openSetCategoryBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openViewBudgetMenu(monthReportList)
   DESKRIPSI : Menampilkan laporan budget per kategori
   I.S.       : Terminal bebas
   F.S.       : Laporan budget ditampilkan
---------------------------------------------------------*/
void openViewBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openResetBudgetMenu(monthReportList)
   DESKRIPSI : Membuka menu reset budget ke default
   I.S.       : Terminal bebas
   F.S.       : Menu reset budget dijalankan
---------------------------------------------------------*/
void openResetBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openConfigurationMenu(monthReportList)
   DESKRIPSI : Membuka menu konfigurasi sistem
   I.S.       : Terminal bebas
   F.S.       : Menu konfigurasi dijalankan
---------------------------------------------------------*/
void openConfigurationMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumAmountMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum jumlah uang
   I.S.       : Terminal bebas
   F.S.       : Minimum jumlah uang dapat diubah
---------------------------------------------------------*/
void openSetMinimumAmountMenu();

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumBudgetMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum budget kategori
   I.S.       : Terminal bebas
   F.S.       : Minimum budget dapat diubah
---------------------------------------------------------*/
void openSetMinimumBudgetMenu();

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumTransactionMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum transaksi
   I.S.       : Terminal bebas
   F.S.       : Minimum transaksi dapat diubah
---------------------------------------------------------*/
void openSetMinimumTransactionMenu();

/* ---------------------------------------------------------
   PROSEDUR : openViewConfigurationMenu()
   DESKRIPSI : Menampilkan pengaturan saat ini
   I.S.       : Terminal bebas
   F.S.       : Konfigurasi saat ini ditampilkan
---------------------------------------------------------*/
void openViewConfigurationMenu();

/* ---------------------------------------------------------
   PROSEDUR : openResetConfigurationMenu()
   DESKRIPSI : Menjalankan reset konfigurasi ke default
   I.S.       : Terminal bebas
   F.S.       : Konfigurasi diset ulang ke default
---------------------------------------------------------*/
void openResetConfigurationMenu();

/* ---------------------------------------------------------
   PROSEDUR : openMainMenu(monthReportList)
   DESKRIPSI : Menjalankan menu utama dan navigasi aplikasi
   I.S.       : Terminal bebas
   F.S.       : Menu utama dijalankan terus hingga exit
---------------------------------------------------------*/
void openMainMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printMainMenuHeader()
   DESKRIPSI : Menampilkan header menu utama
   I.S.       : Terminal bebas
   F.S.       : Header menu utama ditampilkan
---------------------------------------------------------*/
void printMainMenuHeader();

/* ---------------------------------------------------------
   PROSEDUR : printFiturMenu()
   DESKRIPSI : Menampilkan daftar fitur menu utama
   I.S.       : Terminal bebas
   F.S.       : Fitur menu ditampilkan
---------------------------------------------------------*/
void printFiturMenu();

/* ---------------------------------------------------------
   PROSEDUR : printInfoUser()
   DESKRIPSI : Menampilkan informasi user yang login
   I.S.       : Terminal bebas
   F.S.       : Info user ditampilkan
---------------------------------------------------------*/
void printInfoUser();

/* ---------------------------------------------------------
   PROSEDUR : printFiturMenuRole()
   DESKRIPSI : Menampilkan menu sesuai role user/admin
   I.S.       : Terminal bebas
   F.S.       : Menu sesuai role ditampilkan
---------------------------------------------------------*/
void printFiturMenuRole();

/* ---------------------------------------------------------
   PROSEDUR : printMainMenuFooter()
   DESKRIPSI : Menampilkan footer menu utama
   I.S.       : Terminal bebas
   F.S.       : Footer menu ditampilkan
---------------------------------------------------------*/
void printMainMenuFooter();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetHeader()
   DESKRIPSI : Menampilkan header menu budget
   I.S.       : Terminal bebas
   F.S.       : Header budget ditampilkan
---------------------------------------------------------*/
void printBudgetHeader();

/* ---------------------------------------------------------
   FUNGSI : validasiInputBudget()
   DESKRIPSI : Memvalidasi input pilihan budget user
   INPUT  : tidak ada
   OUTPUT : integer pilihan valid user
---------------------------------------------------------*/
int validasiInputBudget();

/* ---------------------------------------------------------
   PROSEDUR : handleInputBudget(choice, monthReportList, shouldExit)
   DESKRIPSI : Menangani aksi input budget sesuai pilihan user
   I.S.       : User memilih menu budget
   F.S.       : Budget diubah sesuai input user
---------------------------------------------------------*/
void handleInputBudget(int choice, struct MonthReportList *monthReportList, bool *shouldExit);

/* ---------------------------------------------------------
   FUNGSI : hasMonthReports(monthReportList)
   DESKRIPSI : Mengecek apakah ada laporan bulanan yang tersedia
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : true jika ada laporan, false jika tidak
---------------------------------------------------------*/
bool hasMonthReports(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetCategoryHeader()
   DESKRIPSI : Menampilkan header tabel kategori budget
   I.S.       : Terminal bebas
   F.S.       : Header tabel ditampilkan
---------------------------------------------------------*/
void printBudgetCategoryHeader();

/* ---------------------------------------------------------
   FUNGSI : selectMonthReport(monthReportList)
   DESKRIPSI : Memilih laporan bulanan dari daftar
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : pointer ke laporan bulan terpilih, NULL jika batal
---------------------------------------------------------*/
struct MonthReport *selectMonthReport(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   FUNGSI : selectCategory(outCategory, bufferSize)
   DESKRIPSI : Memilih kategori budget
   INPUT  : outCategory = buffer untuk menyimpan kategori
            bufferSize = ukuran buffer
   OUTPUT : true jika kategori valid dipilih, false jika dibatalkan
---------------------------------------------------------*/
bool selectCategory(char *outCategory, size_t bufferSize);

/* ---------------------------------------------------------
   FUNGSI : inputNewBudget(outBudget)
   DESKRIPSI : Memasukkan nilai budget baru
   INPUT  : outBudget = pointer untuk menyimpan nilai budget
   OUTPUT : true jika input valid, false jika invalid
---------------------------------------------------------*/
bool inputNewBudget(long long *outBudget);

/* ---------------------------------------------------------
   PROSEDUR : showSetBudgetSuccessMessage(category, budget)
   DESKRIPSI : Menampilkan pesan sukses setelah budget diatur
   I.S.       : Budget sudah diubah
   F.S.       : Pesan sukses ditampilkan
---------------------------------------------------------*/
void showSetBudgetSuccessMessage(const char *category, long long budget);


/* ---------------------------------------------------------
   FUNGSI : hasMonthReports(monthReportList)
   DESKRIPSI : Mengecek apakah ada laporan bulanan yang tersedia
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : true jika ada laporan, false jika tidak
---------------------------------------------------------*/
bool hasMonthReports(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   FUNGSI : hasMonthReportsView(monthReportList)
   DESKRIPSI : Mengecek apakah ada laporan untuk ditampilkan pada menu view
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : true jika ada laporan, false jika tidak
---------------------------------------------------------*/
bool hasMonthReportsView(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printCurrentBudgetHeader()
   DESKRIPSI : Menampilkan header untuk menu view budget saat ini
   I.S.       : Terminal bebas
   F.S.       : Header budget ditampilkan
---------------------------------------------------------*/
void printCurrentBudgetHeader();

/* ---------------------------------------------------------
   FUNGSI : selectReportForBudgetView(monthReportList)
   DESKRIPSI : Memilih laporan bulanan untuk ditampilkan pada menu view budget
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : pointer ke laporan terpilih, NULL jika batal
---------------------------------------------------------*/
struct MonthReport *selectReportForBudgetView(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetPerCategoryHeader()
   DESKRIPSI : Menampilkan header tabel budget per kategori
   I.S.       : Terminal bebas
   F.S.       : Header tabel ditampilkan
---------------------------------------------------------*/
void printBudgetPerCategoryHeader();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableRow(group)
   DESKRIPSI : Menampilkan satu baris tabel budget dari TransactionGroup
   I.S.       : Terminal bebas
   F.S.       : Baris tabel ditampilkan
---------------------------------------------------------*/
void printBudgetTableRow(const struct TransactionGroup *group);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableFooter()
   DESKRIPSI : Menampilkan footer tabel budget
   I.S.       : Terminal bebas
   F.S.       : Footer tabel ditampilkan
---------------------------------------------------------*/
void printBudgetTableFooter();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetReport(report)
   DESKRIPSI : Menampilkan seluruh laporan budget bulan tertentu
   I.S.       : Terminal bebas
   F.S.       : Laporan budget ditampilkan
---------------------------------------------------------*/
void printBudgetReport(const struct MonthReport *report);

/* ---------------------------------------------------------
   PROSEDUR : openViewBudgetMenu(monthReportList)
   DESKRIPSI : Membuka menu untuk melihat budget per kategori
   I.S.       : Terminal bebas
   F.S.       : Menu budget ditampilkan dan bisa berinteraksi
---------------------------------------------------------*/
void openViewBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   FUNGSI : ensureHasMonthReports(monthReportList)
   DESKRIPSI : Memastikan ada laporan bulanan sebelum melanjutkan
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : true jika laporan tersedia, false jika tidak
---------------------------------------------------------*/
bool ensureHasMonthReports(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printUserIcon(boxWidth, isAdmin)
   DESKRIPSI : Menampilkan ikon user atau admin pada menu
   I.S.       : Terminal bebas
   F.S.       : Ikon user/admin ditampilkan
---------------------------------------------------------*/
void printUserIcon(int boxWidth, bool isAdmin);

/* ---------------------------------------------------------
   PROSEDUR : printUserLabel(isAdmin)
   DESKRIPSI : Menampilkan label user atau admin
   I.S.       : Terminal bebas
   F.S.       : Label ditampilkan
---------------------------------------------------------*/
void printUserLabel(bool isAdmin);

/* ---------------------------------------------------------
   PROSEDUR : printUsernameAligned(username, isAdmin)
   DESKRIPSI : Menampilkan username dengan alignment sesuai role
   I.S.       : Terminal bebas
   F.S.       : Username ditampilkan
---------------------------------------------------------*/
void printUsernameAligned(const char *username, bool isAdmin);

/* ---------------------------------------------------------
   PROSEDUR : printAdminNoFolderMessage(boxWidth)
   DESKRIPSI : Menampilkan pesan admin jika tidak ada folder laporan
   I.S.       : Terminal bebas
   F.S.       : Pesan ditampilkan
---------------------------------------------------------*/
void printAdminNoFolderMessage(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printMenuHeaderLine(boxWidth)
   DESKRIPSI : Menampilkan garis header menu
   I.S.       : Terminal bebas
   F.S.       : Garis header ditampilkan
---------------------------------------------------------*/
void printMenuHeaderLine(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printAdminFiturMenu(boxWidth)
   DESKRIPSI : Menampilkan menu fitur admin
   I.S.       : Terminal bebas
   F.S.       : Menu admin ditampilkan
---------------------------------------------------------*/
void printAdminFiturMenu(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printUserFiturMenuRows(boxWidth)
   DESKRIPSI : Menampilkan baris menu fitur user
   I.S.       : Terminal bebas
   F.S.       : Menu user ditampilkan
---------------------------------------------------------*/
void printUserFiturMenuRows(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printAdminAuthMenu(boxWidth)
   DESKRIPSI : Menampilkan menu autentikasi admin
   I.S.       : Terminal bebas
   F.S.       : Menu autentikasi admin ditampilkan
---------------------------------------------------------*/
void printAdminAuthMenu(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printUserAuthMenu(boxWidth)
   DESKRIPSI : Menampilkan menu autentikasi user
   I.S.       : Terminal bebas
   F.S.       : Menu autentikasi user ditampilkan
---------------------------------------------------------*/
void printUserAuthMenu(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printMessagePrefix(prefix, colorFunc)
   DESKRIPSI : Menampilkan prefix pesan dengan warna tertentu
   I.S.       : Terminal bebas
   F.S.       : Prefix pesan ditampilkan
---------------------------------------------------------*/
void printMessagePrefix(const char *prefix, void (*colorFunc)(const char *));

/* ---------------------------------------------------------
   PROSEDUR : printBudgetHeaderLayout()
   DESKRIPSI : Menampilkan layout header tabel budget
   I.S.       : Terminal bebas
   F.S.       : Layout header ditampilkan
---------------------------------------------------------*/
void printBudgetHeaderLayout();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetMenuOptions()
   DESKRIPSI : Menampilkan opsi menu budget
   I.S.       : Terminal bebas
   F.S.       : Opsi menu ditampilkan
---------------------------------------------------------*/
void printBudgetMenuOptions();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetFooter()
   DESKRIPSI : Menampilkan footer menu budget
   I.S.       : Terminal bebas
   F.S.       : Footer menu ditampilkan
---------------------------------------------------------*/
void printBudgetFooter();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetPrompt()
   DESKRIPSI : Menampilkan prompt input menu budget
   I.S.       : Terminal bebas
   F.S.       : Prompt ditampilkan
---------------------------------------------------------*/
void printBudgetPrompt();


/* ---------------------------------------------------------
   PROSEDUR : printSelectMonthReportHeader()
   DESKRIPSI : Menampilkan header untuk menu pemilihan laporan bulan
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printSelectMonthReportHeader();

/* ---------------------------------------------------------
   FUNGSI : readMonthReportChoice(max, outIndex)
   DESKRIPSI : Membaca input pilihan laporan bulan
   INPUT  : max = jumlah maksimum laporan
   OUTPUT : outIndex = indeks laporan terpilih
            true jika valid, false jika input batal/invalid
---------------------------------------------------------*/
bool readMonthReportChoice(int max, int *outIndex);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryList(categories)
   DESKRIPSI : Menampilkan daftar kategori pada terminal
   I.S.       : Terminal bebas
   F.S.       : Daftar kategori ditampilkan
---------------------------------------------------------*/
void printCategoryList(struct CategoryList *categories);

/* ---------------------------------------------------------
   FUNGSI : readCategorySelection(max, choice)
   DESKRIPSI : Membaca input pemilihan kategori
   INPUT  : max = jumlah maksimum kategori
   OUTPUT : choice = indeks kategori yang dipilih
            true jika valid, false jika batal/invalid
---------------------------------------------------------*/
bool readCategorySelection(int max, int *choice);

/* ---------------------------------------------------------
   FUNGSI : hasCategories(categories)
   DESKRIPSI : Mengecek apakah ada kategori yang tersedia
   INPUT  : categories = daftar kategori
   OUTPUT : true jika ada, false jika kosong
---------------------------------------------------------*/
bool hasCategories(struct CategoryList *categories);

/* ---------------------------------------------------------
   FUNGSI : ensureCategoriesAvailable(categories)
   DESKRIPSI : Memastikan ada kategori sebelum melanjutkan
   INPUT  : categories = daftar kategori
   OUTPUT : true jika ada kategori, false jika tidak
---------------------------------------------------------*/
bool ensureCategoriesAvailable(struct CategoryList *categories);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderTop(boxWidth)
   DESKRIPSI : Menampilkan garis atas header kategori
   I.S.       : Terminal bebas
   F.S.       : Header atas ditampilkan
---------------------------------------------------------*/
void printCategoryHeaderTop(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderTitle(boxWidth)
   DESKRIPSI : Menampilkan judul header kategori
   I.S.       : Terminal bebas
   F.S.       : Judul header ditampilkan
---------------------------------------------------------*/
void printCategoryHeaderTitle(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeaderBottom(boxWidth)
   DESKRIPSI : Menampilkan garis bawah header kategori
   I.S.       : Terminal bebas
   F.S.       : Header bawah ditampilkan
---------------------------------------------------------*/
void printCategoryHeaderBottom(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryHeader()
   DESKRIPSI : Menampilkan header kategori lengkap
   I.S.       : Terminal bebas
   F.S.       : Header kategori ditampilkan
---------------------------------------------------------*/
void printCategoryHeader();

/* ---------------------------------------------------------
   PROSEDUR : printSingleCategoryItem(item, index, boxWidth)
   DESKRIPSI : Menampilkan satu item kategori
   I.S.       : Terminal bebas
   F.S.       : Item kategori ditampilkan
---------------------------------------------------------*/
void printSingleCategoryItem(struct CategoryItem *item, int index, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : copyCategoryInternalName(outCategory, internalName, bufferSize)
   DESKRIPSI : Menyalin internalName ke buffer output
   I.S.       : internalName valid, outCategory bebas
   F.S.       : outCategory berisi salinan internalName
---------------------------------------------------------*/
void copyCategoryInternalName(char *outCategory,
                              const char *internalName,
                              size_t bufferSize);

/* ---------------------------------------------------------
   FUNGSI : getBudgetCategoryDisplayName(category)
   DESKRIPSI : Mengambil nama display dari kategori budget
   INPUT  : category = internal name kategori
   OUTPUT : pointer ke nama display
---------------------------------------------------------*/
const char* getBudgetCategoryDisplayName(const char *category);

/* ---------------------------------------------------------
   PROSEDUR : formatBudgetSuccessMessage(buffer, size, displayName, budget)
   DESKRIPSI : Membuat pesan sukses pengaturan budget
   I.S.       : buffer tersedia
   F.S.       : buffer berisi pesan format budget sukses
---------------------------------------------------------*/
void formatBudgetSuccessMessage(char *buffer, size_t size,const char *displayName, long long budget);

/* ---------------------------------------------------------
   PROSEDUR : applyBudgetToReport(report, category, budget)
   DESKRIPSI : Menerapkan budget pada kategori di laporan tertentu
   I.S.       : report valid
   F.S.       : Budget di laporan diperbarui
---------------------------------------------------------*/
void applyBudgetToReport(struct MonthReport *report,
                         const char *category,
                         long long budget);

/* ---------------------------------------------------------
   PROSEDUR : saveUpdatedReport(report)
   DESKRIPSI : Menyimpan laporan bulanan setelah update budget
   I.S.       : report valid
   F.S.       : Data laporan tersimpan
---------------------------------------------------------*/
void saveUpdatedReport(struct MonthReport *report);

/* ---------------------------------------------------------
   FUNGSI : hasMonthReportsOnly(list)
   DESKRIPSI : Mengecek apakah daftar laporan bulanan ada
   INPUT  : list = daftar laporan bulanan
   OUTPUT : true jika ada, false jika tidak
---------------------------------------------------------*/
bool hasMonthReportsOnly(struct MonthReportList *list);

/* ---------------------------------------------------------
   PROSEDUR : showNoMonthReportsMessage()
   DESKRIPSI : Menampilkan pesan jika tidak ada laporan bulan
   I.S.       : Terminal bebas
   F.S.       : Pesan ditampilkan
---------------------------------------------------------*/
void showNoMonthReportsMessage();

/* ---------------------------------------------------------
   FUNGSI : readReportChoice(max, choice)
   DESKRIPSI : Membaca pilihan laporan dari user
   INPUT  : max = jumlah laporan maksimum
   OUTPUT : choice = indeks laporan terpilih
            true jika valid, false jika batal/invalid
---------------------------------------------------------*/
bool readReportChoice(int max, int *choice);

/* ---------------------------------------------------------
   FUNGSI : getReportByIndex(list, index)
   DESKRIPSI : Mengambil laporan bulan berdasarkan indeks
   INPUT  : list = daftar laporan, index = posisi laporan
   OUTPUT : pointer ke laporan terpilih
---------------------------------------------------------*/
struct MonthReport* getReportByIndex(struct MonthReportList *list, int index);

/* ---------------------------------------------------------
   FUNGSI : calculateRemainingBudget(group)
   DESKRIPSI : Menghitung sisa budget untuk satu kelompok transaksi
   INPUT  : group = TransactionGroup kategori tertentu
   OUTPUT : sisa budget dalam long long
---------------------------------------------------------*/
long long calculateRemainingBudget(const struct TransactionGroup *group);

/* ---------------------------------------------------------
   FUNGSI : calculateBudgetPercentage(group)
   DESKRIPSI : Menghitung persentase penggunaan budget untuk satu group
   INPUT  : group = TransactionGroup kategori tertentu
   OUTPUT : persentase penggunaan budget (0.0 - 100.0)
---------------------------------------------------------*/
double calculateBudgetPercentage(const struct TransactionGroup *group);


/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnCategory(displayName)
   DESKRIPSI : Menampilkan kolom nama kategori pada tabel budget
   I.S.       : Terminal bebas
   F.S.       : Nama kategori ditampilkan
---------------------------------------------------------*/
void printBudgetColumnCategory(const char *displayName);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnTotal(total)
   DESKRIPSI : Menampilkan kolom total anggaran pada tabel budget
   I.S.       : Terminal bebas
   F.S.       : Total ditampilkan
---------------------------------------------------------*/
void printBudgetColumnTotal(long long total);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnRemaining(remaining)
   DESKRIPSI : Menampilkan kolom sisa anggaran pada tabel budget
   I.S.       : Terminal bebas
   F.S.       : Sisa anggaran ditampilkan
---------------------------------------------------------*/
void printBudgetColumnRemaining(long long remaining);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetColumnStatus(status, percentage)
   DESKRIPSI : Menampilkan kolom status dan persentase penggunaan budget
   I.S.       : Terminal bebas
   F.S.       : Status dan persentase ditampilkan
---------------------------------------------------------*/
void printBudgetColumnStatus(const char *status, double percentage);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableRow(group)
   DESKRIPSI : Menampilkan satu baris tabel budget dari TransactionGroup
   I.S.       : group valid
   F.S.       : Baris tabel ditampilkan
---------------------------------------------------------*/
void printBudgetTableRow(const struct TransactionGroup *group);

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableFooter()
   DESKRIPSI : Menampilkan footer tabel budget
   I.S.       : Terminal bebas
   F.S.       : Footer tabel ditampilkan
---------------------------------------------------------*/
void printBudgetTableFooter();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetTableHeader()
   DESKRIPSI : Menampilkan header tabel budget
   I.S.       : Terminal bebas
   F.S.       : Header tabel ditampilkan
---------------------------------------------------------*/
void printBudgetTableHeader();

/* ---------------------------------------------------------
   PROSEDUR : printBudgetReport(report)
   DESKRIPSI : Menampilkan laporan budget untuk satu laporan bulanan
   I.S.       : report valid
   F.S.       : Laporan budget ditampilkan
---------------------------------------------------------*/
void printBudgetReport(const struct MonthReport *report);

/* ---------------------------------------------------------
   FUNGSI : hasMonthReportsView(monthReportList)
   DESKRIPSI : Mengecek apakah terdapat laporan bulan untuk dilihat
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : true jika ada, false jika tidak
---------------------------------------------------------*/
bool hasMonthReportsView(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printCurrentBudgetHeader()
   DESKRIPSI : Menampilkan header menu budget saat ini
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printCurrentBudgetHeader();

/* ---------------------------------------------------------
   FUNGSI : selectReportForBudgetView(monthReportList)
   DESKRIPSI : Memilih laporan bulanan untuk ditampilkan
   INPUT  : monthReportList = daftar laporan bulanan
   OUTPUT : pointer ke laporan bulan terpilih, NULL jika batal
---------------------------------------------------------*/
struct MonthReport *selectReportForBudgetView(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : openViewBudgetMenu(monthReportList)
   DESKRIPSI : Membuka menu untuk melihat laporan budget
   I.S.       : Terminal bebas
   F.S.       : Menu budget ditampilkan dan interaksi selesai
---------------------------------------------------------*/
void openViewBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryListHeader()
   DESKRIPSI : Menampilkan header daftar kategori
   I.S.       : Terminal bebas
   F.S.       : Header kategori ditampilkan
---------------------------------------------------------*/
void printCategoryListHeader();

/* ---------------------------------------------------------
   PROSEDUR : printIncomeHeader(boxWidth)
   DESKRIPSI : Menampilkan header kategori pendapatan
   I.S.       : Terminal bebas
   F.S.       : Header pendapatan ditampilkan
---------------------------------------------------------*/
void printIncomeHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printExpenseHeader(boxWidth)
   DESKRIPSI : Menampilkan header kategori pengeluaran
   I.S.       : Terminal bebas
   F.S.       : Header pengeluaran ditampilkan
---------------------------------------------------------*/
void printExpenseHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryRow(cat, index, boxWidth)
   DESKRIPSI : Menampilkan satu baris kategori
   I.S.       : cat valid
   F.S.       : Baris kategori ditampilkan
---------------------------------------------------------*/
void printCategoryRow(struct CategoryItem *cat, int index, int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : printIncomeCategories(categories, boxWidth)
   DESKRIPSI : Menampilkan semua kategori pendapatan
   INPUT  : categories = daftar kategori, boxWidth = lebar tampilan
   OUTPUT : jumlah kategori pendapatan yang ditampilkan
---------------------------------------------------------*/
int printIncomeCategories(struct CategoryList *categories, int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : printExpenseCategories(categories, boxWidth)
   DESKRIPSI : Menampilkan semua kategori pengeluaran
   INPUT  : categories = daftar kategori, boxWidth = lebar tampilan
   OUTPUT : jumlah kategori pengeluaran yang ditampilkan
---------------------------------------------------------*/
int printExpenseCategories(struct CategoryList *categories, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategorySummary(total, income, expense, boxWidth)
   DESKRIPSI : Menampilkan ringkasan jumlah kategori
   I.S.       : Nilai total, income, expense valid
   F.S.       : Ringkasan ditampilkan
---------------------------------------------------------*/
void printCategorySummary(int total, int income, int expense, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : showAllCategories()
   DESKRIPSI : Menampilkan semua kategori (income dan expense)
   I.S.       : Terminal bebas
   F.S.       : Semua kategori ditampilkan
---------------------------------------------------------*/
void showAllCategories();





/* ---------------------------------------------------------
   PROSEDUR : printAddCategoryHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu untuk menambahkan kategori baru
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printAddCategoryHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryTypeOptions(boxWidth)
   DESKRIPSI : Menampilkan opsi tipe kategori (Pendapatan / Pengeluaran)
   I.S.       : Terminal bebas
   F.S.       : Opsi tipe kategori ditampilkan
---------------------------------------------------------*/
void printCategoryTypeOptions(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readCategoryType(type, boxWidth)
   DESKRIPSI : Membaca pilihan tipe kategori dari pengguna
   INPUT  : boxWidth = lebar tampilan, type = pointer ke enum TransactionType
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readCategoryType(enum TransactionType *type, int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readDisplayCategoryName(buffer, size, boxWidth)
   DESKRIPSI : Membaca nama kategori yang akan ditampilkan
   INPUT  : buffer = tempat menyimpan nama, size = ukuran buffer, boxWidth = lebar tampilan
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readDisplayCategoryName(char *buffer, size_t size, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : generateInternalCategoryName(internalName, displayName, size)
   DESKRIPSI : Menghasilkan nama internal dari nama display
   I.S.       : displayName valid
   F.S.       : internalName terisi dengan nama internal yang sudah dinormalisasi
---------------------------------------------------------*/
void generateInternalCategoryName(char *internalName,
                                  const char *displayName,
                                  size_t size);

/* ---------------------------------------------------------
   FUNGSI : isDuplicateCategory(list, internalName)
   DESKRIPSI : Mengecek apakah kategori dengan nama internal sudah ada
   INPUT  : list = daftar kategori, internalName = nama internal yang dicek
   OUTPUT : true jika duplikat, false jika tidak
---------------------------------------------------------*/
bool isDuplicateCategory(struct CategoryList *list,
                         const char *internalName);

/* ---------------------------------------------------------
   FUNGSI : addNewCategory(categories, internalName, displayName, type)
   DESKRIPSI : Menambahkan kategori baru ke daftar kategori pengguna
   INPUT  : categories = daftar kategori, internalName = nama internal,
            displayName = nama display, type = tipe kategori
   OUTPUT : true jika berhasil, false jika gagal
---------------------------------------------------------*/
bool addNewCategory(struct CategoryList *categories,
                    const char *internalName,
                    const char *displayName,
                    enum TransactionType type);

/* ---------------------------------------------------------
   PROSEDUR : showAddCategorySuccess(displayName)
   DESKRIPSI : Menampilkan pesan sukses setelah menambahkan kategori
   I.S.       : displayName valid
   F.S.       : Pesan sukses ditampilkan
---------------------------------------------------------*/
void showAddCategorySuccess(const char *displayName);

/* ---------------------------------------------------------
   PROSEDUR : openAddCategoryMenu()
   DESKRIPSI : Membuka menu untuk menambahkan kategori baru
   I.S.       : Terminal bebas
   F.S.       : Kategori baru ditambahkan jika input valid
---------------------------------------------------------*/
void openAddCategoryMenu();

/* ---------------------------------------------------------
   PROSEDUR : printDeleteCategoryHeader(boxWidth)
   DESKRIPSI : Menampilkan header menu hapus kategori
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printDeleteCategoryHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCustomCategoryListHeader(boxWidth)
   DESKRIPSI : Menampilkan header daftar kategori custom
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printCustomCategoryListHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCustomCategoryItem(cat, index, boxWidth)
   DESKRIPSI : Menampilkan satu item kategori custom
   I.S.       : cat valid
   F.S.       : Item kategori ditampilkan
---------------------------------------------------------*/
void printCustomCategoryItem(struct CategoryItem *cat, int index, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printDefaultCategoryNote(boxWidth)
   DESKRIPSI : Menampilkan catatan bahwa kategori default tidak bisa dihapus
   I.S.       : Terminal bebas
   F.S.       : Catatan ditampilkan
---------------------------------------------------------*/
void printDefaultCategoryNote(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : collectCustomCategoryIndices(list, outIndices)
   DESKRIPSI : Mengumpulkan indeks kategori custom dari daftar
   INPUT  : list = daftar kategori, outIndices = array untuk menyimpan indeks
   OUTPUT : jumlah kategori custom
---------------------------------------------------------*/
int collectCustomCategoryIndices(struct CategoryList *list, int *outIndices);

/* ---------------------------------------------------------
   PROSEDUR : printAllCustomCategories(list, indices, count, boxWidth)
   DESKRIPSI : Menampilkan semua kategori custom berdasarkan indeks
   I.S.       : list valid, indices valid
   F.S.       : Semua kategori custom ditampilkan
---------------------------------------------------------*/
void printAllCustomCategories(struct CategoryList *list,
                              int *indices,
                              int count,
                              int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readDeleteCategoryChoice(max, choice, boxWidth)
   DESKRIPSI : Membaca pilihan kategori untuk dihapus
   INPUT  : max = jumlah kategori, boxWidth = lebar tampilan, choice = pointer untuk hasil
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readDeleteCategoryChoice(int max, int *choice, int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readDeleteConfirmation(categoryName, boxWidth)
   DESKRIPSI : Membaca konfirmasi penghapusan kategori
   INPUT  : categoryName = nama kategori, boxWidth = lebar tampilan
   OUTPUT : true jika user yakin, false jika batal
---------------------------------------------------------*/
bool readDeleteConfirmation(const char *categoryName, int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : deleteCategoryByInternalName(internalName)
   DESKRIPSI : Menghapus kategori berdasarkan nama internal
   INPUT  : internalName = nama internal kategori
   OUTPUT : true jika berhasil dihapus, false jika gagal
---------------------------------------------------------*/
bool deleteCategoryByInternalName(const char *internalName);

/* ---------------------------------------------------------
   PROSEDUR : openDeleteCategoryMenu()
   DESKRIPSI : Membuka menu untuk menghapus kategori custom
   I.S.       : Terminal bebas
   F.S.       : Kategori dihapus jika konfirmasi diberikan
---------------------------------------------------------*/
void openDeleteCategoryMenu();

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu manajemen kategori
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printCategoryManagementHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementMenuItem(index, emoji, label, boxWidth)
   DESKRIPSI : Menampilkan satu item menu manajemen kategori
   I.S.       : index, emoji, label valid
   F.S.       : Item menu ditampilkan
---------------------------------------------------------*/
void printCategoryManagementMenuItem(int index,
                                     const char *emoji,
                                     const char *label,
                                     int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printCategoryManagementFooter(boxWidth)
   DESKRIPSI : Menampilkan footer menu manajemen kategori
   I.S.       : Terminal bebas
   F.S.       : Footer ditampilkan
---------------------------------------------------------*/
void printCategoryManagementFooter(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readCategoryManagementChoice()
   DESKRIPSI : Membaca pilihan user di menu manajemen kategori
   INPUT  : -
   OUTPUT : angka pilihan user
---------------------------------------------------------*/
int readCategoryManagementChoice();

/* ---------------------------------------------------------
   PROSEDUR : openCategoryManagementMenu()
   DESKRIPSI : Membuka menu manajemen kategori (lihat, tambah, hapus)
   I.S.       : Terminal bebas
   F.S.       : Menu dijalankan hingga user kembali
---------------------------------------------------------*/
void openCategoryManagementMenu();



/* ---------------------------------------------------------
   PROSEDUR : printResetBudgetHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu reset semua budget
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printResetBudgetHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printResetBudgetWarnings(boxWidth)
   DESKRIPSI : Menampilkan peringatan bahwa reset budget akan dilakukan
   I.S.       : Terminal bebas
   F.S.       : Peringatan ditampilkan
---------------------------------------------------------*/
void printResetBudgetWarnings(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readResetBudgetConfirmation(boxWidth)
   DESKRIPSI : Membaca konfirmasi user untuk reset budget
   INPUT  : boxWidth = lebar tampilan
   OUTPUT : true jika user setuju, false jika batal
---------------------------------------------------------*/
bool readResetBudgetConfirmation(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : resetBudgetForReport(report, categories)
   DESKRIPSI : Mengatur ulang budget semua kategori dalam satu laporan ke default
   I.S.       : report dan categories valid
   F.S.       : Semua budget kategori di report direset ke nilai default
---------------------------------------------------------*/
void resetBudgetForReport(struct MonthReport *report,
                          struct CategoryList *categories);

/* ---------------------------------------------------------
   PROSEDUR : resetBudgetForAllReports(monthReportList, categories)
   DESKRIPSI : Mengatur ulang budget semua laporan di monthReportList ke default
   I.S.       : monthReportList dan categories valid
   F.S.       : Semua budget semua laporan direset ke nilai default
---------------------------------------------------------*/
void resetBudgetForAllReports(struct MonthReportList *monthReportList,
                              struct CategoryList *categories);

/* ---------------------------------------------------------
   PROSEDUR : openResetBudgetMenu(monthReportList)
   DESKRIPSI : Membuka menu reset semua budget dan mengeksekusi sesuai konfirmasi user
   I.S.       : monthReportList valid
   F.S.       : Semua budget direset jika user konfirmasi
---------------------------------------------------------*/
void openResetBudgetMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu konfigurasi sistem
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printConfigurationHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationMenuItem(index, emoji, label, boxWidth)
   DESKRIPSI : Menampilkan satu item menu konfigurasi
   I.S.       : index, emoji, label valid
   F.S.       : Item menu ditampilkan
---------------------------------------------------------*/
void printConfigurationMenuItem(int index, const char *emoji,
                                const char *label, int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationFooter(boxWidth)
   DESKRIPSI : Menampilkan footer menu konfigurasi
   I.S.       : Terminal bebas
   F.S.       : Footer ditampilkan
---------------------------------------------------------*/
void printConfigurationFooter(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readConfigurationChoice()
   DESKRIPSI : Membaca pilihan user pada menu konfigurasi
   INPUT  : -
   OUTPUT : angka pilihan user
---------------------------------------------------------*/
int readConfigurationChoice();

/* ---------------------------------------------------------
   PROSEDUR : openConfigurationMenu(monthReportList)
   DESKRIPSI : Membuka menu konfigurasi dan mengeksekusi pilihan user
   I.S.       : monthReportList valid
   F.S.       : Pilihan menu konfigurasi dieksekusi sesuai user
---------------------------------------------------------*/
void openConfigurationMenu(struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printMinimumAmountHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu untuk mengatur minimum jumlah uang
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printMinimumAmountHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printMinimumBudgetHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu untuk mengatur minimum budget
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printMinimumBudgetHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumAmount(boxWidth)
   DESKRIPSI : Menampilkan nilai minimum jumlah uang saat ini
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum ditampilkan
---------------------------------------------------------*/
void showCurrentMinimumAmount(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumBudget(boxWidth)
   DESKRIPSI : Menampilkan nilai minimum budget saat ini
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum ditampilkan
---------------------------------------------------------*/
void showCurrentMinimumBudget(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumAmount(outValue)
   DESKRIPSI : Membaca nilai minimum jumlah uang baru dari user
   INPUT  : outValue = pointer untuk menyimpan hasil input
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readNewMinimumAmount(long long *outValue);

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumBudget(outValue)
   DESKRIPSI : Membaca nilai minimum budget baru dari user
   INPUT  : outValue = pointer untuk menyimpan hasil input
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readNewMinimumBudget(long long *outValue);

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumAmount(value)
   DESKRIPSI : Mengatur nilai minimum jumlah uang baru dan menyimpan konfigurasi
   I.S.       : value valid
   F.S.       : Nilai minimum diperbarui, konfigurasi tersimpan, pesan sukses ditampilkan
---------------------------------------------------------*/
void applyNewMinimumAmount(long long value);

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumBudget(value)
   DESKRIPSI : Mengatur nilai minimum budget baru dan menyimpan konfigurasi
   I.S.       : value valid
   F.S.       : Nilai minimum diperbarui, konfigurasi tersimpan, pesan sukses ditampilkan
---------------------------------------------------------*/
void applyNewMinimumBudget(long long value);

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumAmountMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum jumlah uang
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum diperbarui jika input valid
---------------------------------------------------------*/
void openSetMinimumAmountMenu();

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumBudgetMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum budget
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum diperbarui jika input valid
---------------------------------------------------------*/
void openSetMinimumBudgetMenu();



/* ---------------------------------------------------------
   PROSEDUR : printMinimumTransactionHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu untuk mengatur minimum transaksi
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printMinimumTransactionHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : showCurrentMinimumTransaction(boxWidth)
   DESKRIPSI : Menampilkan nilai minimum transaksi saat ini
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum transaksi ditampilkan
---------------------------------------------------------*/
void showCurrentMinimumTransaction(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readNewMinimumTransaction(outValue)
   DESKRIPSI : Membaca nilai minimum transaksi baru dari user
   INPUT  : outValue = pointer untuk menyimpan hasil input
   OUTPUT : true jika input valid, false jika tidak
---------------------------------------------------------*/
bool readNewMinimumTransaction(long long *outValue);

/* ---------------------------------------------------------
   PROSEDUR : applyNewMinimumTransaction(value)
   DESKRIPSI : Mengatur nilai minimum transaksi baru dan menyimpan konfigurasi
   I.S.       : value valid
   F.S.       : Nilai minimum transaksi diperbarui, konfigurasi tersimpan, pesan sukses ditampilkan
---------------------------------------------------------*/
void applyNewMinimumTransaction(long long value);

/* ---------------------------------------------------------
   PROSEDUR : openSetMinimumTransactionMenu()
   DESKRIPSI : Membuka menu untuk mengatur minimum transaksi
   I.S.       : Terminal bebas
   F.S.       : Nilai minimum transaksi diperbarui jika input valid
---------------------------------------------------------*/
void openSetMinimumTransactionMenu();

/* ---------------------------------------------------------
   PROSEDUR : printConfigurationHeaderKeuangan(boxWidth)
   DESKRIPSI : Menampilkan header menu pengaturan konfigurasi keuangan saat ini
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printConfigurationHeaderKeuangan(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printFinanceConfigHeader(boxWidth)
   DESKRIPSI : Menampilkan bagian header tabel pengaturan keuangan
   I.S.       : Terminal bebas
   F.S.       : Header tabel ditampilkan
---------------------------------------------------------*/
void printFinanceConfigHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printFinanceConfigRows(boxWidth)
   DESKRIPSI : Menampilkan baris pengaturan nilai minimum dan maksimum keuangan
   I.S.       : Terminal bebas
   F.S.       : Baris pengaturan ditampilkan
---------------------------------------------------------*/
void printFinanceConfigRows(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printValidationConfigHeader(boxWidth)
   DESKRIPSI : Menampilkan header tabel pengaturan validasi input
   I.S.       : Terminal bebas
   F.S.       : Header validasi ditampilkan
---------------------------------------------------------*/
void printValidationConfigHeader(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : printValidationConfigRows(boxWidth)
   DESKRIPSI : Menampilkan baris pengaturan validasi input
   I.S.       : Terminal bebas
   F.S.       : Baris validasi ditampilkan
---------------------------------------------------------*/
void printValidationConfigRows(int boxWidth);

/* ---------------------------------------------------------
   PROSEDUR : openViewConfigurationMenu()
   DESKRIPSI : Menampilkan menu pengaturan saat ini (keuangan dan validasi)
   I.S.       : Terminal bebas
   F.S.       : Seluruh pengaturan ditampilkan
---------------------------------------------------------*/
void openViewConfigurationMenu();

/* ---------------------------------------------------------
   PROSEDUR : printResetConfigHeader(boxWidth, contentLines)
   DESKRIPSI : Menampilkan header menu reset konfigurasi default
   I.S.       : Terminal bebas
   F.S.       : Header ditampilkan
---------------------------------------------------------*/
void printResetConfigHeader(int boxWidth, int contentLines);

/* ---------------------------------------------------------
   PROSEDUR : printResetConfigWarning(boxWidth)
   DESKRIPSI : Menampilkan peringatan reset konfigurasi default
   I.S.       : Terminal bebas
   F.S.       : Peringatan ditampilkan
---------------------------------------------------------*/
void printResetConfigWarning(int boxWidth);

/* ---------------------------------------------------------
   FUNGSI : readResetConfigConfirmation(boxWidth, outConfirm)
   DESKRIPSI : Membaca konfirmasi user untuk reset konfigurasi
   INPUT  : boxWidth = lebar tampilan
            outConfirm = pointer untuk menyimpan input 'y' atau 'n'
   OUTPUT : true jika user konfirmasi 'y', false jika 'n'
---------------------------------------------------------*/
bool readResetConfigConfirmation(int boxWidth, char *outConfirm);

/* ---------------------------------------------------------
   PROSEDUR : applyResetConfiguration()
   DESKRIPSI : Mengembalikan semua pengaturan ke default dan menyimpan konfigurasi
   I.S.       : Terminal bebas
   F.S.       : Konfigurasi di-reset ke default, pesan sukses ditampilkan
---------------------------------------------------------*/
void applyResetConfiguration();

/* ---------------------------------------------------------
   PROSEDUR : printResetCancelledMessage()
   DESKRIPSI : Menampilkan pesan jika reset konfigurasi dibatalkan
   I.S.       : Terminal bebas
   F.S.       : Pesan dibatalkan ditampilkan
---------------------------------------------------------*/
void printResetCancelledMessage();

/* ---------------------------------------------------------
   PROSEDUR : openResetConfigurationMenu()
   DESKRIPSI : Membuka menu reset konfigurasi default dan mengeksekusi sesuai konfirmasi user
   I.S.       : Terminal bebas
   F.S.       : Konfigurasi di-reset jika user konfirmasi
---------------------------------------------------------*/
void openResetConfigurationMenu();

/* ---------------------------------------------------------
   FUNGSI : isAdmin()
   DESKRIPSI : Mengecek apakah user saat ini adalah admin
   INPUT  : -
   OUTPUT : true jika user admin, false jika user biasa
---------------------------------------------------------*/
bool isAdmin();

/* ---------------------------------------------------------
   FUNGSI : getMainMenuMaxChoice()
   DESKRIPSI : Mengembalikan jumlah maksimum pilihan di menu utama berdasarkan role user
   INPUT  : -
   OUTPUT : Maksimum angka pilihan menu utama
---------------------------------------------------------*/
int getMainMenuMaxChoice();


/* ---------------------------------------------------------
   PROSEDUR : handleMenuChoiceUser(choice, monthReportList)
   DESKRIPSI : Menangani aksi menu utama untuk user biasa sesuai pilihan
   I.S.       : User sudah login, choice valid
   F.S.       : Menu terkait dipanggil, atau keluar dari aplikasi jika user pilih exit
   INPUT      : choice = pilihan menu user
                monthReportList = daftar laporan bulanan user
---------------------------------------------------------*/
void handleMenuChoiceUser(int choice, struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printUserLogoutScreen()
   DESKRIPSI : Menampilkan layar logout untuk user biasa
   I.S.       : User sedang login
   F.S.       : Layar logout ditampilkan
---------------------------------------------------------*/
void printUserLogoutScreen();

/* ---------------------------------------------------------
   PROSEDUR : printUserExitScreen()
   DESKRIPSI : Menampilkan layar exit (ucapan terima kasih) untuk user biasa
   I.S.       : Terminal bebas
   F.S.       : Layar exit ditampilkan
---------------------------------------------------------*/
void printUserExitScreen();

/* ---------------------------------------------------------
   PROSEDUR : handleMenuChoiceAdmin(choice, monthReportList)
   DESKRIPSI : Menangani aksi menu utama untuk admin sesuai pilihan
   I.S.       : Admin sudah login, choice valid
   F.S.       : Menu terkait dipanggil, atau keluar dari aplikasi jika admin pilih exit
   INPUT      : choice = pilihan menu admin
                monthReportList = daftar laporan bulanan (meskipun admin tidak punya laporan pribadi)
---------------------------------------------------------*/
void handleMenuChoiceAdmin(int choice, struct MonthReportList *monthReportList);

/* ---------------------------------------------------------
   PROSEDUR : printAdminMenuScreen()
   DESKRIPSI : Menampilkan layar menu admin
   I.S.       : Terminal bebas
   F.S.       : Layar menu admin ditampilkan
---------------------------------------------------------*/
void printAdminMenuScreen();

/* ---------------------------------------------------------
   FUNGSI : readAdminMenuChoice(outChoice)
   DESKRIPSI : Membaca input pilihan menu admin
   INPUT      : outChoice = pointer untuk menyimpan hasil pilihan
   OUTPUT     : true jika input valid, false jika tidak valid
---------------------------------------------------------*/
bool readAdminMenuChoice(int *outChoice);

/* ---------------------------------------------------------
   PROSEDUR : handleAdminPanelSelection(adminChoice)
   DESKRIPSI : Menangani aksi di panel admin sesuai pilihan
   I.S.       : Admin sedang berada di menu panel admin
   F.S.       : Menu terkait dijalankan, atau logout jika dipilih
   INPUT      : adminChoice = pilihan menu panel admin
---------------------------------------------------------*/
void handleAdminPanelSelection(int adminChoice);

/* ---------------------------------------------------------
   PROSEDUR : printAdminLogoutScreen()
   DESKRIPSI : Menampilkan layar logout untuk admin
   I.S.       : Admin sedang login
   F.S.       : Layar logout admin ditampilkan
---------------------------------------------------------*/
void printAdminLogoutScreen();

/* ---------------------------------------------------------
   PROSEDUR : printAdminExitScreen()
   DESKRIPSI : Menampilkan layar exit (ucapan terima kasih) untuk admin
   I.S.       : Terminal bebas
   F.S.       : Layar exit admin ditampilkan
---------------------------------------------------------*/
void printAdminExitScreen();

/* ---------------------------------------------------------
   PROSEDUR : openMainMenu(monthReportList)
   DESKRIPSI : Menampilkan menu utama sesuai role user/admin dan mengeksekusi pilihan
   I.S.       : Terminal bebas, user/admin sudah login
   F.S.       : Menu utama ditampilkan, aksi terkait dipanggil sesuai pilihan
   INPUT      : monthReportList = daftar laporan bulanan user/admin
---------------------------------------------------------*/
void openMainMenu(struct MonthReportList *monthReportList);
