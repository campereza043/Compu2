#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import subprocess
from pathlib import Path

# Configuración de rutas
PROJECT_ROOT = Path(__file__).parent.parent.parent
RESULTS_DIR = PROJECT_ROOT / "results"
SCRIPTS_DIR = PROJECT_ROOT / "scripts" / "python"

# Asegurar que existe el directorio de resultados
RESULTS_DIR.mkdir(exist_ok=True)

def main():
    # Obtener archivo de datos
    if len(sys.argv) > 1:
        data_file = Path(sys.argv[1])
    else:
        data_file = RESULTS_DIR / "datos.txt"

    if not data_file.exists():
        print(f"❌ Error: Archivo no encontrado: {data_file}")
        print("   Ejecute primero el programa C++ para generar los datos.")
        return 1

    print(f"📊 Procesando: {data_file}")

    try:
        data = np.loadtxt(data_file)
        if data.size == 0:
            print("❌ Error: Archivo vacío")
            return 1
    except Exception as e:
        print(f"❌ Error leyendo archivo: {e}")
        return 1

    t, x1, v1, x2, v2 = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4]
    
    print(f"   {len(t)} puntos de datos procesados")

    # [Todas las gráficas estáticas anteriores se mantienen igual...]
    # 1. Gráfica de posición vs tiempo (x1 y x2)
    print("   Generando gráfica de posición vs tiempo...")
    plt.figure(figsize=(10, 6))
    plt.plot(t, x1, label='x1', alpha=0.8, linewidth=1)
    plt.plot(t, x2, label='x2', alpha=0.8, linewidth=1)
    plt.xlabel('Tiempo')
    plt.ylabel('Posición')
    plt.title('Posición vs Tiempo')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'position_vs_time.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 2. Gráfica de velocidad vs tiempo (v1 y v2)
    print("   Generando gráfica de velocidad vs tiempo...")
    plt.figure(figsize=(10, 6))
    plt.plot(t, v1, label='v1', alpha=0.8, linewidth=1)
    plt.plot(t, v2, label='v2', alpha=0.8, linewidth=1)
    plt.xlabel('Tiempo')
    plt.ylabel('Velocidad')
    plt.title('Velocidad vs Tiempo')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'velocity_vs_time.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 3. Gráfica de Lissajous (x1 vs x2)
    print("   Generando gráfica de Lissajous...")
    plt.figure(figsize=(8, 8))
    plt.plot(x1, x2, 'b-', alpha=0.7, linewidth=0.8)
    plt.xlabel('x1')
    plt.ylabel('x2')
    plt.title('Figura de Lissajous: x1 vs x2')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'lissajous.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 4. Espacio de fase para el oscilador 1 (x1 vs v1)
    print("   Generando espacio de fase para el oscilador 1...")
    plt.figure(figsize=(8, 6))
    plt.plot(x1, v1, 'g-', alpha=0.7, linewidth=0.6)
    plt.xlabel('x1')
    plt.ylabel('v1')
    plt.title('Espacio de Fase (Oscilador 1)')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'phase_space_osc1.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 5. Espacio de fase para el oscilador 2 (x2 vs v2)
    print("   Generando espacio de fase para el oscilador 2...")
    plt.figure(figsize=(8, 6))
    plt.plot(x2, v2, 'r-', alpha=0.7, linewidth=0.6)
    plt.xlabel('x2')
    plt.ylabel('v2')
    plt.title('Espacio de Fase (Oscilador 2)')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'phase_space_osc2.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 6. Diferencia de fases
    print("   Generando gráfica de diferencia de fases...")
    phase1 = np.arctan2(v1, x1)
    phase2 = np.arctan2(v2, x2)
    phase_diff = np.abs(phase1 - phase2)
    plt.figure(figsize=(10, 6))
    plt.plot(t, phase_diff, 'purple', alpha=0.8)
    plt.xlabel('Tiempo')
    plt.ylabel('Diferencia de Fase')
    plt.title('Diferencia de Fase entre Osciladores')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'phase_difference.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 7. Espectro de potencia para x1 y x2
    print("   Generando espectro de potencia...")
    from numpy.fft import rfft, rfftfreq
    dt_sampling = t[1] - t[0]
    yf1 = rfft(x1 - np.mean(x1))
    yf2 = rfft(x2 - np.mean(x2))
    xf = rfftfreq(len(t), dt_sampling)
    
    plt.figure(figsize=(10, 6))
    plt.semilogy(xf, np.abs(yf1), label='x1', alpha=0.8)
    plt.semilogy(xf, np.abs(yf2), label='x2', alpha=0.8)
    plt.xlabel('Frecuencia [Hz]')
    plt.ylabel('Amplitud')
    plt.title('Espectro de Potencia')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'power_spectrum.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 8. Análisis de Poincaré (si existe)
    poincare_file = RESULTS_DIR / "poincare.txt"
    if poincare_file.exists():
        print("   Analizando mapa de Poincaré...")
        try:
            pdata = np.loadtxt(poincare_file)
            if pdata.size > 0:
                if pdata.ndim == 1:
                    pdata = pdata.reshape(1, -1)
                
                # Gráfica de Poincaré: x2 vs v2
                plt.figure(figsize=(8, 8))
                plt.scatter(pdata[:, 3], pdata[:, 4], s=20, alpha=0.6, c='red')
                plt.xlabel('x2')
                plt.ylabel('v2')
                plt.title('Mapa de Poincaré (x1=0, v1>0)\nx2 vs v2')
                plt.grid(True, alpha=0.3)
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'poincare_x2_v2.png', dpi=150, bbox_inches='tight')
                plt.close()

                # Gráfica de Poincaré: x1 vs v1 (deberían estar en x1=0)
                plt.figure(figsize=(8, 8))
                plt.scatter(pdata[:, 1], pdata[:, 2], s=20, alpha=0.6, c='blue')
                plt.xlabel('x1')
                plt.ylabel('v1')
                plt.title('Mapa de Poincaré: x1 vs v1 (sección)')
                plt.grid(True, alpha=0.3)
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'poincare_x1_v1.png', dpi=150, bbox_inches='tight')
                plt.close()

                # Gráfica de Poincaré: t vs x2
                plt.figure(figsize=(10, 6))
                plt.scatter(pdata[:, 0], pdata[:, 3], s=20, alpha=0.6, c='green')
                plt.xlabel('Tiempo')
                plt.ylabel('x2')
                plt.title('Mapa de Poincaré: Tiempo vs x2')
                plt.grid(True, alpha=0.3)
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'poincare_t_vs_x2.png', dpi=150, bbox_inches='tight')
                plt.close()

                # Histograma de x2 en Poincaré
                if len(pdata) > 10:
                    n_bins = min(30, len(pdata) // 5)
                    plt.figure(figsize=(8, 6))
                    plt.hist(pdata[:, 3], bins=n_bins, alpha=0.7, color='blue', density=True)
                    plt.xlabel('x2')
                    plt.ylabel('Densidad')
                    plt.title('Distribución de x2 en Poincaré')
                    plt.grid(True, alpha=0.3)
                    plt.tight_layout()
                    plt.savefig(RESULTS_DIR / 'poincare_histogram.png', dpi=150, bbox_inches='tight')
                    plt.close()
        except Exception as e:
            print(f"   ⚠️  Error procesando Poincaré: {e}")

    # 9. Análisis de Lyapunov (si existe)
    lyap_prog_file = RESULTS_DIR / "lyapunov_progress.txt"
    lyap_final_file = RESULTS_DIR / "lyapunov_final.txt"
    
    if lyap_prog_file.exists():
        print("   Analizando exponentes de Lyapunov...")
        try:
            lyap_data = np.loadtxt(lyap_prog_file)
            if lyap_data.size > 0:
                if lyap_data.ndim == 1:
                    lyap_data = lyap_data.reshape(1, -1)
                
                plt.figure(figsize=(10, 6))
                plt.plot(lyap_data[:, 0], lyap_data[:, 1], 'r-', linewidth=2)
                plt.xlabel('Tiempo')
                plt.ylabel('λ (Exponente de Lyapunov)')
                plt.title('Evolución del Mayor Exponente de Lyapunov')
                plt.grid(True, alpha=0.3)
                
                # Mostrar valor final
                if len(lyap_data) > 10:
                    final_val = np.mean(lyap_data[-10:, 1])
                    plt.axhline(y=final_val, color='blue', linestyle='--', 
                               label=f'λ final ≈ {final_val:.4f}')
                    plt.legend()
                
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'lyapunov_evolution.png', dpi=150, bbox_inches='tight')
                plt.close()
        except Exception as e:
            print(f"   ⚠️  Error procesando Lyapunov: {e}")

    # 10. Leer y mostrar resultado final de Lyapunov
    if lyap_final_file.exists():
        try:
            with open(lyap_final_file, 'r') as f:
                lines = [L.strip() for L in f if L.strip() and not L.startswith('#')]
            
            if lines:
                lyap_final = float(lines[-1])
                print(f"\n📈 RESULTADO LYAPUNOV:")
                print(f"   Exponente de Lyapunov final: {lyap_final:.6f}")
                if lyap_final > 0.001:
                    print("   🌀 SISTEMA CAÓTICO (λ > 0)")
                elif lyap_final < -0.001:
                    print("   🔒 SISTEMA ESTABLE (λ < 0)")
                else:
                    print("   ⚖️  SISTEMA EN BORDE DE ESTABILIDAD (λ ≈ 0)")
        except Exception as e:
            print(f"   ⚠️  Error leyendo Lyapunov final: {e}")

    # 11. Validación de dt (si existe)
    dt_validation_file = RESULTS_DIR / "dt_validation.txt"
    if dt_validation_file.exists():
        print("   Generando gráfica de validación de dt...")
        try:
            dt_data = np.loadtxt(dt_validation_file, comments='#')
            if dt_data.size > 0:
                if dt_data.ndim == 1:
                    dt_data = dt_data.reshape(1, -1)
                
                # Gráfica de error vs dt
                plt.figure(figsize=(10, 6))
                dt_values = dt_data[:-2, 0]  # Los primeros valores son dt
                errors_x1 = dt_data[:-2, 1]  # Errores en x1
                errors_x2 = dt_data[:-2, 2]  # Errores en x2
                
                plt.loglog(dt_values, errors_x1, 'o-', label='Error x1', linewidth=2)
                plt.loglog(dt_values, errors_x2, 's-', label='Error x2', linewidth=2)
                plt.xlabel('Paso de tiempo (dt)')
                plt.ylabel('Error absoluto')
                plt.title('Convergencia del Paso de Tiempo')
                plt.legend()
                plt.grid(True, alpha=0.3, which='both')
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'dt_convergence.png', dpi=150, bbox_inches='tight')
                plt.close()
        except Exception as e:
            print(f"   ⚠️  Error procesando validación de dt: {e}")

    # 12. Generar GIFs si se solicita
    if '--gif' in sys.argv:
        print("   🎬 Generando GIFs animados...")
        animate_script = SCRIPTS_DIR / "animate_phase_spaces.py"
        if animate_script.exists():
            try:
                subprocess.run([
                    'python3', str(animate_script), 
                    str(data_file),
                    str(RESULTS_DIR)
                ], check=True, cwd=PROJECT_ROOT)
                print("   ✓ GIFs creados exitosamente")
            except subprocess.CalledProcessError as e:
                print(f"   ❌ Error generando GIFs: {e}")
            except FileNotFoundError:
                print("   ❌ Python3 no encontrado")
        else:
            print(f"   ❌ Script de animación no encontrado: {animate_script}")

    print(f"\n✅ Análisis completado!")
    print(f"   Gráficas guardadas en: {RESULTS_DIR}")
    
    # Mostrar archivos generados
    generated_files = list(RESULTS_DIR.glob("*.*"))
    image_files = [f for f in generated_files if f.suffix.lower() in ['.png', '.gif']]
    
    if image_files:
        print(f"\n   Archivos generados:")
        for f in sorted(image_files):
            size_mb = f.stat().st_size / (1024 * 1024)
            print(f"   - {f.name} ({size_mb:.2f} MB)")

    return 0

if __name__ == "__main__":
    sys.exit(main())