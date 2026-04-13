#include "client.h"

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
	
	logger = iniciar_logger(); // Usando el logger creado previamente --> en iniciar_logger
	log_info(logger, "Hola! Soy un log");



	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config(); //SE crean un config en iniciar_config

	//Lectura de los valores del config y los dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "Lei la IP: %s, Puerto: %s y Valor: %s", ip, puerto, valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

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
	t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", 1, LOG_LEVEL_INFO);
	
	if(nuevo_logger == NULL){
		perror("No se pudo crear el logger");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	
	if(nuevo_config == NULL){ //importante chequear los valores de retorno
		perror("No se pudo leer el config");
		exit(2);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	//TRUCO: usar un while que siga pidiendo texto hasta que el usuario mande un enter vacío

	while(1) {
		leido = readline("> ");
		
		// Si el string está vacío (solo apretó Enter), cortamos
		if (strcmp(leido, "") == 0) {
			free(leido);
			break;
		}

		log_info(logger, "Leido de consola: %s", leido);
		free(leido); // ¡Liberamos la memoria de la línea leída!!!!!!!!!!!!!!!!!!!!
	}
}

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!

void paquete(int conexion)
{
	char* leido;
	t_paquete* p = crear_paquete(); // Función de utils.c o commons

	while(1) {
		leido = readline("> ");
		if (strcmp(leido, "") == 0) {
			free(leido);
			break;
		}
		// Agregamos el string al paquete indicando su tamaño
		agregar_a_paquete(p, leido, strlen(leido) + 1);
		free(leido);
	}

	enviar_paquete(p, conexion);
	eliminar_paquete(p); // ¡Liberamos el paquete!
}

void terminar_programa(int conexion, t_log* logger, t_config* config) // IMPORTANTE: hay que liberar lo que utilizamos (conexion, log y config) 
{
	log_destroy(logger);

	config_destroy(config);

	liberar_conexion(conexion);

	
}
