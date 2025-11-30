# Test Suite untuk Sistem Kategori

## Deskripsi

Test suite ini menguji implementasi sistem kategori dinamis yang menggunakan string-based categories dengan fitur keamanan memori dan validasi.

## Struktur Test

### 1. test_category.c
Menguji fungsi-fungsi dasar sistem kategori:

- **Category List Management**
  - Pembuatan category list
  - Inisialisasi kategori default
  - Penambahan kategori custom
  - Penghapusan kategori (dengan proteksi default)
  
- **String Handling & Safety**
  - Normalisasi nama kategori (lowercase)
  - Validasi nama kategori
  - Proteksi buffer overflow
  - Safe string input

- **Category Operations**
  - Pencarian kategori berdasarkan internal name
  - Pengecekan keberadaan kategori
  - Filter kategori berdasarkan tipe (INCOME/EXPENSE)
  - Mendapatkan display name

- **Persistence**
  - Penyimpanan kategori ke file (atomic write)
  - Loading kategori dari file
  - Category cache management

### 2. test_transaction_group.c
Menguji integrasi transaction group dengan kategori string:

- **Transaction Group Creation**
  - Pembuatan transaction group dengan kategori string
  - Validasi buffer overflow pada kategori
  - Handling null category

- **Transaction Management**
  - Penambahan transaksi ke group
  - Penghapusan transaksi dari group
  - Update kalkulasi group

- **Budget Tracking**
  - Status budget (Safe/Caution/Warning/Critical/Over Budget)
  - Persentase penggunaan budget
  - Sisa budget
  - Validasi transaksi terhadap budget

## Cara Menjalankan Test

### Metode 1: Menggunakan Script
```bash
cd tests
bash run_tests.sh
```

### Metode 2: Manual Compilation
```bash
# Test category system
gcc -o tests/test_category tests/test_category.c types/transaction_category.c -I. -lm
./tests/test_category

# Test transaction group
gcc -o tests/test_transaction_group tests/test_transaction_group.c db/transaction_group.c -I. -lm
./tests/test_transaction_group
```

## Fitur Keamanan yang Ditest

### 1. Memory Safety
- ✓ Proper allocation dan deallocation
- ✓ No memory leaks (tested dengan valgrind)
- ✓ Proper cleanup pada error conditions
- ✓ Buffer overflow protection

### 2. String Safety
- ✓ Safe string copying (strncpy dengan null termination)
- ✓ Input validation
- ✓ Length checking sebelum operasi string
- ✓ Case-insensitive comparison

### 3. Data Integrity
- ✓ Atomic file writes (temp file + rename)
- ✓ File corruption recovery (fallback to defaults)
- ✓ Category persistence
- ✓ Default category protection (tidak bisa dihapus)

### 4. Type Safety
- ✓ Category type binding (INCOME vs EXPENSE)
- ✓ Type validation saat filtering
- ✓ Type consistency checking

## Expected Output

Semua test harus PASS dengan output seperti:

```
╔══════════════════════════════════════════════════════╗
║      ✓ ALL TESTS PASSED                             ║
╚══════════════════════════════════════════════════════╝
```

Jika ada test yang FAIL, akan muncul assertion error dengan line number yang spesifik.

## Cleanup

Test suite secara otomatis membersihkan test data:
- Menghapus folder ./users/testuser setelah test
- Tidak meninggalkan file temporary

## Kategori Default yang Ditest

### Income Categories (1):
- income (Pendapatan)

### Expense Categories (6):
- transportation (Transportasi)
- work (Pekerjaan)
- food (Makanan)
- entertainment (Hiburan)
- shopping (Belanja)
- other (Lainnya)

## Integration dengan Main System

Test ini memastikan bahwa:
1. Kategori disimpan per-user di `users/{username}/categories.txt`
2. Format file konsisten dan dapat di-parse dengan aman
3. Cache kategori di-refresh saat user logout
4. Kategori custom dapat ditambah dan dihapus dengan aman
5. Transaction group bekerja dengan kategori string
6. Tidak ada memory leak saat operasi kategori

## Debug Mode

Untuk debugging lebih detail, compile dengan flag debug:

```bash
gcc -g -o tests/test_category tests/test_category.c types/transaction_category.c -I. -lm
valgrind --leak-check=full ./tests/test_category
```

## Troubleshooting

### Test Gagal: "No such file or directory"
- Pastikan menjalankan dari root directory project
- Pastikan folder users/ ada atau dapat dibuat

### Test Gagal: Assertion error
- Baca line number dan test name dari output
- Periksa implementasi fungsi yang terkait
- Jalankan test individual untuk isolasi masalah

### Memory Leak Terdeteksi
- Gunakan valgrind untuk trace leak source
- Pastikan semua freeCategoryList() dipanggil
- Check proper cleanup di destroyUserSession()