#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum {
    RENDAH,
    NORMAL,
    TINGGI
} KlasifikasiEmisi;

typedef struct {
    char nama[50];
    float totalEmisi;
    KlasifikasiEmisi klasTotalEmisi;
} Orang;

typedef enum {
    PLN,
    BERSIH,
    HYBRID
} SumberListrik;

typedef union {
    float pln;
    float bersih;
    float hybrid; 
} JumlahListrik;

typedef struct {
    int orgRumah;            // banyak orang dalam rumah
    int tipeSumberListrik;   // kode untuk sumber listrik (1 = PLN, 2 = BERSIH, 3 = HYBRID)
    int kategoriDaya;        // kategori daya listrik PLN (1-7)
    int tagihanListrik;      // tagihan listrik per bulan
    float listrikPLN;        // kWh listrik PLN
    float listrikBersih;     // kWh listrik bersih
    float listrikHybrid;     // kWh listrik hybrid (PLN + bersih)
    float totalEmisiListrik; // total emisi dari daya listrik
} EmisiListrik;


typedef struct{
    char *namaMakanan;
    float emisiPerPorsi; // dalam kg CO2, dan satu porsi = 100 gr 
} Makanan;

// Deklarasi fungsi, silahkan tambahkan parameternya bila perlu
// Function Klasifikasi
KlasifikasiEmisi klasifikasiEmisi(float emisi);
const char *getKlasifikasiLabel(KlasifikasiEmisi klas);

// Function Perhitungan
float hitungTransportasi();
float hitungListrik(EmisiListrik *dayaListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i);
float hitungPeralatan();
float hitungMakanan();

// Function Display
void displayKlasifikasi(KlasifikasiEmisi klas);
void displayTips(float emisiTransportasi, float emisiListrik, float emisiPeralatan, float emisiMakanan);
void displayLeaderboard(Orang *data, int jumlah);

//Function validasi input
void clearInputBuffer();

int main() {
    int i, n;
    printf("========== KALKULATOR JEJAK KARBON ==========\n");
    srand(time(NULL));

    printf("Berapa jumlah orang yang mau mengisi? ");
    scanf("%d", &n);

    Orang *ptrOrang;
    ptrOrang = (Orang*) malloc(n * sizeof(Orang));
  
    EmisiListrik *dayaListrik = (EmisiListrik*) malloc(n * sizeof(EmisiListrik));
    SumberListrik *sumberEnergi = (SumberListrik*) malloc(n * sizeof(SumberListrik));
    JumlahListrik *listrik = (JumlahListrik*) malloc(n * sizeof(JumlahListrik));
    if (ptrOrang == NULL || dayaListrik == NULL || sumberEnergi == NULL || listrik == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        return 1;
    }

    for (i = 0; i < n; i++){
        printf("\n=== Orang ke-%d ===\n", i+1);
        printf("Nama : ");
        getchar();
        scanf("%[^\n]s", (ptrOrang + i)->nama);

        float emisiTransportasi = hitungTransportasi();
        float emisiListrik = hitungListrik(dayaListrik, sumberEnergi, listrik, i);
        float emisiPeralatan = hitungPeralatan();
        float emisiMakanan = hitungMakanan();
        
        float total = emisiTransportasi + emisiListrik + emisiPeralatan + emisiMakanan;
        (ptrOrang + i)->totalEmisi = total;
        (ptrOrang + i)->klasTotalEmisi = klasifikasiEmisi(total);
        
        printf("\nTotal Emisi Karbon: %.2f kg CO2\n", total);

        displayTips(emisiTransportasi, emisiListrik, emisiPeralatan, emisiMakanan);

    }

    printf("\n=================== LEADERBOARD JEJAK KARBON ===================\n");
    displayLeaderboard(ptrOrang, n);

    free(ptrOrang);
    free(dayaListrik);
    free(sumberEnergi);
    free(listrik);
    return 0;
}

// Buat definisi tiap function di bawah sini

