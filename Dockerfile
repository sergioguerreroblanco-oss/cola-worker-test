# Imagen base mínima y estable
FROM ubuntu:22.04

# Evita prompts interactivos en apt
ENV DEBIAN_FRONTEND=noninteractive

# Instala toolchain de C++ y CMake
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
 && rm -rf /var/lib/apt/lists/*

# Copiamos el código al contenedor
WORKDIR /app
COPY . .

# Compilación fuera de árbol (build/) en modo Release
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build -j

# Muestra los fallos de test directamente en la salida
ENV CTEST_OUTPUT_ON_FAILURE=1

# Por defecto, al ejecutar el contenedor, lanza los tests
CMD ["ctest", "--test-dir", "build"]
