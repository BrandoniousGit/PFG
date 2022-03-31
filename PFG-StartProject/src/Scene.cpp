#include "Scene.h"
#include "Utility.h"


/*! \brief Brief description.
*  Scene class is a container for loading all the game objects in your simulation or your game.
*
*/
Scene::Scene()
{
	// Set up your scene here......
	// Set a camera
	_camera = new Camera();
	// Don't start simulation yet
	_simulation_start = false;

	// Position of the light, in world-space
	_lightPosition = glm::vec3(10, 10, 0);

	// Create a game object
	_dynamic_object = new DynamicObject();
	_dynamic_object2 = new DynamicObject();
	_kinematics_object = new KinematicsObject();
	_physics_object2 = new GameObject();
	// Create a game level object
	_level = new GameObject();

	// Create the material for the game object- level
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour(glm::vec3(0.8, 0.8, 0.8));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("assets/textures/diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_level->SetMaterial(modelMaterial);

	// The mesh is the geometry for the object
	Mesh *groundMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	groundMesh->LoadOBJ("assets/models/woodfloor.obj");
	// Tell the game object to use this mesh
	_level->SetMesh(groundMesh);
	_level->SetPosition(0.0f, 0.0f, 0.0f);
	_level->SetRotation(0.0f, 0.0f, 0.0f);


	// Create the material for the game object- level
	Material *objectMaterial = new Material();
	// Shaders are now in files
	objectMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	objectMaterial->SetDiffuseColour(glm::vec3(0.8, 0.1, 0.1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	objectMaterial->SetTexture("assets/textures/default.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	objectMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_kinematics_object->SetMaterial(objectMaterial);
	_dynamic_object->SetMaterial(objectMaterial);
	_dynamic_object2->SetMaterial(objectMaterial);
	_physics_object2->SetMaterial(objectMaterial);

	// Set the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	// Tell the game object to use this mesh
	//Dynamic Object
	_dynamic_object->SetMesh(modelMesh);
	_dynamic_object->SetPosition(glm::vec3(-3.0f, 10.0f, 0.0f));
	_dynamic_object->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	_dynamic_object->SetMass(2.0f);
	_dynamic_object->SetBoundingRadius(_dynamic_object->GetScale());

	//Another Dynamic Object
	_dynamic_object2->SetMesh(modelMesh);
	_dynamic_object2->SetPosition(glm::vec3(-3.2f, 15.0f, 0.0f));
	_dynamic_object2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	_dynamic_object2->SetMass(3.0f);
	_dynamic_object2->SetBoundingRadius(_dynamic_object->GetScale());

	//Kinematics Object 
	_kinematics_object->SetMesh(modelMesh);
	_kinematics_object->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	_kinematics_object->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	//Physics Object (Hard coded motion)
	_physics_object2->SetMesh(modelMesh);
	_physics_object2->SetPosition(3.0f, 50.0f, 0.0f);
	_physics_object2->SetScale(0.5f, 0.5f, 0.5f);
	_physics_object2->SetMass(1.0f);
	_physics_object2->SetInitialVelocity(0.0f, 0.0f, 0.0f);
	_physics_object2->SetBounciness(0.9f);

	
}

Scene::~Scene()
{
	// You should neatly clean everything up here
	delete _kinematics_object;
	delete _level;
	delete _camera;
	delete _physics_object2;
	delete _dynamic_object;
	delete _dynamic_object2;
}

bool travellingDown = true;

void Scene::Update(float deltaTs, Input* input)
{

	// Update the game object (this is currently hard-coded motion)
	if (input->cmd_x)
	{
		_simulation_start = true;
	}
	if (_simulation_start == true)
	{
		glm::vec3 pos2 = _physics_object2->GetPosition();

		glm::vec3 vel2 = _physics_object2->GetVelocity();

		if (pos2.y <= _level->GetPosition().y + 0.5f && travellingDown == true)
		{
			travellingDown = false;
			vel2.y = vel2.y* _physics_object2->GetBounciness();
		}
		//Moving Down
		if (travellingDown == true)
		{ 
			vel2.y += deltaTs / 100;
			pos2 += glm::vec3(vel2.x, _physics_object2->GetMass() * vel2.y * -9.8f * 0.5f, 0.0);
		}
		//Moving Up
		if (travellingDown == false)
		{
			pos2 += glm::vec3(vel2.x, _physics_object2->GetMass() * vel2.y * 9.8f * 0.5f, 0.0);
			vel2.y -= deltaTs / 100;
			if (vel2.y <= 0)
			{
				vel2.y = 0;
				travellingDown = true;
			}
		}

		if (PFG::SphereToSphereCollision(_dynamic_object->GetPosition(), _dynamic_object2->GetPosition(), _dynamic_object->GetBoundingRadius(), _dynamic_object2->GetBoundingRadius(), glm::vec3(0,0,0)))
		{
			std::cout << "yes";
		}

		_physics_object2->SetVelocity(vel2);
		_physics_object2->SetPosition(pos2);
		_kinematics_object->StartSimulation(_simulation_start);
		_dynamic_object->StartSimulation(_simulation_start);
		_dynamic_object2->StartSimulation(_simulation_start);
	}
	_kinematics_object->Update(deltaTs);
	_dynamic_object->Update(deltaTs, 4);
	_dynamic_object2->Update(deltaTs, 4);
	_physics_object2->Update(deltaTs);
	_level->Update(deltaTs);
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
														
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	_dynamic_object->Draw(_viewMatrix, _projMatrix);
	_dynamic_object2->Draw(_viewMatrix, _projMatrix);
	_kinematics_object->Draw(_viewMatrix, _projMatrix);
	_physics_object2->Draw(_viewMatrix, _projMatrix);
	_level->Draw(_viewMatrix, _projMatrix);

}


