#include <stdio.h>
#include <math.h>

// Mendefinisikan konstanta yang digunakan dalam perhitungan
#define PI 3.14159265358979323846

// Prototipe fungsi
double force_distribution(double z);
double moment_function(double z);
double simpsons_1_3_rule(double (*func)(double), double a, double b, int n);
void calculate_reaction_forces(double F, double d, double *V, double *H, double *T);

// Fungsi utama program
int main()
{
    // Inisialisasi parameter dari studi kasus
    double a = 0.0;  // Batas bawah integrasi (dasar tiang)
    double b = 30.0; // Batas atas integrasi (puncak tiang)
    int n = 10;      // Jumlah segmen untuk integrasi

    printf("Program Analisis Gaya pada Tiang Layar Kapal Balap\n");
    printf("====================================================\n");
    printf("Menggunakan Aturan Simpson 1/3 dengan n = %d segmen.\n\n", n);

    // 1. Menghitung Gaya Total (F)
    double total_force = simpsons_1_3_rule(force_distribution, a, b, n);
    printf("Hasil Perhitungan Numerik:\n");
    printf("---------------------------\n");
    printf("Gaya Total Akibat Angin (F) : %.4f lb\n", total_force);

    // 2. Menghitung Momen Total (M0)
    double total_moment = simpsons_1_3_rule(moment_function, a, b, n);
    printf("Momen Total pada Dasar Dek (M0): %.4f lb-ft\n", total_moment);

    // 3. Menghitung Titik Aplikasi Gaya Efektif (d)
    double effective_distance = total_moment / total_force;
    printf("Titik Aplikasi Gaya (d)       : %.4f ft di atas dek\n\n", effective_distance);

    // 4. Menghitung Gaya-gaya Rekayasa
    double V, H, T;
    calculate_reaction_forces(total_force, effective_distance, &V, &H, &T);

    printf("Analisis Gaya pada Struktur Tiang:\n");
    printf("----------------------------------\n");
    printf("Reaksi Vertikal di Dasar (V) : %.2f lb\n", V);
    printf("Reaksi Horizontal di Dasar (H): %.2f lb\n", H);
    printf("Tegangan pada Kabel (T)      : %.2f lb\n", T);

    printf("\nPerhitungan Selesai.\n");

    return 0;
}

double force_distribution(double z)
{
    return 200.0 * (z / (5.0 + z)) * exp(-2.0 * z / 30.0);
}
double moment_function(double z)
{
    return z * force_distribution(z);
}
double simpsons_1_3_rule(double (*func)(double), double a, double b, int n)
{
    // Memastikan jumlah segmen adalah genap
    if (n % 2 != 0)
    {
        printf("Error: Jumlah segmen (n) harus genap untuk Aturan Simpson 1/3.\n");
        return 0.0;
    }

    double h = (b - a) / n;
    double sum = func(a) + func(b);

    // Loop untuk titik dengan indeks ganjil (dikalikan 4)
    for (int i = 1; i < n; i += 2)
    {
        sum += 4 * func(a + i * h);
    }

    // Loop untuk titik dengan indeks genap (dikalikan 2)
    for (int i = 2; i < n - 1; i += 2)
    {
        sum += 2 * func(a + i * h);
    }

    return (h / 3.0) * sum;
}
void calculate_reaction_forces(double F, double d, double *V, double *H, double *T)
{
    // Sudut kabel penyangga (berdasarkan diagram benda bebas)
    double theta = atan(3.0 / 30.0);

    // Momen pada dasar tiang (O)
    // Sigma M_O = 0 = 3*V - F*d
    *V = (F * d) / 3.0;

    // Keseimbangan gaya vertikal
    // Sigma F_V = 0 = V - T*cos(theta)
    *T = *V / cos(theta);

    // Keseimbangan gaya horizontal
    // Sigma F_H = 0 = F - T*sin(theta) - H
    *H = F - *T * sin(theta);
}