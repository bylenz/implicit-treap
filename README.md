# Implicit Treap — EDA, Proyecto 3

BST implícito basado en treap aleatorizado con propagación lazy. Soporta
operaciones de rango (suma, reversión, rotación, mínimo) e inserción/eliminación
por posición. Todas las operaciones corren en **O(log n) esperado**.

---

## Conceptos clave

### ¿Qué es un Treap?

Un treap es un árbol binario de búsqueda (BST) que a cada nodo le asigna una
**prioridad aleatoria** y mantiene la propiedad de heap respecto a ella. La
aleatoriedad garantiza que el árbol quede balanceado con alta probabilidad,
dando O(log n) en todas las operaciones.

### ¿Qué es "implícito"?

En un BST normal la clave de cada nodo es el valor almacenado. En un treap
implícito la **clave es la posición en el arreglo** — y no se guarda
explícitamente. Se deduce en tiempo real contando los nodos del subárbol
izquierdo (`size`). Esto permite usar el árbol como un **arreglo dinámico**
donde cualquier subrango se puede aislar, transformar y reensamblar en O(log n).

### Operaciones primitivas: split y merge

Todo lo demás se construye sobre dos operaciones:

- **`split(root, k)`** — divide el árbol en dos: los primeros `k` elementos y
  el resto. O(log n).
- **`merge(left, right)`** — une dos treaps manteniendo la propiedad de heap.
  O(log n).

Una operación de rango `[l, r]` siempre sigue el mismo patrón:

```
split en l-1  →  split en (r-l+1)  →  operar sobre el segmento medio  →  merge
```

### Lazy propagation

En lugar de bajar a cada nodo del rango, se guarda la operación pendiente en
la raíz del segmento y se propaga (`push_down`) solo cuando se necesita
descender. Dos lazies coexisten en cada nodo:

| Campo        | Propósito                                      |
|--------------|------------------------------------------------|
| `lazy_add`   | Suma acumulada pendiente de propagar a hijos   |
| `lazy_rev`   | Indica si el subárbol está pendiente de invertir |

### Pool de nodos (arena allocator)

Los nodos no se alojan en el heap con `new`. Todos viven en un `std::vector<Node>`
llamado `pool`. Los punteros son **índices enteros** (`int`), no punteros reales.
Ventajas: cache-friendly, sin fragmentación, sin overhead de `malloc`.

---

## Operaciones soportadas

| Código | Firma                          | Descripción                                      |
|--------|--------------------------------|--------------------------------------------------|
| `A`    | `A l r delta`                  | Suma `delta` a todos los elementos en `[l, r]`   |
| `R`    | `R l r`                        | Revierte el subarreglo `[l, r]`                  |
| `O`    | `O l r k`                      | Rota `k` posiciones a la derecha el rango `[l, r]`|
| `I`    | `I pos val`                    | Inserta `val` en la posición `pos` (1-indexed)   |
| `E`    | `E pos`                        | Elimina el elemento en la posición `pos`         |
| `M`    | `M l r`  → imprime el mínimo   | Mínimo del subarreglo `[l, r]`                   |

---

## Build

### 1. Configurar

```bash
cmake -B build                                    # release (O2, NDEBUG)
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug     # debug (ASan + UBSan)
```

### 2. Dar permisos a los scripts (solo la primera vez)

```bash
chmod +x tests/stress.sh tests/bench.sh
```

### 3. Compilar y usar

Todos los targets se invocan con `make -C <directorio>`:

| Comando                      | Qué hace                                                    |
|------------------------------|-------------------------------------------------------------|
| `make -C build`              | Compila el ejecutable `build/main` en modo release          |
| `make -C build-debug`        | Compila con AddressSanitizer y UndefinedBehaviorSanitizer   |
| `make -C build run`          | Compila y ejecuta con `tests/sample.in`                     |
| `make -C build test_sample`  | Compila, ejecuta y compara salida contra `tests/sample.out` |
| `make -C build stress`       | Stress test: 200 casos aleatorios contra solución bruta     |
| `make -C build bench`        | Benchmark con n=q=500 000                                   |

Para cambiar el número de iteraciones del stress test:

```bash
# Editar directamente el target o pasarlo al script manualmente
./tests/stress.sh 500
```

---

## Formato de entrada

```
N Q
a1 a2 ... aN
<operación 1>
<operación 2>
...
```

- `N`: cantidad inicial de elementos  
- `Q`: cantidad de operaciones  
- Índices **1-based**

---

## Estructura del proyecto

```
.
├── include/
│   └── treap.hpp        interfaz pública (Node, namespace treap)
├── src/
│   ├── treap.cpp        split, merge, lazy, operaciones de rango
│   └── main.cpp         I/O, parsing, dispatcher
├── tests/
│   ├── brute.cpp        solución O(n) por operación para validar
│   ├── gen.cpp          generador de casos pequeños aleatorios
│   ├── gen_big.cpp      generador de casos grandes (benchmark)
│   ├── stress.sh        compara main vs brute en N seeds
│   ├── bench.sh         mide tiempo con entrada de 500k operaciones
│   └── sample.in / .out caso de ejemplo del enunciado
├── report/
│   └── report.tex       reporte de implementación
└── CMakeLists.txt
```
