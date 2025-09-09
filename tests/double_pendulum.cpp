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

// ./testbed > /root/output.txt
// close the testbed to record the result see it at /root/output.txt

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#include "test.h"
#include <fstream> 

class DoublePendulum : public Test
{
public:

	DoublePendulum()
	{
		b2Body* b1;
		{
			b2EdgeShape shape;
			shape.SetTwoSided(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));

			b2BodyDef bd;
			b1 = m_world->CreateBody(&bd);
			b1->CreateFixture(&shape, 0.0f);
		}
		// the two blocks below for creating boxes are only for sweetener.
		{
			b2PolygonShape shape;
			shape.SetAsBox(7.0f, 0.25f, b2Vec2_zero, 0.7f); // Gradient is 0.7

			b2BodyDef bd;
			bd.position.Set(6.0f, 6.0f);
			b2Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}
		{
			b2PolygonShape shape;
			shape.SetAsBox(7.0f, 0.25f, b2Vec2_zero, -0.7f); // Gradient is -0.7

			b2BodyDef bd;
			bd.position.Set(-6.0f, 6.0f);
			b2Body* ground = m_world->CreateBody(&bd);
			ground->CreateFixture(&shape, 0.0f);
		}

		b2Body* b2; // the hanging bar for the pendulum
		{
			b2PolygonShape shape;
			shape.SetAsBox(7.25f, 0.25f);

			b2BodyDef bd;
			bd.position.Set(0.0f, 37.0f);
			b2 = m_world->CreateBody(&bd);
			b2->CreateFixture(&shape, 0.0f);
		}

		b2RevoluteJointDef jd;
		b2Vec2 anchor;

		// Create the first pendulum ball
		b2CircleShape ballShape;
		ballShape.m_p.SetZero();
		ballShape.m_radius = 0.5f;
		
		b2FixtureDef ballFixtureDef;
		ballFixtureDef.restitution = 0.75f;
		ballFixtureDef.density = 3.3f; // this will affect the ball mass
		ballFixtureDef.friction = 0.1f;
		ballFixtureDef.shape = &ballShape;
		
		b2BodyDef ballBodyDef;
		ballBodyDef.type = b2_dynamicBody;
		ballBodyDef.position.Set(0.0f, 25.0f);
		// ballBodyDef.angularDamping = 0.2f;

		m_ball = m_world->CreateBody(&ballBodyDef);
		b2Fixture *ballFixture = m_ball->CreateFixture(&ballFixtureDef);	
		
		// Create the anchor and connect it to the ball
		anchor.Set(0.0f, 36.0f); // x and y axis position for the Pendulum anchor
		jd.Initialize(b2, m_ball, anchor);
		//jd.collideConnected = true;
		m_world->CreateJoint(&jd); // Create the Pendulum anchor

		// Create the second pendulum ball
		b2CircleShape ballShape2;
		ballShape2.m_p.SetZero();
		ballShape2.m_radius = 0.5f;
		
		b2FixtureDef ballFixtureDef2;
		ballFixtureDef2.restitution = 0.75f;
		ballFixtureDef2.density = 2.3f; // this will affect the ball mass
		ballFixtureDef2.friction = 0.1f;
		ballFixtureDef2.shape = &ballShape;
		
		b2BodyDef ballBodyDef2;
		ballBodyDef2.type = b2_dynamicBody;
		ballBodyDef2.position.Set(0.0f, 20.0f);
		// ballBodyDef.angularDamping = 0.2f;

		m_ball2 = m_world->CreateBody(&ballBodyDef2);
		b2Fixture *ballFixture2 = m_ball2->CreateFixture(&ballFixtureDef2);	

		// Create the Revolute Joint to connect the first pendulum ball to the second pendulum ball
		b2RevoluteJointDef jd2;
		jd2.bodyA = m_ball;
		jd2.bodyB = m_ball2;
		jd2.collideConnected = false;
		jd2.localAnchorA.Set(0.0f,0.0f); // the anchor A is the center of the m_ball
		jd2.localAnchorB.Set(0.0f,5.0f);// the anchor B is the center of the m_ball2
		m_joint1 = (b2RevoluteJoint*)m_world->CreateJoint(&jd2);
		

		m_time = 0.0f;	
	}
	
	void Step(Settings& settings) override
	{
		m_time += 1.0f / 60.0f;	// assuming we are using frequency of 60 Hertz 
		b2Vec2 v = m_ball->GetLinearVelocity();
		b2Vec2 v2 = m_ball2->GetLinearVelocity();
		float omega = m_ball->GetAngularVelocity();
		float angle = m_ball->GetAngle();
		float omega2 = m_ball2->GetAngularVelocity();
		float angle2 = m_ball2->GetAngle();
		b2MassData massData = m_ball->GetMassData();
		b2Vec2 position = m_ball->GetPosition();
		b2MassData massData2 = m_ball2->GetMassData();
		b2Vec2 position2 = m_ball2->GetPosition();

		float ke = 0.5f * massData.mass * b2Dot(v, v) + 0.5f * massData.I * omega * omega;
		float ke2 = 0.5f * massData2.mass * b2Dot(v2, v2) + 0.5f * massData2.I * omega2 * omega2;

		g_debugDraw.DrawString(5, m_textLine, "Time (in seconds)= %.6f", m_time);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Ball 1 position ( x , y )= (%4.1f, %4.1f)", position.x, position.y);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Ball 1 Mass= %.6f", massData.mass);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Linear velocity fo Ball 1= %.6f", v);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Kinetic energy for Ball 1= %.6f", ke);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Angle (in degrees) for Ball 1= %.6f", angle*RADTODEG);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Ball 2 position ( x , y )= (%4.1f, %4.1f)", position2.x, position2.y);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Ball 2 Mass= %.6f", massData2.mass);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Linear velocity fo Ball 2= %.6f", v2);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Kinetic energy for Ball 2= %.6f", ke2);
		m_textLine += m_textIncrement;
		g_debugDraw.DrawString(5, m_textLine, "Angle (in degrees) for Ball 2= %.6f", angle2*RADTODEG);
		m_textLine += m_textIncrement;
		// Print the result in every time step then plot it into graph with either gnuplot or anything

		printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", position.x, position.y, angle*RADTODEG, v, position2.x, position2.y, angle2*RADTODEG, v);
		//std::ofstream MyFile("/root/output.txt"); ;
		//MyFile << angle << "  " << position.x << "  " << position.y;
			
		Test::Step(settings);
	}
	
	static Test* Create()
	{
		return new DoublePendulum;
	}
	
	b2RevoluteJoint* m_joint1;

	b2Body* m_ball;
	b2Body* m_ball2;
	float m_time;

};

static int testIndex = RegisterTest("Oscillations", "Double Pendulum", DoublePendulum::Create);
