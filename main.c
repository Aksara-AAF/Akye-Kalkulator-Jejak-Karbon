#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RENDAH, NORMAL, TINGGI
} KlasifikasiEmisi;

typedef struct{
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
    int pln;
    int bersih;
    int hybrid; 
} JumlahListrik;

typedef struct {
    int orgRumah;
    int tipeSumberListrik;
    int kategoriDaya;
    int tagihanListrik;
    int listrikPLN;
    int listrikBersih;
    int listrikHybrid;
    int totalListrik;
} EmisiListrik;


// Deklarasi fungsi, silahkan tambahkan parameternya bila perlu  
//Function Klasifikasi
KlasifikasiEmisi klasifikasiEmisi(float emisi);
const char* getKlasifikasiLabel(KlasifikasiEmisi klas);   
void displayKlasifikasi(KlasifikasiEmisi klas);

//Function Perhitungan
float hitungTransportasi();
float hitungPenerbangan();
float hitungListrik(EmisiListrik *emisiListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i);
float hitungPeralatan();
float hitungMakanan();

//Function Display
void displayTips();
void displayLeaderboard(Orang *data, int jumlah);

int main() {
    printf("========== KALKULATOR JEJAK KARBON ==========\n");

    int n, i;
    printf("Berapa jumlah orang yang mau mengisi? ");
    scanf("%d", &n);
    
    Orang *ptrOrang;
    ptrOrang = (Orang*) malloc(n * sizeof(Orang));

    EmisiListrik dayaListrik[i];
    SumberListrik sumberEnergi[i];
    JumlahListrik listrik[i];

    for (i = 0; i < n; i++){
        printf("\n=== Orang ke-%d ===\n", i+1);
        printf("Nama : ");
        getchar();
        scanf("%[^\n]s", (ptrOrang + i)->nama);
        
        float emisiTransportasi = hitungTransportasi();
        float emisiPenerbangan = hitungPenerbangan();
        float emisiListrik = hitungListrik(dayaListrik, sumberEnergi, listrik, i);
        float emisiPeralatan = hitungPeralatan();
        float emisiMakanan = hitungMakanan();
        
        float total = emisiTransportasi + emisiPenerbangan + emisiListrik + emisiPeralatan + emisiMakanan;
        (ptrOrang + i)->totalEmisi = total;
        (ptrOrang + i)->klasTotalEmisi = klasifikasiEmisi(total);
        
        printf("\nTotal Emisi %s: %.2f Ton CO2/tahun\n", ptrOrang[i].nama, total);
        displayTips();
    }

    printf("\n=================== LEADERBOARD JEJAK KARBON ===================\n");
    displayLeaderboard(ptrOrang, n);

    free(ptrOrang);
    return 0;
}

// Buat definisi tiap function di bawah sini

KlasifikasiEmisi klasifikasiEmisi(float emisi){
    if (emisi < 2.0){
        return RENDAH;
    }else if (emisi < 4.0){
        return NORMAL;
    }else {
        return TINGGI;
    }
}

const char* getKlasifikasiLabel(KlasifikasiEmisi klas){
    switch (klas){
        case RENDAH: return "Rendah";
        case NORMAL: return "Normal";
        case TINGGI: return "Tinggi";
        default: return "Tidak diketahui";
    }
}

void displayKlasifikasi(KlasifikasiEmisi klas){
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

void displayLeaderboard(Orang *data, int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = i + 1; j < jumlah; j++) {
            if (data[i].totalEmisi > data[j].totalEmisi) {
                Orang temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }

    printf("Peringkat | Nama                | Emisi (Ton CO2) | Klasifikasi\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < jumlah; i++) {
        printf("   %2d     | %-20s|     %6.2f      | ", i + 1, data[i].nama, data[i].totalEmisi);
		displayKlasifikasi(data[i].klasTotalEmisi);
    }
}

float hitungListrik(EmisiListrik *emisiListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i) {
    int x;
    float dayaBulan; // kWh
    float dayaBersih; // kWh
    printf("\n=== Hitung Emisi Listrik ===\n");
    printf("Berapa jumlah orang yang tinggal di rumah? ");
    scanf("%d", &emisiListrik[i].orgRumah);
    printf("Tipe Sumber Listrik:\n");
    printf("1. PLN (100%%)\n");
    printf("2. Bersih (100%%)\n");
    printf("3. Hybrid\n");
    scanf("%d", &x);
    switch(x) {
        case 1:
            sumberEnergi[i] = PLN;
            break;
        case 2:
            sumberEnergi[i] = BERSIH;
            break; 
        case 3:
            sumberEnergi[i] = HYBRID;
            break;
        default:
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
            return 0;
    }

    switch(sumberEnergi[i]) {
        case PLN:
            printf("Berapa daya yang terpasang (Watt)? ");
            printf("\n1. 450 Watt\n2. 900 Watt\n3. 1300 Watt\n4. 2200 Watt\n5. 3500 Watt\n6. 5500 Watt\n7. >6600 Watt\n");
            scanf("%d", &emisiListrik[i].kategoriDaya);
            printf("Berapa tagihan listrik per bulan (Rp)? ");
            scanf("%d", &emisiListrik[i].tagihanListrik);
            if (emisiListrik[i].kategoriDaya < 3) {
                dayaBulan = emisiListrik[i].tagihanListrik / 1352.00;
            }
            else if (emisiListrik[i].kategoriDaya >= 3) {
                dayaBulan = emisiListrik[i].tagihanListrik / 1445.00;
            }
            listrik[i].pln = dayaBulan*0.01*0.984;
            return listrik[i].pln/emisiListrik[i].orgRumah;
        case BERSIH:
            printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
            scanf("%f", &dayaBersih);
            listrik[i].bersih = dayaBersih;
            return listrik[i].bersih*0;
        case HYBRID:
            printf("Berapa daya yang terpasang (Watt)?");
            printf("\n1. 450 Watt\n2. 900 Watt\n3. 1300 Watt\n4. 2200 Watt\n5. 3500 Watt\n6. 5500 Watt\n7. >6600 Watt\n");
            scanf("%d", &emisiListrik[i].kategoriDaya);
            printf("Berapa tagihan listrik per bulan (Rp)? ");
            scanf("%d", &emisiListrik[i].tagihanListrik);
            if (emisiListrik[i].kategoriDaya < 3) {
                dayaBulan = emisiListrik[i].tagihanListrik / 1352.00;
            }
            else if (emisiListrik[i].kategoriDaya >= 3) {
                dayaBulan = emisiListrik[i].tagihanListrik / 1445.00;
            }
            printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
            scanf("%f", &dayaBersih);
            listrik[i].hybrid = dayaBulan*0.01*0.984+dayaBersih*0;
            return listrik[i].hybrid/emisiListrik[i].orgRumah;
        default:
            return 0;
    }

}