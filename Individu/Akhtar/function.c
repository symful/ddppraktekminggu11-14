int CekDuplikatPos(PosAnggaran daftar[], int jumlahPos, char namaBaru[]) {
    int i;
    for (i = 0; i < jumlahPos; i++) {
        if (strcmp(daftar[i].nama, namaBaru) == 0) {
            return 1;
        }
    }
    return 0;
}