// MIT License 

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "settings.h"
#include "test.h"
#include <fstream> 
#include <iostream>
#include "imgui/imgui.h"
using namespace std;

class BodyFallinginaDenseFluid : public Test
{
public:

	enum
	{
		e_count = 2510
	};

	BodyFallinginaDenseFluid()
	{
		b2Body* ground = NULL;
		m_world->SetGravity(b2Vec2(0.0f,-9.8f));
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);
		}

		b2Body* b2; // the hanging bar for the pendulum
		{
			b2PolygonShape shape;
			shape.SetAsBox(7.25f, 0.25f);

			b2BodyDef bd;
			bd.position.Set(0.0f, 42.0f);
			b2 = m_world->CreateBody(&bd);
			b2->CreateFixture(&shape, 0.0f);
		}

		b2RevoluteJointDef jdpendulum;
		b2Vec2 anchor;


		// Create small ball as representation of a body that will be falling to the dense fluid in the water tank
			b2CircleShape ballShape;
			ballShape.m_p.SetZero();
			ballShape.m_radius = 0.5f;
			
			b2FixtureDef ballFixtureDef;
			ballFixtureDef.restitution = 0.1f;
			ballFixtureDef.density = 3.3f; // this will affect the ball mass
			ballFixtureDef.friction = 0.1f;
			ballFixtureDef.shape = &ballShape;
			
			b2BodyDef ballBodyDef;
			ballBodyDef.type = b2_dynamicBody;
			ballBodyDef.position.Set(0.0f, 37.5f);
			m_ballhanging = m_world->CreateBody(&ballBodyDef);
			b2Fixture *ballFixture = m_ballhanging->CreateFixture(&ballFixtureDef);	
		// End of creating the body

		// Create the anchor and connect it to the crate
		anchor.Set(0.0f, 41.0f); // x and y axis position for the Pendulum anchor
		jdpendulum.Initialize(b2, m_ballhanging, anchor);
		// to create the joint / rope that connect the body, 
		// we use (b2RevoluteJoint*)m_world->CreateJoint(&jdpendulum); so we can destroy it later
		m_jointpendulum = (b2RevoluteJoint*)m_world->CreateJoint(&jdpendulum);
		// End of creating the pendulum set


		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.allowSleep = false;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body = m_world->CreateBody(&bd);

			// Create the container
			b2PolygonShape shape; 
			// (width, height, b2vec(xstartposition,ystartposition) )
			shape.SetAsBox(0.5f, 20.0f, b2Vec2( 10.0f, 10.0f), 0.0); // Right
			body->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(0.5f, 8.2f, b2Vec2(-10.0f, -2.0f), 0.0); // Left
			body->CreateFixture(&shape, 5.0f);
			//shape.SetAsBox(10.0f, 0.5f, b2Vec2(0.0f, 10.0f), 0.0); // Top
			//body->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(10.0f, 0.5f, b2Vec2(0.0f, -10.0f), 0.0); // Bottom
			body->CreateFixture(&shape, 5.0f);

			b2RevoluteJointDef jd;
			jd.bodyA = ground;
			jd.bodyB = body;
			jd.localAnchorA.Set(0.0f, 10.0f);
			jd.localAnchorB.Set(0.0f, 0.0f);
			jd.referenceAngle = 0.0f;
			//jd.motorSpeed = 0.05f * b2_pi;
			jd.maxMotorTorque = 1e8f;
			jd.enableMotor = true;
			m_joint = (b2RevoluteJoint*)m_world->CreateJoint(&jd);
		}
			
		// Create the incoming fluid container / pipe
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.allowSleep = false;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body2 = m_world->CreateBody(&bd);

			// Create the container
			b2PolygonShape shape;
			// SetAsBox(width, length , b2Vec2( x_position, y_position), 0.0);
			//shape.SetAsBox(0.5f, 5.0f, b2Vec2( -12.0f, 25.0f), 0.0); // Right
			//body2->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(0.5f, 10.0f, b2Vec2(-30.0f, 15.0f), 0.0); // Left
			body2->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(10.0f, 0.5f, b2Vec2(-20.0f, 6.77f), 0.0); // Bottom
			body2->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(10.0f, 0.5f, b2Vec2(-20.0f, 10.0f), 0.0); // Top
			body2->CreateFixture(&shape, 5.0f);

			// Create the anchor so the container do not fall
			b2RevoluteJointDef jd2;
			jd2.bodyA = ground;
			jd2.bodyB = body2;
			jd2.localAnchorA.Set(0.0f, 10.0f);
			jd2.localAnchorB.Set(0.0f, 0.0f);
			jd2.referenceAngle = 0.0f;
			//jd.motorSpeed = 0.05f * b2_pi;
			jd2.maxMotorTorque = 1e8f;
			jd2.enableMotor = true;
			m_joint2 = (b2RevoluteJoint*)m_world->CreateJoint(&jd2);
		}

		{
			
			b2EdgeShape shape;
			shape.SetTwoSided(b2Vec2(-40.0f, -10.0f), b2Vec2(999.0f, -10.0f));
			ground->CreateFixture(&shape, 0.0f);
		}
			
		m_count = 0;
		m_count2 = 0;
		
	}
	b2Body* m_box;
	b2Body* m_boxelevator;
	b2RevoluteJoint* m_jointpendulum;

	b2RevoluteJoint* m_joint;
	b2RevoluteJoint* m_joint1;
	b2RevoluteJoint* m_joint2;
	int32 m_count, m_count2;
	float m_time, num_ball, mass_ball1, v_ball1, mass_ballhanging;
	b2Body* m_ball; 
	b2Body* m_ballhanging;

	bool m_button;
	double division(double x, double y)
	{
		return x/y;
	}

	void Step(Settings& settings) override
	{
		Test::Step(settings);
		m_time += 1.0f / 60.0f;	// assuming we are using frequency of 60 Hertz 
		b2MassData massDatabodyhanging = m_ballhanging->GetMassData();
		b2Vec2 velocity = m_ballhanging->GetLinearVelocity();
		b2Vec2 position = m_ballhanging->GetPosition();
		
		float mass_ballhanging = massDatabodyhanging.mass;
		float r = 5, h = 10, r_ball = 0.1;
		float Vwatertank = b2_pi*r*r*h;
		float Vball = division(4,3)*b2_pi*r_ball*r_ball*r_ball;
		float particle = division(Vwatertank,Vball);

		g_debugDraw.DrawString(5, m_textLine, "Time (in seconds) = %.2f", m_time);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Volume of the water tank = %.5f", Vwatertank);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Volume of a fluid particle = %.5f", Vball);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Amount of fluid particles fit in the tank = %.5f", particle);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Speed of the fluid that is entering the tank = %4.2f", v_ball1);
		m_textLine += m_textIncrement;
		//g_debugDraw.DrawString(5, m_textLine, "Salt Water particle entering the tank = %4.0f", num_ball);
		//m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "The mass of a Fluid that is entering the tank = %4.2f", mass_ball1);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "The mass of a body that is hanging = %4.2f", mass_ballhanging);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Body position = (%4.1f, %4.1f)", position.x, position.y);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Body velocity = (%4.1f, %4.1f)", velocity.x, velocity.y);
		m_textLine += m_textIncrement;
				
		printf("%4.2f %4.2f %4.2f\n", velocity.y, position.x, position.y);

		if (m_count < e_count)
		{
			// Create small ball as representation of fluid of water that will be going to the water tank
			b2CircleShape ballShape;
			ballShape.m_p.SetZero();
			ballShape.m_radius = 0.1f;
			
			b2FixtureDef ballFixtureDef;
			ballFixtureDef.restitution = 0.1f;
			ballFixtureDef.density = 1.0f; // this will affect the ball mass
			ballFixtureDef.friction = 0.1f;
			ballFixtureDef.shape = &ballShape;
			
			b2BodyDef ballBodyDef;
			ballBodyDef.type = b2_dynamicBody;
			ballBodyDef.position.Set(-29.7f, 17.5f);
			m_ball = m_world->CreateBody(&ballBodyDef);
			b2Fixture *ballFixture = m_ball->CreateFixture(&ballFixtureDef);	
			m_ball->SetAngularVelocity(1.0f);
			m_ball->SetLinearVelocity(b2Vec2(25.0f, 0.0f));

			b2MassData massDataentering = m_ball->GetMassData();
			mass_ball1 = massDataentering.mass;
			b2Vec2 velocity1 = m_ball->GetLinearVelocity();
			v_ball1 = velocity1.x;
				
			++m_count;	
			num_ball = m_count;
		}
		
	}
	
	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_L:
			m_world->DestroyJoint(m_jointpendulum); //Thanks beautiful..
			break;
		
		}
	}

	static Test* Create()
	{
		return new BodyFallinginaDenseFluid;
	}

};

static int testIndex = RegisterTest("Physics with ODE", "Body Falling in a Dense Fluid", BodyFallinginaDenseFluid::Create);
