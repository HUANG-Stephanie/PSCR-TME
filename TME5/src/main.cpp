#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>
#include "Job.h"
#include <vector>
#include <thread>
#include "Pool.h"

using namespace std;
using namespace pr;


void fillScene(Scene & scene, default_random_engine & re) {
	// Nombre de spheres (rend le probleme plus dur)
	const int NBSPHERES = 250;

	// on remplit la scene de spheres colorees de taille position et couleur aleatoire
	uniform_int_distribution<int> distrib(0, 200);
	uniform_real_distribution<double> distribd(-200, 200);
	for (int i = 0; i < NBSPHERES; i++) {
		// position autour de l'angle de la camera
		// rayon entre 3 et 33, couleur aleatoire
		// distrib(re) rend un entier aleatoire extrait de re
		scene.add(Sphere({50+distribd(re),50 + distribd(re),120 + distribd(re) }, double(distrib(re)%30) + 3.0, Color::random()));
	}
	// quelques spheres de plus pour ajouter du gout a la scene
	scene.add(Sphere({50,50,40},15.0,Color::red));
	scene.add(Sphere({100,20,50},55.0,Color::blue));

}

// return the index of the closest object in the scene that intersects "ray"
// or -1 if the ray does not intersect any object.
int findClosestInter(const Scene & scene, const Rayon & ray) {
	auto minz = std::numeric_limits<float>::max();
	int targetSphere = -1;
	int index = 0;
	for (const auto & obj : scene) {
		// rend la distance de l'objet a la camera
		auto zinter = obj.intersects(ray);
		// si intersection plus proche  ?
		if (zinter < minz) {
			minz = zinter;
			targetSphere = index;
		}
		index++;
	}
	return targetSphere;
}

// Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
// En déduit la couleur d'un pixel de l'écran.
Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights) {
	Color finalcolor = obj.getColor();

	// calcul du rayon et de sa normale a la sphere
	// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
	// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
	Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
	// le point d'intersection
	Vec3D intersection = rayInter + camera;
	// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
	Vec3D normal = obj.getNormale(intersection);
	// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
	double dt = 0;
	// modifier par l'eclairage la couleur
	for (const auto & light : lights) {
		// le vecteur de la lumiere au point d'intersection
		Vec3D tolight = (light - intersection);
		// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
		if (obj.intersects(Rayon(light,intersection)) >= tolight.length() - 0.05 ) {   //  epsilon 0.05 for double issues
			dt += tolight.normalize() & normal ; // l'angle (scalaire) donne la puissance de la lumiere reflechie
		}
	}
	// eclairage total
	finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

	return finalcolor;
}

// produit une image dans path, représentant les pixels.
void exportImage(const char * path, size_t width, size_t height, Color * pixels) {
	// ppm est un format ultra basique
	ofstream img (path);
	// P3 signifie : les pixels un par un en ascii
	img << "P3" << endl; // ascii format, colors
	// largeur hauteur valeur max d'une couleur (=255 un char)
	img << width  << "\n"<< height << "\n" << "255" << endl;
	// tous les pixels au format RGB
	for (size_t  y = 0 ; y < height ; y++) {
		for (size_t x =0 ; x < width ; x++) {
			Color & pixel = pixels[x*height + y];
			img << pixel << '\n';
		}
	}
	// oui ca fait un gros fichier :D
	img.close();
}

// NB : en francais pour le cours, preferez coder en english toujours.
// pas d'accents pour eviter les soucis d'encodage

class DrawJob : public Job {
	public:
	int x;
	int y;
	Scene & scene;
	const Scene::screen_t & screen;
	Color * pixels;
	vector<Vec3D> & lights;

	DrawJob(int x, int y, Scene & scene, const Scene::screen_t & screen, Color* pixels, vector<Vec3D> & lights) 
	: x(x), y(y), scene(scene), screen(screen), pixels(pixels), lights(lights)
	 {};

	void run(){
		
		// le point de l'ecran par lequel passe ce rayon
		auto & screenPoint = screen[y][x];
		// le rayon a inspecter
		Rayon  ray(scene.getCameraPos(), screenPoint);

		int targetSphere = findClosestInter(scene, ray);
	
		if (targetSphere == -1) {
			// keep background color
			return;
		} else {
			const Sphere & obj = *(scene.begin() + targetSphere);
			// pixel prend la couleur de l'objet
			Color finalcolor = computeColor(obj, ray, scene.getCameraPos(), lights);
			// le point de l'image (pixel) dont on vient de calculer la couleur
			Color & pixel = pixels[y*scene.getHeight() + x];
			// mettre a jour la couleur du pixel dans l'image finale.
			pixel = finalcolor;
		}
	}

};


int main () {

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	// on pose une graine basee sur la date
	default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	// definir la Scene : resolution de l'image
	Scene scene (1000,1000);
	// remplir avec un peu d'aléatoire
	fillScene(scene, re);
	
	// lumieres 
	vector<Vec3D> lights;
	lights.reserve(3);
	lights.emplace_back(Vec3D(50, 50, -50));
	lights.emplace_back(Vec3D(50, 50, 120));
	lights.emplace_back(Vec3D(200, 0, 120));

	// les points de l'ecran, en coordonnées 3D, au sein de la Scene.
	// on tire un rayon de l'observateur vers chacun de ces points
	const Scene::screen_t & screen = scene.getScreenPoints();

	// Les couleurs des pixels dans l'image finale
	Color * pixels = new Color[scene.getWidth() * scene.getHeight()];

	time_t time;
	time_t new_time;
	time_t time_min = 1000000;
	time_t old_time = 0;
	int NBTHREADS_min = 0;
	for(int i = 1; i < 11; i++){
		Pool pool(1000);
		pool.start(i); //creation des threads (Pool de thread)
		// pour chaque pixel, calculer sa couleur
		for (int x =0 ; x < scene.getWidth() ; x++) {	
			for (int  y = 0 ; y < scene.getHeight() ; y++) {
				Job * j = new DrawJob(x,y,scene,screen,pixels,lights); //creation de la tache
				pool.submit(j); //ajout de la tache dans la queue pour qu'elle soit traiter avec PoolWorker en interne dans le thread
			}
		}
		pool.stop(); //suppression du pool de thread

		cout << " NBTHREADS = " << i << " ";
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    std::cout << "Total time ";
		time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		new_time = time-old_time;
	              cout << new_time
	              << "ms.\n";
				  
		if(new_time < time_min){
			time_min = new_time;
			NBTHREADS_min = i;
		}
		old_time = time;
	}
	
	cout << "nb de thread associé au temps min = " << NBTHREADS_min << endl;

	/*
	exemple d'une execution:
	NBTHREADS = 1 Total time 31219ms.
	NBTHREADS = 2 Total time 56933ms.
	NBTHREADS = 3 Total time 51725ms.
	NBTHREADS = 4 Total time 46668ms.
	NBTHREADS = 5 Total time 36920ms.
	NBTHREADS = 6 Total time 32769ms.
	NBTHREADS = 7 Total time 33545ms.
	NBTHREADS = 8 Total time 28640ms.
	NBTHREADS = 9 Total time 26158ms.
	NBTHREADS = 10 Total time 23385ms.
	nb de thread associé au temps min = 10
	*/

	exportImage("toto.ppm",scene.getWidth(), scene.getHeight() , pixels);

	return 0;
}

