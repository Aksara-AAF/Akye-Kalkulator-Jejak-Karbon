#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    int orgRumah; // banyak orang dalam rumah
    int tipeSumberListrik; // kode untuk sumber listrik (1 = PLN, 2 = BERSIH, 3 = HYBRID)
    int kategoriDaya; // kategori daya listrik PLN (1-7)
    int tagihanListrik; // tagihan listrik per bulan
    float listrikPLN; // kWh listrik PLN
    float listrikBersih; // kWh listrik bersih
    float listrikHybrid; // kWh listrik hybrid (PLN + bersih)
    float totalEmisiListrik; // total emisi dari daya listrik
} EmisiListrik;


typedef struct{
    char namaMakanan[30];
    float emisiPerPorsi; // dalam kg CO2, dan satu porsi = 100 gr 
} Makanan;

// Deklarasi fungsi, silahkan tambahkan parameternya bila perlu  
//Function Klasifikasi
KlasifikasiEmisi klasifikasiEmisi(float emisi);
const char* getKlasifikasiLabel(KlasifikasiEmisi klas);   

//Function Perhitungan
float hitungTransportasi();
float hitungPenerbangan();
float hitungListrik(EmisiListrik *dayaListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i);
float hitungPeralatan();
float hitungMakanan();

//Function Display
void displayKlasifikasi(KlasifikasiEmisi klas);
void displayTips();
void displayLeaderboard(Orang *data, int jumlah);

int main() {
    int i, n;
    printf("========== KALKULATOR JEJAK KARBON ==========\n");
    srand(time(NULL));

    printf("Berapa jumlah orang yang mau mengisi? ");
    scanf("%d", &n);
    
    Orang *ptrOrang;
    ptrOrang = (Orang*) malloc(n * sizeof(Orang));
  
    for (i = 0; i < n; i++){
        
        EmisiListrik dayaListrik[i];
        SumberListrik sumberEnergi[i];
        JumlahListrik listrik[i];

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
        if ((ptrOrang + i)->klasTotalEmisi == RENDAH){
        	printf("Bagus Pertahankan!\n");
		}else{
			displayTips();
		}

    }

    printf("\n=================== LEADERBOARD JEJAK KARBON ===================\n");
    displayLeaderboard(ptrOrang, n);

    free(ptrOrang);
    return 0;
}

// Buat definisi tiap function di bawah sini

