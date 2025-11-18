#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENT 100
#define MAX_USER 100
#define MAX_JOIN 200
#define FILE_EVENT "event_data.txt"
#define FILE_USER "user_data.txt"
#define FILE_JOIN "join_data.txt"

// STRUCT DATA
typedef struct {
    int id;
    char nama[50];
    char tanggal[20];
    char lokasi[50];
    char deskripsi[100];
} Event;

typedef struct {
    int id;
    char nama[50];
    char role[10]; // "panitia" atau "peserta"
} User;

typedef struct {
    int userId;
    int eventId;
} Join;

Event daftarEvent[MAX_EVENT];
User daftarUser[MAX_USER];
Join daftarJoin[MAX_JOIN];
int jumlahEvent = 0, jumlahUser = 0, jumlahJoin = 0;

// DEKLARASI
void muatData();
void simpanData();
void menuUtama();
void menuPanitia(User *user);
void menuPeserta(User *user);
void buatEvent();
void editEvent();
void hapusEvent();
void lihatSemuaEvent();
void daftarKeEvent(User *user);
void keluarDariEvent(User *user);
void lihatEventDiikuti(User *user);
int cekTanggalBentrok(User *user, char *tanggal);
void buatUser();
void lihatSemuaPeserta();
void detailPeserta();
void hapusPeserta();
void kickPeserta();
void garis();


int main() {
    muatData();
    menuUtama();
    simpanData();
    return 0;
}

