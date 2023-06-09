#pragma once
#define KIND float  

//Parametros principales:
const size_t N = 10000; //Cantidad de agentes.
const KIND   L = 475; //Largo del sistema. Area = L*L.

//Parametros de evolución del sistema.
//Pre-condición para estos parámetros (dt * v < 0.1):
const KIND active_velocity = 0.05; 
const KIND delta_time      = 0.05;
const KIND sqrt_dt 		   = sqrt(delta_time);

const bool animation = true;
const int  anim_step = 3000;

//0 -> dist. exponencial, 1 -> dist. power law. Cualquier otro dist. uniforme.
const int velocity_distribution = -1;
const KIND delta = 100; // angle of refraction border

//Condición inicial SIR
const KIND   p_init  = 0.001, //Infectadas iniciales.
			 p_rinit = 0.0, //Refractarias iniciales (elegidas al azar).
			 p_dinit = 0.0; //Refractarias iniciales (elegidas a partir de lista de velocidades).

const int spin = 3; //Estados internos.


/*Tiempos característicos y probabilidades por unidad de tiempo*/
const KIND  tau_t = 10 , p_transmision = (1 / tau_t) * delta_time, //sane---->infected
			tau_i = 200, p_infection   = (1 / tau_i) * delta_time, //infected--->refractary
			tau_r = 500, p_recfractary = (1 / tau_r) * delta_time; //refractary--->sane

//Pre-condición para tumbling (dt * 1/alpha < 1) -> ask Gustavo this
const KIND alpha = 100, p_rotation    = (1 / alpha) * delta_time; // tumbling rates angle_1 -> angle_2
const KIND alpha_s = 1, p_rotation_s = (1 / alpha_s) * delta_time; // tumbling rate seconde part angle_1 -> angle_2

// Diff. Noise
const KIND eta = sqrt(2. / alpha);
const KIND eta_s = sqrt(2. / alpha_s);

 /* Para distribución power-law hay que fitear las constantes */
// 1.7713537054317126, //  v=0.1
// 1.8663355897425058, //  v=0.08
// 2.0960715631010567, //  v=0.05
// 2.2303794574079356, //  v=0.04

const KIND  k_powerl = 2.09600,            
			v_min    = 0.01,
            v_max    = 4.;

//Constantes:
const KIND  Pi       = 3.14159265358979323846,
			dos_Pi   = 2 * Pi,
			infinity = 1000000000;

//Características geométricas de agentes.
const KIND  radio    = 1.,
			diameter = 2. * radio;

//Características del medio en el que se mueven los agentes.
const KIND beta           = 1;                    //comentario sobre qué representa.
const KIND gamma_friction = 3.92 * active_velocity; //transforma: vel -> fuerza.
