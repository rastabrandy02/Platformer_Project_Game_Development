
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Physics.h"
#include "p2Point.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"

//#ifdef _DEBUG
//#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
//#else
//#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
//#endif



Physics::Physics() : Module()
{
	world = NULL;
	mouseJoint = NULL;
	debug = false;
}

// Destructor
Physics::~Physics()
{
}

bool Physics::Start()
{
	LOG("Creating Physics 2D environment");
	if (app->currentScene == SCENE_LEVEL_1)
	{
		world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
		//world->SetContactListener(this);
		world->SetContactListener(app->player);
		// needed to create joints like mouse joint
	}
	
	name.Create("physics");

	return true;
}


bool Physics::PreUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		world->Step(app->dtSeconds, 6, 2);

		for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
		{
			if (c->GetFixtureA()->IsSensor() && c->IsTouching())
			{
				PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
				PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
				if (pb1 && pb2 && pb1->listener)
					pb1->listener->OnCollision(pb1, pb2);
			}
		}
	}
	

	return true;
}

PhysBody* Physics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* Physics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}
PhysBody* Physics::CreateStaticRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* Physics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* Physics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
bool Physics::PostUpdate()
{
	if (app->currentScene == SCENE_LEVEL_1)
	{
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			debug = !debug;

		if (!debug)
			return true;

		// Bonus code: this will iterate all objects in the world and draw the circles
		// You need to provide your own macro to translate meters to pixels
		b2Body* clickedBody = NULL;
		b2Vec2 mousePos;
		int mPosX, mPosY;
		app->input->GetMousePosition(mPosX, mPosY);
		mousePos = { PIXEL_TO_METERS(mPosX), PIXEL_TO_METERS(mPosY) };

		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					app->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}

				// TODO 1: If mouse button 1 is pressed ...
				// test if the current body contains mouse position

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				{

					if (b->GetFixtureList()->GetShape()->TestPoint(b->GetTransform(), mousePos)) clickedBody = b;
				}

			}
		}

		// If a body was selected we will attach a mouse joint to it
		// so we can pull it around
		// TODO 2: If a body was selected, create a mouse joint
		// using mouse_joint class property

		if (clickedBody != NULL)
		{
			b2MouseJointDef mJoint;
			mJoint.bodyA = ground;
			mJoint.bodyB = clickedBody;
			mJoint.target = mousePos;
			mJoint.dampingRatio = 0.5f;
			mJoint.frequencyHz = 2.0f;
			mJoint.maxForce = 100.0f * clickedBody->GetMass();
			mouseJoint = (b2MouseJoint*)world->CreateJoint(&mJoint);

			clickedBodyPos.x = clickedBody->GetPosition().x;
			clickedBodyPos.y = clickedBody->GetPosition().y;




		}
		// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			mouseJoint->SetTarget(mousePos);
			if (clickedBody != NULL)
			{
				app->render->DrawLine(mousePos.x, mousePos.y, clickedBodyPos.x, clickedBodyPos.y, 255, 0, 0, 255);
			}




		}
		// TODO 4: If the player releases the mouse button, destroy the joint
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			//delete mouseJoint;
			world->DestroyJoint(mouseJoint);
			mouseJoint = NULL;
			clickedBody = NULL;
		}


	}
	
	return true;
}


// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

/*void Physics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}*/

void Physics::BeginContact(b2Contact* contact)
{
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if (((int)fixtureUserData == 3))
	{
		//app->player->isJumping = true;
	}



	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
	
}