KlasifikasiEmisi klasifikasiEmisi(float emisi) {
    if (emisi < 5.0){
        return RENDAH;
    }
    else if (emisi < 20.0){
        return NORMAL;
    }
    else{
        return TINGGI;
    }
}

const char *getKlasifikasiLabel(KlasifikasiEmisi klas)
{
    switch (klas){
    case RENDAH:
        return "Rendah";
    case NORMAL:
        return "Normal";
    case TINGGI:
        return "Tinggi";
    default:
        return "Tidak diketahui";
    }
}

float hitungTransportasi() {
    int vehicle, bbm, dist, trip, amount, loop;
    float x = 0, emisi = 0, emisiM = 0, emisiDarat = 0, emisiUdara = 0;
    
    // Validated input for jumlah kendaraan darat
    do {
        printf("Ada berapa kendaraan darat (Mobil & Motor) yang digunakan? ");
        if (scanf("%d", &amount) != 1 || amount < 0) {
            printf("Input tidak valid!\n");
            clearInputBuffer();
            amount = -1;
        }
    } while (amount < 0);
    for(loop = 0 ; loop < amount ; loop++){
        printf("\n");
        do {
            printf("Apa kendaraan ke-%d ?\n1. Mobil\n2. Motor\n", loop + 1);
            if (scanf("%d", &vehicle) != 1 || (vehicle != 1 && vehicle != 2)) {
                printf("Kendaraan tidak terdaftar.\n");
                clearInputBuffer();
                vehicle = 0;
            }
        } while (vehicle != 1 && vehicle != 2);
        switch(vehicle){
            case 1:
                do{
                    printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
                    if (scanf("%d", &bbm) != 1 || (bbm != 1 && bbm != 2)) {
                        printf("Bahan bakar tidak tersedia, ulangi.\n");
                        clearInputBuffer();
                        bbm = 0;
                    }
                    if(bbm == 1) x = 0.08760;
                    else if(bbm == 2) x = 0.03053;
                }while(bbm != 1 && bbm != 2);
                do{
                    printf("Berapa jarak yang ditempuh per hari (KM/Hari)? ");
                    if (scanf("%d", &dist) != 1 || dist <= 0) {
                        printf("Jarak tidak bisa 0 atau lebih rendah.\n");
                        clearInputBuffer();
                        dist = -1;
                    }
                }while(dist <= 0);
                emisiM = dist * x;
                emisiDarat += emisiM;
                break;
            case 2:
                do{
                    printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
                    if (scanf("%d", &bbm) != 1 || (bbm != 1 && bbm != 2)) {
                        printf("Bahan tidak terdaftar, mohon coba lagi.\n");
                        clearInputBuffer();
                        bbm = 0;
                    }
                    if(bbm == 1) x = 0.03503;
                    else if(bbm == 2) x = 0.0213;
                }while(bbm != 1 && bbm != 2);
                do{
                    printf("Berapa jarak yang ditempuh per hari (KM/Hari)? ");
                    if (scanf("%d", &dist) != 1 || dist <= 0) {
                        printf("Jarak tidak bisa 0 atau lebih rendah.\n");
                        clearInputBuffer();
                        dist = -1;
                    }
                }while(dist <= 0);
                emisiM = dist * x;
                emisiDarat += emisiM;
                break;
        }
    }
    printf("Emisi Karbon dari Kendaraan Darat: %.3f Ton CO2/tahun\n", emisiDarat);
    
    printf("\n");
    do{
        printf("Apakah menggunakan kendaraan udara (Pesawat)?\n1. Iya\n2. Tidak\n");
        if (scanf("%d", &amount) != 1 || (amount != 1 && amount != 2)) {
            printf("Opsi tidak valid.\n");
            clearInputBuffer();
            amount = 0;
        }
        switch(amount){
            case 1:
                do {
                    printf("Jenis perjalanan udara:\n1. One-Way Trip\n2. Roundtrip\n");
                    if (scanf("%d", &vehicle) != 1 || (vehicle != 1 && vehicle != 2)) {
                        printf("Pilihan tidak valid. Diasumsikan one-way trip.\n");
                        clearInputBuffer();
                        vehicle = 1;
                    }
                } while (vehicle != 1 && vehicle != 2);
                trip = (vehicle == 2) ? 2 : 1;
                do {
                    printf("Berapa sering perjalanan dilakukan? ");
                    if (scanf("%d", &bbm) != 1 || bbm < 0) {
                        printf("Input tidak valid!\n");
                        clearInputBuffer();
                        bbm = -1;
                    }
                } while (bbm < 0);
                do {
                    printf("Berapa jarak yang ditempuh (KM/perjalanan)? ");
                    if (scanf("%d", &dist) != 1 || dist < 0) {
                        printf("Input tidak valid!\n");
                        clearInputBuffer();
                        dist = -1;
                    }
                } while (dist < 0);
                break;
            case 2:
                trip = 0;
                bbm = 0;
                dist = 0;
                break;
        }
    }while(amount != 1 && amount != 2);
    emisiUdara = trip * dist * 0.000156 * bbm;
    printf("Emisi Karbon dari Kendaraan Udara: %.3f Ton CO2/tahun\n", emisiUdara);
    emisi = emisiDarat + emisiUdara;
    printf("\nTotal Emisi Karbon dari Kedua Tipe Kendaraan: %.3f Ton CO2/Tahun", emisi);
    return emisi;
}

