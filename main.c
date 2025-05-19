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
float hitungTransportasi();
float hitungPenerbangan();
float hitungListrik();
float hitungPeralatan();
float hitungMakanan();
void displayTips();
void displayLeaderboard(Orang *data, int jumlah);

int main() {
    printf("========== KALKULATOR JEJAK KARBON ==========\n");

    int n;
    printf("Berapa jumlah orang yang mau mengisi? ");
    scanf("%d", &n);
    
    Orang *ptrOrang;
    ptrOrang = (Orang*) malloc(n * sizeof(Orang));

    for (int i = 0; i < n; i++){
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
        // (ptrOrang + i)->klasTotalEmisi = ...;
        
        printf("\nTotal Emisi %s: %.2f Ton CO2/tahun\n", ptrOrang[i].nama, total);
        displayTips();
    }

    printf("\n========== LEADERBOARD JEJAK KARBON ==========\n");
    displayLeaderboard(ptrOrang, n);

    free(ptrOrang);
    return 0;
}

// Buat definisi tiap function di bawah sini