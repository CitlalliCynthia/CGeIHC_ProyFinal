/*
PROYECTO FINAL:
	Cuadriello Valdés Cynthia Citlalli  -- 422020266
	Cuadriello Valdés Diana Sinsuni     -- 422038870
	Gutiérrez Ávila Tristan Bernardo    -- 319111347
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture rejaTexture;

///////////////////
//Definir modelos//
///////////////////

// Rueda Fortuna
Model RuedaFortuna_M;
Model Neuvillette_Genshin_M;
Model Furina_Genshin_M;

// Puesto Helados
Model PuestoHelado_M;
Model MariusCasual_M;
Model Sigwinne_Genshin_M;

// Banca Puesto de Helados
Model Wriothesley_Genshin_M;

// Puesto Dardos
Model PuestoDardos_M;
Model Globo1_M;
Model Globo2_M;
Model Globo3_M;
Model Dardo_M;
Model Firefly_StarRail_M;
Model LukeNormal_M;

// Carrusel
Model Carrusel_Base_M;
Model Carrusel_M;
Model Lyney_M;

// Baño Puesto Dardos
Model Bano_M;
Model MariusChino_M;

// Puesto Hot Dogs
Model PuestoHotDog_M;
Model Jean_Genshin_M;
Model VynNormal_M;

// Banca Puesto de Hot Dogs
Model Klee_Genshin_M;

// Banca Puesto Palomitas
Model Sunday_StarRail_M;

// Artem Protagonista
Model ArtemNormal_M;

// Puesto Palomitas
Model PuestoPalomitas_M;
Model Feixiao_StarRail_M;

// Puesto Lanzamiento Hacha
Model LanzamientoHacha_M;
Model Hacha_M;
Model Wanderer_Genshin_M;

// Puesto Dados
Model MesaDados_M;
Model Aventurine_StarRail_M;

// Puesto Boliche
Model Boliche_M;
Model Rendija_Boliche_M;
Model Bolo_M;
Model Bola_Boliche_M;
Model Ayato_Genshin_M;

// Puesto Golpear al Topo
Model GolpearTopo_M;
Model Yanqing_StarRail_M;

// Puesto Algodón Azúcar
Model MaquinaAlgodon_M;
Model Robin_StarRail_M;

// Puesto Carros Chocones
Model CarrosChocones_M;
Model ArtemCasual_Themis_M;

// Jaula Bateo
Model PuestoBateo_M;
Model Bate_M;
Model PelotaBateo_M;
Model LukeCasual_Themis_M;

// Baño Boliche
Model Ratio_StarTail_M;

// Puesto de Tickets
Model PuestoTickets_M;

// Banca Boliche
Model Banca_Genshin_M;
Model VynChino_Themis_M;

// Puesto de Tacos
Model PuestoTacos_M;
Model Herta_StarTail_M;
Model Xiao_Genshin_M;

// Decoraciones
Model Lampara_Genshin_M;
Model Bote_StarRail_M;

//Plataforma Dodoco
Model Plataforma_Genshin_M;

//Mesa Comida Genshin
Model MesaComida;
Model LecheDango;

//Stand madera Tears of Themis
Model Stand_Dardos_M;
Model Stand_HotDog_M;
Model Stand_Taquilla_M;
Model Stand_Carrusel_M;
Model Stand_Carros_M;
Model Stand_Comida_M;
Model Stand_Dados_M;
Model Stand_Topos_M;

//Puesto Inazuma
Model Ixbalanque_M;
Model PuestoInazuma_M;

//Globos de Genshin
Model Globo_Enojado_M;
Model Globo_Feliz_M;
Model Globo_Verde_M;
Model Globo_Naranja_M;

//Gatos Star Rail
Model Gato_Basura_M;
Model Gato_Blade_M;
Model Gato_Clara_M;
Model Gato_DanHeng_M;
Model Gato_Kafka_M;
Model Gato_March_M;
Model Gato_Naranja_M;
Model Gato_RuanMei_M;
Model Gato_Herta_M;
Model Gato_Verde_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

PointLight pointLights1[MAX_POINT_LIGHTS];
SpotLight spotLights1[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int rejaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat rejaVertices[] = {
		-12.0f, 0.0f, 0.0f,			0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //inf izq
		12.0f, 0.0f, 0.0f,			1.0f, 0.0f,		0.0f, 0.0f, 0.0f, //inf der
		12.0f, 12.0f, 0.0f,			1.0f, 1.0f,		0.0f, 0.0f, 0.0f, //sup der
		-12.0f, 12.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f, //sup izq

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(rejaVertices, rejaIndices, 31, 6);
	meshList.push_back(obj2);

	calcAverageNormals(rejaIndices, 6, rejaVertices, 32, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	rejaTexture = Texture("Textures/Reja.png");
	rejaTexture.LoadTextureA();

	//////////////////////
	//Instanciar modelos//
	//////////////////////

	//Rueda Fortuna
	RuedaFortuna_M = Model();
	RuedaFortuna_M.LoadModel("Models/RuedaFortuna/RuedaFortuna.obj");
	Neuvillette_Genshin_M = Model();
	Neuvillette_Genshin_M.LoadModel("Models/Neuvillette_Genshin/Neuvillette_Genshin.obj");
	Furina_Genshin_M = Model();
	Furina_Genshin_M.LoadModel("Models/Furina_Genshin/Furina_Genshin.obj");

	//Puesto Helados
	PuestoHelado_M = Model();
	PuestoHelado_M.LoadModel("Models/PuestoHelado/puestoHelado.obj");
	MariusCasual_M = Model();
	MariusCasual_M.LoadModel("Models/MariusCasual_Themis/mariusCasual.obj");
	Sigwinne_Genshin_M = Model();
	Sigwinne_Genshin_M.LoadModel("Models/Sigwinne_Genshin/Sigwinne.obj");

	//Banca Puesto de Helados
	Wriothesley_Genshin_M = Model();
	Wriothesley_Genshin_M.LoadModel("Models/Wriothesley_Genshin/Wriothesley_Genshin.obj");

	//Puesto de Dardos
	PuestoDardos_M = Model();
	PuestoDardos_M.LoadModel("Models/PuestoDardos/puestoDardos.obj");
	Globo1_M = Model();
	Globo1_M.LoadModel("Models/PuestoDardos/globo1.obj");
	Globo2_M = Model();
	Globo2_M.LoadModel("Models/PuestoDardos/globo2.obj");
	Globo3_M = Model();
	Globo3_M.LoadModel("Models/PuestoDardos/globo3.obj");
	Dardo_M = Model();
	Dardo_M.LoadModel("Models/PuestoDardos/dardo.obj");
	Firefly_StarRail_M = Model();
	Firefly_StarRail_M.LoadModel("Models/Firefly_StarRail/Firefly_StarRail.obj");
	LukeNormal_M = Model();
	LukeNormal_M.LoadModel("Models/LukeNormal-Themis/lukeNormal.obj");

	//Carrusel
	Carrusel_Base_M = Model();
	Carrusel_Base_M.LoadModel("Models/Carrusel/Base.obj");
	Carrusel_M = Model();
	Carrusel_M.LoadModel("Models/Carrusel/Carrusel.obj");
	Lyney_M = Model();
	Lyney_M.LoadModel("Models/Lyney_Genshin/Lyney_Genshin.obj");

	//Baño Puesto Dardos
	Bano_M = Model();
	Bano_M.LoadModel("Models/PuestoBano/puestoBano.obj");
	MariusChino_M = Model();
	MariusChino_M.LoadModel("Models/MariusChino_Themis/mariusChino.obj");

	//Puesto Hot Dogs
	PuestoHotDog_M = Model();
	PuestoHotDog_M.LoadModel("Models/PuestoHotDog/puestoHotDog.obj");
	Jean_Genshin_M = Model();
	Jean_Genshin_M.LoadModel("Models/Jean_Genshin/Jean.obj");
	VynNormal_M = Model();
	VynNormal_M.LoadModel("Models/VynNormal_Themis/vynNormal.obj");

	//Banca Puesto Hot Dogs
	Klee_Genshin_M = Model();
	Klee_Genshin_M.LoadModel("Models/Klee_Genshin/Klee_Genshin.obj");

	//Banca Puesto Palomitas
	Sunday_StarRail_M = Model();
	Sunday_StarRail_M.LoadModel("Models/Sunday_StarRail/Sunday.obj");

	//Artem Normal
	ArtemNormal_M = Model();
	ArtemNormal_M.LoadModel("Models/ArtemNormal_Themis/artemNormal.obj");

	//Puesto Palomitas
	PuestoPalomitas_M = Model();
	PuestoPalomitas_M.LoadModel("Models/PuestoPalomitas/PuestoPalomitas.obj");
	Feixiao_StarRail_M = Model();
	Feixiao_StarRail_M.LoadModel("Models/Feixiao_StarRail/Feixiao_StarRail.obj");

	//Lanzamiento de Hacha
	LanzamientoHacha_M = Model();
	LanzamientoHacha_M.LoadModel("Models/LanzamientoHacha/LanzamientoHacha.obj");
	Hacha_M = Model();
	Hacha_M.LoadModel("Models/LanzamientoHacha/Hacha.obj");
	Wanderer_Genshin_M = Model();
	Wanderer_Genshin_M.LoadModel("Models/Wanderer_Genshin/Wanderer_Genshin.obj");

	//Puesto Dados
	MesaDados_M = Model();
	MesaDados_M.LoadModel("Models/MesaDados/MesaDados.obj");
	Aventurine_StarRail_M = Model();
	Aventurine_StarRail_M.LoadModel("Models/Aventurine_StarRail/Aventurine_StarRail.obj");

	//Puesto Boliche
	Boliche_M = Model();
	Boliche_M.LoadModel("Models/Boliche/Boliche.obj");
	Rendija_Boliche_M = Model();
	Rendija_Boliche_M.LoadModel("Models/Boliche/RendijaBoliche.obj");
	Bolo_M = Model();
	Bolo_M.LoadModel("Models/Boliche/Bolo.obj");
	Bola_Boliche_M = Model();
	Bola_Boliche_M.LoadModel("Models/Boliche/Bola.obj");
	Ayato_Genshin_M = Model();
	Ayato_Genshin_M.LoadModel("Models/Ayato_Genshin/Ayato_Genshin.obj");

	//Puesto Golpear al Topo
	GolpearTopo_M = Model();
	GolpearTopo_M.LoadModel("Models/GolpearAlTopo/GolpearAlTopo.obj");
	Yanqing_StarRail_M = Model();
	Yanqing_StarRail_M.LoadModel("Models/Yanqing_StarRail/Yanqing_StarRail.obj");

	//Puesto Algodón Azúcar
	MaquinaAlgodon_M = Model();
	MaquinaAlgodon_M.LoadModel("Models/MaquinaAlgodon/MaquinaAlgodon.obj");
	Robin_StarRail_M = Model();
	Robin_StarRail_M.LoadModel("Models/Robin_StarRail/Robin_StarRail.obj");

	//Puesto Carros Chocones
	CarrosChocones_M = Model();
	CarrosChocones_M.LoadModel("Models/PuestoCarrosChocones/puestoCarrosChocones.obj");
	ArtemCasual_Themis_M = Model();
	ArtemCasual_Themis_M.LoadModel("Models/ArtemCasual_Themis/artemCasual.obj");

	//Jaula Bateo
	PuestoBateo_M = Model();
	PuestoBateo_M.LoadModel("Models/PuestoBateo/puestoBateo.obj");
	Bate_M = Model();
	Bate_M.LoadModel("Models/PuestoBateo/bate.obj");
	PelotaBateo_M = Model();
	PelotaBateo_M.LoadModel("Models/PuestoBateo/batePelota.obj");
	LukeCasual_Themis_M = Model();
	LukeCasual_Themis_M.LoadModel("Models/LukeCasual_Themis/LukeCasual.obj");

	//Baño Boliche
	Ratio_StarTail_M = Model();
	Ratio_StarTail_M.LoadModel("Models/Ratio_StarRail/Ratio_StarRail.obj");

	//Puesto de Tickets
	PuestoTickets_M = Model();
	PuestoTickets_M.LoadModel("Models/PuestoTickets/PuestoTickets.obj");

	//Banca Boliche
	Banca_Genshin_M = Model();
	Banca_Genshin_M.LoadModel("Models/BancaTexturizada/BancaTexturizada.obj");
	VynChino_Themis_M = Model();
	VynChino_Themis_M.LoadModel("Models/VynChino_Genshin/vynChino.obj");

	//Puesto Tacos
	PuestoTacos_M = Model();
	PuestoTacos_M.LoadModel("Models/PuestoTacos/puestoTacos.obj");
	Herta_StarTail_M = Model();
	Herta_StarTail_M.LoadModel("Models/Herta_StarRail/herta.obj");
	Xiao_Genshin_M = Model();
	Xiao_Genshin_M.LoadModel("Models/Xiao_Genshin/xiao2.obj");

	//Decoraciones
	Lampara_Genshin_M = Model();
	Lampara_Genshin_M.LoadModel("Models/LamparaGenshin/LamparaGenshin.obj");
	Bote_StarRail_M = Model();
	Bote_StarRail_M.LoadModel("Models/Bote/Bote.obj");

	//Plataforma Dodoco
	Plataforma_Genshin_M = Model();
	Plataforma_Genshin_M.LoadModel("Models/MesaComida/Plataforma.obj");

	//Mesa con comida Genshin
	MesaComida = Model();
	MesaComida.LoadModel("Models/MesaComida/MesaComidas1.obj");
	LecheDango = Model();
	LecheDango.LoadModel("Models/MesaComida/lechedango.obj");

	//Stands de madera de Tears of Themis
	Stand_Dardos_M = Model();
	Stand_Dardos_M.LoadModel("Models/StandMadera/standMadera_ArtemDardos.obj");
	Stand_HotDog_M = Model();
	Stand_HotDog_M.LoadModel("Models/StandMadera/standMadera_ArtemHotDog.obj");
	Stand_Taquilla_M = Model();
	Stand_Taquilla_M.LoadModel("Models/StandMadera/standMadera_CarnivalThemis.obj");
	Stand_Carrusel_M = Model();
	Stand_Carrusel_M.LoadModel("Models/StandMadera/standMadera_Carrucel.obj");
	Stand_Carros_M = Model();
	Stand_Carros_M.LoadModel("Models/StandMadera/standMadera_VynCarrosChocones.obj");
	Stand_Comida_M = Model();
	Stand_Comida_M.LoadModel("Models/StandMadera/standMadera_LukeComida.obj");
	Stand_Dados_M = Model();
	Stand_Dados_M.LoadModel("Models/StandMadera/standMadera_MariusCasino.obj");
	Stand_Topos_M = Model();
	Stand_Topos_M.LoadModel("Models/StandMadera/standMadera_VynTopos.obj");
	
	//Puesto Inazuma
	PuestoInazuma_M = Model();
	PuestoInazuma_M.LoadModel("Models/PuestoInazuma/puestoInazumaPulpo.obj");
	Ixbalanque_M = Model();
	Ixbalanque_M.LoadModel("Models/Ixbalanque_Genshin/Ixbalanque_Genshin.obj");
	
	//Globos Genshin
	Globo_Enojado_M = Model();
	Globo_Enojado_M.LoadModel("Models/Globo/globoEnojado.obj");
	Globo_Feliz_M = Model();
	Globo_Feliz_M.LoadModel("Models/Globo/globoFeliz.obj");
	Globo_Verde_M = Model();
	Globo_Verde_M.LoadModel("Models/Globo/globoVerde.obj");
	Globo_Naranja_M = Model();
	Globo_Naranja_M.LoadModel("Models/Globo/globoNaranja.obj");

	//Gatos Star Rail
	Gato_Basura_M = Model();
	Gato_Basura_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Basura.obj");
	Gato_Blade_M = Model();
	Gato_Blade_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Blade.obj");
	Gato_Clara_M = Model();
	Gato_Clara_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Clara.obj");
	Gato_DanHeng_M = Model();
	Gato_DanHeng_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_DanHeng.obj");
	Gato_Kafka_M = Model();
	Gato_Kafka_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Kafka.obj");
	Gato_March_M = Model();
	Gato_March_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_March.obj");
	Gato_Naranja_M = Model();
	Gato_Naranja_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Naranja.obj");
	Gato_RuanMei_M = Model();
	Gato_RuanMei_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_RuanMei.obj");
	Gato_Herta_M = Model();
	Gato_Herta_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Herta.obj");
	Gato_Verde_M = Model();
	Gato_Verde_M.LoadModel("Models/Gato_StarRail/Gato_StarRail_Verde.obj");
	
	// --------------------------------------
	// -----------SKYBOX TEXTURAS------------
	// --------------------------------------

	std::vector<std::string> skyboxFaces; //Se crea un vector con las texturas que componen al skybox de día
	//Todo esto para que dentro del while se vayan cambiando con los skybox ya existentes creados aquí
	skyboxFaces.push_back("Textures/Skybox/Nubes_Derecha.tga");
	skyboxFaces.push_back("Textures/Skybox/Nubes_Izq.tga");
	skyboxFaces.push_back("Textures/Skybox/Nubes_Abajo.tga");
	skyboxFaces.push_back("Textures/Skybox/Nubes_Arriba.tga");
	skyboxFaces.push_back("Textures/Skybox/Nubes_Detras.tga");
	skyboxFaces.push_back("Textures/Skybox/Nubes_Enfrente.tga");

	std::vector<std::string> skyboxFacesAtard; //Se crea un vector con las texturas que componen al skybox de atardecer
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Derecha.tga");
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Izq.tga");
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Abajo.tga");
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Arriba.tga");
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Detras.tga");
	skyboxFacesAtard.push_back("Textures/Skybox/Atardecer_Enfrente.tga");

	std::vector<std::string> skyboxFacesNoche; //Se crea un vector con las texturas que componen al skybox de noche
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Derecha.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Izq.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Abajo.tga ");
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Arriba.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Detras.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night_Enfrente.tga");

	int skyCount = 0; //Se crea una variable contadora para que lleve la cuenta de vueltas que lleva el while y se vayan cambiando

	//skybox = Skybox(skyboxFaces); ///BORRAR!!!!!!!
	
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	
	//Luz fija Cofre
	spotLights[0] = SpotLight(0.5f, 0.0f, 0.5f, //RGB
		1.0f, 1.0f,
		10.0f, 3.0f, -5.0f, //Posición (de dónde sale la luz)
		1.0f, 0.0f, 0.0f, //Dirección (hacia dónde apunta)
		0.4f, 0.1f, 0.0f, //Constante, lineal, exponencial
		45.0f);
	spotLightCount++;

	//Luz fija Coche frente
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //RGB
		1.0f, 1.0f,
		10.0f, 0.5f, -5.0f, //Posición (de dónde sale la luz)
		1.0f, 0.0f, 0.0f, //Dirección (hacia dónde apunta)
		0.4f, 0.1f, 0.0f,
		45.0f);
	spotLightCount++;

	//Luz fija Coche detrás
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f, //RGB
		1.0f, 1.0f,
		-10.0f, 0.5f, -5.0f, //Posición (de dónde sale la luz)
		-1.0f, 0.0f, 0.0f, //Dirección (hacia dónde apunta)
		0.4f, 0.1f, 0.0f,
		45.0f);
	spotLightCount++;

	spotLights1[0] = spotLights[0]; //Luz fija Cofre
	spotLights1[1] = spotLights[2]; //Luz fija Coche detrás
	spotLights1[2] = spotLights[1]; //Luz fija Coche frente

	//Luz puntual Lámpara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, //RGB
		0.4f, 0.5f,
		13.0f, 10.0f, -5.0f, //Posición de la luz
		0.4f, 0.0f, 0.01f);
	pointLightCount++;

	//Luz puntual Slime
	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f, //RGB
		0.4f, 0.2f,
		0.0f, 5.0f, 6.0f, //Posición de la luz
		0.0f, 0.06f, 0.01f);
	pointLightCount++;

	pointLights1[0] = pointLights[1]; //Luz puntual Slime
	pointLights1[1] = pointLights[0]; //Luz puntual Lámpara

	/*
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	*/


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		//Inicia el if para cambiar de skybox
		//Se cambia solo una vez en lugar de cada vez dentro de un rango para ahorrar recursos

		if (skyCount == 0) { //Si es cero la variable contadora, entonces se pone el skybox de dia
			skybox = Skybox(skyboxFaces);
		}
		else if (skyCount == 10000) {//Si es x la variable contadora, entonces se pone el skybox de atardecer
			skybox = Skybox(skyboxFacesAtard);
		}
		else if (skyCount == 20000) { //Si es y la variable contadora, entonces se pone el skybox de noche
			skybox = Skybox(skyboxFacesNoche);
		}
		else if (skyCount == 30000) { //Si es z la variable contadora, entonces se pone el skybox de dia nuevamente para que se resetee
			skyCount = -1; //Se le resta uno porque abajo se suma 1 y al sumarse 1 ya no queda en e para el reinicio
		}

		skyCount += 1; //La variable contadora aumenta en uno con cada ciclo del while que es lo que lleva la cuenta del tiempo

		//Termina el cambio del skybox




		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		



		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		/*
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		if (mainWindow.getarticulacion4() == 0.0 && mainWindow.getarticulacion5() == 0.0) //ambas encendidas
			shaderList[0].SetPointLights(pointLights, pointLightCount -2);
		else if (mainWindow.getarticulacion4() == 1.0 && mainWindow.getarticulacion5() == 1.0) //ambas apagadas
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		else if (mainWindow.getarticulacion4() == 0.0 && mainWindow.getarticulacion5() == 1.0) //slime encendido
			shaderList[0].SetPointLights(pointLights1, pointLightCount - 1);
		else if (mainWindow.getarticulacion4() == 1.0 && mainWindow.getarticulacion5() == 0.0) //lámpara encendida
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);

		//Movimiento hacia adelante
		if (mainWindow.getarticulacion10() == 1.0) shaderList[0].SetSpotLights(spotLights, spotLightCount -1);
		//Movimiento hacia atrás
		else if (mainWindow.getarticulacion10() == 0.5) shaderList[0].SetSpotLights(spotLights1, spotLightCount - 1);
		else shaderList[0].SetSpotLights(spotLights1, spotLightCount - 2);




		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// --------------------------------------
		// -----------------PISO-----------------
		// --------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[0]->RenderMesh();

		// --------------------------------------
		// ------------RUEDA FORTUNA-------------
		// --------------------------------------

		//Rueda Fortuna
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaFortuna_M.RenderModel();

		//Gato 7 de Marzo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_March_M.RenderModel();
		
		//Neuvillette Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, 0.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Neuvillette_Genshin_M.RenderModel();

		//Furina Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 19.0f));
		model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Furina_Genshin_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		//Globos derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 21.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(32.0f, 0.0f, 22.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(34.0f, 0.0f, 21.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		//Globos izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 21.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-32.0f, 0.0f, 22.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 21.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		// --------------------------------------
		// -----------PUESTO HELADOS-------------
		// --------------------------------------

		//Puesto helado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 25.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoHelado_M.RenderModel();

		//Marius Casual Tears of Themis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 25.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MariusCasual_M.RenderModel();

		//Sigwinne Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(74.0f, 0.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sigwinne_Genshin_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(54.0f, 0.0f, -1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		// --------------------------------------
		// --------BANCA PUESTO HELADOS----------
		// --------------------------------------

		//Wriothesley Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wriothesley_Genshin_M.RenderModel();
		
		//Basura Honkai Star Rail
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, 15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Enojado_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(104.0f, 0.0f, 17.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Feliz_M.RenderModel();

		// --------------------------------------
		// -----------PUESTO DE DARDOS-----------
		// --------------------------------------

		//Puesto Dardos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, 0.0f, -25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoDardos_M.RenderModel();
		
		//Globo1
		Globo1_M.RenderModel();

		//Globo2
		Globo2_M.RenderModel();

		//Globo3
		Globo3_M.RenderModel();

		//Dardo
		Dardo_M.RenderModel();

		//Gato Verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(98.0f, 5.66f, -17.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Verde_M.RenderModel();

		//Firefly Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Firefly_StarRail_M.RenderModel();

		//Luke Normal Tears of Themis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -38.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LukeNormal_M.RenderModel();

		//Globos izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(102.0f, 0.0f, -43.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -45.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(102.0f, 0.0f, -47.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		//Globos derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(102.0f, 0.0f, -6.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(102.0f, 0.0f, -2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		// --------------------------------------
		// ---------------CARRUSEL---------------
		// --------------------------------------

		//Base no movible Carrusel
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(43.0f, 0.0f, -65.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carrusel_Base_M.RenderModel();

		//Carrusel
		Carrusel_M.RenderModel();

		//Lyney Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(69.0f, 0.0f, -65.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lyney_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -33.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		// --------------------------------------
		// -------------BAÑO DARDOS--------------
		// --------------------------------------
		
		//Baño
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, -85.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bano_M.RenderModel();

		//Baño
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, -78.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bano_M.RenderModel();

		//Gato Kafka
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 11.9f, -78.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Kafka_M.RenderModel();

		//Marius Chino Tears of Themis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, -70.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MariusChino_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(104.0f, 0.0f, -98.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(108.0f, 0.0f, -110.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		// --------------------------------------
		// -----------PUESTO HOT DOGS------------
		// --------------------------------------

		//Puesto Hot Dogs
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, 0.0f, -123.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoHotDog_M.RenderModel();

		//Vyn Normal Tears of Themis
		VynNormal_M.RenderModel();

		//Jean Genshin Impact
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 7.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Jean_Genshin_M.RenderModel();

		//Globo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, -119.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Enojado_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(101.0f, 0.0f, -117.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Feliz_M.RenderModel();

		// --------------------------------------
		// --------BANCA PUESTO HOTDOGS----------
		// --------------------------------------

		//Klee Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, -120.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Klee_Genshin_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(43.0f, 0.0f, -127.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -125.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(42.0f, 0.0f, -123.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(44.0f, 0.0f, -125.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		// --------------------------------------
		// -------BANCA PUESTO PALOMITAS---------
		// --------------------------------------

		//Sunday Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, 0.0f, -120.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sunday_StarRail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-43.0f, 0.0f, -127.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, -110.0f));
		model = glm::rotate(model, -200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.0f, 0.0f, -120.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, -125.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-44.0f, 0.0f, -123.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-42.0f, 0.0f, -125.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		// --------------------------------------
		// ----------ARTEM PROTAGONISTA----------
		// --------------------------------------
		//Artem Normal Tears of Themis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -55.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArtemNormal_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Feliz_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Feliz_M.RenderModel();

		// --------------------------------------
		// ----------PUESTO PALOMITAS------------
		// --------------------------------------

		//Puesto de Palomitas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-92.0f, 0.0f, -117.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoPalomitas_M.RenderModel();

		//Feixiao Honkai Star Rail
		Feixiao_StarRail_M.RenderModel();

		//Gato DanHeng
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.0f, 9.17f, -121.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_DanHeng_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-106.0f, 0.0f, -117.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-108.0f, 0.0f, -115.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		// --------------------------------------
		// ----------PUESTO TIRO DADOS-----------
		// --------------------------------------

		//Puesto de Tiro de Dados
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-113.0f, 0.0f, -35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaDados_M.RenderModel();

		//Aventurine Honkai Star Rail
		Aventurine_StarRail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, -20.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globos Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-104.0f, 0.0f, -57.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-103.0f, 0.0f, -55.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-104.0f, 0.0f, -53.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		//Globos Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-104.0f, 0.0f, -22.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-103.0f, 0.0f, -20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-104.0f, 0.0f, -18.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		// --------------------------------------
		// -----------PUESTO BOLICHE-------------
		// --------------------------------------
		
		//Puesto Boliche
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 93.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Boliche_M.RenderModel();

		//Rendija Boliche
		Rendija_Boliche_M.RenderModel();

		// Bolo Boliche
		Bolo_M.RenderModel();

		//Bola Boliche
		Bola_Boliche_M.RenderModel();

		//Ayato Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 83.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ayato_Genshin_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-33.0f, 0.0f, 84.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globos Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-41.0f, 0.0f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 87.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, 0.0f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		//Globos Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-81.0f, 0.0f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-79.0f, 0.0f, 87.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.0f, 0.0f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		// --------------------------------------
		// -----------GOLPEAR AL TOPO------------
		// --------------------------------------

		//Puesto Golpear al Topo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 70.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GolpearTopo_M.RenderModel();

		//Yanqing Honkai Star Rail
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Yanqing_StarRail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(49.5f, 0.0f, 80.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		// --------------------------------------
		// ----------ALGODÓN DE AZÚCAR-----------
		// --------------------------------------

		//Puesto de Algodón de Azúcar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 70.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaquinaAlgodon_M.RenderModel();

		//Robin Honkai Star Rail
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 19.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Robin_StarRail_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(49.0f, 0.0f, 76.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f, 0.0f, 74.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(47.0f, 0.0f, 76.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		// --------------------------------------
		// -----------CARROS CHOCONES------------
		// --------------------------------------

		//Puesto de Carros Chocones
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -125.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarrosChocones_M.RenderModel();

		//Artem Casual Tears of Themis
		ArtemCasual_Themis_M.RenderModel();

		// --------------------------------------
		// ------------BAÑO BOLICHE--------------
		// --------------------------------------

		//Baño
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 93.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bano_M.RenderModel();

		//Baño
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bano_M.RenderModel();

		//Gato Clara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 11.9f, 93.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Clara_M.RenderModel();

		//Ratio Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, 0.0f, 90.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ratio_StarTail_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 80.0f));
		model = glm::rotate(model, -160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, 79.0f));
		model = glm::rotate(model, 75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Verde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-121.0f, 0.0f, 76.0f));
		model = glm::rotate(model, 75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Naranja_M.RenderModel();

		// --------------------------------------
		// ---------PUESTO DE TICKETS------------
		// --------------------------------------

		//Puesto de Tickets
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(86.0f, 0.0f, 110.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTickets_M.RenderModel();

		//Gato Blade
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(86.0f, 7.8f, 113.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Blade_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 105.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(91.0f, 0.0f, 110.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Enojado_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 109.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Enojado_M.RenderModel();

		// --------------------------------------
		// -----------BANCA BOLICHE--------------
		// --------------------------------------
		
		//Banca Boliche
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 60.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_Genshin_M.RenderModel();

		//Vyn Chino Tears of Themis
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VynChino_Themis_M.RenderModel();

		//Gato Basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-114.6f, 2.66f, 62.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Basura_M.RenderModel();

		// --------------------------------------
		// ------------PUESTO TACOS--------------
		// --------------------------------------

		//Puesto Tacos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 8.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTacos_M.RenderModel();

		//Herta Honkai Star Rail
		Herta_StarTail_M.RenderModel();

		//Xiao Genshin Impact
		Xiao_Genshin_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 32.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		// --------------------------------------
		// ------------PUESTO INAZUMA------------
		// --------------------------------------

		//Puesto de Comida de Inazuma
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoInazuma_M.RenderModel();

		//Ixbalanque Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-63.0f, 0.0f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ixbalanque_M.RenderModel();

		//Lámpara Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, -13.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_Genshin_M.RenderModel();

		//Gato Naranja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-58.5f, 0.0f, -14.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Naranja_M.RenderModel();

		//Globos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Enojado_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.0f, 0.0f, 8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo_Feliz_M.RenderModel();

		// --------------------------------------
		// ------------DECORACIONES--------------
		// --------------------------------------

		//Gato RuanMei
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 84.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_RuanMei_M.RenderModel();

		//Plataforma Dodoco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-43.0f, 0.5f, -65.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plataforma_Genshin_M.RenderModel();

		//Mesa de comida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -45.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaComida.RenderModel();

		//Leche de Dango
		LecheDango.RenderModel();

		//Stand de madera en los Dardos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(93.0f, 0.0f, -14.0f));
		model = glm::rotate(model, -80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Dardos_M.RenderModel();

		//Stand de madera en los Hot Dogs
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(93.0f, 0.0f, -115.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_HotDog_M.RenderModel();
		
		//Stand de madera en la taquilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 113.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Taquilla_M.RenderModel();

		//Stand de madera en el carrusel
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(68.0f, 0.0f, -58.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Carrusel_M.RenderModel();

		//Stand de madera en el carros
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-24.0f, 0.0f, -118.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Carros_M.RenderModel();

		//Stand de madera en el de topos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 66.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Topos_M.RenderModel();

		//Stand de madera en dados
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-102.0f, 0.0f, -46.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Dados_M.RenderModel();

		//Stand de madera en puesto inazuma
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-66.0f, 0.0f, 28.0f));
		model = glm::rotate(model, -105 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_Comida_M.RenderModel();

		//Gato Herta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-27.0f, 0.0f, -116.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato_Herta_M.RenderModel();
		

		
		//-----------------¡¡TRANSPARENCIAS!!---------------------------------

		// --------------------------------------
		// ----------LANZAMIENTO HACHA-----------
		// --------------------------------------


		// --------------------------------------
		// -----------JAULA DE BATEO-------------
		// --------------------------------------

		//Wanderer Genshin Impact
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 0.0f, -90.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wanderer_Genshin_M.RenderModel();

		//Hacha
		Hacha_M.RenderModel();

		//Luke Casual Tears of Themis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LukeCasual_Themis_M.RenderModel();

		//Bate
		Bate_M.RenderModel();

		//Pelota de Baseball
		PelotaBateo_M.RenderModel();

		//Basura Honkai Star Rail
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(16.0f, 0.0f, 80.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_StarRail_M.RenderModel();

		//--------------------------------------------------------------------

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Puesto de Lanzamiento de Hacha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 0.0f, -78.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LanzamientoHacha_M.RenderModel();

		//Jaula Bateo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoBateo_M.RenderModel();

		//------
		//REJAS-
		//------

		//Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		rejaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Detrás
		model = glm::translate(model, glm::vec3(11.45f, 0.0f, -11.45));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Izquierda
		model = glm::translate(model, glm::vec3(11.45f, 0.0f, -11.45));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Frente
		model = glm::translate(model, glm::vec3(11.45f, 0.0f, -11.45));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(45.8f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Derecha
		model = glm::translate(model, glm::vec3(11.45f, 0.0f, -11.45));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(22.9f, 0.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glDisable(GL_BLEND);



		/*
		//Coche base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion1(), -0.45f, -5.0f));
		modelaux = model; //Mantiene la jerarquía al cuerpo
		//color = glm::vec3(0.0f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheBase_M.RenderModel();

		// luz ligada al movimiento del automóvil (frente)
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = glm::vec3(10.0f, 0.5f, -5.0f); // Posición (de dónde sale la luz) original
		lowerLight.x += mainWindow.getarticulacion1(); //Modificación en el valor X
		spotLights[1].SetFlash(lowerLight, glm::vec3(1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)
		spotLights1[2].SetFlash(lowerLight, glm::vec3(1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)

		// luz ligada al movimiento del automóvil (detrás)
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		lowerLight = glm::vec3(-10.0f, 0.5f, -5.0f); // Posición (de dónde sale la luz) original
		lowerLight.x += mainWindow.getarticulacion1(); //Modificación en el valor X
		spotLights[2].SetFlash(lowerLight, glm::vec3(-1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)
		spotLights1[1].SetFlash(lowerLight, glm::vec3(-1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)

		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.9f, 3.7f, 0.3f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(1.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheCofre_M.RenderModel();

		// luz ligada al movimiento del cofre
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		lowerLight = glm::vec3(10.0f, 3.0f, -5.0f); // Posición (de dónde sale la luz) original
		lowerLight.y += mainWindow.getarticulacion2()/5; //Modificación en el valor Y
		lowerLight.x += mainWindow.getarticulacion1(); //Modificación en el valor X
		spotLights[0].SetFlash(lowerLight, glm::vec3(1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)
		spotLights1[0].SetFlash(lowerLight, glm::vec3(1.0f, 0.0f, 0.0f)); //lowerlight, Dirección (hacia dónde apunta)

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.7f, 0.6f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheLlantaDelanteraIzquierda_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.7f, 0.8f, -3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheLlantaTraseraIzquierda_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.7f, 0.6f, 3.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheLlantaDelanteraDerecha_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.7f, 0.9f, 3.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CocheLlantaTraseraDerecha_M.RenderModel();
		*/
		/*
		//Slime
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 6.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SlimeFuego_M.RenderModel();

		//Lámpara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, -2.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		//Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 13.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		personaje_M.RenderModel();
		*/

		/*
		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
