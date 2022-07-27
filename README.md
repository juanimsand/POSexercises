# POSexercises

El codigo esta desarrollado en C++.
Para ambos ejercicios se utilizo VS Code en un entorno WSL, con un compilador gcc y debugger gdb. El setup de cada proyecto se hizo acorde a como lo indica la documentacion de VS Code para WSL https://code.visualstudio.com/docs/cpp/config-wsl

## Ejercicio 1

### 1er solucion

El objetivo del software es que pida un ingreso de numeros por consola hasta recibir un valor igual a 0.

Luego, hace un chequeo si el input es valido (solo numeros), sino vuelve a pedir el ingreso por consola. Al recibir un ingreso valido, modulariza el input en lineas de 10 caracteres + CRLF. Si el valor ingresado no es multiplo de 10, la ultima linea modularizada se autocompleta con valores 0 hasta llegar a 10 caracteres + CRLF.

Una vez que se haya ingresado el valor igual a 0, se procede a verificar si hay datos validos ingresados previamente, y de ser asi, se solicita el ingreso de un nombre de archivo por consola, tambien se chequea que el nombre no contenga caracteres invalidos, volviendo a pedir un nuevo nombre de archivo en tal caso. Una vez que el nombre de archivo ingresado sea valido, se procede a guardar el mismo, bajo el nombre recientemente ingresado y con el contenido de numeros ingresado previamente. Este proceso de guardado, devuelve si fue exitoso o no el mismo.

Finaliza la ejecucion del programa.

### 2da solucion

El objetivo del software es que pida un ingreso de numeros por consola hasta recibir un valor igual a 0.

Se hace un chequeo si el input es valido (solo numeros), sino vuelve a pedir el ingreso por consola. Al recibir un ingreso valido, toma solo los numeros que precedan al 1er valor igual a 0 (si no hay ceros en el valor ingresado, toma todos los numeros) y modulariza el input en lineas de 10 caracteres + CRLF. Si el valor ingresado no es multiplo de 10, la ultima linea modularizada se autocompleta con valores 0 hasta llegar a 10 caracteres + CRLF.

Una vez que se ingrese por consola un valor valido, se procede a pedir el nombre con el que se guardara un archivo, verificando si tiene caracteres validos para nombrar al mismo, de no ser asi, se vuelve a solicitar un nombre. Finalmente, informa si la accion de guardado fue exitosa o no. Excepcion: si nunca hubo datos validos y se ingresa solo 0, no hace este proceso de pedir el nombre debido a que no hay datos validos para guardar.

Finaliza la ejecucion del programa.

Importante:

ACCEPT_0_AS_INPUT: Conmuta entre solucion 1 y 2 (0 o 1, respectivamente).
DEBUG_OUTPUT: Habilita/Deshabilita debug por consola.

## Ejercicio 2

Se compone de una arquitectura cliente/servidor con comunicacion por socket para resolver una transaccion. Hay 2 programas que realizan las respectivas tareas, pos_client.cpp y pos_server.cpp respectivamente.

El servidor es un simple programa que levanta un socket en un puerto especifico, y espera solicitudes. Una vez que tiene una solicitud (request) desde un cliente devuelve una respuesta (response) con determinado valor "00" o "01" teniendo 0.5 de probabilidad cada uno (uso de funcion rand()).

Desde el cliente, se solicita al usuario el ingreso de un monto (especificando el formato) y se verifica si es valido. De no serlo, vuelve a pedir el monto.

Una vez que el monto es valido, se pide el numero de tarjeta y se verifica su validez en cuanto a longitud y caracteres ingresados (solo digitos, minimo 14). Si no es valido, se vuelve a pedir el ingreso hasta que lo sea. Una vez que es valido, se verifica si la tarjeta es valida en cuanto a su existencia. Para esta segunda etapa se chequea entre datos de 2 archivos. Si es valida, muestra en consola el nombre de la tarjeta, sino el mensaje 'TARJETA NO SOPORTADA' y termina la ejecucion del programa.

A continuacion, si la tarjeta es valida, se solicita el codigo de seguridad (3 digitos) por consola, y se hace un chequeo del formato, volviendo a pedirlo en el caso de que sea erroneo.

Una vez que se tienen todos los datos correctamente, se procede a armar el mensaje para enviar al server solicitando la transaccion seguiendo una trama especifica. Luego, se intenta establecer la comunicacion con el server a traves del socket (es importante tener en cuenta que lo buscara en localhost). Si la comunicacion no es valida, arroja un mensaje de 'ERROR DE COMUNICACION'. Si se establece, se envia el mensaje al servidor y se espera una respuesta por 5 segundos, si no se obtiene tal respuesta, entonces se imprime en consola el mensaje 'ERROR DE COMUNICACION', en caso de obtenerla se la evalua, si es "00" indica que la transaccion fue exitosa y muestra en pantalla 'APROBADA', de lo contrario muestra 'RECHAZADA'.

Finalizan ambos programas, server y cliente.

Importante:

WRITE_DAT_FILES: Macro que permite escribir ambos archivos donde se buscara si la tarjeta es valida o no. (Solo en pos_client.cpp)
DEBUG_OUTPUT: Habilita/Deshabilita debug por consola.
