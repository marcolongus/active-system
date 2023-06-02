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
    dx = std::fmod(dx + L / 2, L) - L / 2;
    dy = std::fmod(dy + L / 2, L) - L / 2;
    
    return std::sqrt(dx * dx + dy * dy);
}