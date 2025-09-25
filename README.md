[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/xVNuoNce)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=20720002)
# Continuación de la Actividad 1b - Vectores Paralelos

## Problema: Sistema Avanzado de Gestión de Inventario para "El Martillo"

#### Definición del Problema

El sistema de inventario de la ferretería "El Martillo" ha superado sus necesidades iniciales. Ahora, la ferretería tiene múltiples sucursales y necesita un sistema capaz de manejar la información de productos de manera persistente. Los datos ya no pueden estar codificados en el programa, sino que deben cargarse desde un archivo al inicio y guardarse al finalizar. . Esto asegura que el inventario se mantenga actualizado entre sesiones y permite una gestión más robusta. El sistema continuará utilizando arreglos estáticos paralelos para mantener la eficiencia y el rendimiento predecible.

El sistema deberá gestionar los siguientes datos para cada producto:

  * Código de Producto: Un número entero único.
  * Nombre del Producto: Una cadena de caracteres que describe el artículo.
  * Cantidad en Stock: El inventario actual.
  * Precio Unitario: El precio de venta por unidad.
  * Ubicación en Almacén: Un código que indica la ubicación física del producto.

Todos estos datos se almacenarán en arreglos estáticos paralelos de tamaño fijo, con un máximo de 100 productos.

---

#### Requisitos Funcionales y No Funcionales

**Requisitos Funcionales**

  * Persistencia de Datos: El programa debe leer los datos de inventario desde un archivo de texto llamado inventario.txt al iniciar. Este archivo utilizará una estructura Código,Nombre,Cantidad,Precio,Ubicación por cada producto.
  * Guardado de Cambios: Antes de salir, el programa debe escribir todos los datos del inventario de vuelta al archivo inventario.txt, sobrescribiendo el contenido anterior. Esto garantiza que cualquier cambio (como actualizaciones de stock) se guarde de forma permanente.
  * Registro de Nuevo Producto: Se permitirá al usuario agregar un nuevo producto al inventario, pidiendo todos los datos necesarios. Se debe validar que el código de producto no exista ya en el inventario.
  * Actualización de Inventario por Ubicación: En lugar de solo por código, el usuario podrá buscar un producto por su ubicación en el almacén y, una vez encontrado, actualizar su cantidad en stock. Si la ubicación no existe, se deberá mostrar un mensaje de error.
  * Generación de Reporte de Bajo Stock: El programa deberá generar un reporte que liste todos los productos cuya cantidad en stock sea inferior a un umbral predefinido (por ejemplo, 10 unidades). Este reporte deberá ser visible en la consola.
  * Búsqueda del Producto Más Barato: El sistema debe identificar y mostrar el nombre y precio del producto con el precio unitario más bajo.

**Requisitos No Funcionales**

  * Tecnología: El programa debe ser desarrollado en lenguaje de programación C++.
  * El tamaño máximo de productos es 100.
  * Manejo de Archivos: La lectura y escritura de archivos debe ser robusta, manejando correctamente los delimitadores de coma y los saltos de línea.
  * Robustez y Validación: El sistema debe manejar entradas de usuario inválidas (letras en campos numéricos, códigos de producto duplicados, ubicaciones no encontradas) sin terminar la ejecución.

---

**Entregables**

Un único archivo .cpp que contenga la implementación completa del programa.

---
**Ejemplo de Salida del Sistema**

```bash
--- Bienvenido al Sistema de Inventario de "El Martillo" ---

Cargando inventario desde 'inventario.txt'...
Inventario cargado exitosamente. 5 productos encontrados.

Seleccione una opción:
1. Consultar un producto
2. Actualizar inventario por ubicación
3. Registrar nuevo producto
4. Generar reporte de bajo stock
5. Encontrar el producto más barato
6. Guardar y salir

Opción seleccionada: 4

--- Reporte de Productos con Bajo Stock ---
Nombre: Taladro inalámbrico, Stock: 8
Nombre: Sierra eléctrica, Stock: 5

--- Menú principal ---

Opción seleccionada: 2

Ingrese la ubicación del producto a actualizar: A-15
Ingrese la cantidad a agregar/quitar (+/-): +12

Cantidad actualizada exitosamente. El nuevo stock de "Martillo de bola" es 62.

--- Menú principal ---

Opción seleccionada: 5

El producto más barato es: Tornillos (caja) con un precio de $2.50.

--- Menú principal ---

Opción seleccionada: 6

Guardando cambios en 'inventario.txt'...
Cambios guardados. Saliendo del sistema.
```