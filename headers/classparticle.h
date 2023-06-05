#include "parameters.h"    //Módulo con parametros para la simulación.
using namespace std;

//Generador de números aleatorios en (0,1).
mt19937::result_type seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 gen(seed);                             //Standard mersenne_twister_engine seeded time(0)
uniform_real_distribution<KIND> dis(0., 1.); // dis(gen), número aleatorio real entre 0 y 1.
normal_distribution<KIND> norm_dist(0., 1.);

//==================================================================================//
/* Definimos la clase partículas y sus métodos */
/* Memoria que ocupa cada miembro de la clase: */
/* 4 doubles (32 Bytes) + 1 int (4 Bytes) = 36 Bytes*/
//==================================================================================//
class particle{
	private:
		// Estado interno del agente.
		int state;
	public:
		// Estado dinámico del agente.
		KIND x, y;
		KIND velocity;
		KIND angle;

	// Constuctores:
	particle();
	particle(KIND x1, KIND y1, KIND vel, KIND ang);

	// Métodos:
	bool is_healthy()    {return (state <  1);}
	bool is_infected()   {return (state == 1);}
	bool is_refractary() {return (state == 2);}

	int get_state()      {return state;}

	void set_healthy()    {state = 0;}
	void set_infected()   {state = 1;}
	void set_refractary() {state = 2;}
};

/* Constructor de partícula genérica */
particle::particle(){
	velocity = 0;
	angle    = 0;
	x        = 0;
	y        = 0;
}

/*Constructor of a particle in a given phase-state (x,p) of the system */
particle::particle(KIND x1, KIND y1, KIND vel, KIND ang){
	velocity = vel;
	angle    = ang;
	x        = x1;
	y        = y1;
}

//==================================================================================//
particle create_particle(void){
	KIND x, y, velocity, angle;

	x     = dis(gen)*L;
	y     = dis(gen)*L;
	angle = dis(gen)*dos_Pi;

	// Exponential, power law and uniform distributions f(v)
	switch(velocity_distribution){
		case(0):
			velocity = -active_velocity*log(1. - dis(gen));
			break;
		case(1):
			velocity = pow(dis(gen) * (pow(v_max, 1 - k_powerl) - 
				pow(v_min, 1 - k_powerl)) 						+ 
				pow(v_min, 1 - k_powerl), 1. / (1. - k_powerl));
			break;
		default:
			velocity = active_velocity;
			break;
	}
	
	// Creación de la partícula:
	particle A(x, y, velocity, angle);
	
	// Init condition
	if  (((L/5 - 10) < A.x) and (A.x < L/5)) { 
		A.set_infected();
	} else {
		A.set_healthy();
	}
	return A;
}
//==================================================================================//
/* Class Functions */
//==================================================================================//
/*Real boundary condition  and integer boundary condition functions*/
KIND b_condition(KIND a){
    return fmod((fmod(a,L)+L),L);
}

int my_mod(int a, int b){
	return ((a%b)+b)%b;
}

KIND distance(particle A, particle B){
        KIND x1,x2,y1,y2,res;
        res = infinity;
        x2 = B.x; y2 = B.y;
        for(int i=-1;i<2;i++) for(int j=-1;j<2;j++){
            x1 = A.x + i*L;
            y1 = A.y + j*L;
            res = min(res, (KIND)pow((x1-x2),2) + (KIND)pow((y1-y2),2));
        }
        return sqrt(res);
}

KIND distance_x(particle A, particle B){
		KIND x1, x2, res;
		int j = 0;
		vector<KIND> dx;

		dx.resize(3,0);
		res = infinity;
		x2  = B.x;
		for(int i=-1; i<2; i++){
			x1      = A.x + i*L;
			dx[i+1] = x1 - x2;

			if (abs(dx[i+1]) < res ){
				res = abs(dx[i+1]);
				j = i;
			} // if
		} // for
		return dx[j+1];
}


KIND distance_y(particle A, particle B){
		KIND y1, y2, res;
		int j = 0;
		vector<KIND> dy;

		dy.resize(3,0);
		res = infinity;
		y2  = B.y;
		for(int i=-1; i<2; i++){
			y1      = A.y + i*L;
			dy[i+1] = y1 - y2;

			if (abs(dy[i+1]) < res ) {
				res = abs(dy[i+1]);
				j = i;
			} //if
		} // for
		return dy[j+1];
}

KIND distance1(KIND dx, KIND dy){
    return sqrt(pow(dx,2) + pow(dy,2));
}

/* Interact */
bool interact(particle A, particle B){
	return (distance(A,B) < diameter);
} //repensar esta función

//==================================================================================//
/* Time Step Evolution Function */
//==================================================================================//
particle evolution(vector<particle> &system, vector<int> &index, bool inter){
	particle Agent = system[index[0]];
	KIND p_rot=0, eta_rot=0;
	
	/* Dinámica espacial*/	
	if ( (Agent.x >= L/4) and (Agent.y <=  L / delta * (Agent.x - L / 4))) {
		p_rot = p_rotation_s;
		eta_rot = eta_s; 
	} else {
		p_rot=p_rotation;
		eta_rot = eta;
	}

	/* Persistent Random Walk */
	if (dis(gen) < p_rot) Agent.angle += eta_rot *  norm_dist(gen) * sqrt_dt ;
	
	Agent.x = b_condition(Agent.x + Agent.velocity * cos(Agent.angle) * delta_time);
	Agent.y = b_condition(Agent.y + Agent.velocity * sin(Agent.angle) * delta_time);

	// Reflective walls:
	if (Agent.x > L-1 and cos(Agent.angle) > 0) Agent.angle = Pi - Agent.angle ;
	if (Agent.x < 1 and cos(Agent.angle) < 0) Agent.angle = Pi - Agent.angle;

	if (Agent.y > L-1 and sin(Agent.angle) > 0) Agent.angle =  - Agent.angle;
	if (Agent.y < 1 and sin(Agent.angle) < 0) Agent.angle = - Agent.angle;


    /* Epidemic Dynamic - SI*/
	bool flag = true; // Flag de infección.
	for (size_t i=1; i<index.size(); i++) {
		if (Agent.is_healthy() && system[index[i]].is_infected()) {
			if (dis(gen) < p_transmision) {
				Agent.set_infected();
				flag = false; // No puede volverse refractaria en esta instancia de evolución.
			}
		}
	}
    return Agent;
}

