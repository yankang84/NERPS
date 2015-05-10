#include "Particle.h"

using namespace std;

Particle::Particle(std::default_random_engine& rng,
			 	   std::uniform_real_distribution<double>& rngTheta,
			       std::uniform_real_distribution<double>& rngPhi,
			       std::uniform_real_distribution<double>& rngVel,
			       Settings settings_in)
{
	//Set the initial position
	double theta = rngTheta(rng);
	double phi = rngPhi(rng);
	double rho = settings.sphereR;

	//Convert to cartersian coords
	x = (rho*sin(theta)*cos(phi));
	y = (rho*sin(theta)*sin(phi));
	z = (rho*cos(theta));

	//Set the initial velocity
	double xV = rngVel(rng);
	double yV = rngVel(rng);
	double zV = rngVel(rng);

	//Normalize to unit vector
	double norm = sqrt(xV*xV + yV*yV + zV*zV);
	xV = xV / norm;
	yV = yV / norm;
	zV = zV / norm;

	vector<double> vel(3);
	vel[0] = xV;
	vel[1] = yV;
	vel[2] = zV;
	velocity = vel;

	//Bring it to life
	alive = true;
	absorbed = false;
	scatterCount = 0;

	//Save settings
	settings = settings_in;
}

void Particle::Update()
{
	//Move the particle relative to its velocity and step
	x = x + velocity[0]*settings.step;
	y = y + velocity[1]*settings.step;
	z = z + velocity[2]*settings.step;

	//Check if we're out of bounds
	double rho = sqrt(x*x + y*y + z*z);
	if(rho > settings.sphereR)
	{
		alive = false;
		return;
	}

}


void Particle::Scatter(default_random_engine& rng,
			 		   uniform_real_distribution<double>& rngVel)
{
	//Calculate a new velocity unit vector
	vector<double> newVel(3);

	newVel[0] = rngVel(rng);
	newVel[1] = rngVel(rng);
	newVel[2] = rngVel(rng);

	double norm = sqrt( (newVel[0]*newVel[0]) + 
					    (newVel[1]*newVel[1]) + 
					    (newVel[2]*newVel[2]) );

	newVel[0] = newVel[0]/norm;
	newVel[1] = newVel[1]/norm;
	newVel[2] = newVel[2]/norm;

	velocity = newVel;

	scatterCount++;
}

ostream& operator<<(ostream& os, const Particle& particle)
{
	os << particle.x << "," << particle.y << "," << particle.z;
	return os;
}