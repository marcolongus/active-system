particle evolution(const vector<particle>& system, const vector<int>& index, bool inter) {
    const particle& Agent = system[index[0]];

    if (inter) {
        vector<KIND> potencial(2, 0.0);

        for (size_t i = 1; i < index.size(); i++) {
            const particle& other = system[index[i]];
            const KIND dx_0i = distance_x(Agent, other);
            const KIND dy_0i = distance_y(Agent, other);
            const KIND d_0i = distance1(dx_0i, dy_0i);
            const KIND inv_d3 = 1.0 / (d_0i * d_0i * d_0i);
            potencial[0] += inv_d3 * dx_0i;
            potencial[1] += inv_d3 * dy_0i;
        }

        const KIND field_x = Agent.velocity * cos(Agent.angle) + gamma_friction * potencial[0];
        const KIND field_y = Agent.velocity * sin(Agent.angle) + gamma_friction * potencial[1];

        particle updatedAgent(Agent.x + delta_time * field_x, Agent.y + delta_time * field_y);
        updatedAgent.x = b_condition(updatedAgent.x);
        updatedAgent.y = b_condition(updatedAgent.y);

        if (dis(gen) < p_rotation) {
            const KIND ruido = dis(gen) * 2 * Pi;
            updatedAgent.angle += ruido;
        }

        return updatedAgent;
    } else {
        particle updatedAgent(Agent.x + Agent.velocity * cos(Agent.angle) * delta_time, Agent.y + Agent.velocity * sin(Agent.angle) * delta_time);
        updatedAgent.x = b_condition(updatedAgent.x);
        updatedAgent.y = b_condition(updatedAgent.y);

        if (dis(gen) < p_rotation) {
            const KIND ruido = dis(gen) * 2 * Pi;
            updatedAgent.angle += ruido;
        }

        return updatedAgent;
    }
}


KIND distance(const particle& A, const particle& B) {
    KIND dx = A.x - B.x;
    KIND dy = A.y - B.y;
    
    // Apply periodic boundary conditions
    dx = fmod(dx + L / 2, L) - L / 2;
    dy = fmod(dy + L / 2, L) - L / 2;
    
    return sqrt(dx * dx + dy * dy);
}


############################################################################
#include "parameters.h"    
#include <random>
#include <ctime>

using namespace std;

// Generador de números aleatorios en (0,1).
mt19937::result_type seed = time(0);
mt19937 gen(seed);                             // Standard mersenne_twister_engine seeded time(0)
uniform_real_distribution<KIND> dis(0., 1.);    // dis(gen), número aleatorio real entre 0 y 1.

/* Definimos la clase partículas y sus métodos */
/* Memoria que ocupa cada miembro de la clase: 4 doubles (32 Bytes) + 1 int (4 Bytes) = 36 Bytes */

class particle {
private:
    // Estado interno del agente.
    int state;
public:
    // Estado dinámico del agente.
    KIND x, y;
    KIND velocity;
    KIND angle;

    // Constructores:
    particle();
    particle(KIND x1, KIND y1, KIND vel, KIND ang);

    // Métodos:
    bool is_healthy()    { return (state <  1); }
    bool is_infected()   { return (state == 1); }
    bool is_refractary() { return (state == 2); }

    int get_state()      { return state; }

    void set_healthy()    { state = 0; }
    void set_infected()   { state = 1; }
    void set_refractary() { state = 2; }
};

/* Constructor de partícula genérica */
particle::particle() {
    velocity = 0;
    angle    = 0;
    x        = 0;
    y        = 0;
}

/* Constructor de una partícula en una fase dada (x, p) del sistema */
particle::particle(KIND x1, KIND y1, KIND vel, KIND ang) {
    velocity = vel;
    angle    = ang;
    x        = x1;
    y        = y1;
}

/**
 * [Crea una partícula en un punto aleatorio (x, p, s) en la fase del sistema]
 * @return  [partícula]
 */
particle create_particle() {
    KIND x, y, velocity, angle;

    x     = dis(gen) * L;
    y     = dis(gen) * L;
    angle = dis(gen) * dos_Pi;

    // Tres distribuciones para asignar la velocidad al azar:
    switch (velocity_distribution) {
        case 0:
            velocity = -active_velocity * log(1. - dis(gen));
            break;
        case 1:
            velocity = pow(dis(gen) * (pow(v_max, 1 - k_powerl) - pow(v_min, 1 - k_powerl)) + pow(v_min, 1 - k_powerl), 1. / (1. - k_powerl));
            break;
        default:
            velocity = active_velocity;
            break;
    }

    // Creación de la partícula:
    particle A(x, y, velocity, angle);

    // Setting del estado interno de la partícula:
    // if (dis(gen) < p_init) { A.set_infected(); } // Agrega un porcentaje p_init de partículas infectadas.

    // Condición de inicio
    // if ((pow(A.x - (L/2), 2) + pow(A.y - (L/2), 2)) <= 100) { A.set_infected(); } // Onda radial
    if (((L / 5 - 20) < A.x) && (A.x < L / 5)) A.set_infected(); // Onda lineal

    else A.set_healthy();

    // Agrega un porcentaje p_rinit de partículas en estado refractario.
    // No reasigna una previamente infectada.
    // if (dis(gen) < p_rinit && !A.is_infected()) { A.set_refractary(); }

    return A;
}