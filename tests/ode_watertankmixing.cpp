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

using namespace std;

class WaterTank : public Test
{
public:

	enum
	{
		e_count = 5100
	};

	WaterTank()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);
		}

		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.allowSleep = false;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body = m_world->CreateBody(&bd);

			// Create the container
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 8.0f, b2Vec2( 10.0f, 2.0f), 0.0); // Right
			body->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(0.5f, 8.2f, b2Vec2(-10.0f, -2.0f), 0.0); // Left
			body->CreateFixture(&shape, 5.0f);
			shape.SetAsBox(10.0f, 0.5f, b2Vec2(0.0f, 10.0f), 0.0); // Top
			body->CreateFixture(&shape, 5.0f);
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
		// Create the mixer
		{
			b2Body* prevBody = ground;
			// Define crank.
			{
				b2PolygonShape shape;
				shape.SetAsBox(0.2f, 3.0f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 10.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 10.0f));
				rjd.motorSpeed = 2.5f * b2_pi;
				rjd.maxMotorTorque = 10000.0f;
				rjd.enableMotor = true;
				m_joint1 = (b2RevoluteJoint*)m_world->CreateJoint(&rjd);

				prevBody = body;
			}
		}

		// 1st alternative to create salt water in the tank
		/*{
			b2CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = 0.4f;

			float minX = -6.0f;
			float maxX = 0.0f;
			float minY = 4.0f;
			float maxY = 6.0f;
			
			b2FixtureDef ballFixtureDef3;
			ballFixtureDef3.restitution = 1.1f;
			ballFixtureDef3.density = 1.0f; // this will affect the ball mass
			ballFixtureDef3.friction = 0.1f;
			ballFixtureDef3.shape = &shape;
			
			for (int32 i = 0; i < 510; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position = b2Vec2(RandomFloat(minX,maxX),RandomFloat(minY,maxY));
				body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, 0.01f);
				body->SetAngularVelocity(1.0f);
			}
		}*/
			
		m_count = 0;
		m_count2 = 0;
		
	}
	b2RevoluteJoint* m_joint;
	b2RevoluteJoint* m_joint1;
	b2RevoluteJoint* m_joint2;
	int32 m_count, m_count2;
	float m_time, num_ball, num_ball2;
	b2Body* m_ball;
	b2Body* body;
	b2Body* m_ballinside;

	bool m_button;

	void Step(Settings& settings) override
	{
		Test::Step(settings);
		m_time += 1.0f / 60.0f;	// assuming we are using frequency of 60 Hertz 
		
		g_debugDraw.DrawString(5, m_textLine, "Time (in seconds)= %.2f", m_time);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Salt Water particle entering the tank= %4.0f", num_ball);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Salt Water particle in the tank= %4.0f", num_ball2);
		m_textLine += m_textIncrement;
		float torque = m_joint1->GetMotorTorque(settings.m_hertz);
		float omega = m_joint1->GetMotorSpeed();
		g_debugDraw.DrawString(5, m_textLine, "Mixer motor Torque = %5.0f", (float) torque);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Mixer speed= %5.0f", (float) omega);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Press 'a' to add the mixer speed");
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Press 's' to stop the mixer");
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Press 'm' to set the mixer speed to 10");
		m_textLine += m_textIncrement;

				
		if (m_count < e_count)
		{
			// Create small ball as representation of fluid of water that will be going to the water tank
			b2CircleShape ballShape;
			ballShape.m_p.SetZero();
			ballShape.m_radius = 0.2f;
			
			b2FixtureDef ballFixtureDef;
			ballFixtureDef.restitution = 0.1f;
			ballFixtureDef.density = 0.2f; // this will affect the ball mass
			ballFixtureDef.friction = 0.1f;
			ballFixtureDef.shape = &ballShape;
			
			b2BodyDef ballBodyDef;
			ballBodyDef.type = b2_dynamicBody;
			ballBodyDef.position.Set(-20.0f, 19.0f);
			m_ball = m_world->CreateBody(&ballBodyDef);
			b2Fixture *ballFixture = m_ball->CreateFixture(&ballFixtureDef);	
			m_ball->SetAngularVelocity(1.0f);
			m_ball->SetLinearVelocity(b2Vec2(15.5f, 0.0f));
				
			++m_count;	
			num_ball = m_count;
		}
		// Alternative 2 to create fluid inside the tank
		if (m_count2 < 510)
		{
			// Create small ball as representation of fluid of water that is inside the water tank
			b2CircleShape ballShape2;
			ballShape2.m_p.SetZero();
			ballShape2.m_radius = 0.4f;
			//ballShape2.color(0.9f, 0.3f, 0.3f); // how to change color?
			
			b2FixtureDef ballFixtureDef2;
			ballFixtureDef2.restitution = 0.05f;
			ballFixtureDef2.density = 2.3f; // this will affect the ball mass
			ballFixtureDef2.friction = 0.1f;
			ballFixtureDef2.shape = &ballShape2;
			
			b2BodyDef ballBodyDef2;
			ballBodyDef2.type = b2_dynamicBody;
			ballBodyDef2.position.Set(0.0f, 10.0f);
			
			m_ballinside = m_world->CreateBody(&ballBodyDef2);
			b2Fixture *ballFixture2 = m_ballinside->CreateFixture(&ballFixtureDef2);	

			++m_count2;
			num_ball2 = m_count2 ;
			
		}
	}
	
	

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_joint1->SetMotorSpeed(5+m_time);		
			break;

		case GLFW_KEY_S:
			m_joint1->SetMotorSpeed(0.0f);
			break;
		case GLFW_KEY_M:
			m_joint1->SetMotorSpeed(10.0f);
			break;
		}
	}

	static Test* Create()
	{
		return new WaterTank;
	}

};

static int testIndex = RegisterTest("Physics with ODE", "Water Tank", WaterTank::Create);
