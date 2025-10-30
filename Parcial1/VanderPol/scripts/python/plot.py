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

    # 1. Gráfica resumen
    print("   Generando gráfica resumen...")
    plt.figure(figsize=(12, 10))
    
    # Posición vs tiempo
    plt.subplot(3, 2, 1)
    plt.plot(t, x1, label='x1', alpha=0.8, linewidth=1)
    plt.plot(t, x2, label='x2', alpha=0.8, linewidth=1)
    plt.xlabel('Tiempo')
    plt.ylabel('Posición')
    plt.title('Posición vs Tiempo')
    plt.legend()
    plt.grid(True, alpha=0.3)

    # Velocidad vs tiempo
    plt.subplot(3, 2, 2)
    plt.plot(t, v1, label='v1', alpha=0.8, linewidth=1)
    plt.plot(t, v2, label='v2', alpha=0.8, linewidth=1)
    plt.xlabel('Tiempo')
    plt.ylabel('Velocidad')
    plt.title('Velocidad vs Tiempo')
    plt.legend()
    plt.grid(True, alpha=0.3)

    # Lissajous
    plt.subplot(3, 2, 3)
    plt.plot(x1, x2, 'b-', alpha=0.7, linewidth=0.8)
    plt.xlabel('x1')
    plt.ylabel('x2')
    plt.title('Figura de Lissajous')
    plt.grid(True, alpha=0.3)

    # Espacio de fase 1
    plt.subplot(3, 2, 4)
    plt.plot(x1, v1, 'g-', alpha=0.7, linewidth=0.6)
    plt.xlabel('x1')
    plt.ylabel('v1')
    plt.title('Espacio de Fase (Oscilador 1)')
    plt.grid(True, alpha=0.3)

    # Espacio de fase 2
    plt.subplot(3, 2, 5)
    plt.plot(x2, v2, 'r-', alpha=0.7, linewidth=0.6)
    plt.xlabel('x2')
    plt.ylabel('v2')
    plt.title('Espacio de Fase (Oscilador 2)')
    plt.grid(True, alpha=0.3)

    # Diferencia de fases
    phase1 = np.arctan2(v1, x1)
    phase2 = np.arctan2(v2, x2)
    phase_diff = np.abs(phase1 - phase2)
    plt.subplot(3, 2, 6)
    plt.plot(t, phase_diff, 'purple', alpha=0.8)
    plt.xlabel('Tiempo')
    plt.ylabel('Diferencia de Fase')
    plt.title('Diferencia de Fase entre Osciladores')
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(RESULTS_DIR / 'summary_plot.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 2. Gráfica detallada de Lissajous
    print("   Generando gráfica detallada de Lissajous...")
    plt.figure(figsize=(10, 8))
    plt.plot(x1, x2, 'b-', alpha=0.8, linewidth=1)
    plt.xlabel('x1')
    plt.ylabel('x2')
    plt.title('Figura de Lissajous - Osciladores de Van der Pol Acoplados')
    plt.grid(True, alpha=0.3)
    plt.savefig(RESULTS_DIR / 'lissajous_detailed.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 3. Espectro de potencia
    print("   Generando espectro de potencia...")
    from numpy.fft import rfft, rfftfreq
    dt_sampling = t[1] - t[0]
    yf1 = rfft(x1 - np.mean(x1))
    yf2 = rfft(x2 - np.mean(x2))
    xf = rfftfreq(len(t), dt_sampling)
    
    plt.figure(figsize=(10, 5))
    plt.semilogy(xf, np.abs(yf1), label='x1', alpha=0.8)
    plt.semilogy(xf, np.abs(yf2), label='x2', alpha=0.8)
    plt.xlabel('Frecuencia [Hz]')
    plt.ylabel('Amplitud')
    plt.title('Espectro de Potencia')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig(RESULTS_DIR / 'power_spectrum.png', dpi=150, bbox_inches='tight')
    plt.close()

    # 4. Análisis de Poincaré (si existe)
    poincare_file = RESULTS_DIR / "poincare.txt"
    if poincare_file.exists():
        print("   Analizando mapa de Poincaré...")
        try:
            pdata = np.loadtxt(poincare_file)
            if pdata.size > 0:
                if pdata.ndim == 1:
                    pdata = pdata.reshape(1, -1)
                
                plt.figure(figsize=(12, 5))
                
                plt.subplot(1, 2, 1)
                plt.scatter(pdata[:, 3], pdata[:, 4], s=20, alpha=0.6, c='red')
                plt.xlabel('x2')
                plt.ylabel('v2')
                plt.title('Mapa de Poincaré (x1=0, v1>0)\nx2 vs v2')
                plt.grid(True, alpha=0.3)
                
                plt.subplot(1, 2, 2)
                if len(pdata) > 10:
                    n_bins = min(30, len(pdata) // 5)
                    plt.hist(pdata[:, 3], bins=n_bins, alpha=0.7, color='blue', density=True)
                    plt.xlabel('x2')
                    plt.ylabel('Densidad')
                    plt.title('Distribución de x2 en Poincaré')
                    plt.grid(True, alpha=0.3)
                
                plt.tight_layout()
                plt.savefig(RESULTS_DIR / 'poincare_analysis.png', dpi=150, bbox_inches='tight')
                plt.close()
        except Exception as e:
            print(f"   ⚠️  Error procesando Poincaré: {e}")

    # 5. Análisis de Lyapunov (si existe)
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
                
                plt.savefig(RESULTS_DIR / 'lyapunov_evolution.png', dpi=150, bbox_inches='tight')
                plt.close()
        except Exception as e:
            print(f"   ⚠️  Error procesando Lyapunov: {e}")

    # 6. Leer y mostrar resultado final de Lyapunov
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

    # 7. Generar GIF si se solicita
    if '--gif' in sys.argv:
        print("   🎬 Generando GIF animado...")
        animate_script = SCRIPTS_DIR / "animate_lissajous.py"
        if animate_script.exists():
            try:
                subprocess.run([
                    'python3', str(animate_script), 
                    str(data_file),
                    str(RESULTS_DIR / "lissajous_evolution.gif")
                ], check=True, cwd=PROJECT_ROOT)
                print("   ✓ GIF creado exitosamente")
            except subprocess.CalledProcessError as e:
                print(f"   ❌ Error generando GIF: {e}")
            except FileNotFoundError:
                print("   ❌ Python3 no encontrado")
        else:
            print(f"   ❌ Script de animación no encontrado: {animate_script}")

    print(f"\n✅ Análisis completado!")
    print(f"   Gráficas guardadas en: {RESULTS_DIR}")
    
    # Mostrar archivos generados
    generated_files = list(RESULTS_DIR.glob("*.png"))
    if (RESULTS_DIR / "lissajous_evolution.gif").exists():
        generated_files.append(RESULTS_DIR / "lissajous_evolution.gif")
    
    if generated_files:
        print(f"\n   Archivos generados:")
        for f in generated_files:
            print(f"   - {f.name}")

    return 0

if __name__ == "__main__":
    sys.exit(main())
