# Akye-Kalkulator-Jejak-Karbon

Kalkulator Jejak Karbon merupakan salah satu upaya untuk menyadarkan seseorang tentang seberapa banyak produksi emisi karbonnya dalam setahun. Dari hasil penghitungan kalkulator jejak karbon, seseorang dapat mengetahui dampak dari setiap kegiatan yang dianggap kecil ternyata dapat berpengaruh besar terhadap perubahan iklim bumi.
  
**Panduan :**  
1. Penamaan 
Variabel dan Function menggunakan Camel Case, sedangkan untuk Branch, contoh: `function/emisi-transportasi`, `bugfix/validasi-input-transportasi`, `improvement/refactor-display` 
2. Komentar untuk hal penting dan dokumentasi siapa yang mengerjakan
3. Output akhir jumlah emisi menggunakan detail `%.3f` dan satuan Ton CO2/tahun
4. Selalu testing sebelum push ke branch ataupun ke main
5. Catat nama dan kegunaan dari variabel, function, array, structs
6. Function, prototype di atas main dan definition di bawah mainnya

**Nama Akun GitHub**  
Akbar Anvasa Faraby = Aksara-AAF  
Muhammad Daffa Rizki = mdaffrzk1  
Rana Aqila Karim = RanaAqilaK  
Zhafarrel Alvarezqi P K =  Zhafarrel  
Ahmad Malik Prasetyo = malik-dk  

**Pembagian Tugas**  
Malik = Transportasi darat dan udara  
Akbar = Makanan, main  
Zhafarrel = Display, tips, dan leaderboard  
Daffa = Peralatan rumah tangga  
Rana = Daya rumah tangga (listrik)   

Ada struct yang berisi nama, total emisi, dan klasifikasi total emisi untuk leaderboard. Klasifikasi total emisi menggunakan Enum.

Awalnya, minta input jumlah berapa orang yang bakalan isi. Lalu, bakalan ada for loop yang ngeloop sebanyak jumlah orangnya. Pada awal tiap loop, minta nama pengisinya untuk dimasukkin ke struct orang. Pada tengah main, terdapat berbagai function yang dipanggil untuk menghitung emisi jejak karbon per kategori. Di akhir loopnya bakalan ada penyimpanan total emisi tiap orang ke struct orang. Tips diberikan pada tiap akhir for loop berdasarkan total emisi (rendah, normal, tinggi). Setelah seluruh for loop selesai, akan dimunculkan leaderboard yang berisikan nama, total emisi, dan klasifikasi total emisi.
