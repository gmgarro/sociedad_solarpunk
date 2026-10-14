# Sociedad Solarpunk

Proyecto desarrollado por estudiantes de primer año de la **Escuela de Ingeniería en Computación del Tecnológico de Costa Rica**, durante el II semestre del año 2026, para el curso de Estructuras de Datos.

## Autores

- **Gabriel Montero Garro**
- **Esteban Solano Orozco**

**Profesor:** Aurelio Sanabria Rodríguez

## Descripción del proyecto

Sociedad Solarpunk es un proyecto desarrollado en lenguaje C que simula el funcionamiento de una sociedad formada por diferentes comunas. Cada comuna cuenta con una población y existencias de bienes y servicios que pueden variar a lo largo de la simulación.

El programa permite administrar las existencias de las comunas, realizar trueques de bienes y servicios entre ellas y simular el consumo diario de los recursos disponibles. Además, contempla la generación de emergencias que provocan pérdidas de bienes y servicios y afectan el bienestar de la comuna donde ocurren y de algunas comunas cercanas.

Durante la simulación se actualizan los índices de necesidad y satisfacción de las comunas, lo que permite observar cómo cambian sus condiciones a lo largo del tiempo. Al finalizar, se presenta un resumen del estado de la sociedad.

El proyecto permite poner en práctica conceptos de programación en C, estructuras de datos, manejo de memoria y organización del código en diferentes archivos.

## Tecnologías utilizadas

- Lenguaje de programación C.
- Biblioteca estándar de C.
- Estructuras de datos dinámicas.
- Archivos para cargar los datos iniciales de la sociedad.
- Terminal para compilar y ejecutar el programa.

## Requisitos

Para compilar y ejecutar el proyecto se necesita:

- Un compilador de C, como GCC.
- Una terminal compatible con los comandos de compilación.
- Los archivos del proyecto y los datos iniciales incluidos en el repositorio.

## Instalación y ejecución

### 1. Clonar el repositorio

```bash
git clone https://github.com/gmgarro/sociedad_solarpunk.git
```

### 2. Entrar en la carpeta del proyecto

```bash
cd sociedad_solarpunk
```

### 3. Compilar el programa

Desde la carpeta principal del proyecto, ejecutar:

```bash
gcc -Wall -Wextra -std=c11 main.c listas.c archivos.c sociedad.c consumo.c emergencias.c economia.c -o programa
```

### 4. Ejecutar el programa

```bash
./programa
```

**Importante:** los archivos y las carpetas con los datos iniciales deben conservar sus rutas para que el programa pueda cargar correctamente la información de la sociedad.

## Repositorio

El código fuente y los archivos del proyecto se encuentran disponibles en GitHub:

[https://github.com/gmgarro/sociedad_solarpunk.git](https://github.com/gmgarro/sociedad_solarpunk.git)

## Propósito académico

Este proyecto fue realizado como parte del curso de Estructuras de Datos, impartido por el profesor **Aurelio Sanabria Rodríguez**, en la Escuela de Ingeniería en Computación del Tecnológico de Costa Rica, durante el II semestre de 2026.

Su desarrollo tiene como propósito aplicar los conocimientos adquiridos durante el curso mediante la implementación de una simulación de una sociedad, utilizando el lenguaje C y estructuras de datos.

---

**Tecnológico de Costa Rica**  
**Escuela de Ingeniería en Computación**  
**II semestre, 2026**
