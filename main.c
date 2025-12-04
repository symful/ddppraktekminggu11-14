#include "auth/user_reports.c"
#include "db/month_report_list.c"
#include "ui/include.c"
#include <dirent.h>

/* ---------------------------------------------------------
   PROSEDUR : handleUserAuthentication()
   DESKRIPSI : Menangani proses login/registrasi hingga user berhasil login.
   I.S.       : User belum login (currentUser == NULL).
   F.S.       : User berhasil login dan currentUser berisi data pengguna.
---------------------------------------------------------*/
void handleUserAuthentication() {
  while (currentUser == NULL) {
    clearScreen();
    showAuthMenu();

    int authResult = handleAuthMenuChoice();
    if (authResult == 0) {
      printf("\nPress Enter to continue...");
      getchar();
    }
  }
}

/* ---------------------------------------------------------
   PROSEDUR : loadUserConfiguration()
   DESKRIPSI : Memuat konfigurasi default, lalu menimpa dengan konfigurasi milik
user jika user bukan admin. Jika admin, memuat konfigurasi global. I.S.       :
currentUser sudah terisi user yang login. F.S.       : Konfigurasi aplikasi
terset sesuai pengguna (atau default).
---------------------------------------------------------*/
void loadUserConfiguration() {
  initializeDefaultConfig();

  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    loadConfigFromFile(userConfigPath);
  } else {
    loadConfigFromFile("./config.txt");
  }
}

/* ---------------------------------------------------------
   FUNGSI : loadUserReports()
   DESKRIPSI : Memuat daftar laporan bulanan milik user.
               Jika admin, memuat semua laporan dari seluruh user.
               Jika tidak ada laporan, membuat list kosong.
   INPUT     : -
   OUTPUT    : Pointer ke MonthReportList yang berisi laporan terkait.
---------------------------------------------------------*/
struct MonthReportList *loadUserReports() {
  struct MonthReportList *list;

  if (currentUser != NULL && !currentUser->isAdmin) {
    list = listUserMonthReports();
  } else {
    list = listAllUsersReports();
  }

  if (list == NULL) {
    list = createMonthReportList();
  }

  return list;
}

/* ---------------------------------------------------------
   PROSEDUR : saveUserConfiguration()
   DESKRIPSI : Menyimpan konfigurasi user ke file masing-masing.
               Admin menyimpan konfigurasi global.
   I.S.       : Konfigurasi telah dimodifikasi selama aplikasi berjalan.
   F.S.       : Konfigurasi tersimpan ke file.
---------------------------------------------------------*/
void saveUserConfiguration() {
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    saveConfigToFile(userConfigPath);
  } else {
    saveConfigToFile("./config.txt");
  }
}

void runApplication() {

  if (!initializeAuthSystem()) {
    printf("Gagal menginisialisasi sistem Autentikasi.\n");
    return;
  }
  handleUserAuthentication();
  loadUserConfiguration();
  struct MonthReportList *monthReportList = loadUserReports();
  openMainMenu(monthReportList);
  saveUserConfiguration();
  logoutUser();
}

/* =======================================================
    PROGRAM MAIN
   =======================================================*/
int main() {
  runApplication();
  return 0;
}
