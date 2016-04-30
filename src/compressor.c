#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double max_pressure_ratio = 1.2;
const double r_univ = 8.314;		// in SI units.

struct compressor {
	double p_in; 		// the pressure flowing in, in Pascals.
	double p_out;		// the pressure flowing out, in Pascals.

	double t_in;		// the temperature of air flowing in,
				// in Kelvin.
	double t_out;		// the temperature of air flowing out,
				// in Kelvin.

	double w_req;		// the amount of work required in Joules per
				// second for 1 kg of gas.
};



/*
 * Prints the details of the compressor
 */
void print_compressor(struct compressor c)
{
	printf("P_in = %.0f Pa\tT_in = %.2f K\tP_out = %.0f Pa\tT_out = %.2f K\tW_req = %.2f J/s\n",
		c.p_in,
		c.t_in,
		c.p_out,
		c.t_out,
		c.w_req
	);
}


/*
 * Computes and returns the number of stages to reach given pressure ratio rp.
 */
int find_num_stages(double desired_rp)
{
	int n = 1;

	double current_rp = max_pressure_ratio;

	while (current_rp < desired_rp) {
		current_rp *= max_pressure_ratio;
		n++;
	}

	return n;
}

/*
 * Simulates the compressor for given inputs.
 */
struct compressor *simulate_compressor(
	int num_stages,		// the number of stages of the compressor.
	double p_atm,		// the inlet pressure, in Pa.
	double t_inlet,		// the inlet temp, in K.
	double gamma,		// the gamma of the gas.
	double gas_flow_rate,	// the rate at which gas flows in, in kg/s.
	double molecular_mass	// mass of 1 mol of gas molecules, in kg.
) {
	struct compressor *cs = malloc (num_stages * sizeof(*cs));

	// input pressure and temperature from previous compressor.
	// initially from atmosphere
	double p_prev = p_atm;
	double t_prev = t_inlet;

	for (int i = 0; i < num_stages; i++) {
		cs[i].p_in = p_prev;
		cs[i].t_in = t_prev;

		cs[i].p_out = cs[i].p_in * 1.2;

		// assuming that compressor is adiabatic
		cs[i].t_out = pow (cs[i].p_in / cs[i].p_out, 1 - gamma)
			* cs[i].t_in;

		p_prev = cs[i].p_out;
		t_prev = cs[i].t_out;

		cs[i].w_req = (gas_flow_rate / molecular_mass) * r_univ
				* (cs[i].t_out - cs[i].t_in)/ (gamma - 1);
	}

	return cs;
}