KlasifikasiEmisi klasifikasiEmisi(float emisi){
    if (emisi < 5.0){
        return RENDAH;
    }else if (emisi < 20.0){
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


float hitungPeralatan()
{
    // Lampu
    int pilih_lampu;
    float jml_pijar, jam_pijar;
    float jml_neon, jam_neon;
    float jml_led, jam_led;
    float daya_lampu, emisi_lampu = 0;

    printf("\n=== Peralatan Lampu ===\n");
    printf("Lampu apa yang Anda gunakan?\n1. Pijar\n2. Neon\n3. Led\n");
    printf("Pilih 1-3: ");
    scanf("%d", &pilih_lampu);

    switch (pilih_lampu)
    {
    case 1:
        printf("Jumlah lampu pijar: ");
        scanf("%f", &jml_pijar);
        printf("Jam pemakaian/hari: ");
        scanf("%f", &jam_pijar);
        daya_lampu = 65;
        emisi_lampu = (jml_pijar * daya_lampu * jam_pijar * 365 * 0.9) / 1000000;
        break;
    case 2:
        printf("Jumlah lampu neon: ");
        scanf("%f", &jml_neon);
        printf("Jam pemakaian/hari: ");
        scanf("%f", &jam_neon);
        daya_lampu = 16;
        emisi_lampu = (jml_neon * daya_lampu * jam_neon * 365 * 0.9) / 1000000;
        break;
    case 3:
        printf("Jumlah lampu led: ");
        scanf("%f", &jml_led);
        printf("Jam pemakaian/hari: ");
        scanf("%f", &jam_led);
        daya_lampu = 7;
        emisi_lampu = (jml_led * daya_lampu * jam_led * 365 * 0.9) / 1000000;
        break;
    default:
        printf("Invalid\n");
    }

    printf("\n=== Peralatan AC ===\n");
    int pilih_ac;
    float jml_ac, jam_ac, daya_ac, emisi_ac;
    printf("Apakah sudah menggunakan AC dengan teknologi inverter?\n1. Ya\n2. Tidak\n");
    printf("Pilih 1-2: ");
    scanf("%d", &pilih_ac);
    printf("Jumlah AC: ");
    scanf("%f", &jml_ac);
    printf("Jam pemakaian/hari: ");
    scanf("%f", &jam_ac);
    daya_ac = (pilih_ac == 1) ? 273 : 318;
    emisi_ac = (jml_ac * daya_ac * jam_ac * 365 * 0.9) / 1000000;

    printf("\n=== Peralatan Kulkas ===\n");
    int pilih_kulkas;
    float daya_kulkas, emisi_kulkas;
    printf("Apakah sudah menggunakan Kulkas dengan teknologi inverter?\n1. Ya\n2. Tidak\n");
    printf("Pilih 1-2: ");
    scanf("%d", &pilih_kulkas);
    daya_kulkas = (pilih_kulkas == 1) ? 150 : 200;
    emisi_kulkas = (daya_kulkas * 24 * 365 * 0.9) / 1000000;

    float total_emisi = emisi_lampu + emisi_ac + emisi_kulkas;
    printf("\nEmisi Karbon dari Peralatan Rumah Tangga: %.3f Ton CO2/tahun\n", total_emisi);
    return total_emisi;
}

float hitungListrik(EmisiListrik *dayaListrik, SumberListrik *sumberEnergi, JumlahListrik *listrik, int i) {
    int x;
    float dayaBulan; // kWh
    float dayaBersih; // kWh
    printf("\n=== Hitung Emisi Listrik ===\n");
    printf("Berapa jumlah orang yang tinggal di rumah? ");
    scanf("%d", &dayaListrik[i].orgRumah);
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
        scanf("%d", &dayaListrik[i].kategoriDaya);
        printf("Berapa tagihan listrik per bulan (Rp)? ");
        scanf("%d", &dayaListrik[i].tagihanListrik);
        if (dayaListrik[i].kategoriDaya < 3) {
            dayaBulan = dayaListrik[i].tagihanListrik / 1352.00;
        }
        else if (dayaListrik[i].kategoriDaya >= 3) {
            dayaBulan = dayaListrik[i].tagihanListrik / 1445.00;
        }
        listrik[i].pln = dayaBulan;
        dayaListrik[i].totalEmisiListrik = listrik[i].pln*0.01*0.984;
        printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik);;
        return listrik[i].pln/dayaListrik[i].orgRumah;
        case BERSIH:
        printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
        scanf("%f", &dayaBersih);
        listrik[i].bersih = dayaBersih;
        dayaListrik[i].totalEmisiListrik = listrik[i].bersih*0;
        printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik);
        return dayaListrik[i].totalEmisiListrik;
        case HYBRID:
        printf("Berapa daya yang terpasang (Watt)?");
        printf("\n1. 450 Watt\n2. 900 Watt\n3. 1300 Watt\n4. 2200 Watt\n5. 3500 Watt\n6. 5500 Watt\n7. >6600 Watt\n");
        scanf("%d", &dayaListrik[i].kategoriDaya);
        printf("Berapa tagihan listrik per bulan (Rp)? ");
        scanf("%d", &dayaListrik[i].tagihanListrik);
        if (dayaListrik[i].kategoriDaya < 3) {
            dayaBulan = dayaListrik[i].tagihanListrik / 1352.00;
        }
        else if (dayaListrik[i].kategoriDaya >= 3) {
            dayaBulan = dayaListrik[i].tagihanListrik / 1445.00;
            }
            printf("Berapa banyak listrik yang dihasilkan sumber energi bersih (dalam kWh)? ");
            scanf("%f", &dayaBersih);
            listrik[i].hybrid = dayaBulan+dayaBersih*0;
            dayaListrik[i].totalEmisiListrik = listrik[i].hybrid*0.01*0.984;
            printf("Emisi Karbon dari Daya Rumah Tangga: %.3f Ton CO2/tahun\n", dayaListrik[i].totalEmisiListrik);
            return listrik[i].hybrid/dayaListrik[i].orgRumah;
            default:
            return 0;
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
    
    void displayTips(){
        const char *tips[] = {
        "Kurangi penggunaan kendaraan bermotor pribadi.",
        "Gunakan transportasi umum atau sepeda.",
        "Matikan lampu dan peralatan listrik saat tidak digunakan.",
        "Gunakan peralatan hemat energi.",
        "Kurangi konsumsi daging merah.",
        "Belilah produk lokal dan musiman.",
        "Gunakan air dengan bijak, hindari pemborosan.",
        "Daur ulang sampah yang bisa digunakan kembali.",
        "Gunakan tas belanja yang dapat digunakan ulang.",
        "Tanam pohon atau tanaman di sekitar rumah."
    };

    int jumlahTips = sizeof(tips) / sizeof(tips[0]);
    int index = rand() % jumlahTips;
    printf("Tips: %s\n", tips[index]);
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
    printf("----------------------------------------------------------------\n");
    for (i = 0; i < jumlah; i++) {
        printf("   %2d     | %-20s |%6.2f          |  ", i + 1, data[i].nama, data[i].totalEmisi);
        displayKlasifikasi(data[i].klasTotalEmisi);
    }
}

float hitungMakanan(){
    // Assign nama makanan serta emisi per porsinya
    Makanan daftarMakanan[11] = {
        {"Kentang", 0.046},
        {"Pisang", 0.086},
        {"Nasi", 0.445},
        {"Telur", 0.467},
        {"Keju", 2.388},
        {"Susu", 0.315},
        {"Daging Sapi", 3.330},
        {"Daging Domba", 3.972},
        {"Daging Babi", 1.231},
        {"Daging Unggas", 0.987},
        {"Udang", 2.687}
    };

    int frekuensi[11], i;
    float totalEmisiMakanan = 0;

    printf("\n=== Hitung Emisi Makanan ===\n");
    printf("Masukkan frekuensi konsumsi tiap makanan per minggu (0-21):\n");
    
    for (i = 0; i < 11; i++){
        printf("%s : ", daftarMakanan[i].namaMakanan);
        scanf("%d", &frekuensi[i]);
        
        float konsumsiTahunan = frekuensi[i] * 52; // 52 minggu per tahun
        float emisiTahunan = konsumsiTahunan * daftarMakanan[i].emisiPerPorsi;
        totalEmisiMakanan += emisiTahunan;
    }

    totalEmisiMakanan /= 1000; // Konversi dari kg CO2/tahun menjadi Ton CO2/tahun
    printf("Emisi Karbon dari Makanan : %.3f Ton CO2/tahun\n", totalEmisiMakanan);
    return totalEmisiMakanan;
}

float hitungTransportasi() {
	int choice, vehicle, bbm, dist, trip;
    float x = 0, emisi = 0, emisiDarat = 0, emisiUdara = 0;
           
	printf("Kendaraan apa yang digunakan?\n1. Mobil\n2. Motor\n");
    scanf("%d", &vehicle);
    do{
		switch(vehicle){
	    	case 1:
	            printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
	            scanf("%d", &bbm);
	            if(bbm == 1) x = 0.08760;
	            else if(bbm == 2) x = 0.03053;
	            else {
	                printf("Bahan tidak terdaftar, mohon coba lagi.\n");
	                printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
	            	scanf("%d", &bbm);
	            }
	            printf("Berapa jarak yang ditempuh per hari (KM/Hari)? ");
	            scanf("%d", &dist);
	            emisiDarat = dist * x;
	            break;
			case 2:
	            printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
	            scanf("%d", &bbm);
	            do{
					
		            if(bbm == 1) x = 0.03503;
		            else if(bbm == 2) x = 0.0213;
		            else {
		                printf("Bahan tidak terdaftar, mohon coba lagi.\n");
		                printf("Bahan bakar apa yang digunakan?\n1. Bensin\n2. Listrik\n");
		            	scanf("%d", &bbm);
		            }
		        }while(bbm != 1 && bbm != 2);
	            printf("Berapa jarak yang ditempuh per hari (KM/Hari)? ");
	            scanf("%d", &dist);
	            emisiDarat = dist * x;
	            break;
	
	        default:
	            printf("Kendaraan tidak terdaftar.\n");
	            scanf("%d", &vehicle);
	        }
	}while(vehicle != 1 && vehicle != 2);	
    printf("Emisi Karbon dari Kendaraan Darat: %.3f Ton CO2/tahun\n", emisiDarat);
	printf("\n");
    printf("Jenis perjalanan udara:\n1. One-Way Trip\n2. Roundtrip\n");
    scanf("%d", &vehicle);
    if(vehicle == 1) trip = 1;
    else if(vehicle == 2) trip = 2;
    else {
    	printf("Pilihan tidak valid. Diasumsikan one-way trip.\n");
        trip = 1;
	}		 

    printf("Berapa sering perjalanan dilakukan? ");
    scanf("%d", &bbm);
    printf("Berapa jarak yang ditempuh? ");
    scanf("%d", &dist);

    emisiUdara = trip * dist * 0.000156 * bbm;
    printf("Emisi Karbon dari Kendaraan Udara: %.3f Ton CO2/tahun\n", emisiUdara);
    emisi = emisiDarat + emisiUdara;
    printf("\nTotal Emisi Karbon dari Kedua Tipe Kendaraan: %.3f Ton CO2/Tahun", emisi);
    return emisi;
}