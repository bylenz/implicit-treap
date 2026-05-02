# Implicit Treap — Estructuras de Datos Avanzadas, Proyecto 3

BST implícito basado en treap aleatorizado, con soporte para operaciones
de rango (suma, reversión, rotación, mínimo) e inserción/eliminación por
posición. Todas las operaciones en O(log n) esperado.

## Build

    make            # release
    make debug      # con sanitizers
    make test       # corre el ejemplo del enunciado y compara

## Estructura

- `include/treap.hpp` — interfaz
- `src/treap.cpp`     — implementación (split, merge, lazy)
- `src/main.cpp`      — I/O y dispatcher de operaciones
- `tests/`            — casos de prueba
- `report/`           — reporte LaTeX de 2-3 páginas
