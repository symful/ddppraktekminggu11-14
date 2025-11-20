#include <stdio.h>
#include <string.h>

void kesimpulanKondisiKeuangan(int persentaseKondisiKeuangan,
                               char kondisiKeuangan[8]) {
  if (persentaseKondisiKeuangan > 25 &&
      strcmp(kondisiKeuangan, "Surplus") == 0) {
    printf("Anda termasuk mahasiswa hemat dan produktif.");
  } else if (persentaseKondisiKeuangan > 0 &&
             strcmp(kondisiKeuangan, "Surplus") == 0) {
    printf("Keuangan Anda seimbang, tetap waspada terhadap pengeluaran tidak "
           "perlu.");
  } else if (persentaseKondisiKeuangan < 10 &&
             strcmp(kondisiKeuangan, "Defisit") == 0) {
    printf("Anda mulai boros, perhatikan pengeluaran harian.");
  } else if (strcmp(kondisiKeuangan, "Defisit") == 0) {
    printf("Kondisi keuangan kurang sehat. Kurangi pengeluaran dan cari "
           "tambahan pemasukan.");
  } else {
    printf("Keuangan Anda Seimbang");
  }
}