void menuUtama() {
    int pilihan;
    do {
        garis();
        printf("\n=== SISTEM EVENT ORGANIZER ===\n");
        printf("1. Login sebagai Panitia\n");
        printf("2. Login sebagai Peserta\n");
        printf("3. Daftar Akun Baru\n");
        printf("4. Exit\n");
        printf("Pilih: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid!\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (pilihan == 1 || pilihan == 2) {
            char nama[50];
            printf("Masukkan nama akun: ");
            fgets(nama, sizeof(nama), stdin);
            nama[strcspn(nama, "\n")] = '\0';

            int found = -1;
            for (int i = 0; i < jumlahUser; i++) {
                if (strcmp(daftarUser[i].nama, nama) == 0) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                printf("Akun tidak ditemukan!\n");
                continue;
            }

            if (pilihan == 1 && strcmp(daftarUser[found].role, "panitia") == 0)
                menuPanitia(&daftarUser[found]);
            else if (pilihan == 2 && strcmp(daftarUser[found].role, "peserta") == 0)
                menuPeserta(&daftarUser[found]);
            else
                printf("Role akun tidak sesuai!\n");
        } 
        else if (pilihan == 3) {
            buatUser();
        } 
        else if (pilihan == 4) {
            printf("Terima kasih telah menggunakan sistem ini!\n");
            break;
        } 
        else {
            printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 4);
}

void menuPanitia(User *user) {
    int pilihan;
    do {
        garis();
        printf("\n=== MENU PANITIA (%s) ===\n", user->nama);
        printf("1. Buat Event\n");
        printf("2. Edit Event\n");
        printf("3. Hapus Event\n");
        printf("4. Lihat Semua Event\n");
        printf("5. Lihat Semua Peserta\n");
        printf("6. Lihat Detail Peserta\n");
        printf("7. Hapus Peserta\n");
        printf("8. Kick Peserta dari Event\n");
        printf("9. Kembali\n");
        printf("Pilih: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input salah!\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (pilihan) {
            case 1: buatEvent(); break;
            case 2: editEvent(); break;
            case 3: hapusEvent(); break;
            case 4: lihatSemuaEvent(); break;
            case 5: lihatSemuaPeserta(); break;
            case 6: detailPeserta(); break;
            case 7: hapusPeserta(); break;
            case 8: kickPeserta(); break;
            case 9: return;
            default: printf("Pilihan tidak valid.\n");
        }
    } while (1);
}


void menuPeserta(User *user) {
    int pilihan;
    do {
        garis();
        printf("\n=== MENU PESERTA (%s) ===\n", user->nama);
        printf("1. Lihat Daftar Event\n");
        printf("2. Ikut Event\n");
        printf("3. Keluar dari Event\n");
        printf("4. Lihat Event yang Diikuti\n");
        printf("5. Kembali\n");
        printf("Pilih: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input salah!\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (pilihan) {
            case 1: lihatSemuaEvent(); break;
            case 2: daftarKeEvent(user); break;
            case 3: keluarDariEvent(user); break;
            case 4: lihatEventDiikuti(user); break;
            case 5: return;
            default: printf("Pilihan tidak valid.\n");
        }
    } while (1);
}


void buatEvent() {
    if (jumlahEvent >= MAX_EVENT) return;
    Event e;
    e.id = jumlahEvent + 1;

    printf("Nama event: ");
    fgets(e.nama, sizeof(e.nama), stdin);
    e.nama[strcspn(e.nama, "\n")] = '\0';

    printf("Tanggal (dd/mm/yyyy): ");
    fgets(e.tanggal, sizeof(e.tanggal), stdin);
    e.tanggal[strcspn(e.tanggal, "\n")] = '\0';

    printf("Lokasi: ");
    fgets(e.lokasi, sizeof(e.lokasi), stdin);
    e.lokasi[strcspn(e.lokasi, "\n")] = '\0';

    printf("Deskripsi: ");
    fgets(e.deskripsi, sizeof(e.deskripsi), stdin);
    e.deskripsi[strcspn(e.deskripsi, "\n")] = '\0';

    daftarEvent[jumlahEvent++] = e;
    printf("Event berhasil dibuat!\n");
}

void editEvent() {
    int id;
    printf("Masukkan ID event: ");
    scanf("%d", &id);
    getchar();
    if (id <= 0 || id > jumlahEvent) {
        printf("Event tidak ditemukan!\n");
        return;
    }

    Event *e = &daftarEvent[id - 1];
    printf("Edit nama event (%s): ", e->nama);
    fgets(e->nama, sizeof(e->nama), stdin);
    e->nama[strcspn(e->nama, "\n")] = '\0';

    printf("Edit tanggal (%s): ", e->tanggal);
    fgets(e->tanggal, sizeof(e->tanggal), stdin);
    e->tanggal[strcspn(e->tanggal, "\n")] = '\0';

    printf("Edit lokasi (%s): ", e->lokasi);
    fgets(e->lokasi, sizeof(e->lokasi), stdin);
    e->lokasi[strcspn(e->lokasi, "\n")] = '\0';

    printf("Edit deskripsi (%s): ", e->deskripsi);
    fgets(e->deskripsi, sizeof(e->deskripsi), stdin);
    e->deskripsi[strcspn(e->deskripsi, "\n")] = '\0';

    printf("Event berhasil diedit!\n");
}

void hapusEvent() {
    int id;
    printf("Masukkan ID event yang ingin dihapus: ");
    scanf("%d", &id);
    getchar();
    if (id <= 0 || id > jumlahEvent) {
        printf("Event tidak ditemukan!\n");
        return;
    }

    for (int i = id - 1; i < jumlahEvent - 1; i++)
        daftarEvent[i] = daftarEvent[i + 1];
    jumlahEvent--;

    printf("Event berhasil dihapus.\n");
}

void lihatSemuaEvent() {
    garis();
    if (jumlahEvent == 0) {
        printf("Tidak ada event.\n");
        return;
    }
    for (int i = 0; i < jumlahEvent; i++) {
        printf("ID: %d | %s | %s | %s\n", daftarEvent[i].id, daftarEvent[i].nama, daftarEvent[i].tanggal, daftarEvent[i].lokasi);
    }
}


void daftarKeEvent(User *user) {
    lihatSemuaEvent();
    int id;
    printf("Masukkan ID event yang ingin diikuti: ");
    scanf("%d", &id);
    getchar();

    if (id <= 0 || id > jumlahEvent) {
        printf("Event tidak ditemukan.\n");
        return;
    }

    if (cekTanggalBentrok(user, daftarEvent[id - 1].tanggal)) {
        printf("Anda sudah mengikuti event lain di tanggal tersebut!\n");
        return;
    }

    daftarJoin[jumlahJoin].userId = user->id;
    daftarJoin[jumlahJoin].eventId = id;
    jumlahJoin++;
    printf("Berhasil mengikuti event %s!\n", daftarEvent[id - 1].nama);
}

void keluarDariEvent(User *user) {
    int id;
    printf("Masukkan ID event yang ingin Anda keluar: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < jumlahJoin; i++) {
        if (daftarJoin[i].userId == user->id && daftarJoin[i].eventId == id) {
            for (int j = i; j < jumlahJoin - 1; j++)
                daftarJoin[j] = daftarJoin[j + 1];
            jumlahJoin--;
            printf("Anda berhasil keluar dari event.\n");
            return;
        }
    }
    printf("Anda tidak terdaftar di event tersebut.\n");
}

void lihatEventDiikuti(User *user) {
    garis();
    printf("Event yang diikuti %s:\n", user->nama);
    int found = 0;
    for (int i = 0; i < jumlahJoin; i++) {
        if (daftarJoin[i].userId == user->id) {
            Event *e = &daftarEvent[daftarJoin[i].eventId - 1];
            printf("ID: %d | %s | %s | %s\n", e->id, e->nama, e->tanggal, e->lokasi);
            found = 1;
        }
    }
    if (!found) printf("Belum ada event yang diikuti.\n");
}

int cekTanggalBentrok(User *user, char *tanggal) {
    for (int i = 0; i < jumlahJoin; i++) {
        if (daftarJoin[i].userId == user->id) {
            if (strcmp(daftarEvent[daftarJoin[i].eventId - 1].tanggal, tanggal) == 0)
                return 1;
        }
    }
    return 0;
}

void buatUser() {
    if (jumlahUser >= MAX_USER) return;
    User u;
    u.id = jumlahUser + 1;
    printf("Masukkan nama: ");
    fgets(u.nama, sizeof(u.nama), stdin);
    u.nama[strcspn(u.nama, "\n")] = '\0';
    printf("Role (panitia/peserta): ");
    fgets(u.role, sizeof(u.role), stdin);
    u.role[strcspn(u.role, "\n")] = '\0';
    daftarUser[jumlahUser++] = u;
    printf("Akun berhasil dibuat!\n");
}

void lihatSemuaPeserta() {
    garis();
    printf("Daftar semua peserta:\n");
    for (int i = 0; i < jumlahUser; i++) {
        if (strcmp(daftarUser[i].role, "peserta") == 0)
            printf("ID: %d | %s\n", daftarUser[i].id, daftarUser[i].nama);
    }
}

void detailPeserta() {
    int id;
    printf("Masukkan ID peserta: ");
    scanf("%d", &id);
    getchar();

    if (id <= 0 || id > jumlahUser) {
        printf("Peserta tidak ditemukan!\n");
        return;
    }

    User *u = &daftarUser[id - 1];
    printf("Nama: %s\nRole: %s\nEvent yang diikuti:\n", u->nama, u->role);
    lihatEventDiikuti(u);
}

void hapusPeserta() {
    int id;
    printf("Masukkan ID peserta yang ingin dihapus: ");
    scanf("%d", &id);
    getchar();

    if (id <= 0 || id > jumlahUser) {
        printf("Peserta tidak ditemukan!\n");
        return;
    }

    for (int i = id - 1; i < jumlahUser - 1; i++)
        daftarUser[i] = daftarUser[i + 1];
    jumlahUser--;

    printf("Peserta berhasil dihapus!\n");
}

void kickPeserta() {
    int uid, eid;
    printf("Masukkan ID peserta: ");
    scanf("%d", &uid);
    printf("Masukkan ID event: ");
    scanf("%d", &eid);
    getchar();

    for (int i = 0; i < jumlahJoin; i++) {
        if (daftarJoin[i].userId == uid && daftarJoin[i].eventId == eid) {
            for (int j = i; j < jumlahJoin - 1; j++)
                daftarJoin[j] = daftarJoin[j + 1];
            jumlahJoin--;
            printf("Peserta dikeluarkan dari event.\n");
            return;
        }
    }
    printf("Peserta tidak terdaftar di event tersebut.\n");
}

void muatData() {
    FILE *f;
    if ((f = fopen(FILE_EVENT, "r"))) {
        while (fscanf(f, "%d|%49[^|]|%19[^|]|%49[^|]|%99[^\n]\n",
                      &daftarEvent[jumlahEvent].id,
                      daftarEvent[jumlahEvent].nama,
                      daftarEvent[jumlahEvent].tanggal,
                      daftarEvent[jumlahEvent].lokasi,
                      daftarEvent[jumlahEvent].deskripsi) == 5)
            jumlahEvent++;
        fclose(f);
    }
    if ((f = fopen(FILE_USER, "r"))) {
        while (fscanf(f, "%d|%49[^|]|%9[^\n]\n",
                      &daftarUser[jumlahUser].id,
                      daftarUser[jumlahUser].nama,
                      daftarUser[jumlahUser].role) == 3)
            jumlahUser++;
        fclose(f);
    }
    if ((f = fopen(FILE_JOIN, "r"))) {
        while (fscanf(f, "%d|%d\n",
                      &daftarJoin[jumlahJoin].userId,
                      &daftarJoin[jumlahJoin].eventId) == 2)
            jumlahJoin++;
        fclose(f);
    }
}

void simpanData() {
    FILE *f;
    f = fopen(FILE_EVENT, "w");
    for (int i = 0; i < jumlahEvent; i++)
        fprintf(f, "%d|%s|%s|%s|%s\n", daftarEvent[i].id, daftarEvent[i].nama, daftarEvent[i].tanggal, daftarEvent[i].lokasi, daftarEvent[i].deskripsi);
    fclose(f);

    f = fopen(FILE_USER, "w");
    for (int i = 0; i < jumlahUser; i++)
        fprintf(f, "%d|%s|%s\n", daftarUser[i].id, daftarUser[i].nama, daftarUser[i].role);
    fclose(f);

    f = fopen(FILE_JOIN, "w");
    for (int i = 0; i < jumlahJoin; i++)
        fprintf(f, "%d|%d\n", daftarJoin[i].userId, daftarJoin[i].eventId);
    fclose(f);
}

void garis() {
    printf("---------------------------------------------\n");
}