float hitungListrik(EmisiListrik *dayaListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i) {
    float dayaBulan; // kWh
    float dayaBersih; // kWh
    printf("\n=== Hitung Emisi Listrik ===\n");

    // Input jumlah orang yang tinggal di rumah
    int valid = 0;
    while (!valid) {
        printf("Berapa jumlah orang yang tinggal di rumah? ");
        if (scanf("%d", &dayaListrik[i].orgRumah) != 1) {
            printf("Input tidak valid!\n");
            clearInputBuffer();
        } else if (dayaListrik[i].orgRumah <= 0) {
            printf("Jumlah orang harus lebih dari 0.\n");
        } else {
            valid = 1;
        }
    }

    // Input tipe sumber listrik
    printf("Tipe Sumber Listrik:\n");
    printf("1. PLN (100%%)\n");
    printf("2. Bersih (100%%)\n");
    printf("3. Hybrid\n");
    do {
        printf("Pilih tipe sumber listrik (1-3): ");
        if (scanf("%d", &dayaListrik[i].tipeSumberListrik) != 1 || dayaListrik[i].tipeSumberListrik < 1 || dayaListrik[i].tipeSumberListrik > 3) {
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
            clearInputBuffer();
            dayaListrik[i].tipeSumberListrik = 0;
        }
    } while (dayaListrik[i].tipeSumberListrik < 1 || dayaListrik[i].tipeSumberListrik > 3);

    switch(dayaListrik[i].tipeSumberListrik) {
        case 1: sumberEnergi[i] = PLN; break;
        case 2: sumberEnergi[i] = BERSIH; break;
        case 3: sumberEnergi[i] = HYBRID; break;
    }

    switch(sumberEnergi[i]) {
        case PLN:
            printf("Berapa daya yang terpasang (Watt)?\n");
            printf("1. 450 Watt\n2. 900 Watt\n3. 1300 Watt\n4. 2200 Watt\n5. 3500 Watt\n6. 5500 Watt\n7. >6600 Watt\n");
            do {
                printf("Pilih kategori daya (1-7): ");
                if (scanf("%d", &dayaListrik[i].kategoriDaya) != 1 || dayaListrik[i].kategoriDaya < 1 || dayaListrik[i].kategoriDaya > 7) {
                    printf("Kategori daya tidak valid. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaListrik[i].kategoriDaya = 0;
                }
            } while (dayaListrik[i].kategoriDaya < 1 || dayaListrik[i].kategoriDaya > 7);

            do {
                printf("Berapa tagihan listrik per bulan (Rp)? ");
                if (scanf("%d", &dayaListrik[i].tagihanListrik) != 1 || dayaListrik[i].tagihanListrik <= 0) {
                    printf("Tagihan listrik tidak bisa negatif. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaListrik[i].tagihanListrik = 0;
                }
            } while (dayaListrik[i].tagihanListrik <= 0);

            if (dayaListrik[i].kategoriDaya < 3) {
                dayaBulan = dayaListrik[i].tagihanListrik / 1352.00;
            } else {
                dayaBulan = dayaListrik[i].tagihanListrik / 1445.00;
            }
            listrik[i].pln = dayaBulan;
            dayaListrik[i].listrikPLN = listrik[i].pln;
            dayaListrik[i].totalEmisiListrik = listrik[i].pln * 0.01 * 0.984;
            printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah);
            return dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah;

        case BERSIH:
            do {
                printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
                if (scanf("%f", &dayaBersih) != 1 || dayaBersih < 0) {
                    printf("Daya bersih tidak bisa negatif. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaBersih = -1;
                }
            } while (dayaBersih < 0);
            listrik[i].bersih = dayaBersih;
            dayaListrik[i].listrikBersih = listrik[i].bersih;
            dayaListrik[i].totalEmisiListrik = listrik[i].bersih * 0;
            printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah);
            return dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah;

        case HYBRID:
            printf("Berapa daya yang terpasang (Watt)?\n");
            printf("1. 450 Watt\n2. 900 Watt\n3. 1300 Watt\n4. 2200 Watt\n5. 3500 Watt\n6. 5500 Watt\n7. >6600 Watt\n");
            do {
                printf("Pilih kategori daya (1-7): ");
                if (scanf("%d", &dayaListrik[i].kategoriDaya) != 1 || dayaListrik[i].kategoriDaya < 1 || dayaListrik[i].kategoriDaya > 7) {
                    printf("Kategori daya tidak valid. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaListrik[i].kategoriDaya = 0;
                }
            } while (dayaListrik[i].kategoriDaya < 1 || dayaListrik[i].kategoriDaya > 7);

            do {
                printf("Berapa tagihan listrik per bulan (Rp)? ");
                if (scanf("%d", &dayaListrik[i].tagihanListrik) != 1 || dayaListrik[i].tagihanListrik <= 0) {
                    printf("Tagihan listrik tidak bisa negatif. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaListrik[i].tagihanListrik = 0;
                }
            } while (dayaListrik[i].tagihanListrik <= 0);

            if (dayaListrik[i].kategoriDaya < 3) {
                dayaBulan = dayaListrik[i].tagihanListrik / 1352.00;
            } else {
                dayaBulan = dayaListrik[i].tagihanListrik / 1445.00;
            }

            do {
                printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
                if (scanf("%f", &dayaBersih) != 1 || dayaBersih < 0) {
                    printf("Daya bersih tidak bisa negatif. Silakan coba lagi.\n");
                    clearInputBuffer();
                    dayaBersih = -1;
                }
            } while (dayaBersih < 0);

            listrik[i].hybrid = dayaBulan + dayaBersih * 0;
            dayaListrik[i].listrikHybrid = listrik[i].hybrid;
            dayaListrik[i].totalEmisiListrik = listrik[i].hybrid * 0.01 * 0.984;
            printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah);
            return dayaListrik[i].totalEmisiListrik / dayaListrik[i].orgRumah;

        default:
            printf("Terjadi kesalahan input sumber energi.\n");
            return 0;
    }
}

float hitungPeralatan() {
    // Lampu
    printf("=== Peralatan Lampu ===\n");
    int pilih_lampu;
    float jml_pijar = 0, jam_pijar = 0;
    float jml_neon = 0, jam_neon = 0;
    float jml_led = 0, jam_led = 0;
    float daya_lampu = 0, emisi_lampu = 0;
    printf("Lampu apa yang Anda gunakan?\n1. Pijar\n2. Neon\n3. Led\n");
    do
    {
        printf("Pilih 1-3: ");
        if (scanf("%d", &pilih_lampu) != 1) {
            printf("Input harus angka!\n");
            clearInputBuffer();
            pilih_lampu = 0;
            continue;
        }
        switch (pilih_lampu)
        {
        case 1:
            do {
                printf("Jumlah lampu pijar: ");
                if (scanf("%f", &jml_pijar) != 1 || jml_pijar < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jml_pijar = -1;
                }
            } while (jml_pijar < 0);
            do {
                printf("Jam pemakaian/hari: ");
                if (scanf("%f", &jam_pijar) != 1 || jam_pijar < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jam_pijar = -1;
                }
            } while (jam_pijar < 0);
            if (jam_pijar > 24) {
                jam_pijar = 24;
                printf("Lama pemakaian maksimal 24 jam\n");
            }
            daya_lampu = 65;
            emisi_lampu = (jml_pijar * daya_lampu * jam_pijar * 365 * 0.9) / 1000000;
            break;
        case 2:
            do {
                printf("Jumlah lampu neon: ");
                if (scanf("%f", &jml_neon) != 1 || jml_neon < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jml_neon = -1;
                }
            } while (jml_neon < 0);
            do {
                printf("Jam pemakaian/hari: ");
                if (scanf("%f", &jam_neon) != 1 || jam_neon < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jam_neon = -1;
                }
            } while (jam_neon < 0);
            if (jam_neon > 24) {
                jam_neon = 24;
                printf("Lama pemakaian maksimal 24 jam\n");
            }
            daya_lampu = 16;
            emisi_lampu = (jml_neon * daya_lampu * jam_neon * 365 * 0.9) / 1000000;
            break;
        case 3:
            do {
                printf("Jumlah lampu led: ");
                if (scanf("%f", &jml_led) != 1 || jml_led < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jml_led = -1;
                }
            } while (jml_led < 0);
            do {
                printf("Jam pemakaian/hari: ");
                if (scanf("%f", &jam_led) != 1 || jam_led < 0) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jam_led = -1;
                }
            } while (jam_led < 0);
            if (jam_led > 24) {
                jam_led = 24;
                printf("Lama pemakaian maksimal 24 jam\n");
            }
            daya_lampu = 7;
            emisi_lampu = (jml_led * daya_lampu * jam_led * 365 * 0.9) / 1000000;
            break;
        default:
            printf("Pilihan tidak valid\n");
        }
    } while (pilih_lampu < 1 || pilih_lampu > 3);

    // AC
    printf("\n=== Peralatan AC ===\n");
    int pilih_ac;
    float jml_ac = 0, jam_ac = 0, daya_ac = 0, emisi_ac = 0;
    do {
        printf("Jumlah AC: ");
        if (scanf("%f", &jml_ac) != 1 || jml_ac < 0) {
            printf("Input tidak valid!\n");
            clearInputBuffer();
            jml_ac = -1;
        }
    } while (jml_ac < 0);
    printf("Apakah sudah menggunakan AC dengan teknologi inverter?\n1. Ya\n2. Tidak\n");
    do {
        printf("Pilih 1-2: ");
        if (scanf("%d", &pilih_ac) != 1) {
            printf("Input harus angka!\n");
            clearInputBuffer();
            pilih_ac = 0;
            continue;
        }
        switch (pilih_ac) {
        case 1:
        case 2:
            do {
                printf("Jam pemakaian/hari: ");
                if (scanf("%f", &jam_ac) != 1 || jam_ac < 0){
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    jam_ac = -1;
                }
            } while (jam_ac < 0);
            if (jam_ac > 24) {
                jam_ac = 24;
                printf("Lama pemakaian maksimal 24 jam\n");
            }
            daya_ac = (pilih_ac == 1) ? 273 : 318;
            emisi_ac = (jml_ac * daya_ac * jam_ac * 365 * 0.9) / 1000000;
            break;
        default:
            printf("Pilihan tidak valid\n");
        }
    } while (pilih_ac < 1 || pilih_ac > 2);

    // Kulkas
    printf("\n=== Peralatan Kulkas ===\n");
    int pilih_kulkas;
    float jml_kulkas = 0, daya_kulkas = 0, emisi_kulkas = 0;
    do {
        printf("Jumlah kulkas: ");
        if (scanf("%f", &jml_kulkas) != 1 || jml_kulkas < 0) {
            printf("Input tidak valid!\n");
            clearInputBuffer();
            jml_kulkas = -1;
        }
    } while (jml_kulkas < 0);
    printf("Apakah sudah menggunakan Kulkas dengan teknologi inverter?\n1. Ya\n2. Tidak\n");
    do {
        printf("Pilih 1-2: ");
        if (scanf("%d", &pilih_kulkas) != 1) {
            printf("Input harus angka!\n");
            clearInputBuffer();
            pilih_kulkas = 0;
            continue;
        }
        switch (pilih_kulkas){
        case 1:
            daya_kulkas = 150;
            emisi_kulkas = (jml_kulkas * daya_kulkas * 24 * 365 * 0.9) / 1000000;
            break;
        case 2:
            daya_kulkas = 200;
            emisi_kulkas = (jml_kulkas * daya_kulkas * 24 * 365 * 0.9) / 1000000;
            break;
        default:
            printf("Pilihan tidak valid\n");
        }
    } while (pilih_kulkas < 1 || pilih_kulkas > 2);

    // Total
    float total_emisi = emisi_lampu + emisi_ac + emisi_kulkas;
    printf("\nEmisi Karbon dari Peralatan Rumah Tangga: %.3f Ton CO2/tahun\n", total_emisi);
    return total_emisi;
}

float hitungMakanan() {
    int jumlahMakanan = 11;

    // Alokasi dinamis untuk array daftarMakanan dari struct Makanan
    Makanan* daftarMakanan = (Makanan*) malloc(jumlahMakanan * sizeof(Makanan));
    if (daftarMakanan == NULL) {
        printf("Gagal mengalokasikan memori untuk daftar makanan.\n");
        return 0;
    }

    // Data makanan dan emisi 
    const char* namaList[] = {
        "Kentang", "Pisang", "Nasi", "Telur", "Keju",
        "Susu", "Daging Sapi", "Daging Domba", "Daging Babi",
        "Daging Unggas", "Udang"
    };
    float emisiList[] = {
        0.046, 0.086, 0.445, 0.467, 2.388,
        0.315, 3.330, 3.972, 1.231,
        0.987, 2.687
    };

    // Alokasi dinamis untuk frekuensi
    int* frekuensi = (int*) malloc(jumlahMakanan * sizeof(int));
    if (frekuensi == NULL) {
        printf("Gagal mengalokasikan memori untuk frekuensi.\n");
        return 0;
    }

    float totalEmisiMakanan = 0;

    printf("\n=== Hitung Emisi Makanan ===\n");
    printf("Masukkan frekuensi konsumsi tiap makanan per minggu (0-21 kali/porsi):\n");

    for (int i = 0; i < jumlahMakanan; i++) {
        // Assign value namaMakanan dan emisiPerPorsi untuk array daftarMakanan
        daftarMakanan[i].namaMakanan = strdup(namaList[i]); // malloc + copy
        daftarMakanan[i].emisiPerPorsi = emisiList[i];
        do {
            printf("%s : ", daftarMakanan[i].namaMakanan);
            if (scanf("%d", &frekuensi[i]) != 1) {
                printf("Input tidak valid!\n");
                clearInputBuffer();
                frekuensi[i] = -1;
            } else if (frekuensi[i] < 0 || frekuensi[i] > 21) {
                printf("Jumlah frekuensi tidak valid. Masukkan antara 0-21.\n");
            }
        } while (frekuensi[i] < 0 || frekuensi[i] > 21);

        float konsumsiTahunan = frekuensi[i] * 52;
        float emisiTahunan = konsumsiTahunan * daftarMakanan[i].emisiPerPorsi;
        totalEmisiMakanan += emisiTahunan;
    }

    totalEmisiMakanan /= 1000; // dari kg ke ton
    printf("Emisi Karbon dari Makanan : %.3f Ton CO2/tahun\n", totalEmisiMakanan);

    // Free memori
    for (int i = 0; i < jumlahMakanan; i++) {
        free(daftarMakanan[i].namaMakanan); 
    }
    free(daftarMakanan);
    free(frekuensi);

    return totalEmisiMakanan;
}
    
void displayKlasifikasi(KlasifikasiEmisi klas) {
    switch(klas){
        case RENDAH:
            printf("Rendah \n");
            break;
        case NORMAL:
            printf("Normal \n");
            break;
        case TINGGI:
            printf("Tinggi \n");
            break;
        default:
            printf("Tidak diketahui\n");
    }
}
    
void displayTips(float emisiTransportasi, float emisiListrik, float emisiPeralatan, float emisiMakanan) {
    char *tipsTransportasi[] = {
        "Gunakan transportasi umum lebih sering.",
        "Cobalah bersepeda untuk perjalanan pendek.",
        "Kurangi penggunaan mobil pribadi.",
        "Berjalan kaki jika jaraknya dekat."
    };

    char *tipsListrik[] = {
        "Gunakan lampu LED hemat energi.",
        "Matikan perangkat listrik saat tidak digunakan.",
        "Cabut charger setelah dipakai.",
        "Gunakan AC seperlunya, jangan berlebihan."
    };

    char *tipsPeralatan[] = {
        "Gunakan peralatan elektronik berlabel hemat energi.",
        "Kurangi penggunaan microwave dan oven listrik.",
        "Jangan gunakan mesin cuci terlalu sering.",
        "Matikan TV atau komputer jika tidak dipakai."
    };
    
    char *tipsMakanan[] = {
        "Kurangi konsumsi daging merah.",
        "Pilih makanan lokal dan musiman.",
        "Kurangi makanan olahan dan kemasan.",
        "Masak secukupnya agar tidak ada sisa makanan."
    };

    int idx;
    float tertinggi = emisiTransportasi;
    char kategori[20] = "Transportasi";

    if (emisiListrik > tertinggi) {
        tertinggi = emisiListrik;
        strcpy(kategori, "Listrik");
    }
    if (emisiPeralatan > tertinggi) {
        tertinggi = emisiPeralatan;
        strcpy(kategori, "Peralatan");
    }
    if (emisiMakanan > tertinggi) {
        tertinggi = emisiMakanan;
        strcpy(kategori, "Makanan");
    }

    printf("\n--- Tips Pengurangan Jejak Karbon ---\n");
    printf("Kategori Tertinggi: %s\n", kategori);

    if (strcmp(kategori, "Transportasi") == 0) {
        idx = rand() % 4;
        printf("Tips: %s\n", tipsTransportasi[idx]);
    } else if (strcmp(kategori, "Listrik") == 0) {
        idx = rand() % 4;
        printf("Tips: %s\n", tipsListrik[idx]);
    } else if (strcmp(kategori, "Peralatan") == 0) {
        idx = rand() % 4;
        printf("Tips: %s\n", tipsPeralatan[idx]);
    } else if (strcmp(kategori, "Makanan") == 0) {
        idx = rand() % 4;
        printf("Tips: %s\n", tipsMakanan[idx]);
    }
}



void displayLeaderboard(Orang *data, int jumlah) {
    int i, j;
    for (i = 0; i < jumlah - 1; i++)
    {
        for (j = i + 1; j < jumlah; j++)
        {
            if (data[i].totalEmisi > data[j].totalEmisi)
            {
                Orang temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }

    printf("Peringkat | Nama                 | Emisi (Ton CO2)| Klasifikasi\n");
    printf("----------------------------------------------------------------\n");
    for (i = 0; i < jumlah; i++)
    {
        printf("   %2d     | %-20s |%6.2f          |  ", i + 1, data[i].nama, data[i].totalEmisi);
        displayKlasifikasi(data[i].klasTotalEmisi);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}