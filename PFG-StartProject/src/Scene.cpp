#include "Scene.h"
#include "Utility.h"
#include <vector>
#include <string>


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
	_physics_object2 = new GameObject();
	// Create a game level object
	_level = new GameObject();

	Objects.push_back(new DynamicObject());
	Objects.push_back(new DynamicObject());
	Objects.push_back(new DynamicObject());
	Objects.push_back(new DynamicObject());

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
	Objects[0]->SetMaterial(objectMaterial);
	Objects[1]->SetMaterial(objectMaterial);
	Objects[2]->SetMaterial(objectMaterial);
	Objects[3]->SetMaterial(objectMaterial);
	_physics_object2->SetMaterial(objectMaterial);

	// Set the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	// Tell the game object to use this mesh
	//Dynamic Object
	Objects[0]->SetMesh(modelMesh);
	Objects[0]->SetPosition(glm::vec3(-3.1f, 10.0f, 0.0f));
	Objects[0]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	Objects[0]->SetMass(2.0f);
	Objects[0]->SetBoundingRadius(Objects[0]->GetScale());
	Objects[0]->name = "Sphere1";

	//Another Dynamic Object
	Objects[1]->SetMesh(modelMesh);
	Objects[1]->SetPosition(glm::vec3(-3.0f, 15.0f, 0.0f));
	Objects[1]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	Objects[1]->SetMass(3.0f);
	Objects[1]->SetBoundingRadius(Objects[1]->GetScale());
	Objects[1]->name = "Sphere2";

	//Another Dynamic Object
	Objects[2]->SetMesh(modelMesh);
	Objects[2]->SetPosition(glm::vec3(3.0f, 20.0f, -0.1f));
	Objects[2]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	Objects[2]->SetMass(3.0f);											  
	Objects[2]->SetBoundingRadius(Objects[2]->GetScale());				  
	Objects[2]->name = "Sphere3";

	//Another Dynamic Object
	Objects[3]->SetMesh(modelMesh);
	Objects[3]->SetPosition(glm::vec3(4.0f, 7.0f, 0.1f));
	Objects[3]->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	Objects[3]->SetMass(3.0f);
	Objects[3]->SetBoundingRadius(Objects[3]->GetScale());
	Objects[3]->name = "Sphere4";
																		  
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
	delete _level;
	delete _camera;
	delete _physics_object2;
	delete Objects[0];
	delete Objects[1];
	delete Objects[2];
	delete Objects[3];
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
			vel2.y = vel2.y * _physics_object2->GetBounciness();
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

		glm::vec3 cp;

		for (int i = 0; i < Objects.size(); i++)
		{
			for (int j = i + 1; j < Objects.size(); j++)
			{
				if (PFG::SphereToSphereCollision(Objects[i]->GetPosition(), Objects[j]->GetPosition(), Objects[i]->GetBoundingRadius(), Objects[j]->GetBoundingRadius(), cp))
				{
					//Objects[i]->AddVelocity(glm::vec3(0.5f, 0.0f, 0.0f));

					float magnitude1 = glm::dot(Objects[i]->GetForce(), cp/Objects[i]->GetBoundingRadius());
					glm::vec3 currentVel1 = cp/Objects[i]->GetBoundingRadius() * magnitude1;

					float magnitude2 = glm::dot(Objects[j]->GetForce(), cp / Objects[j]->GetBoundingRadius());
					glm::vec3 currentVel2 = cp / Objects[j]->GetBoundingRadius() * magnitude2;

					float restitution = Objects[i]->_elasticity * Objects[j]->_elasticity;

					Objects[i]->AddVelocity(currentVel1 * restitution /2.0f);
					Objects[j]->AddVelocity(-currentVel2 * restitution /2.0f);
				}
			}
		}
		_physics_object2->SetVelocity(vel2);
		_physics_object2->SetPosition(pos2);
		Objects[0]->StartSimulation(_simulation_start);
		Objects[1]->StartSimulation(_simulation_start);
		Objects[2]->StartSimulation(_simulation_start);
		Objects[3]->StartSimulation(_simulation_start);
	}

	Objects[0]->Update(deltaTs, 3);
	Objects[1]->Update(deltaTs, 3);
	Objects[2]->Update(deltaTs, 3);
	Objects[3]->Update(deltaTs, 3);
	_physics_object2->Update(deltaTs);
	_level->Update(deltaTs);
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
														
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	Objects[0]->Draw(_viewMatrix, _projMatrix);
	Objects[1]->Draw(_viewMatrix, _projMatrix);
	Objects[2]->Draw(_viewMatrix, _projMatrix);
	Objects[3]->Draw(_viewMatrix, _projMatrix);
	_physics_object2->Draw(_viewMatrix, _projMatrix);
	_level->Draw(_viewMatrix, _projMatrix);
}