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

// Deklarasi fungsi, silahkan tambahkan parameternya bila perlu  
//Function Klasifikasi
KlasifikasiEmisi klasifikasiEmisi(float emisi);
const char* getKlasifikasiLabel(KlasifikasiEmisi klas);   
void displayKlasifikasi(KlasifikasiEmisi klas);

//Function Perhitungan
float hitungTransportasi();
float hitungPenerbangan();
float hitungListrik();
float hitungPeralatan();
float hitungMakanan();

//Function Display
void displayTips();
void displayLeaderboard(Orang *data, int jumlah);

int main() {
    int i;
    printf("========== KALKULATOR JEJAK KARBON ==========\n");

    int n;
    printf("Berapa jumlah orang yang mau mengisi? ");
    scanf("%d", &n);
    
    Orang *ptrOrang;
    ptrOrang = (Orang*) malloc(n * sizeof(Orang));

    for (i = 0; i < n; i++){
        printf("\n=== Orang ke-%d ===\n", i+1);
        printf("Nama : ");
        getchar();
        scanf("%[^\n]s", (ptrOrang + i)->nama);
        
        float emisiTransportasi = hitungTransportasi();
        float emisiPenerbangan = hitungPenerbangan();
        float emisiListrik = hitungListrik();
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
    int i,j;
    for (i = 0; i < jumlah - 1; i++) {
        for (j = i + 1; j < jumlah; j++) {
            if (data[i].totalEmisi > data[j].totalEmisi) {
                Orang temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }

    printf("Peringkat | Nama                 | Emisi (Ton CO2)| Klasifikasi\n");
    printf("------------------------------------------------------------------\n");
    for (i = 0; i < jumlah; i++) {
        printf("   %2d     | %-20s |%6.2f          |  ", i + 1, data[i].nama, data[i].totalEmisi);
        displayKlasifikasi(data[i].klasTotalEmisi);
    }
}                                                                                                