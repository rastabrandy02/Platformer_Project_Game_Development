#pragma once
#include "Module.h"
#include "Box2D/Box2D/Box2D.h"
#include "App.h"
#include "Player.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

//#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define PIXELS_PER_METER 32.0f
#define METER_PER_PIXEL 0.03125f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// Small class to return to other modules to track position and rotation of physics bodies
enum userData
{
	DATA_NULL,
	DATA_PLAYER,
	DATA_FIREBALL,
	DATA_GROUND,
	DATA_ENEMY,
	DATA_HEART,
	DATA_STAR,
	DATA_DEATH
};
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener // TODO
{
public:
	Physics();
	~Physics();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateStaticRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	

	List<PhysBody*> groundColliders;
	b2World* world;
	bool debug;
private:

	
	
	b2MouseJoint* mouseJoint;
	b2Body* ground;
	b2Vec2 clickedBodyPos;
	
};