#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip=config_get_string_value(config, "IP" );
	puerto= config_get_string_value(config, "PUERTO");
	valor= config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, "El valor de la clave es: %s", valor);
	log_info(logger, "El valor del puerto es: %s", puerto);
	log_info(logger, "El valor IP es: %s", ip);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger= log_create("tp0.log", "LOGGER_INFO", 1, LOG_LEVEL_INFO);
	if (nuevo_logger==NULL) perror("No se pudo crear el logger");
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config= config_create("cliente.config");
	if(nuevo_config==NULL) perror("No se pudo instanciar el Config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, ">> %s", leido);
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "")!=0) {//siempre tiene el "" es en realidad el caracter vacio /0
		free(leido);//el free va al inicio porque borras el caracter anterior y no lo eliminas antes de volver al bucle debido a que necesitas ese espacio en memoria para la comparacion. 
		leido = readline("> ");
		log_info(logger, ">> %s", leido);

	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}
//todos los puertos que uses que sean del 4000 al 4500, evitar los numeros redondos
//una vez que se termina de usar un socket, hay que liberarlo con close
void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete=crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	agregar_a_paquete(paquete,leido, strlen(leido)+1);
	while (strcmp(leido, "")!=0) {//siempre tiene el "" es en realidad el caracter vacio /0
		free(leido);//el free va al inicio porque borras el caracter anterior y no lo eliminas antes de volver al bucle debido a que necesitas ese espacio en memoria para la comparacion. 
		leido = readline("> ");
		agregar_a_paquete(paquete,leido, strlen(leido)+1);
	}
	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
}
